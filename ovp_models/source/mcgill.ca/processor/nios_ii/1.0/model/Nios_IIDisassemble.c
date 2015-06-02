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

#include <stdio.h>
#include "vmi/vmiCxt.h"
#include "Nios_IIDecode.h"
#include "Nios_IIInstructions.h"
#include "Nios_IIFunctions.h"

//
// Generate the disassembler alias lookup table for CTL
//
const char *CTL[32] = {
    [0] = "status",
    [1] = "estatus",
    [2] = "bstatus",
    [3] = "ienable",
    [4] = "ipending",
    [5] = "cpuid",
    [6] = "ctl6",
    [7] = "exception",
    [8] = "pteaddr",
    [9] = "tlbacc",
    [10] = "tlbmisc",
    [11] = "eccinj",
    [12] = "badaddr",
    [13] = "config",
    [14] = "mpubase",
    [15] = "mpuacc",
};

//
// Generate the disassembler alias lookup table for GPR
//
const char *GPR[32] = {
    [0] = "zero",
    [1] = "at",
    [24] = "et",
    [25] = "bt",
    [26] = "gp",
    [27] = "sp",
    [28] = "fp",
    [29] = "ea",
    [30] = "ba",
    [31] = "ra",
};

//
// Generate the disassembler statemachine definitions
//
#define EMITA      "\200"
#define EMITA_CASE '\200'
#define EMITB      "\201"
#define EMITB_CASE '\201'
#define EMITBRPC      "\202"
#define EMITBRPC_CASE '\202'
#define EMITC      "\203"
#define EMITC_CASE '\203'
#define EMITCA      "\204"
#define EMITCA_CASE '\204'
#define EMITCALLPC      "\205"
#define EMITCALLPC_CASE '\205'
#define EMITCB      "\206"
#define EMITCB_CASE '\206'
#define EMITCC      "\207"
#define EMITCC_CASE '\207'
#define EMITCTLN      "\210"
#define EMITCTLN_CASE '\210'
#define EMITHUIMM16      "\211"
#define EMITHUIMM16_CASE '\211'
#define EMITN      "\212"
#define EMITN_CASE '\212'
#define EMITOP      "\213"
#define EMITOP_CASE '\213'
#define EMITOP0      "\214"
#define EMITOP0_CASE '\214'
#define EMITSIMM16      "\215"
#define EMITSIMM16_CASE '\215'
#define EMITUIMM16      "\216"
#define EMITUIMM16_CASE '\216'
#define EMITUIMM5      "\217"
#define EMITUIMM5_CASE '\217'

//
// Generate the instruction formatting table
//
#define FMT_NONE ""
const char *formats[NIOS_II_IT_LAST+1] = {
    //
    // Instruction Format C1 Insns
    //
    [NIOS_II_IT_CUSTOM_C1]   = EMITOP " " EMITN "," EMITCC "," EMITCA "," EMITCB,

    //
    // Instruction Format I0 Insns
    //
    [NIOS_II_IT_BRET_I0]     = EMITOP0,
    [NIOS_II_IT_ERET_I0]     = EMITOP0,
    [NIOS_II_IT_FLUSHP_I0]   = EMITOP0,
    [NIOS_II_IT_NOP_I0]      = EMITOP0,
    [NIOS_II_IT_RET_I0]      = EMITOP0,
    [NIOS_II_IT_SYNC_I0]     = EMITOP0,

    //
    // Instruction Format I1S Insns
    //
    [NIOS_II_IT_ADDI_I1S]    = EMITOP " " EMITB "," EMITA "," EMITSIMM16,
    [NIOS_II_IT_CMPEQI_I1S]  = EMITOP " " EMITB "," EMITA "," EMITSIMM16,
    [NIOS_II_IT_CMPGEI_I1S]  = EMITOP " " EMITB "," EMITA "," EMITSIMM16,
    [NIOS_II_IT_CMPLTI_I1S]  = EMITOP " " EMITB "," EMITA "," EMITSIMM16,
    [NIOS_II_IT_CMPNEI_I1S]  = EMITOP " " EMITB "," EMITA "," EMITSIMM16,
    [NIOS_II_IT_MULI_I1S]    = EMITOP " " EMITB "," EMITA "," EMITSIMM16,
    [NIOS_II_IT_RDPRS_I1S]   = EMITOP " " EMITB "," EMITA "," EMITSIMM16,

    //
    // Instruction Format I1U Insns
    //
    [NIOS_II_IT_ANDI_I1U]    = EMITOP " " EMITB "," EMITA "," EMITUIMM16,
    [NIOS_II_IT_CMPGEUI_I1U] = EMITOP " " EMITB "," EMITA "," EMITUIMM16,
    [NIOS_II_IT_CMPLTUI_I1U] = EMITOP " " EMITB "," EMITA "," EMITUIMM16,
    [NIOS_II_IT_ORI_I1U]     = EMITOP " " EMITB "," EMITA "," EMITUIMM16,
    [NIOS_II_IT_XORI_I1U]    = EMITOP " " EMITB "," EMITA "," EMITUIMM16,

    //
    // Instruction Format I2S Insns
    //
    [NIOS_II_IT_MOVI_I2S]    = EMITOP " " EMITB ","           EMITSIMM16,

    //
    // Instruction Format I2U Insns
    //
    [NIOS_II_IT_ANDHI_I2U]   = EMITOP " " EMITB "," EMITA "," EMITHUIMM16,
    [NIOS_II_IT_ORHI_I2U]    = EMITOP " " EMITB "," EMITA "," EMITHUIMM16,
    [NIOS_II_IT_XORHI_I2U]   = EMITOP " " EMITB "," EMITA "," EMITHUIMM16,

    //
    // Instruction Format I3S Insns
    //
    [NIOS_II_IT_FLUSHD_I3S]  = EMITOP " " EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_FLUSHDA_I3S] = EMITOP " " EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_INITD_I3S]   = EMITOP " " EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_INITDA_I3S]  = EMITOP " " EMITSIMM16 "(" EMITA ")",

    //
    // Instruction Format I3U Insns
    //
    [NIOS_II_IT_MOVHI_I3U]   = EMITOP " " EMITB "," EMITHUIMM16,

    //
    // Instruction Format I4S Insns
    //
    [NIOS_II_IT_LDB_I4S]     = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_LDBIO_I4S]   = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_LDBU_I4S]    = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_LDBUIO_I4S]  = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_LDH_I4S]     = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_LDHIO_I4S]   = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_LDHU_I4S]    = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_LDHUIO_I4S]  = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_LDW_I4S]     = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_LDWIO_I4S]   = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_STB_I4S]     = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_STBIO_I4S]   = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_STH_I4S]     = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_STHIO_I4S]   = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_STW_I4S]     = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",
    [NIOS_II_IT_STWIO_I4S]   = EMITOP " " EMITB "," EMITSIMM16 "(" EMITA ")",

    //
    // Instruction Format I4U Insns
    //
    [NIOS_II_IT_MOVUI_I4U]   = EMITOP " " EMITB "," EMITUIMM16,

    //
    // Instruction Format IB1 Insns
    //
    [NIOS_II_IT_BEQ_IB1]     = EMITOP " " EMITA "," EMITB "," EMITBRPC,
    [NIOS_II_IT_BGE_IB1]     = EMITOP " " EMITA "," EMITB "," EMITBRPC,
    [NIOS_II_IT_BGEU_IB1]    = EMITOP " " EMITA "," EMITB "," EMITBRPC,
    [NIOS_II_IT_BLT_IB1]     = EMITOP " " EMITA "," EMITB "," EMITBRPC,
    [NIOS_II_IT_BLTU_IB1]    = EMITOP " " EMITA "," EMITB "," EMITBRPC,
    [NIOS_II_IT_BNE_IB1]     = EMITOP " " EMITA "," EMITB "," EMITBRPC,

    //
    // Instruction Format IB2 Insns
    //
    [NIOS_II_IT_BR_IB2]      = EMITOP " "                     EMITBRPC,

    //
    // Instruction Format J1 Insns
    //
    [NIOS_II_IT_CALL_J1]     = EMITOP " " EMITCALLPC,
    [NIOS_II_IT_JMPI_J1]     = EMITOP " " EMITCALLPC,

    //
    // Instruction Format R1 Insns
    //
    [NIOS_II_IT_ADD_R1]      = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_AND_R1]      = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_CMPEQ_R1]    = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_CMPGE_R1]    = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_CMPGEU_R1]   = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_CMPLT_R1]    = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_CMPLTU_R1]   = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_CMPNE_R1]    = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_DIV_R1]      = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_DIVU_R1]     = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_MUL_R1]      = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_MULXSS_R1]   = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_MULXSU_R1]   = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_MULXUU_R1]   = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_NOR_R1]      = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_OR_R1]       = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_ROL_R1]      = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_ROR_R1]      = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_SLL_R1]      = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_SRA_R1]      = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_SRL_R1]      = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_SUB_R1]      = EMITOP " " EMITC "," EMITA "," EMITB,
    [NIOS_II_IT_XOR_R1]      = EMITOP " " EMITC "," EMITA "," EMITB,

    //
    // Instruction Format R2 Insns
    //
    [NIOS_II_IT_MOV_R2]      = EMITOP " " EMITC "," EMITA,
    [NIOS_II_IT_WRPRS_R2]    = EMITOP " " EMITC "," EMITA,

    //
    // Instruction Format R2U Insns
    //
    [NIOS_II_IT_ROLI_R2U]    = EMITOP " " EMITC "," EMITA "," EMITUIMM5,
    [NIOS_II_IT_SLLI_R2U]    = EMITOP " " EMITC "," EMITA "," EMITUIMM5,
    [NIOS_II_IT_SRAI_R2U]    = EMITOP " " EMITC "," EMITA "," EMITUIMM5,
    [NIOS_II_IT_SRLI_R2U]    = EMITOP " " EMITC "," EMITA "," EMITUIMM5,

    //
    // Instruction Format R3 Insns
    //
    [NIOS_II_IT_NEXTPC_R3]   = EMITOP " " EMITC,

    //
    // Instruction Format R4 Insns
    //
    [NIOS_II_IT_CALLR_R4]    = EMITOP " " EMITA,
    [NIOS_II_IT_FLUSHI_R4]   = EMITOP " " EMITA,
    [NIOS_II_IT_INITI_R4]    = EMITOP " " EMITA,
    [NIOS_II_IT_JMP_R4]      = EMITOP " " EMITA,

    //
    // Instruction Format R5 Insns
    //
    [NIOS_II_IT_RDCTL_R5]    = EMITOP " " EMITC "," EMITCTLN,

    //
    // Instruction Format R6 Insns
    //
    [NIOS_II_IT_WRCTL_R6]    = EMITOP " " EMITCTLN "," EMITA,

    //
    // Instruction Format RU Insns
    //
    [NIOS_II_IT_BREAK_RU]    = EMITOP " " EMITUIMM5,
    [NIOS_II_IT_TRAP_RU]     = EMITOP " " EMITUIMM5,

    [NIOS_II_IT_LAST] = "UNDEFINED"
};

//
// generate builtin disassembler writer functions
//
static void putChar(char **result, char ch) {
    char *tail = *result;
    *tail++ = ch;
    *tail = 0;
    *result = tail;
}

static void putString(char **result, const char *string) {
    char *tail = *result;
    char  ch;
    while((ch=*string++)) {
        *tail++ = ch;
    }
    *tail = 0;
    *result = tail;
}

//
// generate generated disassembler writer functions
//

//
// Function to format the info field #A#
//
static void EMITA_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = (GPR[info->A] ? GPR[info->A] : "r%d");
    sprintf(tmp, fmt, info->A);
    putString(result, tmp);
}
//
// Function to format the info field #B#
//
static void EMITB_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = (GPR[info->B] ? GPR[info->B] : "r%d");
    sprintf(tmp, fmt, info->B);
    putString(result, tmp);
}
//
// Function to format the info field ( #SIMM16# + #thisPC# + 4)
//
static void EMITBRPC_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = "0x%x";
    sprintf(tmp, fmt, ( info->SIMM16 + info->thisPC + 4));
    putString(result, tmp);
}
//
// Function to format the info field #C#
//
static void EMITC_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = (GPR[info->C] ? GPR[info->C] : "r%d");
    sprintf(tmp, fmt, info->C);
    putString(result, tmp);
}
//
// Function to format the info field ( #readra# ? 'r' : 'c' ), #A#
//
static void EMITCA_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = "%c%d";
    sprintf(tmp, fmt, ( info->readra ? 'r' : 'c' ), info->A);
    putString(result, tmp);
}
//
// Function to format the info field ( ( #thisPC# & 0xF0000000 ) | #UIMM26# )
//
static void EMITCALLPC_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = "0x%x";
    sprintf(tmp, fmt, ( ( info->thisPC & 0xF0000000 ) | info->UIMM26 ));
    putString(result, tmp);
}
//
// Function to format the info field ( #readrb# ? 'r' : 'c' ), #B#
//
static void EMITCB_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = "%c%d";
    sprintf(tmp, fmt, ( info->readrb ? 'r' : 'c' ), info->B);
    putString(result, tmp);
}
//
// Function to format the info field ( #readrc# ? 'r' : 'c' ), #C#
//
static void EMITCC_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = "%c%d";
    sprintf(tmp, fmt, ( info->readrc ? 'r' : 'c' ), info->C);
    putString(result, tmp);
}
//
// Function to format the info field #N#
//
static void EMITCTLN_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = (CTL[info->N] ? CTL[info->N] : "ctl%d");
    sprintf(tmp, fmt, info->N);
    putString(result, tmp);
}
//
// Function to format the info field ( #UIMM16# >> 16 )
//
static void EMITHUIMM16_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = "%u";
    sprintf(tmp, fmt, ( info->UIMM16 >> 16 ));
    putString(result, tmp);
}
//
// Function to format the info field #N#
//
static void EMITN_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = "%d";
    sprintf(tmp, fmt, info->N);
    putString(result, tmp);
}
//
// Function to format the info field #opcode#
//
static void EMITOP_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = "%-8s";
    sprintf(tmp, fmt, info->opcode);
    putString(result, tmp);
}
//
// Function to format the info field #opcode#
//
static void EMITOP0_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = "%s";
    sprintf(tmp, fmt, info->opcode);
    putString(result, tmp);
}
//
// Function to format the info field #SIMM16#
//
static void EMITSIMM16_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = "%d";
    sprintf(tmp, fmt, info->SIMM16);
    putString(result, tmp);
}
//
// Function to format the info field #UIMM16#
//
static void EMITUIMM16_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = "%u";
    sprintf(tmp, fmt, info->UIMM16);
    putString(result, tmp);
}
//
// Function to format the info field #UIMM5#
//
static void EMITUIMM5_FN(char **result, Nios_IIInstructionInfoP info) {
    char tmp[32];
    const char *fmt = "%u";
    sprintf(tmp, fmt, info->UIMM5);
    putString(result, tmp);
}

//
// generate disassembler state machine
//
static void disassembleFormat(
    Nios_IIP Nios_II,
    Nios_IIInstructionInfoP info,
    char **result,
    const char *format) {

    //
    // Default disassembler output
    //
    char ifmt[64];
    Uns8 ifmtchars = 2 * info->instrsize;
    sprintf(ifmt, "??? instruction(%d bytes)=0x%%0%0dx ", info->instrsize, ifmtchars);
    sprintf(*result, ifmt, info->instruction);

    char ch;
    while((ch=*format++)) {
         switch(ch) {
             case EMITA_CASE:
                 EMITA_FN(result, info);
                 break;

             case EMITB_CASE:
                 EMITB_FN(result, info);
                 break;

             case EMITBRPC_CASE:
                 EMITBRPC_FN(result, info);
                 break;

             case EMITC_CASE:
                 EMITC_FN(result, info);
                 break;

             case EMITCA_CASE:
                 EMITCA_FN(result, info);
                 break;

             case EMITCALLPC_CASE:
                 EMITCALLPC_FN(result, info);
                 break;

             case EMITCB_CASE:
                 EMITCB_FN(result, info);
                 break;

             case EMITCC_CASE:
                 EMITCC_FN(result, info);
                 break;

             case EMITCTLN_CASE:
                 EMITCTLN_FN(result, info);
                 break;

             case EMITHUIMM16_CASE:
                 EMITHUIMM16_FN(result, info);
                 break;

             case EMITN_CASE:
                 EMITN_FN(result, info);
                 break;

             case EMITOP_CASE:
                 EMITOP_FN(result, info);
                 break;

             case EMITOP0_CASE:
                 EMITOP0_FN(result, info);
                 break;

             case EMITSIMM16_CASE:
                 EMITSIMM16_FN(result, info);
                 break;

             case EMITUIMM16_CASE:
                 EMITUIMM16_FN(result, info);
                 break;

             case EMITUIMM5_CASE:
                 EMITUIMM5_FN(result, info);
                 break;

             default:
                 putChar(result, ch);
                 break;
        }
    }
}

//
// Nios_II disassembler, decoded instruction interface
//
const char *Nios_IIDisassembleInfo(Nios_IIP Nios_II, Nios_IIInstructionInfoP info) {
    static char result[256];
    const char *format;
    char *tail = result;

    //
    // disassemble using the format for the type
    //
    if((format=formats[info->type])) {
        disassembleFormat(Nios_II, info, &tail, format);
    } else {
        sprintf(result, "??? instruction:0x" FMT_640Nx, info->instruction);
    }
    return result;
}

//
// Nios_II disassembler
//
VMI_DISASSEMBLE_FN(Nios_IIDis) {
    // static buffer to hold disassembly result
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_IIInstructionInfo info = {0};
    // get instruction and instruction type
    Nios_IIDecode(Nios_II, thisPC, &info);
    // return disassembled instruction
    return Nios_IIDisassembleInfo(Nios_II, &info);
}
