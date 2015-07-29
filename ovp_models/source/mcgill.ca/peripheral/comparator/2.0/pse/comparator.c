
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Thu Jul 23 16:21:49 2015
//
////////////////////////////////////////////////////////////////////////////////

#include "comparator.igen.h"
//////////////////////////////// Callback stubs ////////////////////////////////


int fprint[3][32];
int assignment[3][16];
int checkin[3][16];
int checkout[3][16];
int fprintsReady[3][16];
int nmrReg[16];
int successReg;
int failedReg;
int successCounterReg[16];
int taskCounter[16];
// exception_reg exception;

void writeSuccessReg(){
   COMP_CSR_SLAVE_ab32_data.successReg.value = successReg;
}

void writeFailedReg(){
    COMP_CSR_SLAVE_ab32_data.failedReg.value = failedReg;
}

void mismatch_occurred(int task){
    //write the exception register
    //write the checkin and checkout
    //reset the head and tail

    if(diagnosticLevel == 3){
        bhmPrintf("mismatch!\n");
        bhmPrintf("task %d fprint %x and %x don't match\n",task,fprint[0][task],fprint[1][task]);
    }
    failedReg |= (1 << task);
    writeFailedReg();
    taskCounter[task] = 0;
    fprintsReady[0][task] = 0;
    fprintsReady[1][task] = 0;
    checkin[0][task] = 0;
    checkout[0][task] = 0;
    checkin[1][task] = 0;
    checkout[1][task] = 0;
    ppmWriteNet(handles.COMP_IRQ, 1);


}


int check_fprint(int task){
    int test = 1;
    int i;
         //if the comparisons don't match
    if(fprint[0][task] != fprint[1][task]){
        test = 0;
    }

  
    for(i = 0; i < 2; i++){
        fprintsReady[i][task] = 0;
    }
    return test;
}


void do_comparison(void){
    int fp;
    int task;
    for(task = 0; task < 16; task++){
        //If a fingerprint is ready on both cores or the task is complete
        //on both cores
        fp = (fprintsReady[0][task] == 1 && fprintsReady[1][task] == 1);
        if(fp)
            break;
    }

    //If the previous search gets a hit
    if(task != 16){
        //If different number of fingerprints then collision
        bhmPrintf("fingerprints ready\n");
        int test = check_fprint(task);
        taskCounter[task]++;
        //check final condition of test
        if(test != 1) {
            mismatch_occurred(task);
            bhmPrintf("comparison failed!\n");
        } else {
            if(diagnosticLevel == 3){
                bhmPrintf("task %d fprint %x and %x match\n",task,fprint[0][task],fprint[1][task]);
            }
            fprintsReady[0][task] = 0;
            fprintsReady[0][task] = 0;       
        }
    }
}

void checkTaskComplete(int task){
    int i;
    if(checkin[0][task] == 1 && checkin[1][task] == 1){
        if(taskCounter[task] == successCounterReg[task]){
            for(i = 0; i < 2; i++){
                checkin[i][task] = 0;
                checkout[i][task] = 0;
            }
            successReg |= (1 << task);
            writeSuccessReg();
            successCounterReg[task]++;
            bhmPrintf("success task %d, successReg = %d!\n",task, successReg);
            ppmWriteNet(handles.COMP_IRQ, 1);
         } else {
            bhmPrintf("COMPARATOR: task counter %d wrong (should be %d)!\n",taskCounter[task],successCounterReg[task]);
            mismatch_occurred(task);
        }
    }
}

PPM_REG_WRITE_CB(catRegWrite) {
    *(Uns32*)user = data;
    COMP_CSR_SLAVE_ab32_dataTP catRegData = handles.COMP_CSR_SLAVE;
    unsigned coreID = catRegData->catReg.bits.coreID;
    unsigned taskID = catRegData->catReg.bits.taskID;
    unsigned catData   = catRegData->catReg.bits.data;
    assignment[coreID][taskID] = catData;
}

PPM_REG_READ_CB(csrRd32) {
    bhmPrintf("COMP: register read value: %d\n", *(Uns32*)user);
    return *(Uns32*)user;
}

PPM_REG_WRITE_CB(currentStateWrite) {
    *(Uns32*)user = data;
    COMP_FPRINT_SLAVE_ab32_dataTP fprintData =  handles.COMP_FPRINT_SLAVE;
    unsigned coreID = fprintData->currentState.bits.coreID;
    unsigned taskID = fprintData->currentState.bits.taskID;
    switch(fprintData->currentState.bits.enable){        
    case 0:
        if(checkout[coreID][taskID]){
            checkin[coreID][taskID] = 1;
            checkTaskComplete(taskID);
        }
        break;
    case 1:
        checkout[coreID][taskID] = 1;
        break;
    }
}

PPM_REG_WRITE_CB(exceptionRegWrite) {
    successReg = 0;
    writeSuccessReg();
    failedReg = 0;
    writeFailedReg();
    ppmWriteNet(handles.COMP_IRQ, 0);
}

PPM_REG_WRITE_CB(fprintWrite) {
    *(Uns32*)user = data;
    COMP_FPRINT_SLAVE_ab32_dataTP fprintData = handles.COMP_FPRINT_SLAVE;
    unsigned taskID = fprintData->fprint.bits.taskID;
    unsigned coreID = fprintData->fprint.bits.coreID;
    unsigned messageID = fprintData->fprint.bits.messageID;
    unsigned fprintBlock = fprintData->fprint.bits.fprint;

    bhmPrintf("COMP: data %x, messageID %d, coreID %d, taskID %d, fprintBlock %d\n",
        data,messageID,coreID,taskID,fprintBlock);
    switch(messageID){
    case 0:
        bhmPrintf("COMP: comparator receives block 0\n");
        fprint[coreID][taskID] = fprintBlock;
        break;
    case 1:
        bhmPrintf("COMP: comparator receives block 1\n");
        fprint[coreID][taskID] |= fprintBlock << 16;
        fprintsReady[coreID][taskID] = 1;
        do_comparison();
        break;
    }
}

PPM_REG_WRITE_CB(nmrRegWrite) {
    *(Uns32*)user = data;
    COMP_CSR_SLAVE_ab32_dataTP nmrRegData = handles.COMP_CSR_SLAVE;
    unsigned taskID = nmrRegData->nmrReg.bits.taskID;
    unsigned nmrData   = nmrRegData->nmrReg.bits.data;
    nmrReg[taskID] = nmrData;
}

PPM_REG_WRITE_CB(successCounterRegWrite) {
    *(Uns32*)user = data;
    COMP_CSR_SLAVE_ab32_dataTP successCounterRegData = handles.COMP_CSR_SLAVE;
    unsigned taskID = successCounterRegData->successCounterReg.bits.taskID;
    unsigned successData = successCounterRegData->successCounterReg.bits.data;
    successCounterReg[taskID] = successData;
}

PPM_CONSTRUCTOR_CB(constructor) {
    // YOUR CODE HERE (pre constructor)
    periphConstructor();
    // YOUR CODE HERE (post constructor)
}

PPM_DESTRUCTOR_CB(destructor) {
    // YOUR CODE HERE (destructor)
}

