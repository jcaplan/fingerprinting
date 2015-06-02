
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Tue Jul 29 17:25:54 2014
//
////////////////////////////////////////////////////////////////////////////////

#include "fprint.igen.h"
#include "peripheral/impTypes.h"
#include "peripheral/ppm.h"
#include "peripheral/bhm.h"
#include "crc.h"
#include "fprint.igen.h"

#define COMPARATOR_BASE_ADDRESS 0x1000000
//#define COUNT
//////////////////////////////// Callback stubs ////////////////////////////////

Uns32 fprint[8];
Uns32 fprint_old;
Uns32 count[8];
Uns32 count_old;
Uns32 pauseReg;
Uns32 currentState[8];
Uns32 pause_index;
Uns32 coreID;
Uns32 mode = 1;

Uns32 old_address = 0xcafebabe;
Uns32 old_data    = 0xcafebabe;
#define FPRINT_VALID 1
#define FPRINT_EMPTY 0



Uns32 fprint_count = 0;
Uns32 store_count = 0;

Uns32 fprint_pause_hold[2] = {FPRINT_EMPTY, 0};
PPM_NET_CB(do_store) {

    
    
      //We reach here because of a net
    Uns32 address = ppmReadNet(handles.FPRINT_WRITE_ADDRESS );
    Uns32 data    = ppmReadNet(handles.FPRINT_WRITE_DATA); 

        if(fprint_pause_hold[0] == FPRINT_VALID){
           Uns32 fprint_address = COMPARATOR_BASE_ADDRESS + (((coreID << 4) + 1) << 2);
          ppmWriteAddressSpace(handles.FPRINT_MASTER,fprint_address, 4, &fprint_pause_hold[1]);
          fprint_count++;
          fprint_pause_hold[0] = FPRINT_EMPTY;
                    if(diagnosticLevel >= 1){
            bhmPrintf("core %d completed fingerprint: %x\n",coreID, fprint[pause_index]);
          }
        }

        if(FPRINT_CSR_SLAVE_ab32_data.currentState.bits.enable == 1){
   //           if(((address & 0xff0000) != 0x450000) && (old_data != data || old_address != address)){  
          if(diagnosticLevel >= 2){
            bhmPrintf("core %d fingerprint address,data: %x,%x\n",coreID,address,data);
          }

       		fprint_old = fprint[pause_index];
#ifdef COUNT
         	fprint[pause_index] = 0;
#else
          fprint[pause_index] = crcFast(address,data,fprint[pause_index]);
#endif          
          store_count++;
          count_old = count[pause_index];
    	       	if(mode == 1){
    	       		count[pause_index] += 2;
    			}else{
    				count[pause_index] += 1;
    			}

          if(diagnosticLevel == 3){
            bhmPrintf("core %d intermediate fingerprint: %x\n",coreID, fprint[pause_index]);
          }
  //  	}



      if(count[pause_index] >= FPRINT_CSR_SLAVE_ab32_data.counterMax.value){
          // bhmPrintf("fingerprint complete on core %d: %x\n",coreID,fprint[pause_index]);
          //ppmWriteNet(handles.FPRINT_WRITE_DATA_OUT, fprint[pause_index]);
         
         // ppmWriteNet(handles.FPRINT_WRITE_ADDRESS_OUT, address);
       //   bhmPrintf("fingerprint writes: %x: %x\n",address,fprint[pause_index]);

          fprint_pause_hold[0] = FPRINT_VALID;
          fprint_pause_hold[1] = fprint[pause_index];
          fprint[pause_index] = 0;
          count[pause_index] = 0;
          // ppmWriteAddressSpace(handles.FPRINT_MASTER,address, 4, &fprint[pause_index]);
         // ppmWriteNet(handles.FPRINT_WRITE_OUT, 1);

      }

       old_address = address;
       old_data    = data;
    }

}

PPM_REG_READ_CB(pause) {

    ////////////////////////////////
    if(FPRINT_CSR_SLAVE_ab32_data.currentState.bits.enable == 1){

      fprint_pause_hold[0] = FPRINT_EMPTY;
     // bhmPrintf("task paused on core %d!\n",coreID);
//      bhmPrintf("fprint old/new: : %x,%x\n",fprint_old, fprint[pause_index]);
//      bhmPrintf("count  old/new: : %d,%d\n",count_old, count[pause_index]);
      fprint[pause_index] = fprint_old;
      count[pause_index]  = count_old;
      currentState[pause_index] = FPRINT_CSR_SLAVE_ab32_data.currentState.value;
      Uns32 address = COMPARATOR_BASE_ADDRESS + (((coreID << 4) + 2) << 2);
      ppmWriteAddressSpace(handles.FPRINT_MASTER,address, 4, &currentState[pause_index]);
//      bhmPrintf("state:     %d,%x,%x\n",count[pause_index],fprint[pause_index],FPRINT_CSR_SLAVE_ab32_data.currentState.value);
      pause_index++;
      fprint[pause_index] = 0;
      count[pause_index]  = 0;
      FPRINT_CSR_SLAVE_ab32_data.currentState.value = 0;
      
//      bhmPrintf("new state: %d,%x,%x\n",count[pause_index],fprint[pause_index],FPRINT_CSR_SLAVE_ab32_data.currentState.value);
    }
    return *(Uns32*)user;
}

PPM_REG_READ_CB(regRd32) {
    bhmPrintf("%d\n",*(Uns32*)user);
    // YOUR CODE HERE (regRd32)
    return *(Uns32*)user;
}

PPM_REG_WRITE_CB(regWr32) {
    // YOUR CODE HERE (regWr32)
    *(Uns32*)user = data;
    //if(diagnosticLevel >= 2){
        bhmPrintf("core %d writes fingerprint: %x, fprint count = %d, store_count = %d\n",coreID, data,fprint_count,store_count);
    //  }
    //If writing to current state then forward info to comparator
    
    if((Uns32*)user == &(FPRINT_CSR_SLAVE_ab32_data.currentState.value)){
      //If task is complete then write the current fingerprint
    Uns32 address;
      if(FPRINT_CSR_SLAVE_ab32_data.currentState.bits.enable == 0){
        address = COMPARATOR_BASE_ADDRESS + (((coreID << 4) + 1) << 2);
        ppmWriteAddressSpace(handles.FPRINT_MASTER,address, 4, &fprint[pause_index]);

      }
      address = COMPARATOR_BASE_ADDRESS + (((coreID << 4) + 0) << 2);
      ppmWriteAddressSpace(handles.FPRINT_MASTER,address, 4, &FPRINT_CSR_SLAVE_ab32_data.currentState.value);
    }
   }

PPM_REG_READ_CB(unpause) {
    if(pause_index > 0){
  //  bhmPrintf("task unpaused on core %d!\n",coreID);
    pause_index--;
    FPRINT_CSR_SLAVE_ab32_data.currentState.value = currentState[pause_index];
   // bhmPrintf("state: %d,%x,%x\n",count[pause_index],fprint[pause_index],FPRINT_CSR_SLAVE_ab32_data.currentState.value);
     Uns32 address = COMPARATOR_BASE_ADDRESS + (((coreID << 4) + 3) << 2);
      ppmWriteAddressSpace(handles.FPRINT_MASTER,address, 4, &currentState[pause_index]);
    }
     
    return *(Uns32*)user;
    
}

PPM_CONSTRUCTOR_CB(constructor) {
    // YOUR CODE HERE (pre constructor)
    periphConstructor();
    // YOUR CODE HERE (post constructor)
    crcInit();
    FPRINT_CSR_SLAVE_ab32_data.counterMax.value = 0xfff;
    bhmIntegerAttribute("coreID", &coreID);       
    pause_index = 0;
}

PPM_DESTRUCTOR_CB(destructor) {
    // YOUR CODE HERE (destructor)
}

