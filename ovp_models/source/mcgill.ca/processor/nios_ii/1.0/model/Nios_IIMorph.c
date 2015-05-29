/*
 * Copyright (c) 2005-2015 Imperas Software Ltd., www.imperas.com
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

#include "vmi/vmiMessage.h"
#include "vmi/vmiMt.h"
#include "vmi/vmiRt.h"
#include "Nios_IIMorph.h"
#include "Nios_IIDecode.h"
#include "Nios_IIExceptions.h"
#include "Nios_IIFunctions.h"
#include "Nios_IIInstructions.h"
#include "Nios_IIStructure.h"
#include "Nios_IIVariant.h"

//
// Write Morpher Table
//
const static Nios_IIMorphAttr dispatchTable[NIOS_II_IT_LAST+1] = {
    //
    // Format C1 Insns
    //
    [NIOS_II_IT_CUSTOM_C1] = {arch:ISA_0, morph:morphCUSTOM_C1},

    //
    // Format I0 Insns
    //
    [NIOS_II_IT_BRET_I0] = {arch:ISA_0, morph:morphBRET_I0},
    [NIOS_II_IT_ERET_I0] = {arch:ISA_0, morph:morphERET_I0},
    [NIOS_II_IT_FLUSHP_I0] = {arch:ISA_0, morph:morphFLUSHP_I0},
    [NIOS_II_IT_NOP_I0] = {arch:ISA_0, morph:morphNOP_I0},
    [NIOS_II_IT_RET_I0] = {arch:ISA_0, morph:morphRET_I0},
    [NIOS_II_IT_SYNC_I0] = {arch:ISA_0, morph:morphSYNC_I0},

    //
    // Format I1S Insns
    //
    [NIOS_II_IT_ADDI_I1S] = {arch:ISA_0, morph:morphADDI_I1S},
    [NIOS_II_IT_CMPEQI_I1S] = {arch:ISA_0, morph:morphCMPEQI_I1S},
    [NIOS_II_IT_CMPGEI_I1S] = {arch:ISA_0, morph:morphCMPGEI_I1S},
    [NIOS_II_IT_CMPLTI_I1S] = {arch:ISA_0, morph:morphCMPLTI_I1S},
    [NIOS_II_IT_CMPNEI_I1S] = {arch:ISA_0, morph:morphCMPNEI_I1S},
    [NIOS_II_IT_MULI_I1S] = {arch:ISA_0, morph:morphMULI_I1S},
    [NIOS_II_IT_RDPRS_I1S] = {arch:ISA_0, morph:morphRDPRS_I1S},

    //
    // Format I1U Insns
    //
    [NIOS_II_IT_ANDI_I1U] = {arch:ISA_0, morph:morphANDI_I1U},
    [NIOS_II_IT_CMPGEUI_I1U] = {arch:ISA_0, morph:morphCMPGEUI_I1U},
    [NIOS_II_IT_CMPLTUI_I1U] = {arch:ISA_0, morph:morphCMPLTUI_I1U},
    [NIOS_II_IT_ORI_I1U] = {arch:ISA_0, morph:morphORI_I1U},
    [NIOS_II_IT_XORI_I1U] = {arch:ISA_0, morph:morphXORI_I1U},

    //
    // Format I2S Insns
    //
    [NIOS_II_IT_MOVI_I2S] = {arch:ISA_0, morph:morphMOVI_I2S},

    //
    // Format I2U Insns
    //
    [NIOS_II_IT_ANDHI_I2U] = {arch:ISA_0, morph:morphANDHI_I2U},
    [NIOS_II_IT_ORHI_I2U] = {arch:ISA_0, morph:morphORHI_I2U},
    [NIOS_II_IT_XORHI_I2U] = {arch:ISA_0, morph:morphXORHI_I2U},

    //
    // Format I3S Insns
    //
    [NIOS_II_IT_FLUSHD_I3S] = {arch:ISA_0, morph:morphFLUSHD_I3S},
    [NIOS_II_IT_FLUSHDA_I3S] = {arch:ISA_0, morph:morphFLUSHDA_I3S},
    [NIOS_II_IT_INITD_I3S] = {arch:ISA_0, morph:morphINITD_I3S},
    [NIOS_II_IT_INITDA_I3S] = {arch:ISA_0, morph:morphINITDA_I3S},

    //
    // Format I3U Insns
    //
    [NIOS_II_IT_MOVHI_I3U] = {arch:ISA_0, morph:morphMOVHI_I3U},

    //
    // Format I4S Insns
    //
    [NIOS_II_IT_LDB_I4S] = {arch:ISA_0, morph:morphLDB_I4S},
    [NIOS_II_IT_LDBIO_I4S] = {arch:ISA_0, morph:morphLDBIO_I4S},
    [NIOS_II_IT_LDBU_I4S] = {arch:ISA_0, morph:morphLDBU_I4S},
    [NIOS_II_IT_LDBUIO_I4S] = {arch:ISA_0, morph:morphLDBUIO_I4S},
    [NIOS_II_IT_LDH_I4S] = {arch:ISA_0, morph:morphLDH_I4S},
    [NIOS_II_IT_LDHIO_I4S] = {arch:ISA_0, morph:morphLDHIO_I4S},
    [NIOS_II_IT_LDHU_I4S] = {arch:ISA_0, morph:morphLDHU_I4S},
    [NIOS_II_IT_LDHUIO_I4S] = {arch:ISA_0, morph:morphLDHUIO_I4S},
    [NIOS_II_IT_LDW_I4S] = {arch:ISA_0, morph:morphLDW_I4S},
    [NIOS_II_IT_LDWIO_I4S] = {arch:ISA_0, morph:morphLDWIO_I4S},
    [NIOS_II_IT_STB_I4S] = {arch:ISA_0, morph:morphSTB_I4S},
    [NIOS_II_IT_STBIO_I4S] = {arch:ISA_0, morph:morphSTBIO_I4S},
    [NIOS_II_IT_STH_I4S] = {arch:ISA_0, morph:morphSTH_I4S},
    [NIOS_II_IT_STHIO_I4S] = {arch:ISA_0, morph:morphSTHIO_I4S},
    [NIOS_II_IT_STW_I4S] = {arch:ISA_0, morph:morphSTW_I4S},
    [NIOS_II_IT_STWIO_I4S] = {arch:ISA_0, morph:morphSTWIO_I4S},

    //
    // Format I4U Insns
    //
    [NIOS_II_IT_MOVUI_I4U] = {arch:ISA_0, morph:morphMOVUI_I4U},

    //
    // Format IB1 Insns
    //
    [NIOS_II_IT_BEQ_IB1] = {arch:ISA_0, morph:morphBEQ_IB1},
    [NIOS_II_IT_BGE_IB1] = {arch:ISA_0, morph:morphBGE_IB1},
    [NIOS_II_IT_BGEU_IB1] = {arch:ISA_0, morph:morphBGEU_IB1},
    [NIOS_II_IT_BLT_IB1] = {arch:ISA_0, morph:morphBLT_IB1},
    [NIOS_II_IT_BLTU_IB1] = {arch:ISA_0, morph:morphBLTU_IB1},
    [NIOS_II_IT_BNE_IB1] = {arch:ISA_0, morph:morphBNE_IB1},

    //
    // Format IB2 Insns
    //
    [NIOS_II_IT_BR_IB2] = {arch:ISA_0, morph:morphBR_IB2},

    //
    // Format J1 Insns
    //
    [NIOS_II_IT_CALL_J1] = {arch:ISA_0, morph:morphCALL_J1},
    [NIOS_II_IT_JMPI_J1] = {arch:ISA_0, morph:morphJMPI_J1},

    //
    // Format R1 Insns
    //
    [NIOS_II_IT_ADD_R1] = {arch:ISA_0, morph:morphADD_R1},
    [NIOS_II_IT_AND_R1] = {arch:ISA_0, morph:morphAND_R1},
    [NIOS_II_IT_CMPEQ_R1] = {arch:ISA_0, morph:morphCMPEQ_R1},
    [NIOS_II_IT_CMPGE_R1] = {arch:ISA_0, morph:morphCMPGE_R1},
    [NIOS_II_IT_CMPGEU_R1] = {arch:ISA_0, morph:morphCMPGEU_R1},
    [NIOS_II_IT_CMPLT_R1] = {arch:ISA_0, morph:morphCMPLT_R1},
    [NIOS_II_IT_CMPLTU_R1] = {arch:ISA_0, morph:morphCMPLTU_R1},
    [NIOS_II_IT_CMPNE_R1] = {arch:ISA_0, morph:morphCMPNE_R1},
    [NIOS_II_IT_DIV_R1] = {arch:ISA_0, morph:morphDIV_R1},
    [NIOS_II_IT_DIVU_R1] = {arch:ISA_0, morph:morphDIVU_R1},
    [NIOS_II_IT_MUL_R1] = {arch:ISA_0, morph:morphMUL_R1},
    [NIOS_II_IT_MULXSS_R1] = {arch:ISA_0, morph:morphMULXSS_R1},
    [NIOS_II_IT_MULXSU_R1] = {arch:ISA_0, morph:morphMULXSU_R1},
    [NIOS_II_IT_MULXUU_R1] = {arch:ISA_0, morph:morphMULXUU_R1},
    [NIOS_II_IT_NOR_R1] = {arch:ISA_0, morph:morphNOR_R1},
    [NIOS_II_IT_OR_R1] = {arch:ISA_0, morph:morphOR_R1},
    [NIOS_II_IT_ROL_R1] = {arch:ISA_0, morph:morphROL_R1},
    [NIOS_II_IT_ROR_R1] = {arch:ISA_0, morph:morphROR_R1},
    [NIOS_II_IT_SLL_R1] = {arch:ISA_0, morph:morphSLL_R1},
    [NIOS_II_IT_SRA_R1] = {arch:ISA_0, morph:morphSRA_R1},
    [NIOS_II_IT_SRL_R1] = {arch:ISA_0, morph:morphSRL_R1},
    [NIOS_II_IT_SUB_R1] = {arch:ISA_0, morph:morphSUB_R1},
    [NIOS_II_IT_XOR_R1] = {arch:ISA_0, morph:morphXOR_R1},

    //
    // Format R2 Insns
    //
    [NIOS_II_IT_MOV_R2] = {arch:ISA_0, morph:morphMOV_R2},
    [NIOS_II_IT_WRPRS_R2] = {arch:ISA_0, morph:morphWRPRS_R2},

    //
    // Format R2U Insns
    //
    [NIOS_II_IT_ROLI_R2U] = {arch:ISA_0, morph:morphROLI_R2U},
    [NIOS_II_IT_SLLI_R2U] = {arch:ISA_0, morph:morphSLLI_R2U},
    [NIOS_II_IT_SRAI_R2U] = {arch:ISA_0, morph:morphSRAI_R2U},
    [NIOS_II_IT_SRLI_R2U] = {arch:ISA_0, morph:morphSRLI_R2U},

    //
    // Format R3 Insns
    //
    [NIOS_II_IT_NEXTPC_R3] = {arch:ISA_0, morph:morphNEXTPC_R3},

    //
    // Format R4 Insns
    //
    [NIOS_II_IT_CALLR_R4] = {arch:ISA_0, morph:morphCALLR_R4},
    [NIOS_II_IT_FLUSHI_R4] = {arch:ISA_0, morph:morphFLUSHI_R4},
    [NIOS_II_IT_INITI_R4] = {arch:ISA_0, morph:morphINITI_R4},
    [NIOS_II_IT_JMP_R4] = {arch:ISA_0, morph:morphJMP_R4},

    //
    // Format R5 Insns
    //
    [NIOS_II_IT_RDCTL_R5] = {arch:ISA_0, morph:morphRDCTL_R5},

    //
    // Format R6 Insns
    //
    [NIOS_II_IT_WRCTL_R6] = {arch:ISA_0, morph:morphWRCTL_R6},

    //
    // Format RU Insns
    //
    [NIOS_II_IT_BREAK_RU] = {arch:ISA_0, morph:morphBREAK_RU},
    [NIOS_II_IT_TRAP_RU] = {arch:ISA_0, morph:morphTRAP_RU},

};

//
// Default undecoded/badvariant/unimplemented instruction behavior
//
static void defaultMorphCallback(
    vmiProcessorP processor,
    Nios_IIAddr thisPC,
    Uns64 instruction,
    char *message
) {
    vmiPrintf(
        "CPU '%s' 0x%08x:" FMT_640Nx " *** %s instruction: exiting ***\n",
        vmirtProcessorName(processor),
        thisPC,
        instruction,
        message
    );
    vmirtExit(processor);
}


//
// Default morphed code call
//
static NIOS_II_MORPH_FN(emitUndefined) {
    vmimtArgProcessor();
    vmimtArgUns32(EXC_ILLEGAL_INSTRUCTION);
    vmimtCall((vmiCallFn)Nios_IIExceptionCB);
}
static NIOS_II_MORPH_FN(emitNotVariant) {
    vmimtArgProcessor();
    vmimtArgUns32(state->info.thisPC);
    vmimtArgUns64(state->info.instruction);
    vmimtArgNatAddress("badvariant");
    vmimtCall((vmiCallFn)defaultMorphCallback);
    vmimtEndBlock();
}
static NIOS_II_MORPH_FN(emitAbsentFeature) {
    vmimtArgProcessor();
    vmimtArgUns32(EXC_UNIMPLEMENTED_INSTRUCTION);
    vmimtCall((vmiCallFn)Nios_IIExceptionCB);
}
static NIOS_II_MORPH_FN(emitUnimplemented) {
    vmimtArgProcessor();
    vmimtArgUns32(state->info.thisPC);
    vmimtArgUns64(state->info.instruction);
    vmimtArgNatAddress("unimplemented");
    vmimtCall((vmiCallFn)defaultMorphCallback);
    vmimtEndBlock();
}

static Bool disableMorph(Nios_IIMorphStateP state) {
    Nios_IIP Nios_II = state->Nios_II;
    return NIOS_II_DISASSEMBLE(Nios_II);
}

//
// Call to implemented morph code instruction generator
//
static NIOS_II_MORPH_FN(emitImplemented) {
    state->attrs->morph(state);
}

//
// Return a boolean indicating whether the processor supports the required
// architecture
//
static Bool supportedOnVariant(Nios_IIP Nios_II, Nios_IIMorphStateP state) {

    Nios_IIArchitecture configVariant = Nios_II->configInfo.arch;
    Nios_IIArchitecture requiredVariant = state->attrs->arch;

    if (configVariant & requiredVariant) {
        return True;
    } else {
        return False;
    }
}

//
// Determine if this feature is enabled in the core
//
typedef enum Nios_IIFeatureE {
    FR_NONE = 0,                // no special feature requirement
    FR_HW_MULTIPLY,             // hardware multiply
    FR_HW_MULX,                 // hardware multiply Extended
    FR_HW_DIVIDE,               // hardware divider
    FR_HW_RWPRS,                // Read/Write PRS
} Nios_IIFeature;

#define FEATURE_REQUIRE(_NAME, _FR) [NIOS_II_IT_##_NAME] = _FR
const static Nios_IIFeature featureRequired[NIOS_II_IT_LAST+1] = {

        // Hardware Multiply
        FEATURE_REQUIRE(MUL_R1,    FR_HW_MULTIPLY),
        FEATURE_REQUIRE(MULI_I1S,  FR_HW_MULTIPLY),
        FEATURE_REQUIRE(MULXSS_R1, FR_HW_MULX),
        FEATURE_REQUIRE(MULXSU_R1, FR_HW_MULX),
        FEATURE_REQUIRE(MULXUU_R1, FR_HW_MULX),

        // Hardware Divide
        FEATURE_REQUIRE(DIV_R1,    FR_HW_DIVIDE),
        FEATURE_REQUIRE(DIVU_R1,   FR_HW_DIVIDE),

        // Read/Write PRS
        FEATURE_REQUIRE(WRPRS_R2,  FR_HW_RWPRS),
        FEATURE_REQUIRE(RDPRS_I1S, FR_HW_RWPRS),

};

static Bool absentFeature(Nios_IIP Nios_II, Nios_IIMorphStateP state) {
    Nios_IIFeature fr = featureRequired[state->info.type];
    Bool     isAbsent = False;

    switch (fr) {
        case FR_NONE:
            break;

        case FR_HW_MULTIPLY:
            //
            // Do we need to test Nios_II->params.HW_MULTIPLY ?
            //
            switch (Nios_II->configInfo.arch) {
                case NIOS_II_E:
                    isAbsent = True;
                    break;

                case NIOS_II_S:
                case NIOS_II_F:
                    isAbsent = (Nios_II->params.HW_MULTIPLY == 0);
                    break;

                default:
                    isAbsent = True;
                    break;
            }
            break;

        case FR_HW_MULX:
            //
            // Do we need to test Nios_II->params.HW_MULTIPLY ?
            //
            switch (Nios_II->configInfo.arch) {
                case NIOS_II_E:
                    isAbsent = True;
                    break;

                case NIOS_II_S:
                case NIOS_II_F:
                    isAbsent = (Nios_II->params.HW_MULX == 0);
                    break;

                default:
                    isAbsent = True;
                    break;
            }
            break;

        case FR_HW_DIVIDE:
            switch (Nios_II->configInfo.arch) {
                case NIOS_II_E:
                    isAbsent = True;
                    break;

                case NIOS_II_S:
                case NIOS_II_F:
                    isAbsent = (Nios_II->params.HW_DIVIDE == 0);
                    break;

                default:
                    isAbsent = True;
                    break;
            }
            break;

        case FR_HW_RWPRS:
            switch (Nios_II->configInfo.arch) {
                case NIOS_II_E:
                    isAbsent = True;
                    break;

                case NIOS_II_S:
                case NIOS_II_F:
                    isAbsent = (Nios_II->params.NUMBER_SHADOW_REGISTER_SETS == 0);
                    break;

                default:
                    isAbsent = True;
                    break;
            }
            break;

        default:
            break;
    }

    return isAbsent;
}

//
// Instruction Morpher
//
VMI_MORPH_FN(Nios_IIMorph) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_IIMorphState state;

    // get instruction and instruction type
    Nios_IIDecode(Nios_II, thisPC, &state.info);

    state.attrs = &dispatchTable[state.info.type];
    state.Nios_II = Nios_II;

    if(disableMorph(&state)) {
        // no action if in disassembly mode

    } else if(state.info.type==NIOS_II_IT_LAST) {
        // take UndefinedInstruction exception
        emitUndefined(&state);

    } else if(!supportedOnVariant(Nios_II, &state)) {
        // instruction not supported on this variant
        emitNotVariant(&state);

    } else if(absentFeature(Nios_II, &state)) {
        // instruction not supported on this configuration
        emitAbsentFeature(&state);

    } else if(state.attrs->morph) {
        // translate the instruction
        emitImplemented(&state);

    } else {
        // here if no morph callback specified
        emitUnimplemented(&state);
    }
}

VMI_FETCH_SNAP_FN(Nios_IIFetchSnap) {
    return thisPC & ~3;
}

VMI_RD_WR_SNAP_FN(Nios_IIRdSnap) {
//    return bytes;
    return 0;
}

VMI_RD_WR_SNAP_FN(Nios_IIWrSnap) {
//    return bytes;
    return 0;
}
