/*
 * Copyright (c) 2005-2014 Imperas Software Ltd., www.imperas.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// Imperas header files
#include "hostapi/impAlloc.h"
#include "vmi/vmiMessage.h"
#include "vmi/vmiRt.h"

// Model header files
#include "Nios_IIStructure.h"
#include "Nios_IIExceptions.h"
#include "Nios_IIFunctions.h"
#include "Nios_IIPorts.h"

#define NUM_MEMBERS(_A) (sizeof(_A)/sizeof((_A)[0]))

static Bool posedge(Uns32 old, Uns32 new) {
    if (old == 0 && new == 1) {
        return True;
    }
    return False;
}

static Bool negedge(Uns32 old, Uns32 new) {
    if (old == 1 && new == 0) {
        return True;
    }
    return False;
}

static VMI_NET_CHANGE_FN(reset_n_event) {

    Nios_IIP Nios_II = (Nios_IIP)processor;

    //
    // The reset is low level sensitive
    //
    if (posedge(Nios_II->reset_n, newValue)) {
        //
        // Enter Reset
        //
    }
    if (negedge(Nios_II->reset_n, newValue)) {
        //
        // Exit Reset
        //
    }

//    //
//    // If we have a rising edge, and we are edge triggered and
//    //
//    if (posedge(Nios_II->reset_n, newValue)) {
//        Nios_II->ResetPending = True;
//        // halt the processor while reset is active
//        vmirtHalt((vmiProcessorP)Nios_II);
//
//    } else {
//
//        Nios_II->ResetPending = False;
//
//        // reset the processor
//        vmirtRestartNext((vmiProcessorP)Nios_II);
//
//        Nios_IIProcessReset(processor);
//
//        Nios_IIDoException(Nios_II);
//
//    }

    Nios_II->reset_n = newValue;
}

static VMI_NET_CHANGE_FN(irq_event) {

    Nios_IIP Nios_II = (Nios_IIP)processor;
    Uns32 index = (Uns32)(UnsPS)userData;

    Uns32 mask = 0x1 << index;
    Uns32 irq  = (Nios_II->d_irq >> index) & 0x1;

    if (posedge(irq, newValue)) {
        //
        // Activated
        //
        Nios_II->d_irq |= mask;
    }
    if (negedge(irq, newValue)) {
        //
        // DeActivated
        //
        Nios_II->d_irq &= ~mask;
    }

    // process the interrupt
    Nios_IICheckExceptions(Nios_II);
}



static VMI_NET_CHANGE_FN(fprint_write_in_address) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_II->fprint_write_in_address = newValue;
}

static VMI_NET_CHANGE_FN(fprint_write_in_data) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_II->fprint_write_in_data = newValue;
}


static VMI_NET_CHANGE_FN(fprint_write_in) {

   Nios_IIP Nios_II = (Nios_IIP)processor;

   vmiPrintf("fprint event = %d,%x,%x\n",newValue,Nios_II->fprint_write_in_address,Nios_II->fprint_write_in_data);
    

}

typedef struct Nios_IINetPortS {
    vmiNetPort desc;
    Uns32      variantMask;
} Nios_IINetPort;

static Uns32 countPorts(Nios_IINetPortP template, Nios_IIP Nios_II) {
    Nios_IINetPortP p = template;
    Uns32 c = 0;
    Uns32 variant = Nios_II->configInfo.arch;

    while(p && p->desc.name) {
        if (p->variantMask & variant) {
            c++;
        }
        p++;
    }
    return c;
}

void Nios_IINewNetPorts(Nios_IIP Nios_II) {
    Uns32 variant = Nios_II->configInfo.arch;
    Nios_IINetPort template[] = {
        { {"reset_n", vmi_NP_INPUT, (void*)0,  reset_n_event}, (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq0",  vmi_NP_INPUT, (void*)0,  irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq1",  vmi_NP_INPUT, (void*)1,  irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq2",  vmi_NP_INPUT, (void*)2,  irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq3",  vmi_NP_INPUT, (void*)3,  irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq4",  vmi_NP_INPUT, (void*)4,  irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq5",  vmi_NP_INPUT, (void*)5,  irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq6",  vmi_NP_INPUT, (void*)6,  irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq7",  vmi_NP_INPUT, (void*)7,  irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq8",  vmi_NP_INPUT, (void*)8,  irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq9",  vmi_NP_INPUT, (void*)9,  irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq10", vmi_NP_INPUT, (void*)10, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq11", vmi_NP_INPUT, (void*)11, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq12", vmi_NP_INPUT, (void*)12, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq13", vmi_NP_INPUT, (void*)13, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq14", vmi_NP_INPUT, (void*)14, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq15", vmi_NP_INPUT, (void*)15, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq16", vmi_NP_INPUT, (void*)16, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq17", vmi_NP_INPUT, (void*)17, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq18", vmi_NP_INPUT, (void*)18, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq19", vmi_NP_INPUT, (void*)19, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq20", vmi_NP_INPUT, (void*)20, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq21", vmi_NP_INPUT, (void*)21, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq22", vmi_NP_INPUT, (void*)22, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq23", vmi_NP_INPUT, (void*)23, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq24", vmi_NP_INPUT, (void*)24, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq25", vmi_NP_INPUT, (void*)25, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq26", vmi_NP_INPUT, (void*)26, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq27", vmi_NP_INPUT, (void*)27, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq28", vmi_NP_INPUT, (void*)28, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq29", vmi_NP_INPUT, (void*)29, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq30", vmi_NP_INPUT, (void*)30, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"d_irq31", vmi_NP_INPUT, (void*)31, irq_event},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"fprint_write_out", vmi_NP_OUTPUT, (void*)0, 0, &Nios_II->fprint_write_out},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"fprint_write_out_address", vmi_NP_OUTPUT, (void*)1, 0, &Nios_II->fprint_write_out_address},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"fprint_write_out_data", vmi_NP_OUTPUT, (void*)2, 0, &Nios_II->fprint_write_out_data},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"fprint_write_in", vmi_NP_INPUT, (void*)0, fprint_write_in, 0},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"fprint_write_in_data", vmi_NP_INPUT, (void*)1, fprint_write_in_data, 0},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        { {"fprint_write_in_address", vmi_NP_INPUT, (void*)2, fprint_write_in_address, 0},     (NIOS_II_E | NIOS_II_S | NIOS_II_F) },
        
        { { 0 } }
    };

    // count members
    Uns32 numMembers = countPorts(template, Nios_II);
    Uns32 i, j;

    // allocate permanent port structure (including terminator)
    Nios_IINetPortP result = STYPE_CALLOC_N(Nios_IINetPort, numMembers+1);

    // fill permanent port structure
    for(i=0, j=0; i<NUM_MEMBERS(template); i++) {
        if(template[i].variantMask & variant) {
            result[j++] = template[i];
        }
    }

    // save ports on processor
    Nios_II->netPorts = result;

    // //Add nets for store and address:
    // Nios_IINetPort fprintTemplate[64];
    // for(i = 0; i < 64; i++){
    //     char buf[12];
    //     sprintf(buf,"fprint_%d",i);
    //     fprintTemplate[i] =  {buf, vmi_NP_OUTPUT, (void*)i,  0, }, (NIOS_II_E | NIOS_II_S | NIOS_II_F) }
    // }


}

//
// Free ports
//
void Nios_IIFreeNetPorts(Nios_IIP Nios_II) {
    if(Nios_II->netPorts) {
        STYPE_FREE(Nios_II->netPorts);
        Nios_II->netPorts = 0;
    }
}

//
// Skip port descriptions not used on this variant
//
static void skipUnusedPorts(Nios_IINetPortPP p, Uns32 variant) {
    while(*p && (*p)->desc.name && !((*p)->variantMask & variant)) {
        (*p)++;
    }
}

//
// Get the next net port
//
VMI_NET_PORT_SPECS_FN(Nios_IINetPortSpecs) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_IINetPortP c = (Nios_IINetPortP)prev;
    Uns32 variant = Nios_II->configInfo.arch;

    if(!c) {
        c = Nios_II->netPorts;
    } else {
        c++;
    }
    skipUnusedPorts(&c, variant);

    return (vmiNetPortP)(c && (c->desc.name) ? c : 0);
}
