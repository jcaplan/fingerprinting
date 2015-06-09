
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Mon Jun  8 13:53:30 2015
//
////////////////////////////////////////////////////////////////////////////////

#include "sw_reset.igen.h"
//////////////////////////////// Callback stubs ////////////////////////////////

PPM_REG_READ_CB(regRd32) {
    // YOUR CODE HERE (regRd32)
    return *(Uns32*)user;
}

PPM_REG_WRITE_CB(regWr32) {
    // YOUR CODE HERE (regWr32)
    *(Uns32*)user = data;

    char core = 0;


    if((Uns32*)user == &(SW_RESET_SLAVE_ab32_data.cpum.value)){
    	ppmWriteNet(handles.cpum_reset, 0);
        ppmWriteNet(handles.cpum_reset, 1);
        core = 'M';
    }
    else if((Uns32*)user == &(SW_RESET_SLAVE_ab32_data.cpu0.value)){
    	ppmWriteNet(handles.cpu0_reset, 0);
        ppmWriteNet(handles.cpu0_reset, 1);
        core = '0';
    }
 	else if((Uns32*)user == &(SW_RESET_SLAVE_ab32_data.cpu1.value)){
    	ppmWriteNet(handles.cpu1_reset, 0);
        ppmWriteNet(handles.cpu1_reset, 1);
        core = '1';
    }
    else if((Uns32*)user == &(SW_RESET_SLAVE_ab32_data.cpu2.value)){
        ppmWriteNet(handles.cpu2_reset, 0);
        ppmWriteNet(handles.cpu2_reset, 1);
        core = '2';
    }
    else if((Uns32*)user == &(SW_RESET_SLAVE_ab32_data.cpu3.value)){
        ppmWriteNet(handles.cpu3_reset, 0);
        ppmWriteNet(handles.cpu3_reset, 1);
        core = '3';
    }
    else if((Uns32*)user == &(SW_RESET_SLAVE_ab32_data.cpu4.value)){
        ppmWriteNet(handles.cpu4_reset, 0);
        ppmWriteNet(handles.cpu4_reset, 1);
        core = '4';
    }
    else if((Uns32*)user == &(SW_RESET_SLAVE_ab32_data.cpu5.value)){
        ppmWriteNet(handles.cpu5_reset, 0);
        ppmWriteNet(handles.cpu5_reset, 1);
        core = '5';
    }
    else if((Uns32*)user == &(SW_RESET_SLAVE_ab32_data.cpu6.value)){
        ppmWriteNet(handles.cpu6_reset, 0);
        ppmWriteNet(handles.cpu6_reset, 1);
        core = '6';
    }
    else if((Uns32*)user == &(SW_RESET_SLAVE_ab32_data.cpu7.value)){
        ppmWriteNet(handles.cpu7_reset, 0);
        ppmWriteNet(handles.cpu7_reset, 1);
        core = '7';
    }


    bhmPrintf("Reset core %c\n",core);
}

PPM_CONSTRUCTOR_CB(constructor) {
    // YOUR CODE HERE (pre constructor)
    periphConstructor();
    // YOUR CODE HERE (post constructor)
}

PPM_DESTRUCTOR_CB(destructor) {
    // YOUR CODE HERE (destructor)
}

