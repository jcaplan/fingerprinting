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

#ifndef NIOS_II_DECODE_H
#define NIOS_II_DECODE_H

#include "hostapi/impTypes.h"
#include "Nios_IIStructure.h"

typedef enum Nios_IIInstructionTypeE {
    //
    // format C1 Insns
    //
    NIOS_II_IT_CUSTOM_C1,

    //
    // format I0 Insns
    //
    NIOS_II_IT_BRET_I0,
    NIOS_II_IT_ERET_I0,
    NIOS_II_IT_FLUSHP_I0,
    NIOS_II_IT_NOP_I0,
    NIOS_II_IT_RET_I0,
    NIOS_II_IT_SYNC_I0,

    //
    // format I1S Insns
    //
    NIOS_II_IT_ADDI_I1S,
    NIOS_II_IT_CMPEQI_I1S,
    NIOS_II_IT_CMPGEI_I1S,
    NIOS_II_IT_CMPLTI_I1S,
    NIOS_II_IT_CMPNEI_I1S,
    NIOS_II_IT_MULI_I1S,
    NIOS_II_IT_RDPRS_I1S,

    //
    // format I1U Insns
    //
    NIOS_II_IT_ANDI_I1U,
    NIOS_II_IT_CMPGEUI_I1U,
    NIOS_II_IT_CMPLTUI_I1U,
    NIOS_II_IT_ORI_I1U,
    NIOS_II_IT_XORI_I1U,

    //
    // format I2S Insns
    //
    NIOS_II_IT_MOVI_I2S,

    //
    // format I2U Insns
    //
    NIOS_II_IT_ANDHI_I2U,
    NIOS_II_IT_ORHI_I2U,
    NIOS_II_IT_XORHI_I2U,

    //
    // format I3S Insns
    //
    NIOS_II_IT_FLUSHD_I3S,
    NIOS_II_IT_FLUSHDA_I3S,
    NIOS_II_IT_INITD_I3S,
    NIOS_II_IT_INITDA_I3S,

    //
    // format I3U Insns
    //
    NIOS_II_IT_MOVHI_I3U,

    //
    // format I4S Insns
    //
    NIOS_II_IT_LDB_I4S,
    NIOS_II_IT_LDBIO_I4S,
    NIOS_II_IT_LDBU_I4S,
    NIOS_II_IT_LDBUIO_I4S,
    NIOS_II_IT_LDH_I4S,
    NIOS_II_IT_LDHIO_I4S,
    NIOS_II_IT_LDHU_I4S,
    NIOS_II_IT_LDHUIO_I4S,
    NIOS_II_IT_LDW_I4S,
    NIOS_II_IT_LDWIO_I4S,
    NIOS_II_IT_STB_I4S,
    NIOS_II_IT_STBIO_I4S,
    NIOS_II_IT_STH_I4S,
    NIOS_II_IT_STHIO_I4S,
    NIOS_II_IT_STW_I4S,
    NIOS_II_IT_STWIO_I4S,

    //
    // format I4U Insns
    //
    NIOS_II_IT_MOVUI_I4U,

    //
    // format IB1 Insns
    //
    NIOS_II_IT_BEQ_IB1,
    NIOS_II_IT_BGE_IB1,
    NIOS_II_IT_BGEU_IB1,
    NIOS_II_IT_BLT_IB1,
    NIOS_II_IT_BLTU_IB1,
    NIOS_II_IT_BNE_IB1,

    //
    // format IB2 Insns
    //
    NIOS_II_IT_BR_IB2,

    //
    // format J1 Insns
    //
    NIOS_II_IT_CALL_J1,
    NIOS_II_IT_JMPI_J1,

    //
    // format R1 Insns
    //
    NIOS_II_IT_ADD_R1,
    NIOS_II_IT_AND_R1,
    NIOS_II_IT_CMPEQ_R1,
    NIOS_II_IT_CMPGE_R1,
    NIOS_II_IT_CMPGEU_R1,
    NIOS_II_IT_CMPLT_R1,
    NIOS_II_IT_CMPLTU_R1,
    NIOS_II_IT_CMPNE_R1,
    NIOS_II_IT_DIV_R1,
    NIOS_II_IT_DIVU_R1,
    NIOS_II_IT_MUL_R1,
    NIOS_II_IT_MULXSS_R1,
    NIOS_II_IT_MULXSU_R1,
    NIOS_II_IT_MULXUU_R1,
    NIOS_II_IT_NOR_R1,
    NIOS_II_IT_OR_R1,
    NIOS_II_IT_ROL_R1,
    NIOS_II_IT_ROR_R1,
    NIOS_II_IT_SLL_R1,
    NIOS_II_IT_SRA_R1,
    NIOS_II_IT_SRL_R1,
    NIOS_II_IT_SUB_R1,
    NIOS_II_IT_XOR_R1,

    //
    // format R2 Insns
    //
    NIOS_II_IT_MOV_R2,
    NIOS_II_IT_WRPRS_R2,

    //
    // format R2U Insns
    //
    NIOS_II_IT_ROLI_R2U,
    NIOS_II_IT_SLLI_R2U,
    NIOS_II_IT_SRAI_R2U,
    NIOS_II_IT_SRLI_R2U,

    //
    // format R3 Insns
    //
    NIOS_II_IT_NEXTPC_R3,

    //
    // format R4 Insns
    //
    NIOS_II_IT_CALLR_R4,
    NIOS_II_IT_FLUSHI_R4,
    NIOS_II_IT_INITI_R4,
    NIOS_II_IT_JMP_R4,

    //
    // format R5 Insns
    //
    NIOS_II_IT_RDCTL_R5,

    //
    // format R6 Insns
    //
    NIOS_II_IT_WRCTL_R6,

    //
    // format RU Insns
    //
    NIOS_II_IT_BREAK_RU,
    NIOS_II_IT_TRAP_RU,

    //
    // Drop through catch for instruction widths 32
    //
    NIOS_II_IT_INSTR32,

    //
    // Drop through catch all
    //
    NIOS_II_IT_LAST
} Nios_IIInstructionType;

#define NIOS_II_DISPATCH_FN(_NAME) void _NAME( \
    Nios_IIP Nios_II, \
    Nios_IIInstructionInfoP info, \
    void *userData \
)

typedef NIOS_II_DISPATCH_FN((*Nios_IIDispatchFn));
typedef const Nios_IIDispatchFn Nios_IIDispatchTableC[NIOS_II_IT_LAST];
typedef Nios_IIDispatchTableC *Nios_IIDispatchTableCP;

vmidDecodeTableP Nios_IICreateDecodeTable_ISA_0_32Bit(vmidDecodeTableP current);

void Nios_IIGenInstructionInfo (Nios_IIInstructionInfoP info);

void Nios_IIDecode(
    Nios_IIP Nios_II,
    Nios_IIAddr thisPC,
    Nios_IIInstructionInfoP info
);

//
// info structure accessed from decoder
//
typedef struct Nios_IIInstructionInfoS {
    //
    // Default structure members
    //
    const char *opcode;
    Nios_IIAddr thisPC;
    Nios_IIAddr nextPC;
    Uns64 instruction;
    Uns8  instrsize;
    Nios_IIInstructionType type;
    Nios_IIArchitecture arch;

    //
    // Instruction format generated structure members
    //
    Uns8 A;
    Uns8 B;
    Uns8 C;
    Uns8 N;
    Int16 SIMM16;
    Uns32 UIMM16;
    Uns32 UIMM26;
    Uns8 UIMM5;
    Uns8 readra;
    Uns8 readrb;
    Uns8 readrc;
} Nios_IIInstructionInfo;

#endif // NIOS_II_DECODE_H
