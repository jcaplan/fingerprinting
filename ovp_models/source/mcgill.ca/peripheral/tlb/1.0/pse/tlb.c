
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Tue Sep  2 15:57:17 2014
//
////////////////////////////////////////////////////////////////////////////////

#include "tlb.igen.h"
#include "peripheral/bhm.h"
#include <stdio.h>
#include <stdlib.h>
#define PAGESIZE 0x1000

void update_mapping(void);
//////////////////////////////// Callback stubs ////////////////////////////////
Uns32 virtual_address[32];
Uns32 physical_address[32];
Uns32 fprint_table[33];

typedef union CSR_SLAVE_address {
    Uns32 value;
        struct {
            unsigned offset       : 2;
            unsigned index        : 5;
            unsigned message_type : 2;
        } bits;
} CSR_SLAVE_address;



PPM_NET_CB(do_reset){

    if(value == 0){
        bhmPrintf("Resetting TLB!\n");
        TLB_CSR_ab32_data.tlbActivateReg.value = 0;
        TLB_CSR_ab32_data.lineValidReg.value = 0;
        TLB_CSR_ab32_data.lineEnableReg.value = 0;

        int i;
        for(i = 0; i < 32; i++){
            virtual_address[i] = 0;
            physical_address[i] = 0;
            fprint_table[i] = 0;
        }
        fprint_table[32] = 0;
        ppmCreateDynamicBridge("TLB_SLAVE", 0x0, 0xffffffff, "TLB_MASTER", 0x0);
    }
}


int cmpfunc (const void * a, const void * b)
{
   return ( *(Uns32*)a - *(Uns32*)b );
}

PPM_REG_READ_CB(regRd32) {
    return *(Uns32*)user;
}


void update_mapping(void){
 
    //Delete all the bridges
    ppmDeleteDynamicBridge("TLB_SLAVE",0x0,0xfffffff);
  
  
    //Create all valid mappings and build a list of the starting virtual addresses
    Uns32 enabled[32];
    fprint_table[0] = 0;
    int en_reg = TLB_CSR_ab32_data.lineEnableReg.value;
    int i; 
    for(i = 31; i >= 0; i--){
        if(en_reg & (1 << i)){
            fprint_table[++fprint_table[0]] = physical_address[i];
            ppmCreateDynamicBridge("TLB_SLAVE",virtual_address[i] << 12,0x1000, "TLB_MASTER", physical_address[i] << 12);
            
            if(diagnosticLevel == 3){
                bhmMessage("I","TLB","Translation create %x -> %x\n",virtual_address[i] << 12,physical_address[i] << 12);
            }
            enabled[i] = (virtual_address[i] << 12) ;
        }else{
            enabled[i] = 0;
        }
    }

    //Sort the virtual addresses
    qsort(enabled, 32, sizeof(Uns32), cmpfunc);

    


    //Fill in the blanks in the bridge
    Uns32 low = 0;
    Uns32 high;
    //For each line

    if(enabled[31] == 0){
       ppmCreateDynamicBridge("TLB_SLAVE",0,0xffffffff, "TLB_MASTER", 0x0);
        if(diagnosticLevel == 3){
            bhmPrintf("Dynamic bridge create %x,%x -> %x,%x\n", 0,0xffffffff,0,0xffffffff);
        }
        return; 
    }

    for(i = 0; i < 32; i ++){
        
        //First deal with all the zeros and the first non-zero entry:
        high = enabled[i];
        if(high > low && low == 0){
            ppmCreateDynamicBridge("TLB_SLAVE",0,high, "TLB_MASTER", 0);
            if(diagnosticLevel == 3){
                bhmPrintf("Dynamic bridge create %x,%x -> %x,%x\n", low,high,low,high);
            }
        }
        //Then skip consecutive translations (hence the 0x1000 offset)
        //And plug in any gaps in the bridging
        else if(high > low + 0x1000){
            ppmCreateDynamicBridge("TLB_SLAVE",low+0x1000,high - (low + 0x1000), "TLB_MASTER", low + 0x1000);
            if(diagnosticLevel == 3){
                bhmPrintf("Dynamic bridge create %x,%x -> %x,%x\n", low+0x1000,high,low+0x1000,high);    
            }
        }
        low = high;
    }

    //Finally top off the bridge at the high end of the address space
    if(high < 0xfffff000){
        ppmCreateDynamicBridge("TLB_SLAVE",high+0x1000,0xffffffff-high-0x1000, "TLB_MASTER", high+0x1000);
        if(diagnosticLevel == 3){
            bhmPrintf("Dynamic bridge create %x,%x -> %x,%x\n", high+0x1000,0xffffffff,high+0x1000,0xffffffff);
        }
    }


}
//Line_ enable
PPM_REG_WRITE_CB(regWr32) {
    *(Uns32*)user = data;


//When the line enable or tlb activate registers change,
// all the dynamic bridges should be destroyed
//and then a new set of bridges set up to reflect the mappings
//Then all the rest of the space needs to be filled in accordingly

    if((((Uns32*)user == &(TLB_CSR_ab32_data.lineEnableReg.value)) || \
        ((Uns32*)user == &(TLB_CSR_ab32_data.tlbActivateReg.value))) \
        && (TLB_CSR_ab32_data.tlbActivateReg.value != 0)){
        update_mapping();

        //Send updated table to FPRINT unit
        bhmPrintf("data to fprint unit: {");
        int i;
        for(i = 0; i <= fprint_table[0]; i++){
            bhmPrintf("%x, ",fprint_table[i]);
        }
        bhmPrintf("}\n");

        ppmWriteAddressSpace(handles.TLB_FPRINT_MASTER, \
                            0x0, \
                            132, \
                            &fprint_table);
    }




}

#define VIRTUAL_OFFSET  0x2
#define PHYSICAL_OFFSET 0x3
PPM_WRITE_CB(memregWr) {
    
    //Here we update the physical and virtual address tables.

    CSR_SLAVE_address address;
    address.value = (Uns32)(addr- handles.TLB_CSR);

        if(address.bits.message_type == VIRTUAL_OFFSET){
            if(diagnosticLevel == 3){
                bhmPrintf("virtual table %x, %x\n",address.bits.index,data);
            }
            virtual_address[address.bits.index] = data;
        }
        else if(address.bits.message_type == PHYSICAL_OFFSET){
            if(diagnosticLevel == 3){
                bhmPrintf("physical table %x, %x\n",address.bits.index,data);
            }
            physical_address[address.bits.index] = data;
        }
    


}

PPM_CONSTRUCTOR_CB(constructor) {
    // YOUR CODE HERE (pre constructor)

    ppmCreateDynamicBridge("TLB_SLAVE", 0x0, 0xffffffff, "TLB_MASTER", 0x0);
    periphConstructor();

    // YOUR CODE HERE (post constructor)
}
