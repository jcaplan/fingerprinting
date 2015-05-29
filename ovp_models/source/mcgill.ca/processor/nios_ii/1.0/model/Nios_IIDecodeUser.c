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
#include "Nios_IIStructure.h"

//
// Decoder function for a given address
//
void Nios_IIDecode(
    Nios_IIP Nios_II,
    Nios_IIAddr thisPC,
    Nios_IIInstructionInfoP info
) {
    //
    // Initialize the following info fields before table creation
    //
    info->thisPC = thisPC;
    info->arch = Nios_II->configInfo.arch;
    vmiProcessorP processor = (vmiProcessorP)Nios_II;

    //
    // Initialize the UISA decoder table
    //
    static vmidDecodeTableP decodeTable_UISA_32Bit;
    if(!decodeTable_UISA_32Bit && (info->arch == UISA)) {
        decodeTable_UISA_32Bit = Nios_IICreateDecodeTable_ISA_0_32Bit(decodeTable_UISA_32Bit);
    }

    //
    // Initialize the Nios_II_F_32Bit decoder table
    //
    static vmidDecodeTableP decodeTable_Nios_II_F_32Bit;
    if(!decodeTable_Nios_II_F_32Bit && (info->arch == NIOS_II_F)) {
        decodeTable_Nios_II_F_32Bit = Nios_IICreateDecodeTable_ISA_0_32Bit(decodeTable_Nios_II_F_32Bit);
    }

    //
    // Initialize the Nios_II_S_32Bit decoder table
    //
    static vmidDecodeTableP decodeTable_Nios_II_S_32Bit;
    if(!decodeTable_Nios_II_S_32Bit && (info->arch == NIOS_II_S)) {
        decodeTable_Nios_II_S_32Bit = Nios_IICreateDecodeTable_ISA_0_32Bit(decodeTable_Nios_II_S_32Bit);
    }

    //
    // Initialize the Nios_II_E_32Bit decoder table
    //
    static vmidDecodeTableP decodeTable_Nios_II_E_32Bit;
    if(!decodeTable_Nios_II_E_32Bit && (info->arch == NIOS_II_E)) {
        decodeTable_Nios_II_E_32Bit = Nios_IICreateDecodeTable_ISA_0_32Bit(decodeTable_Nios_II_E_32Bit);
    }

    //
    // Initialize the following info fields before lookup
    //

    info->instrsize = 0;
    info->type = NIOS_II_IT_LAST;
    Uns64 instruction = 0;

    if (info->arch == UISA) {
        if (info->type == NIOS_II_IT_LAST) info->type = NIOS_II_IT_INSTR32;
        //
        // Attempt to match a UISA 32 Bit instruction
        //
        if (info->type == NIOS_II_IT_INSTR32) {
            instruction  = ((Uns64)vmicxtFetch4Byte(processor, thisPC+0) << 0) | instruction;
            info->type = NIOS_II_IT_LAST;
            if (decodeTable_UISA_32Bit) {
                info->type = vmidDecode(decodeTable_UISA_32Bit, instruction);
            }
            info->nextPC = info->thisPC + 4;
            info->instrsize = 4;
            info->instruction = instruction;
        }
    }

    if (info->arch == NIOS_II_F) {
        if (info->type == NIOS_II_IT_LAST) info->type = NIOS_II_IT_INSTR32;
        //
        // Attempt to match a Nios_II_F 32 Bit instruction
        //
        if (info->type == NIOS_II_IT_INSTR32) {
            instruction  = ((Uns64)vmicxtFetch4Byte(processor, thisPC+0) << 0) | instruction;
            info->type = NIOS_II_IT_LAST;
            info->type = vmidDecode(decodeTable_Nios_II_F_32Bit, instruction);
            info->nextPC = info->thisPC + 4;
            info->instrsize = 4;
            info->instruction = instruction;
        }
    }

    if (info->arch == NIOS_II_S) {
        if (info->type == NIOS_II_IT_LAST) info->type = NIOS_II_IT_INSTR32;
        //
        // Attempt to match a Nios_II_S 32 Bit instruction
        //
        if (info->type == NIOS_II_IT_INSTR32) {
            instruction  = ((Uns64)vmicxtFetch4Byte(processor, thisPC+0) << 0) | instruction;
            info->type = NIOS_II_IT_LAST;
            info->type = vmidDecode(decodeTable_Nios_II_S_32Bit, instruction);
            info->nextPC = info->thisPC + 4;
            info->instrsize = 4;
            info->instruction = instruction;
        }
    }

    if (info->arch == NIOS_II_E) {
        if (info->type == NIOS_II_IT_LAST) info->type = NIOS_II_IT_INSTR32;
        //
        // Attempt to match a Nios_II_E 32 Bit instruction
        //
        if (info->type == NIOS_II_IT_INSTR32) {
            instruction  = ((Uns64)vmicxtFetch4Byte(processor, thisPC+0) << 0) | instruction;
            info->type = NIOS_II_IT_LAST;
            info->type = vmidDecode(decodeTable_Nios_II_E_32Bit, instruction);
            info->nextPC = info->thisPC + 4;
            info->instrsize = 4;
            info->instruction = instruction;
        }
    }

    Nios_IIGenInstructionInfo(info);
}
