
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Tue Jun 23 14:43:06 2015
//
////////////////////////////////////////////////////////////////////////////////

#include "reset_monitor.igen.h"
//////////////////////////////// Callback stubs ////////////////////////////////

PPM_REG_READ_CB(regRd32) {
    // YOUR CODE HERE (regRd32)
    return *(Uns32*)user;
}

PPM_REG_WRITE_CB(writeCoreReg) {
    // YOUR CODE HERE (writeCoreReg)


    if(diagnosticLevel == 3){
        bhmPrintf("core %d writes to core register (old value: %x)\n",data,*(Uns32*)user);
    }

    *(Uns32*)user |= (1 << data) ;
    if(diagnosticLevel == 3){
        bhmPrintf("new value: %x\n",*(Uns32*)user);
    }

    if(RESET_MONITOR_SLAVE_ab32_data.enableReg.value == 1){
        if(RESET_MONITOR_SLAVE_ab32_data.monitorReg.value == 
            RESET_MONITOR_SLAVE_ab32_data.coreStatusReg.value){
            ppmWriteNet(handles.cpum_notify, 1);
            *(Uns32*)user = 0;
        }
    }

}

PPM_REG_WRITE_CB(writeEnableReg) {
    *(Uns32*)user = data;
    if(diagnosticLevel == 3){
        bhmPrintf("MONITOR: Enable register write: %d\n",data);
    }

    if(data == 0){
        ppmWriteNet(handles.cpum_notify, 0);
    }
}

PPM_REG_WRITE_CB(writeMonitorReg) {
    *(Uns32*)user = data;
    if(diagnosticLevel == 3){
        bhmPrintf("Monitor writes data %d\n",data);
    }
}

PPM_CONSTRUCTOR_CB(constructor) {
    // YOUR CODE HERE (pre constructor)
    periphConstructor();
    // YOUR CODE HERE (post constructor)
}

PPM_DESTRUCTOR_CB(destructor) {
    // YOUR CODE HERE (destructor)
}

