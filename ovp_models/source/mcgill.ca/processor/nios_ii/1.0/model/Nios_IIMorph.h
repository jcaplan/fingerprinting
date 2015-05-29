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

#ifndef NIOS_II_MORPH_H
#define NIOS_II_MORPH_H

#include "vmi/vmiTypes.h"
#include "Nios_IIDecode.h"

#define NIOS_II_MORPH_FN(_NAME) void _NAME(Nios_IIMorphStateP state)
typedef NIOS_II_MORPH_FN((*Nios_IIMorphFn));

//
// Write Default Morpher stub Functions
//
NIOS_II_MORPH_FN(morphCUSTOM_C1);
NIOS_II_MORPH_FN(morphBRET_I0);
NIOS_II_MORPH_FN(morphERET_I0);
NIOS_II_MORPH_FN(morphFLUSHP_I0);
NIOS_II_MORPH_FN(morphNOP_I0);
NIOS_II_MORPH_FN(morphRET_I0);
NIOS_II_MORPH_FN(morphSYNC_I0);
NIOS_II_MORPH_FN(morphADDI_I1S);
NIOS_II_MORPH_FN(morphCMPEQI_I1S);
NIOS_II_MORPH_FN(morphCMPGEI_I1S);
NIOS_II_MORPH_FN(morphCMPLTI_I1S);
NIOS_II_MORPH_FN(morphCMPNEI_I1S);
NIOS_II_MORPH_FN(morphMULI_I1S);
NIOS_II_MORPH_FN(morphRDPRS_I1S);
NIOS_II_MORPH_FN(morphANDI_I1U);
NIOS_II_MORPH_FN(morphCMPGEUI_I1U);
NIOS_II_MORPH_FN(morphCMPLTUI_I1U);
NIOS_II_MORPH_FN(morphORI_I1U);
NIOS_II_MORPH_FN(morphXORI_I1U);
NIOS_II_MORPH_FN(morphMOVI_I2S);
NIOS_II_MORPH_FN(morphANDHI_I2U);
NIOS_II_MORPH_FN(morphORHI_I2U);
NIOS_II_MORPH_FN(morphXORHI_I2U);
NIOS_II_MORPH_FN(morphFLUSHD_I3S);
NIOS_II_MORPH_FN(morphFLUSHDA_I3S);
NIOS_II_MORPH_FN(morphINITD_I3S);
NIOS_II_MORPH_FN(morphINITDA_I3S);
NIOS_II_MORPH_FN(morphMOVHI_I3U);
NIOS_II_MORPH_FN(morphLDB_I4S);
NIOS_II_MORPH_FN(morphLDBIO_I4S);
NIOS_II_MORPH_FN(morphLDBU_I4S);
NIOS_II_MORPH_FN(morphLDBUIO_I4S);
NIOS_II_MORPH_FN(morphLDH_I4S);
NIOS_II_MORPH_FN(morphLDHIO_I4S);
NIOS_II_MORPH_FN(morphLDHU_I4S);
NIOS_II_MORPH_FN(morphLDHUIO_I4S);
NIOS_II_MORPH_FN(morphLDW_I4S);
NIOS_II_MORPH_FN(morphLDWIO_I4S);
NIOS_II_MORPH_FN(morphSTB_I4S);
NIOS_II_MORPH_FN(morphSTBIO_I4S);
NIOS_II_MORPH_FN(morphSTH_I4S);
NIOS_II_MORPH_FN(morphSTHIO_I4S);
NIOS_II_MORPH_FN(morphSTW_I4S);
NIOS_II_MORPH_FN(morphSTWIO_I4S);
NIOS_II_MORPH_FN(morphMOVUI_I4U);
NIOS_II_MORPH_FN(morphBEQ_IB1);
NIOS_II_MORPH_FN(morphBGE_IB1);
NIOS_II_MORPH_FN(morphBGEU_IB1);
NIOS_II_MORPH_FN(morphBLT_IB1);
NIOS_II_MORPH_FN(morphBLTU_IB1);
NIOS_II_MORPH_FN(morphBNE_IB1);
NIOS_II_MORPH_FN(morphBR_IB2);
NIOS_II_MORPH_FN(morphCALL_J1);
NIOS_II_MORPH_FN(morphJMPI_J1);
NIOS_II_MORPH_FN(morphADD_R1);
NIOS_II_MORPH_FN(morphAND_R1);
NIOS_II_MORPH_FN(morphCMPEQ_R1);
NIOS_II_MORPH_FN(morphCMPGE_R1);
NIOS_II_MORPH_FN(morphCMPGEU_R1);
NIOS_II_MORPH_FN(morphCMPLT_R1);
NIOS_II_MORPH_FN(morphCMPLTU_R1);
NIOS_II_MORPH_FN(morphCMPNE_R1);
NIOS_II_MORPH_FN(morphDIV_R1);
NIOS_II_MORPH_FN(morphDIVU_R1);
NIOS_II_MORPH_FN(morphMUL_R1);
NIOS_II_MORPH_FN(morphMULXSS_R1);
NIOS_II_MORPH_FN(morphMULXSU_R1);
NIOS_II_MORPH_FN(morphMULXUU_R1);
NIOS_II_MORPH_FN(morphNOR_R1);
NIOS_II_MORPH_FN(morphOR_R1);
NIOS_II_MORPH_FN(morphROL_R1);
NIOS_II_MORPH_FN(morphROR_R1);
NIOS_II_MORPH_FN(morphSLL_R1);
NIOS_II_MORPH_FN(morphSRA_R1);
NIOS_II_MORPH_FN(morphSRL_R1);
NIOS_II_MORPH_FN(morphSUB_R1);
NIOS_II_MORPH_FN(morphXOR_R1);
NIOS_II_MORPH_FN(morphMOV_R2);
NIOS_II_MORPH_FN(morphWRPRS_R2);
NIOS_II_MORPH_FN(morphROLI_R2U);
NIOS_II_MORPH_FN(morphSLLI_R2U);
NIOS_II_MORPH_FN(morphSRAI_R2U);
NIOS_II_MORPH_FN(morphSRLI_R2U);
NIOS_II_MORPH_FN(morphNEXTPC_R3);
NIOS_II_MORPH_FN(morphCALLR_R4);
NIOS_II_MORPH_FN(morphFLUSHI_R4);
NIOS_II_MORPH_FN(morphINITI_R4);
NIOS_II_MORPH_FN(morphJMP_R4);
NIOS_II_MORPH_FN(morphRDCTL_R5);
NIOS_II_MORPH_FN(morphWRCTL_R6);
NIOS_II_MORPH_FN(morphBREAK_RU);
NIOS_II_MORPH_FN(morphTRAP_RU);
typedef struct Nios_IIMorphAttrS {
    Nios_IIMorphFn morph; // callback function to morph the instruction
    Nios_IIArchitecture arch;
    vmiFlagsCP flags; // flags Constant pointer
} Nios_IIMorphAttr;

typedef struct Nios_IIMorphStateS {
    Nios_IIInstructionInfo info; // instruction description - from decoder
    Nios_IIMorphAttrCP attrs; // instruction attributes
    Nios_IIP Nios_II; // current processor
    Nios_IIAddr nextPC; // next instruction address in sequence
} Nios_IIMorphState;

#endif // NIOS_II_MORPH_H
