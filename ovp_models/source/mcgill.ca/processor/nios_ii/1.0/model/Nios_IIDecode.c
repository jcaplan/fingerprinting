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

#include "vmi/vmiCxt.h"
#include "vmi/vmiDecode.h"
#include "vmi/vmiMessage.h"
#include "Nios_IIDecode.h"
#include "Nios_IIInstructions.h"

//
// Generate the instruction opcode table
//
static const char *opcode[NIOS_II_IT_LAST+1] = {
    //
    // Instruction Opcode C1 Insns
    //
    [NIOS_II_IT_CUSTOM_C1] = "custom",

    //
    // Instruction Opcode I0 Insns
    //
    [NIOS_II_IT_BRET_I0] = "bret",
    [NIOS_II_IT_ERET_I0] = "eret",
    [NIOS_II_IT_FLUSHP_I0] = "flushp",
    [NIOS_II_IT_NOP_I0] = "nop",
    [NIOS_II_IT_RET_I0] = "ret",
    [NIOS_II_IT_SYNC_I0] = "sync",

    //
    // Instruction Opcode I1S Insns
    //
    [NIOS_II_IT_ADDI_I1S] = "addi",
    [NIOS_II_IT_CMPEQI_I1S] = "cmpeqi",
    [NIOS_II_IT_CMPGEI_I1S] = "cmpgei",
    [NIOS_II_IT_CMPLTI_I1S] = "cmplti",
    [NIOS_II_IT_CMPNEI_I1S] = "cmpnei",
    [NIOS_II_IT_MULI_I1S] = "muli",
    [NIOS_II_IT_RDPRS_I1S] = "rdprs",

    //
    // Instruction Opcode I1U Insns
    //
    [NIOS_II_IT_ANDI_I1U] = "andi",
    [NIOS_II_IT_CMPGEUI_I1U] = "cmpgeui",
    [NIOS_II_IT_CMPLTUI_I1U] = "cmpltui",
    [NIOS_II_IT_ORI_I1U] = "ori",
    [NIOS_II_IT_XORI_I1U] = "xori",

    //
    // Instruction Opcode I2S Insns
    //
    [NIOS_II_IT_MOVI_I2S] = "movi",

    //
    // Instruction Opcode I2U Insns
    //
    [NIOS_II_IT_ANDHI_I2U] = "andhi",
    [NIOS_II_IT_ORHI_I2U] = "orhi",
    [NIOS_II_IT_XORHI_I2U] = "xorhi",

    //
    // Instruction Opcode I3S Insns
    //
    [NIOS_II_IT_FLUSHD_I3S] = "flushd",
    [NIOS_II_IT_FLUSHDA_I3S] = "flushda",
    [NIOS_II_IT_INITD_I3S] = "initd",
    [NIOS_II_IT_INITDA_I3S] = "initda",

    //
    // Instruction Opcode I3U Insns
    //
    [NIOS_II_IT_MOVHI_I3U] = "movhi",

    //
    // Instruction Opcode I4S Insns
    //
    [NIOS_II_IT_LDB_I4S] = "ldb",
    [NIOS_II_IT_LDBIO_I4S] = "ldbio",
    [NIOS_II_IT_LDBU_I4S] = "ldbu",
    [NIOS_II_IT_LDBUIO_I4S] = "ldbuio",
    [NIOS_II_IT_LDH_I4S] = "ldh",
    [NIOS_II_IT_LDHIO_I4S] = "ldhio",
    [NIOS_II_IT_LDHU_I4S] = "ldhu",
    [NIOS_II_IT_LDHUIO_I4S] = "ldhuio",
    [NIOS_II_IT_LDW_I4S] = "ldw",
    [NIOS_II_IT_LDWIO_I4S] = "ldwio",
    [NIOS_II_IT_STB_I4S] = "stb",
    [NIOS_II_IT_STBIO_I4S] = "stbio",
    [NIOS_II_IT_STH_I4S] = "sth",
    [NIOS_II_IT_STHIO_I4S] = "sthio",
    [NIOS_II_IT_STW_I4S] = "stw",
    [NIOS_II_IT_STWIO_I4S] = "stwio",

    //
    // Instruction Opcode I4U Insns
    //
    [NIOS_II_IT_MOVUI_I4U] = "movui",

    //
    // Instruction Opcode IB1 Insns
    //
    [NIOS_II_IT_BEQ_IB1] = "beq",
    [NIOS_II_IT_BGE_IB1] = "bge",
    [NIOS_II_IT_BGEU_IB1] = "bgeu",
    [NIOS_II_IT_BLT_IB1] = "blt",
    [NIOS_II_IT_BLTU_IB1] = "bltu",
    [NIOS_II_IT_BNE_IB1] = "bne",

    //
    // Instruction Opcode IB2 Insns
    //
    [NIOS_II_IT_BR_IB2] = "br",

    //
    // Instruction Opcode J1 Insns
    //
    [NIOS_II_IT_CALL_J1] = "call",
    [NIOS_II_IT_JMPI_J1] = "jmpi",

    //
    // Instruction Opcode R1 Insns
    //
    [NIOS_II_IT_ADD_R1] = "add",
    [NIOS_II_IT_AND_R1] = "and",
    [NIOS_II_IT_CMPEQ_R1] = "cmpeq",
    [NIOS_II_IT_CMPGE_R1] = "cmpge",
    [NIOS_II_IT_CMPGEU_R1] = "cmpgeu",
    [NIOS_II_IT_CMPLT_R1] = "cmplt",
    [NIOS_II_IT_CMPLTU_R1] = "cmpltu",
    [NIOS_II_IT_CMPNE_R1] = "cmpne",
    [NIOS_II_IT_DIV_R1] = "div",
    [NIOS_II_IT_DIVU_R1] = "divu",
    [NIOS_II_IT_MUL_R1] = "mul",
    [NIOS_II_IT_MULXSS_R1] = "mulxss",
    [NIOS_II_IT_MULXSU_R1] = "mulxsu",
    [NIOS_II_IT_MULXUU_R1] = "mulxuu",
    [NIOS_II_IT_NOR_R1] = "nor",
    [NIOS_II_IT_OR_R1] = "or",
    [NIOS_II_IT_ROL_R1] = "rol",
    [NIOS_II_IT_ROR_R1] = "ror",
    [NIOS_II_IT_SLL_R1] = "sll",
    [NIOS_II_IT_SRA_R1] = "sra",
    [NIOS_II_IT_SRL_R1] = "srl",
    [NIOS_II_IT_SUB_R1] = "sub",
    [NIOS_II_IT_XOR_R1] = "xor",

    //
    // Instruction Opcode R2 Insns
    //
    [NIOS_II_IT_MOV_R2] = "mov",
    [NIOS_II_IT_WRPRS_R2] = "wrprs",

    //
    // Instruction Opcode R2U Insns
    //
    [NIOS_II_IT_ROLI_R2U] = "roli",
    [NIOS_II_IT_SLLI_R2U] = "slli",
    [NIOS_II_IT_SRAI_R2U] = "srai",
    [NIOS_II_IT_SRLI_R2U] = "srli",

    //
    // Instruction Opcode R3 Insns
    //
    [NIOS_II_IT_NEXTPC_R3] = "nextpc",

    //
    // Instruction Opcode R4 Insns
    //
    [NIOS_II_IT_CALLR_R4] = "callr",
    [NIOS_II_IT_FLUSHI_R4] = "flushi",
    [NIOS_II_IT_INITI_R4] = "initi",
    [NIOS_II_IT_JMP_R4] = "jmp",

    //
    // Instruction Opcode R5 Insns
    //
    [NIOS_II_IT_RDCTL_R5] = "rdctl",

    //
    // Instruction Opcode R6 Insns
    //
    [NIOS_II_IT_WRCTL_R6] = "wrctl",

    //
    // Instruction Opcode RU Insns
    //
    [NIOS_II_IT_BREAK_RU] = "break",
    [NIOS_II_IT_TRAP_RU] = "trap",

    [NIOS_II_IT_LAST] = "UNDEFINED"
};

#define DECODE_ENTRY(_PRIORITY, _NAME, _FMT) \
    vmidNewEntryFmtBin( \
        table, \
        #_NAME, \
        NIOS_II_IT_##_NAME, \
        _FMT, \
        _PRIORITY \
    )

// ISA_0 Decoder
vmidDecodeTableP Nios_IICreateDecodeTable_ISA_0_32Bit(vmidDecodeTableP current) {
    vmidDecodeTableP table = current;
    if (!table) {
        table = vmidNewDecodeTable(32, NIOS_II_IT_LAST);
    }

    //                             C1   |    A|    B|    C|r|r|r|       N|    OP|
    DECODE_ENTRY( 6,        CUSTOM_C1, "|.....|.....|.....|.|.|.|........|110010|");

    //                             I0   |                              OP|
    DECODE_ENTRY(32,          BRET_I0, "|11110000000000000100100000111010|");
    DECODE_ENTRY(32,          ERET_I0, "|11101111100000000000100000111010|");
    DECODE_ENTRY(32,        FLUSHP_I0, "|00000000000000000010000000111010|");
    DECODE_ENTRY(32,           NOP_I0, "|00000000000000011000100000111010|");
    DECODE_ENTRY(32,           RET_I0, "|11111000000000000010100000111010|");
    DECODE_ENTRY(32,          SYNC_I0, "|00000000000000011011000000111010|");

    //                            I1S   |    A|    B|          SIMM16|    OP|
    DECODE_ENTRY( 6,         ADDI_I1S, "|.....|.....|................|000100|");
    DECODE_ENTRY( 6,       CMPEQI_I1S, "|.....|.....|................|100000|");
    DECODE_ENTRY( 6,       CMPGEI_I1S, "|.....|.....|................|001000|");
    DECODE_ENTRY( 6,       CMPLTI_I1S, "|.....|.....|................|010000|");
    DECODE_ENTRY( 6,       CMPNEI_I1S, "|.....|.....|................|011000|");
    DECODE_ENTRY( 6,         MULI_I1S, "|.....|.....|................|100100|");
    DECODE_ENTRY( 6,        RDPRS_I1S, "|.....|.....|................|111000|");

    //                            I1U   |    A|    B|          UIMM16|    OP|
    DECODE_ENTRY( 6,         ANDI_I1U, "|.....|.....|................|001100|");
    DECODE_ENTRY( 6,      CMPGEUI_I1U, "|.....|.....|................|101000|");
    DECODE_ENTRY( 6,      CMPLTUI_I1U, "|.....|.....|................|110000|");
    DECODE_ENTRY( 6,          ORI_I1U, "|.....|.....|................|010100|");
    DECODE_ENTRY( 6,         XORI_I1U, "|.....|.....|................|011100|");

    //                            I2S   |   ZA|    B|          SIMM16|    OP|
    DECODE_ENTRY(11,         MOVI_I2S, "|00000|.....|................|000100|");

    //                            I2U   |    A|    B|          UIMM16|    OP|
    DECODE_ENTRY( 6,        ANDHI_I2U, "|.....|.....|................|101100|");
    DECODE_ENTRY( 6,         ORHI_I2U, "|.....|.....|................|110100|");
    DECODE_ENTRY( 6,        XORHI_I2U, "|.....|.....|................|111100|");

    //                            I3S   |    A|   ZB|          SIMM16|    OP|
    DECODE_ENTRY(11,       FLUSHD_I3S, "|.....|00000|................|111011|");
    DECODE_ENTRY(11,      FLUSHDA_I3S, "|.....|00000|................|011011|");
    DECODE_ENTRY(11,        INITD_I3S, "|.....|00000|................|110011|");
    DECODE_ENTRY(11,       INITDA_I3S, "|.....|00000|................|010011|");

    //                            I3U   |   ZA|    B|          UIMM16|    OP|
    DECODE_ENTRY(11,        MOVHI_I3U, "|00000|.....|................|110100|");

    //                            I4S   |    A|    B|          SIMM16|    OP|
    DECODE_ENTRY( 6,          LDB_I4S, "|.....|.....|................|000111|");
    DECODE_ENTRY( 6,        LDBIO_I4S, "|.....|.....|................|100111|");
    DECODE_ENTRY( 6,         LDBU_I4S, "|.....|.....|................|000011|");
    DECODE_ENTRY( 6,       LDBUIO_I4S, "|.....|.....|................|100011|");
    DECODE_ENTRY( 6,          LDH_I4S, "|.....|.....|................|001111|");
    DECODE_ENTRY( 6,        LDHIO_I4S, "|.....|.....|................|101111|");
    DECODE_ENTRY( 6,         LDHU_I4S, "|.....|.....|................|001011|");
    DECODE_ENTRY( 6,       LDHUIO_I4S, "|.....|.....|................|101011|");
    DECODE_ENTRY( 6,          LDW_I4S, "|.....|.....|................|010111|");
    DECODE_ENTRY( 6,        LDWIO_I4S, "|.....|.....|................|110111|");
    DECODE_ENTRY( 6,          STB_I4S, "|.....|.....|................|000101|");
    DECODE_ENTRY( 6,        STBIO_I4S, "|.....|.....|................|100101|");
    DECODE_ENTRY( 6,          STH_I4S, "|.....|.....|................|001101|");
    DECODE_ENTRY( 6,        STHIO_I4S, "|.....|.....|................|101101|");
    DECODE_ENTRY( 6,          STW_I4S, "|.....|.....|................|010101|");
    DECODE_ENTRY( 6,        STWIO_I4S, "|.....|.....|................|110101|");

    //                            I4U   |   ZA|    B|          UIMM16|    OP|
    DECODE_ENTRY(11,        MOVUI_I4U, "|00000|.....|................|010100|");

    //                            IB1   |    A|    B|          SIMM16|    OP|
    DECODE_ENTRY( 6,          BEQ_IB1, "|.....|.....|................|100110|");
    DECODE_ENTRY( 6,          BGE_IB1, "|.....|.....|................|001110|");
    DECODE_ENTRY( 6,         BGEU_IB1, "|.....|.....|................|101110|");
    DECODE_ENTRY( 6,          BLT_IB1, "|.....|.....|................|010110|");
    DECODE_ENTRY( 6,         BLTU_IB1, "|.....|.....|................|110110|");
    DECODE_ENTRY( 6,          BNE_IB1, "|.....|.....|................|011110|");

    //                            IB2   |   ZA|   ZB|          SIMM16|    OP|
    DECODE_ENTRY(16,           BR_IB2, "|00000|00000|................|000110|");

    //                             J1   |                    UIMM26|    OP|
    DECODE_ENTRY( 6,          CALL_J1, "|..........................|000000|");
    DECODE_ENTRY( 6,          JMPI_J1, "|..........................|000001|");

    //                             R1   |    A|    B|    C|   OPX|   ZC|    OP|
    DECODE_ENTRY(17,           ADD_R1, "|.....|.....|.....|110001|00000|111010|");
    DECODE_ENTRY(17,           AND_R1, "|.....|.....|.....|001110|00000|111010|");
    DECODE_ENTRY(17,         CMPEQ_R1, "|.....|.....|.....|100000|00000|111010|");
    DECODE_ENTRY(17,         CMPGE_R1, "|.....|.....|.....|001000|00000|111010|");
    DECODE_ENTRY(17,        CMPGEU_R1, "|.....|.....|.....|101000|00000|111010|");
    DECODE_ENTRY(17,         CMPLT_R1, "|.....|.....|.....|010000|00000|111010|");
    DECODE_ENTRY(17,        CMPLTU_R1, "|.....|.....|.....|110000|00000|111010|");
    DECODE_ENTRY(17,         CMPNE_R1, "|.....|.....|.....|011000|00000|111010|");
    DECODE_ENTRY(17,           DIV_R1, "|.....|.....|.....|100101|00000|111010|");
    DECODE_ENTRY(17,          DIVU_R1, "|.....|.....|.....|100100|00000|111010|");
    DECODE_ENTRY(17,           MUL_R1, "|.....|.....|.....|100111|00000|111010|");
    DECODE_ENTRY(17,        MULXSS_R1, "|.....|.....|.....|011111|00000|111010|");
    DECODE_ENTRY(17,        MULXSU_R1, "|.....|.....|.....|010111|00000|111010|");
    DECODE_ENTRY(17,        MULXUU_R1, "|.....|.....|.....|000111|00000|111010|");
    DECODE_ENTRY(17,           NOR_R1, "|.....|.....|.....|000110|00000|111010|");
    DECODE_ENTRY(17,            OR_R1, "|.....|.....|.....|010110|00000|111010|");
    DECODE_ENTRY(17,           ROL_R1, "|.....|.....|.....|000011|00000|111010|");
    DECODE_ENTRY(17,           ROR_R1, "|.....|.....|.....|001011|00000|111010|");
    DECODE_ENTRY(17,           SLL_R1, "|.....|.....|.....|010011|00000|111010|");
    DECODE_ENTRY(17,           SRA_R1, "|.....|.....|.....|111011|00000|111010|");
    DECODE_ENTRY(17,           SRL_R1, "|.....|.....|.....|011011|00000|111010|");
    DECODE_ENTRY(17,           SUB_R1, "|.....|.....|.....|111001|00000|111010|");
    DECODE_ENTRY(17,           XOR_R1, "|.....|.....|.....|011110|00000|111010|");

    //                             R2   |    A|   ZB|    C|   OPX|   ZC|    OP|
    DECODE_ENTRY(22,           MOV_R2, "|.....|00000|.....|110001|00000|111010|");
    DECODE_ENTRY(22,         WRPRS_R2, "|.....|00000|.....|010100|00000|111010|");

    //                            R2U   |    A|   ZB|    C|   OPX|UIMM5|    OP|
    DECODE_ENTRY(17,         ROLI_R2U, "|.....|00000|.....|000010|.....|111010|");
    DECODE_ENTRY(17,         SLLI_R2U, "|.....|00000|.....|010010|.....|111010|");
    DECODE_ENTRY(17,         SRAI_R2U, "|.....|00000|.....|111010|.....|111010|");
    DECODE_ENTRY(17,         SRLI_R2U, "|.....|00000|.....|011010|.....|111010|");

    //                             R3   |   ZA|   ZB|    C|   OPX|   ZC|    OP|
    DECODE_ENTRY(27,        NEXTPC_R3, "|00000|00000|.....|011100|00000|111010|");

    //                             R4   |    A|   ZB|  OPY|   OPX|   ZC|    OP|
    DECODE_ENTRY(27,         CALLR_R4, "|.....|00000|11111|011101|00000|111010|");
    DECODE_ENTRY(27,        FLUSHI_R4, "|.....|00000|00000|001100|00000|111010|");
    DECODE_ENTRY(27,         INITI_R4, "|.....|00000|00000|101001|00000|111010|");
    DECODE_ENTRY(27,           JMP_R4, "|.....|00000|00000|001101|00000|111010|");

    //                             R5   |   ZA|   ZB|    C|   OPX|    N|    OP|
    DECODE_ENTRY(22,         RDCTL_R5, "|00000|00000|.....|100110|.....|111010|");

    //                             R6   |    A|   ZB|   ZC|   OPX|    N|    OP|
    DECODE_ENTRY(22,         WRCTL_R6, "|.....|00000|00000|101110|.....|111010|");

    //                             RU   |   ZA|   ZB|  OPY|   OPX|UIMM5|    OP|
    DECODE_ENTRY(27,         BREAK_RU, "|00000|00000|11110|110100|.....|111010|");
    DECODE_ENTRY(27,          TRAP_RU, "|00000|00000|11101|101101|.....|111010|");

    return table;
}


//
// Initialization of the info structure per instruction
//
void Nios_IIGenInstructionInfo (Nios_IIInstructionInfoP info) {
    info->opcode = opcode[info->type];
    switch (info->type) {
        case NIOS_II_IT_CUSTOM_C1:
            info->A = VECINDEX(info->instruction,31,27,0); // 31:27
            info->B = VECINDEX(info->instruction,26,22,0); // 26:22
            info->C = VECINDEX(info->instruction,21,17,0); // 21:17
            info->N = VECINDEX(info->instruction,13,6,0); // 13:6
            info->readra = VECINDEX(info->instruction,16,16,0); // 16
            info->readrb = VECINDEX(info->instruction,15,15,0); // 15
            info->readrc = VECINDEX(info->instruction,14,14,0); // 14
            break;

        case NIOS_II_IT_BRET_I0:
        case NIOS_II_IT_ERET_I0:
        case NIOS_II_IT_FLUSHP_I0:
        case NIOS_II_IT_NOP_I0:
        case NIOS_II_IT_RET_I0:
        case NIOS_II_IT_SYNC_I0:
            break;

        case NIOS_II_IT_ADDI_I1S:
        case NIOS_II_IT_CMPEQI_I1S:
        case NIOS_II_IT_CMPGEI_I1S:
        case NIOS_II_IT_CMPLTI_I1S:
        case NIOS_II_IT_CMPNEI_I1S:
        case NIOS_II_IT_MULI_I1S:
        case NIOS_II_IT_RDPRS_I1S:
            info->A = VECINDEX(info->instruction,31,27,0); // 31:27
            info->B = VECINDEX(info->instruction,26,22,0); // 26:22
            info->SIMM16 = VECINDEX(info->instruction,21,6,0); // 21:6
            info->SIMM16 = SEXTEND(info->SIMM16,15);
            break;

        case NIOS_II_IT_ANDI_I1U:
        case NIOS_II_IT_CMPGEUI_I1U:
        case NIOS_II_IT_CMPLTUI_I1U:
        case NIOS_II_IT_ORI_I1U:
        case NIOS_II_IT_XORI_I1U:
            info->A = VECINDEX(info->instruction,31,27,0); // 31:27
            info->B = VECINDEX(info->instruction,26,22,0); // 26:22
            info->UIMM16 = VECINDEX(info->instruction,21,6,0); // 21:6
            break;

        case NIOS_II_IT_MOVI_I2S:
            info->B = VECINDEX(info->instruction,26,22,0); // 26:22
            info->SIMM16 = VECINDEX(info->instruction,21,6,0); // 21:6
            info->SIMM16 = SEXTEND(info->SIMM16,15);
            break;

        case NIOS_II_IT_ANDHI_I2U:
        case NIOS_II_IT_ORHI_I2U:
        case NIOS_II_IT_XORHI_I2U:
            info->A = VECINDEX(info->instruction,31,27,0); // 31:27
            info->B = VECINDEX(info->instruction,26,22,0); // 26:22
            info->UIMM16 = VECINDEX(info->instruction,21,6,0); // 21:6
            info->UIMM16 = SHIFTL(info->UIMM16,16);
            break;

        case NIOS_II_IT_FLUSHD_I3S:
        case NIOS_II_IT_FLUSHDA_I3S:
        case NIOS_II_IT_INITD_I3S:
        case NIOS_II_IT_INITDA_I3S:
            info->A = VECINDEX(info->instruction,31,27,0); // 31:27
            info->SIMM16 = VECINDEX(info->instruction,21,6,0); // 21:6
            info->SIMM16 = SEXTEND(info->SIMM16,15);
            break;

        case NIOS_II_IT_MOVHI_I3U:
            info->B = VECINDEX(info->instruction,26,22,0); // 26:22
            info->UIMM16 = VECINDEX(info->instruction,21,6,0); // 21:6
            info->UIMM16 = SHIFTL(info->UIMM16,16);
            break;

        case NIOS_II_IT_LDB_I4S:
        case NIOS_II_IT_LDBIO_I4S:
        case NIOS_II_IT_LDBU_I4S:
        case NIOS_II_IT_LDBUIO_I4S:
        case NIOS_II_IT_LDH_I4S:
        case NIOS_II_IT_LDHIO_I4S:
        case NIOS_II_IT_LDHU_I4S:
        case NIOS_II_IT_LDHUIO_I4S:
        case NIOS_II_IT_LDW_I4S:
        case NIOS_II_IT_LDWIO_I4S:
        case NIOS_II_IT_STB_I4S:
        case NIOS_II_IT_STBIO_I4S:
        case NIOS_II_IT_STH_I4S:
        case NIOS_II_IT_STHIO_I4S:
        case NIOS_II_IT_STW_I4S:
        case NIOS_II_IT_STWIO_I4S:
            info->A = VECINDEX(info->instruction,31,27,0); // 31:27
            info->B = VECINDEX(info->instruction,26,22,0); // 26:22
            info->SIMM16 = VECINDEX(info->instruction,21,6,0); // 21:6
            info->SIMM16 = SEXTEND(info->SIMM16,15);
            break;

        case NIOS_II_IT_MOVUI_I4U:
            info->B = VECINDEX(info->instruction,26,22,0); // 26:22
            info->UIMM16 = VECINDEX(info->instruction,21,6,0); // 21:6
            break;

        case NIOS_II_IT_BEQ_IB1:
        case NIOS_II_IT_BGE_IB1:
        case NIOS_II_IT_BGEU_IB1:
        case NIOS_II_IT_BLT_IB1:
        case NIOS_II_IT_BLTU_IB1:
        case NIOS_II_IT_BNE_IB1:
            info->A = VECINDEX(info->instruction,31,27,0); // 31:27
            info->B = VECINDEX(info->instruction,26,22,0); // 26:22
            info->SIMM16 = VECINDEX(info->instruction,21,6,0); // 21:6
            info->SIMM16 = SEXTEND(info->SIMM16,15);
            break;

        case NIOS_II_IT_BR_IB2:
            info->SIMM16 = VECINDEX(info->instruction,21,6,0); // 21:6
            info->SIMM16 = SEXTEND(info->SIMM16,15);
            break;

        case NIOS_II_IT_CALL_J1:
        case NIOS_II_IT_JMPI_J1:
            info->UIMM26 = VECINDEX(info->instruction,31,6,0); // 31:6
            info->UIMM26 = SHIFTL(info->UIMM26,2);
            break;

        case NIOS_II_IT_ADD_R1:
        case NIOS_II_IT_AND_R1:
        case NIOS_II_IT_CMPEQ_R1:
        case NIOS_II_IT_CMPGE_R1:
        case NIOS_II_IT_CMPGEU_R1:
        case NIOS_II_IT_CMPLT_R1:
        case NIOS_II_IT_CMPLTU_R1:
        case NIOS_II_IT_CMPNE_R1:
        case NIOS_II_IT_DIV_R1:
        case NIOS_II_IT_DIVU_R1:
        case NIOS_II_IT_MUL_R1:
        case NIOS_II_IT_MULXSS_R1:
        case NIOS_II_IT_MULXSU_R1:
        case NIOS_II_IT_MULXUU_R1:
        case NIOS_II_IT_NOR_R1:
        case NIOS_II_IT_OR_R1:
        case NIOS_II_IT_ROL_R1:
        case NIOS_II_IT_ROR_R1:
        case NIOS_II_IT_SLL_R1:
        case NIOS_II_IT_SRA_R1:
        case NIOS_II_IT_SRL_R1:
        case NIOS_II_IT_SUB_R1:
        case NIOS_II_IT_XOR_R1:
            info->A = VECINDEX(info->instruction,31,27,0); // 31:27
            info->B = VECINDEX(info->instruction,26,22,0); // 26:22
            info->C = VECINDEX(info->instruction,21,17,0); // 21:17
            break;

        case NIOS_II_IT_MOV_R2:
        case NIOS_II_IT_WRPRS_R2:
            info->A = VECINDEX(info->instruction,31,27,0); // 31:27
            info->C = VECINDEX(info->instruction,21,17,0); // 21:17
            break;

        case NIOS_II_IT_ROLI_R2U:
        case NIOS_II_IT_SLLI_R2U:
        case NIOS_II_IT_SRAI_R2U:
        case NIOS_II_IT_SRLI_R2U:
            info->A = VECINDEX(info->instruction,31,27,0); // 31:27
            info->C = VECINDEX(info->instruction,21,17,0); // 21:17
            info->UIMM5 = VECINDEX(info->instruction,10,6,0); // 10:6
            break;

        case NIOS_II_IT_NEXTPC_R3:
            info->C = VECINDEX(info->instruction,21,17,0); // 21:17
            break;

        case NIOS_II_IT_CALLR_R4:
        case NIOS_II_IT_FLUSHI_R4:
        case NIOS_II_IT_INITI_R4:
        case NIOS_II_IT_JMP_R4:
            info->A = VECINDEX(info->instruction,31,27,0); // 31:27
            break;

        case NIOS_II_IT_RDCTL_R5:
            info->C = VECINDEX(info->instruction,21,17,0); // 21:17
            info->N = VECINDEX(info->instruction,10,6,0); // 10:6
            break;

        case NIOS_II_IT_WRCTL_R6:
            info->A = VECINDEX(info->instruction,31,27,0); // 31:27
            info->N = VECINDEX(info->instruction,10,6,0); // 10:6
            break;

        case NIOS_II_IT_BREAK_RU:
        case NIOS_II_IT_TRAP_RU:
            info->UIMM5 = VECINDEX(info->instruction,10,6,0); // 10:6
            break;

        case NIOS_II_IT_INSTR32:
        case NIOS_II_IT_LAST:
            break;
    }
    return;
}
