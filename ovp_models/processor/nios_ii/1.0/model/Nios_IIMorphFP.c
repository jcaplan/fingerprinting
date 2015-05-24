#include "Nios_IIMorphFP.h"

#include "Nios_IIFunctions.h"
#include "Nios_IIInstructions.h"
#include "Nios_IIMorph.h"
#include "Nios_IIMorphFP.h"
#include "vmi/vmiMessage.h"
#include "vmi/vmiRt.h"



void Nios_IIUpdateFPControlWord(Nios_IIP Nios_II) {

    // TODO: When exception support is added for Nios_IIe2r the
    //       masks need to be set here when fp exceptions are enabled
    vmiFPControlWord cwNoExceptions = {
            .IM  = 0,
            .DM  = 0,
            .ZM  = 1,
            .OM  = 1,
            .UM  = 1,
            .PM  = 1,
            .RC  = vmi_FPR_NEAREST,
            .FZ  = 0,
            .DAZ = 0
    };
//    vmiFPControlWord cwNoExceptions = {
//            .IM  = 1,
//            .DM  = 0,
//            .ZM  = 1,
//            .OM  = 1,
//            .UM  = 1,
//            .PM  = 1,
//            .RC  = vmi_FPR_NEAREST,
//            .FZ  = 0,
//            .DAZ = 0
//    };


    vmirtSetFPControlWord((vmiProcessorP)Nios_II, cwNoExceptions);
}


void Nios_IIConfigureFPU(Nios_IIP Nios_II) {

    vmiPrintf("Configure FPU*************************\n");

    const static vmiFPConfig config = {
        .QNaN32          = 0xFFC00000
    };

    // set up QNaN values and handlers
    vmirtConfigureFPU(
        (vmiProcessorP)Nios_II,
        &config
    );
    Nios_IIUpdateFPControlWord(Nios_II);

}
