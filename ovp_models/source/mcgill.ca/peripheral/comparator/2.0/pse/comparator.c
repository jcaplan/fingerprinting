
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

#define TMR_ALL_CORES_MATCH     5
#define TMR_CORE_0_MISMATCH     4
#define TMR_CORE_1_MISMATCH     3
#define TMR_CORE_2_MISMATCH     2
#define TMR_ALL_CORES_MISMATCH  1

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
int tmrResult[16];
    
int getLogicalCoreID(int task, int coreID){
    int id = -1;
    if(assignment[0][task] == coreID){
        id = 0;
    } else if (assignment[1][task] == coreID){
        id = 1;
    } else if (assignment[2][task] == coreID){
        id = 2;
    }
    return id;
}

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

int check_fprint_nmr(int task){
   int c0_matches_c1 = 1;
   int c1_matches_c2 = 1;
   int c2_matches_c0 = 1;
   int result;
   int i;
   if(fprint[0][task] != fprint[1][task]){
        c0_matches_c1 = 0;
   }
   if(fprint[1][task] != fprint[2][task]){
        c1_matches_c2 = 0;
   }
   if(fprint[2][task] != fprint[0][task]){
        c2_matches_c0 = 0;
   }
   
    if(c0_matches_c1 && c1_matches_c2){
        result = TMR_ALL_CORES_MATCH;
   } else if(c0_matches_c1){
        result = TMR_CORE_2_MISMATCH;
   } else if(c1_matches_c2){
        result = TMR_CORE_0_MISMATCH;
   } else if(c2_matches_c0){
        result = TMR_CORE_1_MISMATCH;
   } else {
        result = TMR_ALL_CORES_MISMATCH;
   }

    for(i = 0; i < 3; i++){
        fprintsReady[i][task] = 0;
    }
    tmrResult[task] = result;
    return result;
}


void do_comparison(void){
    int fp;
    int task;
    for(task = 0; task < 16; task++){
        //If a fingerprint is ready on both cores or the task is complete
        //on both cores


        fp = (fprintsReady[0][task] == 1 && fprintsReady[1][task] == 1);
        if(nmrReg[task]){
            bhmPrintf("nmr reg\n");
            fp &= fprintsReady[2][task];
        }

        if(fp)
            break;
    }

    //If the previous search gets a hit
    if(task != 16){
        //If different number of fingerprints then collision
        bhmPrintf("fingerprints ready for task %d\n",task);
        
        if(nmrReg[task]){
            int test = check_fprint_nmr(task);
            taskCounter[task]++;
            const char *message = "";
            switch(test){
            case TMR_ALL_CORES_MATCH:
                message = "COMP::All cores match";
                break;
            case TMR_CORE_0_MISMATCH:
                message = "COMP::Core 0 does not match";
                break;
            case TMR_CORE_1_MISMATCH:
                message = "COMP::Core 1 does not match";
                break;
            case TMR_CORE_2_MISMATCH:
                message = "COMP::Core 2 does not match";
                break;
            case TMR_ALL_CORES_MISMATCH:
                message = "COMP::None of the cores match";
                break;
            }
            bhmPrintf("%s\n",message);


        } else {
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
            }
        }
    }
}

void checkTaskComplete(int task){
    int i;
    if(checkin[0][task] == 1 && checkin[1][task] == 1
        && (nmrReg[task] ? checkin[2][task] : 1)){
        if(taskCounter[task] == successCounterReg[task]){
            for(i = 0; i < 3; i++){
                checkin[i][task] = 0;
                checkout[i][task] = 0;
            }
            if(nmrReg[task]){
                switch(tmrResult[task]){
                case TMR_CORE_2_MISMATCH:
                case TMR_CORE_1_MISMATCH:
                case TMR_ALL_CORES_MATCH:
                    successReg |= (1 << (task*2));
                    break;
                case TMR_CORE_0_MISMATCH:
                    successReg |= (2 << (task*2));
                    break;
                case TMR_ALL_CORES_MISMATCH:    
                    break;
                }
            } else {
                //1 -> core 0 is safe
                successReg |= (1 << (task*2));
            }
            writeSuccessReg();
            taskCounter[task] = 0;
            bhmPrintf("success task %d, (%d/%d), successReg = %d!\n",task, taskCounter[task],
                successCounterReg[task],successReg);
            ppmWriteNet(handles.COMP_IRQ, 1);
         } else {
            // bhmPrintf("COMPARATOR: task counter %d wrong (should be %d)!\n",taskCounter[task],successCounterReg[task]);
            // mismatch_occurred(task);
            bhmPrintf("intermediate result for task %d (%d/%d fingerprints have arrived), keep going\n",task,taskCounter[task],successCounterReg[task]);
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
    bhmPrintf("COMP:: writing cat: logical core %d, task %d, core %d\n",coreID,taskID,catData);
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
    int id = getLogicalCoreID(taskID,coreID);

    switch(fprintData->currentState.bits.enable){        
    case 0:
        if(checkout[id][taskID]){
            checkin[id][taskID] = 1;
            checkTaskComplete(taskID);
        }
        break;
    case 1:
        checkout[id][taskID] = 1;
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
    int id = getLogicalCoreID(taskID,coreID);

    bhmPrintf("COMP: data %x, messageID %d, coreID %d, taskID %d, fprintBlock %d\n",
        data,messageID,coreID,taskID,fprintBlock);
    switch(messageID){
    case 0:
        bhmPrintf("COMP: comparator receives block 0\n");
        fprint[id][taskID] = fprintBlock;
        break;
    case 1:
        bhmPrintf("COMP: comparator receives block 1\n");
        fprint[id][taskID] |= fprintBlock << 16;
        fprintsReady[id][taskID] = 1;
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
    bhmPrintf("writing successCounterReg task %d = %d\n",taskID,successData);
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

