
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Tue Aug 19 12:50:21 2014
//
////////////////////////////////////////////////////////////////////////////////


#include <string.h>
#include <stdlib.h>

#include "icm/icmCpuManager.h"

// Be careful to avoid overwriting any edits if igen is run a second time.
// Only use -overwrite if you wish to overwrite this file.

#include "platform.options.igen.h"
#include "platform.handles.igen.h"
#include "platform.constructor.igen.h"

static Bool postCmdParser(void) {
    // postCmdParser code goes here
    return True;
}

static Bool postPlatformConstruct(void) {
   
     icmSimulationStarting ();
    icmAdvanceTime(0);

    icmLoadProcessorMemory(handles.cpu0_c, 
        "/home/jonah/fingerprinting/code_gen/output2/cpu0/cpu0.elf", 0, 0, 1);
    icmLoadProcessorMemory(handles.cpu1_c, 
        "/home/jonah/fingerprinting/code_gen/output2/cpu1/cpu1.elf", 0, 0, 1);
    icmLoadProcessorMemory(handles.cpum_c, 
        "/home/jonah/fingerprinting/code_gen/output2/cpuM/cpuM.elf", 0, 0, 1);
    return True;
}

static Bool postSimulate(void) {
    // postSimulate code goes here
    return True;
}


#include "platform.clp.igen.h"
////////////////////////////////////////////////////////////////////////////////
//                                   M A I N
////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char *argv[]) {
    if(!cmdParser(argc, argv)) {
        return 1;
    }
    if(!postCmdParser())  {
        return 1;
    }
    platformConstructor();

    if(!postPlatformConstruct()) {
        return 1;
    }

    icmSimulatePlatform();
    if(!postSimulate()) {
        return 1;
    }
    icmTerminate();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
