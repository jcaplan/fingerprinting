
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Fri Oct  3 10:15:33 2014
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "peripheral/impTypes.h"
#include "peripheral/bhm.h"
#include "peripheral/ppm.h"
#include "dma.igen.h"
//////////////////////////////// Callback stubs ////////////////////////////////
#define THREAD_STACK      (8*1024)
#define NUM_CHANNELS      2
#define BYTES_PER_ACCESS  8


bhmThreadHandle       thread;
bhmEventHandle        start;
char                  stack[THREAD_STACK];


PPM_REG_WRITE_CB(controlWr) {
    // YOUR CODE HERE (controlWr)
    *(Uns32*)user = data;
    if(DMACSP_ab8_data.control.bits.GO == 1 && DMACSP_ab8_data.length.value > 0){
        //start transfering
        bhmTriggerEvent(start);
    }

}

PPM_REG_READ_CB(regRd) {
    // YOUR CODE HERE (regRd)
    return *(Uns32*)user;
}

PPM_REG_WRITE_CB(regWr) {
    // YOUR CODE HERE (regWr)
    *(Uns32*)user = data;
    if(DMACSP_ab8_data.control.bits.GO == 1 && DMACSP_ab8_data.length.value > 0){
        //start transfering
        bhmTriggerEvent(start);
    }
}

PPM_REG_WRITE_CB(statusWr) {
    // YOUR CODE HERE (statusWr)
    if(data == 0){
        DMACSP_ab8_data.status.bits.DONE = 0;
        DMACSP_ab8_data.status.bits.REOP = 0;
        DMACSP_ab8_data.status.bits.WEOP = 0;
        DMACSP_ab8_data.status.bits.LEN = 0;
        if(DMACSP_ab8_data.control.bits.I_EN == 1){
            ppmWriteNet(handles.DMA_IRQ, 0); 
        } 
    }
}
static void dmaBurst(void) {   
    Uns32 len = DMACSP_ab8_data.length.value;
    char* buff = malloc(len);
    ppmReadAddressSpace (handles.MREAD,  DMACSP_ab8_data.readaddress.value, len, buff);
    ppmWriteAddressSpace(handles.MWRITE, DMACSP_ab8_data.writeaddress.value, len, buff);
    free(buff);
    DMACSP_ab8_data.length.value = 0;
    DMACSP_ab8_data.status.bits.LEN = 1;
}


static void channelThread(void *user)
{

    while(1) {
        if (diagnosticLevel >= 2) bhmPrintf("waiting\n");
        bhmWaitEvent(start);
        
        //Start transaction
        DMACSP_ab8_data.status.bits.BUSY = 1;
        if (diagnosticLevel >= 2) 
            bhmPrintf("ch running\n");

        dmaBurst();
        if (diagnosticLevel >= 2) 
            bhmPrintf("ch finished\n");
        //finish transaction
        DMACSP_ab8_data.status.bits.BUSY = 0;
        DMACSP_ab8_data.status.bits.DONE = 1;
        DMACSP_ab8_data.status.bits.REOP = 1;
        DMACSP_ab8_data.status.bits.WEOP = 1;
        if(DMACSP_ab8_data.control.bits.I_EN == 1){
            ppmWriteNet(handles.DMA_IRQ, 1);   
        }
    }
}

PPM_CONSTRUCTOR_CB(constructor) {
    periphConstructor();
    start = bhmCreateEvent();
    thread = bhmCreateThread(channelThread,0,"DMA", &stack[THREAD_STACK]);
}


PPM_DESTRUCTOR_CB(destructor) {
    // YOUR CODE HERE (destructor)
}

