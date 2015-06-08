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
#include "Nios_IIExceptions.h"
#include "Nios_IIFunctions.h"
#include "Nios_IIInstructions.h"
#include "Nios_IIMorph.h"
#include "Nios_IITLB.h"
#include "Nios_IIMPU.h"
#include <stdlib.h>
//
// Write Flag Const structures
//

static void unimplemented(const char *name) {
    vmiMessage("F", "MORPH_UNIMP", "Instruction %s unimplemented", name);
}

static void morphUnimplemented(const char *name) {
    vmimtArgNatAddress(name);
    vmimtCall((vmiCallFn)unimplemented);
    vmimtExit();
}

//static void ignored(const char *name) {
//    vmiMessage("W", "MORPH_IGNORE", "Instruction %s ignored", name);
//}
//
//static void morphIgnored(const char *name) {
//    vmimtArgNatAddress(name);
//    vmimtCall((vmiCallFn)ignored);
//}

static void validateSupervisorMode(Nios_IIP Nios_II) {
    //
    // Check status.U bit
    //
    if (Nios_II->status.bits.U) {
        vmimtArgProcessor();
        vmimtArgUns32(EXC_SUPERVISOR_ONLY_INSTRUCTION);
        vmimtCall((vmiCallFn)Nios_IIExceptionCB);
    }
}

static void niosAddressAlignCheckReg(Nios_IIMorphStateP state, vmiReg targetReg, Uns8 align) {

    Nios_IIP Nios_II = state->Nios_II;

    // NIOS_II_TEMP_RD(0)

    //
    // Are misaligned address checks enabled
    //
    Uns8 idx = Nios_IIGetExceptionIndex(EXC_MISALIGNED_DESTINATION_ADDRESS);
    if (Nios_II->exceptionTable[idx].Available == EXC_MISALIGNED_DESTINATION_ADDRESS) {
        //
        // Exceptions are enabled for EXC_MISALIGNED_DATA_ADDRESS
        //

        vmiLabelP ok = vmimtNewLabel();

        //
        // Register Target PC Test for 4/2 byte align
        //
        Uns8 alignMask = align - 1;
        vmimtTestRCJumpLabel(32, vmi_COND_Z, targetReg, alignMask, ok);

        // call exception handler
        vmimtArgProcessor();
        vmimtArgUns32(EXC_MISALIGNED_DESTINATION_ADDRESS);
        vmimtArgReg(NIOS_II_GPR_BITS, targetReg);
        vmimtCall((vmiCallFn)Nios_IIMemoryExceptCommon);

        //
        // Alignment OK
        //
        vmimtInsertLabel(ok);
    }
}

static void niosAddressAlignCheckImmRelative(Nios_IIMorphStateP state, Int32 offset, Uns8 align) {
    Nios_IIP Nios_II = state->Nios_II;

    // NIOS_II_TEMP_RD(0)

    //
    // Are misaligned address checks enabled
    //
    Uns8 idx = Nios_IIGetExceptionIndex(EXC_MISALIGNED_DESTINATION_ADDRESS);
    if (Nios_II->exceptionTable[idx].Available == EXC_MISALIGNED_DESTINATION_ADDRESS) {
        //
        // Exceptions are enabled for EXC_MISALIGNED_DATA_ADDRESS
        //

        vmiLabelP ok = vmimtNewLabel();

        //
        // Immediate Target PC Test for 2/4 byte align
        //
        Uns8 alignMask = align - 1;
        if ((offset & alignMask) == 0) {
            vmimtUncondJumpLabel(ok);
        }

        vmimtMoveRSimPC(NIOS_II_GPR_BITS, NIOS_II_TEMP_WR(0));
        vmimtBinopRC(NIOS_II_GPR_BITS, vmi_ADD, NIOS_II_TEMP_WR(0), offset, 0);

        // call exception handler
        vmimtArgProcessor();
        vmimtArgUns32(EXC_MISALIGNED_DESTINATION_ADDRESS);
        vmimtArgReg(NIOS_II_GPR_BITS, NIOS_II_TEMP_RD(0));
        vmimtCall((vmiCallFn)Nios_IIMemoryExceptCommon);

        //
        // Alignment OK
        //
        vmimtInsertLabel(ok);
    }
}

static void niosAddressAlignCheckImmAbsolute(Nios_IIMorphStateP state, Uns32 targetPC, Uns8 align) {
    Nios_IIP Nios_II = state->Nios_II;

    // NIOS_II_TEMP_RD(0)

    //
    // Are misaligned address checks enabled
    //
    Uns8 idx = Nios_IIGetExceptionIndex(EXC_MISALIGNED_DESTINATION_ADDRESS);
    if (Nios_II->exceptionTable[idx].Available == EXC_MISALIGNED_DESTINATION_ADDRESS) {
        //
        // Exceptions are enabled for EXC_MISALIGNED_DATA_ADDRESS
        //

        vmiLabelP ok = vmimtNewLabel();

        //
        // Immediate Target PC Test for 2/4 byte align
        //
        Uns8 alignMask = align - 1;
        if ((targetPC & alignMask) == 0) {
            vmimtUncondJumpLabel(ok);
        }

        // call exception handler
        vmimtArgProcessor();
        vmimtArgUns32(EXC_MISALIGNED_DESTINATION_ADDRESS);
        vmimtArgUns32(targetPC);
        vmimtCall((vmiCallFn)Nios_IIMemoryExceptCommon);

        //
        // Alignment OK
        //
        vmimtInsertLabel(ok);
    }
}

//
// Write status register from GPR[r]
//
static void vmic_WriteStatus(Nios_IIP Nios_II, Uns32 value, Uns32 wmask) {
    TYPE_status status = Nios_IIReadStatus(Nios_II);
    status.reg &= ~wmask;
    status.reg |= (value & wmask);

    Nios_IIWriteStatus(Nios_II, status);
}

//
// Write config register from GPR[r]
//
static void vmic_WriteConfig(Nios_IIP Nios_II, Uns32 value, Uns32 wmask) {
    TYPE_config config = Nios_IIReadConfig(Nios_II);
    config.reg &= ~wmask;
    config.reg |= (value & wmask);

    Nios_IIWriteConfig(Nios_II, config);
}

static void branchOnCondRR(vmiCondition cond, Nios_IIMorphStateP state) {
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Int16 SIMM16 = state->info.SIMM16;

    Int32 offset  = SIMM16 + 4;
    Addr targetPC = offset + state->info.thisPC;

    vmimtCompareRR(NIOS_II_GPR_BITS, cond, gpr_A, gpr_B, NIOS_II_TEMP_WR(0));

    //
    // Check for potential exception on target address
    //
    niosAddressAlignCheckImmRelative(state, offset, 4);
    vmimtCondJump(NIOS_II_TEMP_RD(0), 1, 0, targetPC, VMI_NOREG, vmi_JH_RELATIVE);
}

static void compareCondRR(vmiCondition cond, Nios_IIMorphStateP state) {
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtCompareRR(NIOS_II_GPR_BITS, cond, gpr_A, gpr_B, NIOS_II_TEMP_WR(0));
    vmimtMoveExtendRR(NIOS_II_GPR_BITS, gpr_C, 8, NIOS_II_TEMP_RD(0), False);
}

static void compareCondRI(vmiCondition cond, Nios_IIMorphStateP state, Bool isSign) {
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_WR(B);
    Int32 imm32;
    if (isSign) {
        imm32 = state->info.SIMM16;
    } else {
        imm32 = state->info.UIMM16;
    }

    vmimtCompareRC(NIOS_II_GPR_BITS, cond, gpr_A, imm32, NIOS_II_TEMP_WR(0));
    vmimtMoveExtendRR(NIOS_II_GPR_BITS, gpr_B, 8, NIOS_II_TEMP_RD(0), False);
}

static void loadRegister(Nios_IIMorphStateP state, Uns32 size, Bool signExtend) {
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_WR(B);
    Int16 SIMM16 = state->info.SIMM16;

    memEndian dataEndian = Nios_IIGetEndian((vmiProcessorP)state->Nios_II, False);
    vmimtLoadRRO(NIOS_II_GPR_BITS, size, SIMM16, gpr_B, gpr_A, dataEndian, signExtend, True);
}

static void storeRegister(Nios_IIMorphStateP state, Uns32 size) {
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Int16 SIMM16 = state->info.SIMM16;

    memEndian dataEndian = Nios_IIGetEndian((vmiProcessorP)state->Nios_II, False);
    vmimtStoreRRO(size, SIMM16, gpr_A, gpr_B, dataEndian, True);
}

static void vmic_BankSwitch(Nios_IIP Nios_II, Uns32 old_status, Uns32 new_status) {
    TYPE_status status1 = {.reg = old_status};
    TYPE_status status2 = {.reg = new_status};

    //
    // Does the bank we are switching to exist ?
    //
    Uns32 BANKS = Nios_II->params.NUMBER_SHADOW_REGISTER_SETS;
    Uns32 PRS   = status1.bits.CRS;
    Uns32 CRS   = status2.bits.CRS;
    Bool  validSet = True;
    if (CRS > BANKS) {
        const char *name = vmirtProcessorName((vmiProcessorP) Nios_II);
        vmiMessage("W", "ILL_SHADOW_SET", "Attempting to Switch to Shadow Register Set %d, %s/NUMBER_SHADOW_REGISTER_SETS=%d",
                CRS, name, BANKS);
        validSet = False;
    }

    //
    // Switch Banks
    //
    Int32 i;
    for(i=0; i<32; i++) {
        //
        // Cache the previous set into the bank
        //
        Nios_II->shadow[PRS][i] = Nios_II->gpr[i];

        //
        // extract the current set from the bank
        //
        Nios_II->gpr[i] = validSet ? Nios_II->shadow[CRS][i] : 0xdeadbeef;
    }

}

static void niosReturnException (Nios_IIMorphStateP state, vmiReg returnStatus, Uns8 returnPC) {
    //
    // Check for potential exception on target address
    // returning to misaligned address
    //
    niosAddressAlignCheckReg(state, NIOS_II_GPR_RD(returnPC), 4);

    //
    // Cache the returnPC
    //
    vmimtMoveRR(NIOS_II_GPR_BITS, NIOS_II_TEMP_WR(0), NIOS_II_GPR_RD(returnPC));

    //
    // If status.CRS != restore.CRS, we need to do some housekeeping!
    // to patch up the register set switch
    //
    vmiLabelP noswitch = vmimtNewLabel();
    TYPE_status mask = {.reg = 0x00000000};
    mask.bits.CRS = ~0;

    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_AND, NIOS_II_TEMP_WR(1), NIOS_II_CPU_REG(status), mask.reg, 0);
    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_AND, NIOS_II_TEMP_WR(2), returnStatus,            mask.reg, 0);
    vmimtCompareRR(NIOS_II_GPR_BITS, vmi_COND_EQ, NIOS_II_TEMP_WR(1), NIOS_II_TEMP_WR(2), NIOS_II_TEMP_WR(3));
    vmimtCondJumpLabel(NIOS_II_TEMP_WR(3), True, noswitch);
    //
    // else there was a switch - swap register sets
    //
    vmimtArgProcessor();
    vmimtArgReg(NIOS_II_GPR_BITS, NIOS_II_CPU_REG(status));
    vmimtArgReg(NIOS_II_GPR_BITS, returnStatus);
    vmimtCall((vmiCallFn)vmic_BankSwitch);

    vmimtInsertLabel(noswitch);

    //
    // update the status register
    //
    vmimtArgProcessor();
    vmimtArgReg(32, returnStatus); // estatus || bstatus
    vmimtArgUns32(~0);             // Allwritable
    vmimtCall((vmiCallFn)vmic_WriteStatus);

    //
    // No exception so return from exception
    //
    vmimtUncondJumpReg(0, NIOS_II_TEMP_RD(0), VMI_NOREG, vmi_JH_RETURNINT);
}

//
// Write Default Morpher stub Functions
//
NIOS_II_MORPH_FN(morphCUSTOM_C1) {
  Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);
    Uns8 N = state->info.N;


    vmiReg* flag = calloc(1,sizeof(vmiReg));
    

    switch(N){
    case 225: // fnegs
        vmimtFBinopRRR(vmi_FT_32_IEEE_754, vmi_FNEG, gpr_C, gpr_B, gpr_A, *flag, 0);
        break;
    case 230: // fcmples
        vmimtFBinopRRR(vmi_FT_32_IEEE_754, vmi_FQCMPLE, gpr_C, gpr_B, gpr_A, *flag, 0);
        break;
    case 252: // fmuls
        vmimtFBinopRRR(vmi_FT_32_IEEE_754, vmi_FMUL, gpr_C, gpr_B, gpr_A, *flag, 0);
        break;
    case 253: // fadds    
        vmimtFBinopRRR(vmi_FT_32_IEEE_754, vmi_FADD, gpr_C, gpr_B, gpr_A, *flag, 0);
        break;
    case 254: // fsubs
        vmimtFBinopRRR(vmi_FT_32_IEEE_754, vmi_FSUB, gpr_C, gpr_B, gpr_A, *flag, 0);
        break;
    case 255: // fdvis
        vmimtFBinopRRR(vmi_FT_32_IEEE_754, vmi_FDIV, gpr_C, gpr_B, gpr_A, *flag, 0);
        break;
    default: //etc
        vmiPrintf("Custom instruction error: N = %d not found",N);
        morphUnimplemented("CUSTOM_C1");
        break;
    } 

    free(flag);
}

NIOS_II_MORPH_FN(morphBRET_I0) {
    validateSupervisorMode(state->Nios_II);

    niosReturnException(state, NIOS_II_CPU_REG(bstatus), 30);
}

NIOS_II_MORPH_FN(morphERET_I0) {
    validateSupervisorMode(state->Nios_II);

    niosReturnException(state, NIOS_II_CPU_REG(estatus), 29);
}

NIOS_II_MORPH_FN(morphFLUSHP_I0) {
    //
    // Supported Architecture=ISA_0
    //
    // morphUnimplemented("FLUSHP_I0");

    // NOP INSTRUCTION
}

NIOS_II_MORPH_FN(morphNOP_I0) {
}

NIOS_II_MORPH_FN(morphRET_I0) {
    //
    // Check for potential exception on target address
    //
    niosAddressAlignCheckReg(state, NIOS_II_GPR_WR(31), 4);
    vmimtUncondJumpReg(0, NIOS_II_GPR_WR(31), VMI_NOREG, vmi_JH_RETURN);
}

NIOS_II_MORPH_FN(morphSYNC_I0) {
//    morphIgnored("SYNC_I0");
}

NIOS_II_MORPH_FN(morphADDI_I1S) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_WR(B);
    Int16 SIMM16 = state->info.SIMM16;

    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_ADD, gpr_B, gpr_A, SIMM16, 0);
}

NIOS_II_MORPH_FN(morphCMPEQI_I1S) {
    compareCondRI(vmi_COND_EQ, state, True);
}

NIOS_II_MORPH_FN(morphCMPGEI_I1S) {
    compareCondRI(vmi_COND_NL, state, True);
}

NIOS_II_MORPH_FN(morphCMPLTI_I1S) {
    compareCondRI(vmi_COND_L, state, True);
}

NIOS_II_MORPH_FN(morphCMPNEI_I1S) {
    compareCondRI(vmi_COND_NE, state, True);
}

NIOS_II_MORPH_FN(morphMULI_I1S) {
    //
    // Supported Architecture=ISA_0
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_WR(B);
    Int16 SIMM16 = state->info.SIMM16;

    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_MUL, gpr_B, gpr_A, SIMM16, 0);
}

//
// GPR[B] = (BANK[PRS][A] + SIMM16)
//
static void vmic_RDPRS (Nios_IIP Nios_II, Uns32 rB, Uns32 rA, Int16 simm16) {
    Uns32 PRS = Nios_II->status.bits.PRS;
    Nios_II->gpr[rB] = Nios_II->shadow[PRS][rA];
}
NIOS_II_MORPH_FN(morphRDPRS_I1S) {
    validateSupervisorMode(state->Nios_II);

    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_WR(B);
    Int16 SIMM16 = state->info.SIMM16;

    //
    // Copy from Bank into GPR
    //
    vmimtArgProcessor();
    vmimtArgUns32(B);
    vmimtArgUns32(A);
    vmimtArgUns32(SIMM16);
    vmimtCall((vmiCallFn)vmic_RDPRS);

    if (SIMM16) {
        vmimtBinopRC(NIOS_II_GPR_BITS, vmi_ADD, gpr_B, SIMM16, 0);
    }
}

NIOS_II_MORPH_FN(morphANDI_I1U) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_WR(B);
    Uns32 UIMM16 = state->info.UIMM16;

    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_AND, gpr_B, gpr_A, UIMM16, 0);
}

NIOS_II_MORPH_FN(morphCMPGEUI_I1U) {
    compareCondRI(vmi_COND_NB, state, False);
}

static void vmic_ExitSimulatorStatus(Nios_IIP Nios_II, Uns32 imm32) {
    if (imm32==0xABC1) {
            //
            // ALT_SIM_FAIL
            // Finish with fail - 1
            //
            vmiPrintf("TEST_MODE_EXIT - ALT_SIM_FAIL\n");
//            vmirtFinish(1);
            vmirtHalt((vmiProcessorP)Nios_II);

    } else if (imm32==0xABC2) {
            //
            // ALT_SIM_PASS
            // Finish with success - 0
            //
            vmiPrintf("TEST_MODE_EXIT - ALT_SIM_PASS\n");
//            vmirtFinish(0);
            vmirtHalt((vmiProcessorP)Nios_II);

    } else if (imm32==0xABC1) {
        //
        // ALT_SIM_FAIL
        // Finish with fail - 1
        //
        vmiPrintf("TEST_MODE_EXIT - ALT_SIM_FAIL\n");
//        vmirtFinish(1);
        vmirtHalt((vmiProcessorP)Nios_II);

    } else if (imm32==0xABC3) {
        //
        // ALT_SIM_IN_TOP_OF_HOT_LOOP
        // Finish with fail - 1
        //
        vmiPrintf("TEST_MODE_EXIT - ALT_SIM_IN_TOP_OF_HOT_LOOP\n");
//        vmirtFinish(1);
        vmirtHalt((vmiProcessorP)Nios_II);

    } else {
        //
        // Finish with unknown - 2
        //
        vmiPrintf("TEST_MODE_EXIT -ALT_SIM_????\n");
//        vmirtFinish(2);
        vmirtHalt((vmiProcessorP)Nios_II);
    }
}

NIOS_II_MORPH_FN(morphCMPLTUI_I1U) {
    //
    // Detect Altera Simulation Mode Exit
    // PASS = cmpltui r0, r0, 0xabc2
    // FAIL = cmpltui r0, r0, 0xabc1
    //
    Uns8  A     = state->info.A;
    Uns8  B     = state->info.B;
    Uns32 imm32 = state->info.UIMM16;

    if (state->Nios_II->params.TEST_MODE_EXIT) {
        if (A==0 && B==0 && (imm32==0xABC2 || imm32==0xABC1)) {
            //
            // Report Test Pass/Fail - if enabled in parameter
            //
            vmimtArgProcessor();
            vmimtArgUns32(imm32);
            vmimtCall((vmiCallFn)vmic_ExitSimulatorStatus);
        }
    }

    compareCondRI(vmi_COND_B, state, False);
}

NIOS_II_MORPH_FN(morphORI_I1U) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_WR(B);
    Uns32 UIMM16 = state->info.UIMM16;

    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_OR, gpr_B, gpr_A, UIMM16, 0);
}

NIOS_II_MORPH_FN(morphXORI_I1U) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_WR(B);
    Uns32 UIMM16 = state->info.UIMM16;

    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_XOR, gpr_B, gpr_A, UIMM16, 0);
}

NIOS_II_MORPH_FN(morphMOVI_I2S) {
    //
    // Supported Architecture=ISA_0
    //
    state->info.A = 0;
    morphADDI_I1S(state);
}

NIOS_II_MORPH_FN(morphANDHI_I2U) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_WR(B);
    Uns32 UIMM16 = state->info.UIMM16;

    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_AND, gpr_B, gpr_A, UIMM16, 0);
}

NIOS_II_MORPH_FN(morphORHI_I2U) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_WR(B);
    Uns32 UIMM16 = state->info.UIMM16;

    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_OR, gpr_B, gpr_A, UIMM16, 0);
}

NIOS_II_MORPH_FN(morphXORHI_I2U) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_WR(B);
    Uns32 UIMM16 = state->info.UIMM16;

    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_XOR, gpr_B, gpr_A, UIMM16, 0);
}

NIOS_II_MORPH_FN(morphFLUSHD_I3S) {
    //
    // Supported Architecture=ISA_0
    //
    // Uns8 A = state->info.A;
    // vmiReg gpr_A = NIOS_II_GPR_RD(A);
    // vmiReg gpr_A = NIOS_II_GPR_WR(A);
    // Int16 SIMM16 = state->info.SIMM16;
    // morphUnimplemented("FLUSHD_I3S");

    // NOP INSTRUCTION
}

NIOS_II_MORPH_FN(morphFLUSHDA_I3S) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    // vmiReg gpr_A = NIOS_II_GPR_WR(A);
    Int16 SIMM16 = state->info.SIMM16;
    // morphUnimplemented("FLUSHDA_I3S");

    //
    // Need to check for exception in user mode
    // when calculated address is in Supervisor region
    //
//    vmimtTryStoreRC(32, SIMM16, gpr_A, False);
    vmimtTryLoadRC(32, SIMM16, gpr_A, False);
}

NIOS_II_MORPH_FN(morphINITD_I3S) {
    //
    // Supported Architecture=ISA_0
    //

    validateSupervisorMode(state->Nios_II);

    // Uns8 A = state->info.A;
    // vmiReg gpr_A = NIOS_II_GPR_RD(A);
    // vmiReg gpr_A = NIOS_II_GPR_WR(A);
    // Int16 SIMM16 = state->info.SIMM16;
    // morphUnimplemented("INITD_I3S");

    // NOP INSTRUCTION
}

NIOS_II_MORPH_FN(morphINITDA_I3S) {
    //
    // Supported Architecture=ISA_0
    //
    validateSupervisorMode(state->Nios_II);

    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    // vmiReg gpr_A = NIOS_II_GPR_WR(A);
    Int16 SIMM16 = state->info.SIMM16;

    vmimtTryLoadRC(32, SIMM16, gpr_A, False);

    // NOP INSTRUCTION
}

NIOS_II_MORPH_FN(morphMOVHI_I3U) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_WR(B);
    Uns32 UIMM16 = state->info.UIMM16;

    vmimtMoveRC(NIOS_II_GPR_BITS, gpr_B, UIMM16);
}

NIOS_II_MORPH_FN(morphLDB_I4S) {
    loadRegister(state, 8, True);
}

NIOS_II_MORPH_FN(morphLDBIO_I4S) {
    morphLDB_I4S(state);
}

NIOS_II_MORPH_FN(morphLDBU_I4S) {
    loadRegister(state, 8, False);
}

NIOS_II_MORPH_FN(morphLDBUIO_I4S) {
    morphLDBU_I4S(state);
}

NIOS_II_MORPH_FN(morphLDH_I4S) {
    loadRegister(state, 16, True);
}

NIOS_II_MORPH_FN(morphLDHIO_I4S) {
    morphLDH_I4S(state);
}

NIOS_II_MORPH_FN(morphLDHU_I4S) {
    loadRegister(state, 16, False);
}

NIOS_II_MORPH_FN(morphLDHUIO_I4S) {
    morphLDHU_I4S(state);
}

NIOS_II_MORPH_FN(morphLDW_I4S) {
    loadRegister(state, 32, True);
}

NIOS_II_MORPH_FN(morphLDWIO_I4S) {
    morphLDW_I4S(state);
}

NIOS_II_MORPH_FN(morphSTB_I4S) {
    storeRegister(state, 8);
}

NIOS_II_MORPH_FN(morphSTBIO_I4S) {
    morphSTB_I4S(state);
}

NIOS_II_MORPH_FN(morphSTH_I4S) {
    storeRegister(state, 16);
}

NIOS_II_MORPH_FN(morphSTHIO_I4S) {
    morphSTH_I4S(state);
}

NIOS_II_MORPH_FN(morphSTW_I4S) {
    storeRegister(state, 32);
}

NIOS_II_MORPH_FN(morphSTWIO_I4S) {
    morphSTW_I4S(state);
}

NIOS_II_MORPH_FN(morphMOVUI_I4U) {
    //
    // Supported Architecture=ISA_0
    //
    state->info.A = 0;
    morphORI_I1U(state);
}

NIOS_II_MORPH_FN(morphBEQ_IB1) {
    branchOnCondRR(vmi_COND_EQ, state);
}

NIOS_II_MORPH_FN(morphBGE_IB1) {
    branchOnCondRR(vmi_COND_NL, state);
}

NIOS_II_MORPH_FN(morphBGEU_IB1) {
    branchOnCondRR(vmi_COND_NB, state);
}

NIOS_II_MORPH_FN(morphBLT_IB1) {
    branchOnCondRR(vmi_COND_L, state);
}

NIOS_II_MORPH_FN(morphBLTU_IB1) {
    branchOnCondRR(vmi_COND_B, state);
}

NIOS_II_MORPH_FN(morphBNE_IB1) {
    branchOnCondRR(vmi_COND_NE, state);
}

NIOS_II_MORPH_FN(morphBR_IB2) {
    //
    // Supported Architecture=ISA_0
    //
    Int32 offset  = state->info.SIMM16 + 4;
    Addr targetPC = offset + state->info.thisPC;

    //
    // A branch to self is basically a halt, if we detect
    // this and the parameter HALT_EXIT is true, then exit
    //
    if (state->Nios_II->params.TEST_HALT_EXIT && offset==0) {
        vmimtExit();

    } else {

        //
        // Check for potential exception on target address
        //
        niosAddressAlignCheckImmRelative(state, offset, 4);
        vmimtUncondJump(0, targetPC, VMI_NOREG, vmi_JH_RELATIVE);
    }
}

NIOS_II_MORPH_FN(morphCALL_J1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns32 UIMM26 = state->info.UIMM26;
    Uns32 targetPC = (state->info.thisPC & 0xF0000000) | UIMM26;

    //
    // Check for potential exception on target address
    //
    niosAddressAlignCheckImmAbsolute(state, targetPC, 4);
    vmimtUncondJump(state->info.nextPC, targetPC, NIOS_II_GPR_WR(31), vmi_JH_CALL);
}

NIOS_II_MORPH_FN(morphJMPI_J1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns32 UIMM26  = state->info.UIMM26;
    Uns32 targetPC = (state->info.thisPC & 0xF0000000) | UIMM26;

    //
    // Check for potential exception on target address
    //
    niosAddressAlignCheckImmAbsolute(state, targetPC, 4);
    vmimtUncondJump(0, targetPC, VMI_NOREG, vmi_JH_NONE);
}

NIOS_II_MORPH_FN(morphADD_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_ADD, gpr_C, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphAND_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_AND, gpr_C, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphCMPEQ_R1) {
    compareCondRR(vmi_COND_EQ, state);
}

NIOS_II_MORPH_FN(morphCMPGE_R1) {
    compareCondRR(vmi_COND_NL, state);
}

NIOS_II_MORPH_FN(morphCMPGEU_R1) {
    compareCondRR(vmi_COND_NB, state);
}

NIOS_II_MORPH_FN(morphCMPLT_R1) {
    compareCondRR(vmi_COND_L, state);
}

NIOS_II_MORPH_FN(morphCMPLTU_R1) {
    compareCondRR(vmi_COND_B, state);
}

NIOS_II_MORPH_FN(morphCMPNE_R1) {
    compareCondRR(vmi_COND_NE, state);
}

NIOS_II_MORPH_FN(morphDIV_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_IDIV, gpr_C, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphDIVU_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_DIV, gpr_C, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphMUL_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_MUL, gpr_C, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphMULXSS_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtMulopRRR(NIOS_II_GPR_BITS, vmi_IMUL, gpr_C, VMI_NOREG, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphMULXSU_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    //
    // Need to extend to 64 bit source registers
    //
    vmimtMoveExtendRR(NIOS_II_GPR_BITS*2, NIOS_II_TEMP_RD(0), NIOS_II_GPR_BITS, gpr_A, True);
    vmimtMoveExtendRR(NIOS_II_GPR_BITS*2, NIOS_II_TEMP_RD(2), NIOS_II_GPR_BITS, gpr_B, False);

    vmimtBinopRRR(NIOS_II_GPR_BITS*2, vmi_IMUL, NIOS_II_TEMP_WR(4), NIOS_II_TEMP_RD(0), NIOS_II_TEMP_RD(2), 0);

    //
    // Now get the upper word in TEMP(4) - which is TEMP(5)
    //
    vmimtMoveRR(NIOS_II_GPR_BITS, gpr_C, NIOS_II_TEMP_RD(5));
}

NIOS_II_MORPH_FN(morphMULXUU_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtMulopRRR(NIOS_II_GPR_BITS, vmi_MUL, gpr_C, VMI_NOREG, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphNOR_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_NOR, gpr_C, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphOR_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_OR, gpr_C, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphROL_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtSetShiftMask(31);
    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_ROL, gpr_C, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphROR_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtSetShiftMask(31);
    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_ROR, gpr_C, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphSLL_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtSetShiftMask(31);
    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_SHL, gpr_C, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphSRA_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtSetShiftMask(31);
    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_SAR, gpr_C, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphSRL_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtSetShiftMask(31);
    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_SHR, gpr_C, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphSUB_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_SUB, gpr_C, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphXOR_R1) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 B = state->info.B;
    vmiReg gpr_B = NIOS_II_GPR_RD(B);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_XOR, gpr_C, gpr_A, gpr_B, 0);
}

NIOS_II_MORPH_FN(morphMOV_R2) {
    //
    // Supported Architecture=ISA_0
    //
    state->info.B = 0;
    morphADD_R1(state);
}

//
// BANK[PRS][C] = GPR[A]
//
static void vmic_WRPRS (Nios_IIP Nios_II, Uns32 rC, Uns32 rA) {
    Uns32 PRS = Nios_II->status.bits.PRS;
    Nios_II->shadow[PRS][rC] = Nios_II->gpr[rA];
}
NIOS_II_MORPH_FN(morphWRPRS_R2) {
    validateSupervisorMode(state->Nios_II);

    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    Uns8 C = state->info.C;

    //
    // Copy from GPR into Bank
    //
    vmimtArgProcessor();
    vmimtArgUns32(C);
    vmimtArgUns32(A);
    vmimtCall((vmiCallFn)vmic_WRPRS);
}

NIOS_II_MORPH_FN(morphROLI_R2U) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);
    Uns8 UIMM5 = state->info.UIMM5;

    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_ROL, gpr_C, gpr_A, UIMM5, 0);
}

NIOS_II_MORPH_FN(morphSLLI_R2U) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);
    Uns8 UIMM5 = state->info.UIMM5;

    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_SHL, gpr_C, gpr_A, UIMM5, 0);
}

NIOS_II_MORPH_FN(morphSRAI_R2U) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);
    Uns8 UIMM5 = state->info.UIMM5;

    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_SAR, gpr_C, gpr_A, UIMM5, 0);
}

NIOS_II_MORPH_FN(morphSRLI_R2U) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);
    Uns8 UIMM5 = state->info.UIMM5;

    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_SHR, gpr_C, gpr_A, UIMM5, 0);
}

NIOS_II_MORPH_FN(morphNEXTPC_R3) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);

    vmimtMoveRSimPC(NIOS_II_GPR_BITS, gpr_C);
    vmimtBinopRC(NIOS_II_GPR_BITS, vmi_ADD, gpr_C, 4, 0);
}

NIOS_II_MORPH_FN(morphCALLR_R4) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);

    //
    // Check for potential exception on target address
    //
    niosAddressAlignCheckReg(state, gpr_A, 4);
    vmimtUncondJumpReg(state->info.nextPC, gpr_A, NIOS_II_GPR_WR(31), vmi_JH_CALL);
}

NIOS_II_MORPH_FN(morphFLUSHI_R4) {
    //
    // Supported Architecture=ISA_0
    //
    // Uns8 A = state->info.A;
    // vmiReg gpr_A = NIOS_II_GPR_RD(A);
    // vmiReg gpr_A = NIOS_II_GPR_WR(A);
    // morphUnimplemented("FLUSHI_R4");

    // NOP INSTRUCTION
}

NIOS_II_MORPH_FN(morphINITI_R4) {
    //
    // Supported Architecture=ISA_0
    //

    validateSupervisorMode(state->Nios_II);

    // Uns8 A = state->info.A;
    // vmiReg gpr_A = NIOS_II_GPR_RD(A);
    // vmiReg gpr_A = NIOS_II_GPR_WR(A);
    // morphUnimplemented("INITI_R4");

    // NOP INSTRUCTION
}

NIOS_II_MORPH_FN(morphJMP_R4) {
    //
    // Supported Architecture=ISA_0
    //
    Uns8 A = state->info.A;
    vmiReg gpr_A = NIOS_II_GPR_RD(A);

    //
    // Check for potential exception on target address
    //
    niosAddressAlignCheckReg(state, gpr_A, 4);
    vmimtUncondJumpReg(0, gpr_A, VMI_NOREG, vmi_JH_NONE);
}

static void getCtlReg(Nios_IIP Nios_II, Uns8 index, Bool isWrite, vmiReg *reg, Uns32 *wmask, Uns32 *rmask) {
    switch (index) {
    case 0: // status
        *reg  = NIOS_II_CPU_REG(status);
        *wmask = Nios_II->status_wmask.reg;
        *rmask = Nios_II->status_rmask.reg;
        break;
    case 1: // estatus
        *reg  = NIOS_II_CPU_REG(estatus);
        *wmask = Nios_II->estatus_wmask.reg;
        *rmask = Nios_II->estatus_rmask.reg;
        break;
    case 2: // bstatus
        *reg  = NIOS_II_CPU_REG(bstatus);
        *wmask = Nios_II->bstatus_wmask.reg;
        *rmask = Nios_II->bstatus_rmask.reg;
        break;
    case 3: // ienable
        *reg  = NIOS_II_CPU_REG(ienable);
        *wmask = ~0;
        *rmask = ~0;
        break;
    case 4: // ipending - readonly
        *reg = NIOS_II_CPU_REG(ipending);
        *wmask = 0;
        *rmask = ~0;
        break;
    case 5: // cpuid - readonly
        *reg  = NIOS_II_CPU_REG(cpuid);
        *wmask = 0;
        *rmask = ~0;
        break;
    case 6: // reserved
        *reg  = VMI_NOREG;
        *wmask = 0;
        *rmask = ~0;
        break;
    case 7: // exception - readonly
        *reg  = NIOS_II_CPU_REG(except);
        *wmask = Nios_II->except_wmask.reg;
        *rmask = Nios_II->except_rmask.reg;
        break;
    case 8:
        *reg  = NIOS_II_CPU_REG(pteaddr);
        *wmask = Nios_II->pteaddr_wmask.reg;
        *rmask = Nios_II->pteaddr_rmask.reg;
        break;
    case 9:
        *reg  = NIOS_II_CPU_REG(tlbacc);
        *wmask = Nios_II->tlbacc_wmask.reg;
        *rmask = Nios_II->tlbacc_rmask.reg;
        break;
    case 10:
        *reg  = NIOS_II_CPU_REG(tlbmisc);
        *wmask = Nios_II->tlbmisc_wmask.reg;
        *rmask = Nios_II->tlbmisc_rmask.reg;
        break;
    case 12:
        *reg  = NIOS_II_CPU_REG(badaddr);
        *wmask = 0;
        *rmask = ~0;
        break;
    case 13:
        *reg  = NIOS_II_CPU_REG(config);
        *wmask = Nios_II->config_wmask.reg;
        *rmask = Nios_II->config_rmask.reg;
        break;
    case 14:
        *reg  = NIOS_II_CPU_REG(mpubase);
        *wmask = Nios_II->mpubase_wmask.reg;
        *rmask = Nios_II->mpubase_rmask.reg;
        break;
    case 15:
        *reg  = NIOS_II_CPU_REG(mpuacc);
        *wmask = Nios_II->mpuacc_wmask.reg;
        *rmask = Nios_II->mpuacc_rmask.reg;
        break;

    default:
        *reg  = VMI_NOREG;
        *wmask = 0;
        *rmask = 0;
        break;
    }

    return;
}

static void vmic_RDCTL(Nios_IIP Nios_II, Uns32 ctl, Uns32 r) {
    //    vmiMessage("F", "RDCTL", "ERROR write to CTL%d=GPR[%d]=0x%08x\n", ctl, r, Nios_II->gpr[r]);
}

NIOS_II_MORPH_FN(morphRDCTL_R5) {
    //
    // Supported Architecture=ISA_0
    //
    validateSupervisorMode(state->Nios_II);

    Uns8 C = state->info.C;
    vmiReg gpr_C = NIOS_II_GPR_WR(C);
    Uns8 N = state->info.N;

    //
    // Get the Control Register Read Mask and perform the write
    //
    Uns32 wmask, rmask;
    vmiReg ctl;
    getCtlReg(state->Nios_II, N, False, &ctl, &wmask, &rmask);

    //
    // Do we need to perform any side effect action ?
    //
    switch (N) {
    case 0:  // status
    case 1:  // estatus
    case 2:  // bstatus
    case 3:  // ienable
    case 4:  // ipending - readonly
    case 5:  // cpuid - readonly
    case 6:  // reserved
    case 7:  // exception
    case 12: // badaddr - readonly
        vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_AND, gpr_C, ctl, rmask, 0);
        break;

        //
        // MMU Access Registers
        //
    case 8:  // pteaddr
    case 9:  // tlbacc
    case 10: // tlbmisc
        vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_AND, gpr_C, ctl, rmask, 0);
        break;

        //
        // MPU Access Registers
        //
    case 13: // config
    case 14: // mpubase
    case 15: // mpuacc
        vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_AND, gpr_C, ctl, rmask, 0);
        break;

    default:
        vmimtArgProcessor();
        vmimtArgUns32(N);
        vmimtArgUns32(C);
        vmimtCall((vmiCallFn)vmic_RDCTL);
        break;
    }
}

static void vmic_readTLB (Nios_IIP Nios_II) {
    Uns32 VA  = Nios_II->pteaddr.bits.VPN << NIOS_II_VPN_SHIFT;
    Uns32 way = Nios_II->tlbmisc.bits.WAY;

    TLBentryP tlbe = Nios_IIReadTLB(Nios_II, way, VA);

    /*
     * only the Tag field is updated, not the line
     */
    Uns32 line = Nios_II->pteaddr.bits.VPN & ((Nios_II->tlb.rows)-1);
    Nios_II->pteaddr.bits.VPN = (tlbe->VA >> NIOS_II_VPN_SHIFT) | line;

    //vmiPrintf("vmic_readTLB VA=0x%08x Nios_II->pteaddr.bits.VPN=%08x tlbe->VA=%08x\n",
    //        VA,
    //        Nios_II->pteaddr.bits.VPN, tlbe->VA);

    Nios_II->tlbmisc.bits.PID = tlbe->PID;

    Nios_II->tlbacc.bits.C   = tlbe->C;
    Nios_II->tlbacc.bits.R   = (tlbe->priv & MEM_PRIV_R) && True;
    Nios_II->tlbacc.bits.W   = (tlbe->priv & MEM_PRIV_W) && True;
    Nios_II->tlbacc.bits.X   = (tlbe->priv & MEM_PRIV_X) && True;
    Nios_II->tlbacc.bits.G   = tlbe->G;
    Nios_II->tlbacc.bits.PFN = tlbe->PA >> NIOS_II_VPN_SHIFT;
}

static void vmic_writeTLB (Nios_IIP Nios_II) {

    memPriv priv = MEM_PRIV_NONE;

    if(Nios_II->tlbacc.bits.R) priv |= MEM_PRIV_R;
    if(Nios_II->tlbacc.bits.W) priv |= MEM_PRIV_W;
    if(Nios_II->tlbacc.bits.X) priv |= MEM_PRIV_X;

    Uns32 way = Nios_II->tlbmisc.bits.WAY;

    TLBentry entry = {
        VA   : Nios_II->pteaddr.bits.VPN << NIOS_II_VPN_SHIFT,
        PA   : Nios_II->tlbacc.bits.PFN  << NIOS_II_VPN_SHIFT,
        PID  : Nios_II->tlbmisc.bits.PID,
        priv : priv,
        G    : Nios_II->tlbacc.bits.G,
        C    : Nios_II->tlbacc.bits.C
    };

    Nios_IIWriteTLB(Nios_II, entry, way);
}

static void vmic_WRCTL_Reserved(Nios_IIP Nios_II, Uns32 N, Uns32 A) {
    vmiMessage("W", "WRCTL", "Write to Reserved Control Register CTL%d=GPR[%d]=0x%08x\n", N, A, Nios_II->gpr[A]);
}

static void vmic_WRCTL_tlbacc(Nios_IIP Nios_II) {
    if (Nios_II->tlbmisc.bits.WE) {
        vmic_writeTLB(Nios_II);
        Nios_II->tlbmisc.bits.WAY++;
        Nios_II->tlbmisc.bits.WAY &= Nios_II->tlb.ways-1;
    }
}

static void vmic_WRCTL_tlbmisc(Nios_IIP Nios_II) {
    vmirtSetProcessorASID((vmiProcessorP)Nios_II, Nios_II->tlbmisc.bits.PID);

    if (Nios_II->tlbmisc.bits.WE && Nios_II->tlbmisc.bits.RD) {
        vmiMessage("F", "RDWE", "tlbmisc.WE && lbmisc.RD ASSERTED");
    }
    if (Nios_II->tlbmisc.bits.RD) {
        Nios_II->tlbmisc.bits.RD = 0;
        vmic_readTLB(Nios_II);
    }
}

static void vmic_WRCTL_mpuacc(Nios_IIP Nios_II) {
    if (Nios_II->mpuacc.bits.RD && Nios_II->mpuacc.bits.WR) {
        vmiMessage("F", "RDWE", "mpuacc.WE && mpuacc.RD ASSERTED");
    }
    if (Nios_II->mpuacc.bits.WR) {
        Nios_IIWriteMPU(Nios_II);
        Nios_II->mpuacc.bits.WR = 0;
    }
    if (Nios_II->mpuacc.bits.RD) {
        Nios_IIReadMPU(Nios_II);
        Nios_II->mpuacc.bits.RD = 0;
    }
}

static void writeCtlRegisterMasked(vmiReg ctl, Uns32 A, Uns32 wmask) {
    vmiReg gpr = NIOS_II_GPR_RD(A);

    // (WMASK & NEW) | (!WMASK & OLD)
    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_AND, NIOS_II_TEMP_WR(0), gpr,  wmask, 0);
    vmimtBinopRRC(NIOS_II_GPR_BITS, vmi_AND, NIOS_II_TEMP_WR(1), ctl, ~wmask, 0);
    vmimtBinopRRR(NIOS_II_GPR_BITS, vmi_OR,  ctl, NIOS_II_TEMP_WR(0), NIOS_II_TEMP_WR(1), 0);
}

static void writeCtlArgsFn(Uns32 N, Uns32 A, vmiCallFn func) {
    if (func) {
        vmimtArgProcessor();
        vmimtArgUns32(N);
        vmimtArgUns32(A);
        vmimtCall((vmiCallFn)func);
    }
}

//
// test condition, if true perform mask write and function update
// else call reserved register function
//
static void writeCtlReg(Bool ifTrue, vmiReg ctl, Uns32 N, Uns32 A, Uns32 wmask, vmiCallFn func) {
    if (ifTrue) {
        writeCtlRegisterMasked(ctl, A, wmask);
        writeCtlArgsFn(N, A, func);
    } else {
        writeCtlArgsFn(N, A, (vmiCallFn)vmic_WRCTL_Reserved);
    }
}

NIOS_II_MORPH_FN(morphWRCTL_R6) {
    //
    // Supported Architecture=ISA_0
    //
    validateSupervisorMode(state->Nios_II);

    Uns8 A = state->info.A;
    Uns8 N = state->info.N;

    //
    // Get the Control Register Write Mask and perform the write
    //
    Uns32 wmask, rmask;
    vmiReg ctl;
    getCtlReg(state->Nios_II, N, True, &ctl, &wmask, &rmask);

    //
    // Do we need to perform any side effect action ?
    //
    switch (N) {
    case 0:  // status
        vmimtArgProcessor();
        vmimtArgReg(32, NIOS_II_GPR_RD(A));
        vmimtArgUns32(wmask);
        vmimtCall((vmiCallFn)vmic_WriteStatus);
        break;

    case 1:  // estatus
    case 2:  // bstatus
    case 4:  // ipending - readonly
    case 5:  // cpuid - readonly
    case 6:  // reserved
    case 7:  // exception
    case 12: // badaddr - readonly
        writeCtlRegisterMasked(ctl, A, wmask);
        break;

    case 3:  // ienable
        writeCtlRegisterMasked(ctl, A, wmask);
        //
        // Check to see if we have just enabled a pending interrupt
        //
        vmimtArgProcessor();
        vmimtCall((vmiCallFn)Nios_IICheckExceptions);
        break;

    //
    // MMU Access Registers
    //
    case 8:  // pteaddr
        writeCtlReg(state->Nios_II->params.INCLUDE_MMU, ctl, N, A, wmask, 0);
        break;

    case 9:  // tlbacc
        writeCtlReg(state->Nios_II->params.INCLUDE_MMU, ctl, N, A, wmask, (vmiCallFn)vmic_WRCTL_tlbacc);
        break;

    case 10: // tlbmisc
        writeCtlReg(state->Nios_II->params.INCLUDE_MMU, ctl, N, A, wmask, (vmiCallFn)vmic_WRCTL_tlbmisc);
        break;

        //
        // MPU Access Registers
        //
    case 13: // config
        if (state->Nios_II->params.INCLUDE_MPU) {
            vmimtArgProcessor();
            vmimtArgReg(32, NIOS_II_GPR_RD(A));
            vmimtArgUns32(wmask);
            vmimtCall((vmiCallFn)vmic_WriteConfig);
        } else {
            //
            // Just generate Write warning
            //
            writeCtlReg(0, ctl, N, A, 0, 0);
        }
        break;

    case 14: // mpubase
        writeCtlReg(state->Nios_II->params.INCLUDE_MPU, ctl, N, A, wmask, 0);
        break;

    case 15: // mpuacc
        writeCtlReg(state->Nios_II->params.INCLUDE_MPU, ctl, N, A, wmask, (vmiCallFn)vmic_WRCTL_mpuacc);
        break;

    default:
        writeCtlReg(False, ctl, N, A, wmask, 0);
        break;
    }
}

static void niosCallException (Nios_IIMorphStateP state, vmiReg saveStatus, vmiReg linkReg, Addr vector) {
    //
    // save status
    //
    vmimtMoveRR(NIOS_II_GPR_BITS, saveStatus, NIOS_II_CPU_REG(status));

    //
    // PIE <- 0, U <- 0
    //
    TYPE_status status, wmask;
    status.reg      = 0; wmask.reg      = 0;
    status.bits.PIE = 0; wmask.bits.PIE = 1;
    status.bits.U   = 0; wmask.bits.U   = 1;

    //
    // If we have an MMU the EH is set
    //
    if (state->Nios_II->params.INCLUDE_MMU) {
        status.bits.EH = 1; wmask.bits.EH = 1;
    }

    //
    // Runtime callback for Status Register
    //
    vmimtArgProcessor();
    vmimtArgUns32(status.reg);
    vmimtArgUns32(wmask.reg);
    vmimtCall((vmiCallFn)vmic_WriteStatus);

    //
    // ea (r29) <- PC+4
    // Handler Address
    //
    vmimtUncondJump(state->info.nextPC, vector, linkReg, vmi_JH_CALLINT);
}

NIOS_II_MORPH_FN(morphBREAK_RU) {
    niosCallException(state, NIOS_II_CPU_REG(bstatus), NIOS_II_GPR_WR(30), state->Nios_II->params.BREAK_VECTOR);
}

NIOS_II_MORPH_FN(morphTRAP_RU) {
    //
    // Conditionally write the Cause register
    //
    if (state->Nios_II->params.EXCEPTION_EXTRA_INFORMATION) {
        TYPE_except mask;
        mask.reg        = 0;
        mask.bits.CAUSE = 3;

        vmimtMoveRC(NIOS_II_GPR_BITS, NIOS_II_CPU_REG(except), mask.reg);
    }
    niosCallException(state, NIOS_II_CPU_REG(estatus), NIOS_II_GPR_WR(29), state->Nios_II->params.EXCEPTION_VECTOR);
}

