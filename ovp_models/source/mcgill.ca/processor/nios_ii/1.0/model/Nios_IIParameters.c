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

#include "hostapi/impAlloc.h"

#include "vmi/vmiAttrs.h"
#include "vmi/vmiParameters.h"
#include "vmi/vmiMessage.h"

#include "Nios_IIFunctions.h"
#include "Nios_IIParameters.h"
#include "Nios_IIStructure.h"
#include "Nios_IIVariant.h"

static vmiEnumParameter INTERRUPT_CONTROLLER_INTERFACEValues[] = {
    {"Internal", 0},
    {"External", 1},
    {0},
};
static vmiEnumParameter MMU_TLB_ENTRIESValues[] = {
    {"128", 0},
    {"256", 1},
    {"512", 2},
    {"1024", 3},
    {0},
};
static vmiEnumParameter MMU_TLB_SET_ASSOCIATIVITYValues[] = {
    {"8", 0},
    {"16", 1},
    {0},
};

//
// Table of parameter specs
//
static vmiParameter formals[] = {
    VMI_ENUM_PARAM_SPEC(Nios_IIParamValues, variant, 0, "Selects variant (either a generic UISA or a specific model)"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, verbose, True, "Specify verbose output messages"),
    VMI_ENDIAN_PARAM_SPEC(Nios_IIParamValues, endian,   "Specify Model endian"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, TEST_MODE_EXIT, False, "Enable Test mode exit for instruction, cmpltui r0, r0, (0xabc1||0xabc2)"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, TEST_HALT_EXIT, False, "Enable a Halt on 'br 0', branch to self"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, INST_ADDR_WIDTH, 0, 0, 32, "Instruction bus Width"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, DATA_ADDR_WIDTH, 0, 0, 32, "Data bus Width"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, HW_MULTIPLY, False, "Hardware Multiply"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, HW_MULX, False, "Hardware Extended Multiply"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, HW_DIVIDE, False, "Hardware Divide"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, RESET_VECTOR, 0, 0, 0xffffffff, "Reset Vector Address"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, EXCEPTION_VECTOR, 0, 0, 0xffffffff, "Exception Vector Address"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, BREAK_VECTOR, 0, 0, 0xffffffff, "Break Vector Address"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, INCLUDE_MMU, False, "MMU Available"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, FAST_TLB_MISS_EXCEPTION_VECTOR, 0, 0, 0xffffffff, "Fast TLB Exception Vector Address"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, INCLUDE_MPU, False, "MPU Available"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, INCLUDE_CPURESETREQUEST, False, "CPU Reset Request Signal"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, INCLUDE_CPURESETTAKEN, False, "CPU Reset Taken Signal"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, CPUID_CONTROL_VALUE, 0, 0, 0xffffffff, "CPUID Control Register Value"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, EXCEPTION_ILLEGAL_INSTRUCTION, False, "Generate Illegal Instruction Exception"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, EXCEPTION_DIVISION_ERROR, False, "Generate Division Error Exception"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, EXCEPTION_MISALIGNED_MEMORY_ACCESS, False, "Generate Misaligned Memory Access Exception"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, EXCEPTION_EXTRA_INFORMATION, False, "Generate Extra Exception information"),
    VMI_ENUM_PARAM_SPEC(Nios_IIParamValues, INTERRUPT_CONTROLLER_INTERFACE, INTERRUPT_CONTROLLER_INTERFACEValues, "Interrupt Controller Interface"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, NUMBER_SHADOW_REGISTER_SETS, 0, 0, 63, "Number of Shadow Register Sets"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, HARDCOPY_COMPATIBILITY, False, "Hardcopy Compatibility"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, MMU_PID_BITS, 8, 8, 14, "MMU Process ID (PID) Bits"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, MMU_OPTIMIZE_NUMBER_OF_TLB_ENTRIES, False, "MMU Optimize Number of TLB Entries based on device family"),
    VMI_ENUM_PARAM_SPEC(Nios_IIParamValues, MMU_TLB_ENTRIES, MMU_TLB_ENTRIESValues, "MMU TLB Entries"),
    VMI_ENUM_PARAM_SPEC(Nios_IIParamValues, MMU_TLB_SET_ASSOCIATIVITY, MMU_TLB_SET_ASSOCIATIVITYValues, "MMU TLB Entries"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, MMU_MICRO_DTLB_ENTRIES, 0, 0, 0xffffffff, "MMU Micro data TLB Entries"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, MMU_MICRO_ITLB_ENTRIES, 0, 0, 0xffffffff, "MMU Micro instruction TLB Entries"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, MPU_USE_LIMIT_FOR_REGION_RANGE, False, "Controls Memory"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, MPU_NUMBER_DATA_REGIONS, 2, 2, 32, "Number of Data Regions to Allocate"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, MPU_MINIMUM_DATA_REGION_SIZE, 0, 0, 0xffffffff, "Minimum Data Region Size 64Bytes to 1MBytes (power of 2)"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, MPU_NUMBER_INSTRUCTION_REGIONS, 2, 2, 32, "Number of Instruction Regions to Allocate"),
    VMI_UNS32_PARAM_SPEC(Nios_IIParamValues, MPU_MINIMUM_INSTRUCTION_REGION_SIZE, 0, 0, 0xffffffff, "Minimum Instruction Region Size 64Bytes to 1MBytes (power of 2)"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, CUSTOM_FP_DIVISION, False, "Enable Custom Hardware for FP Division Instruction"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, CUSTOM_BITSWAP, False, "Enable Custom Hardware for Bit Swap Instruction"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, CUSTOM_ENDIAN_CONVERT, False, "Enable Custom Hardware for Endian Conversion Instruction"),
    VMI_BOOL_PARAM_SPEC(Nios_IIParamValues, CUSTOM_INTERRUPT_VECTOR, False, "Enable Custom Interrupt Vector Instruction"),
    VMI_END_PARAM
};

static Uns32 countVariants(void) {
    Nios_IIConfigCP cfg = Nios_IIConfigTable;
    Uns32 i = 0;
    while(cfg->name) {
        cfg++;
        i++;
    }
    return i;
}

//
// First time through, malloc and fill the variant list from the config table
//
static vmiEnumParameterP getVariantList() {
    static vmiEnumParameterP list = NULL;
    if (!list) {
        Uns32 v = 1 + countVariants();
        list = STYPE_CALLOC_N(vmiEnumParameter, v);
        vmiEnumParameterP prm;
        Nios_IIConfigCP cfg;
        Uns32 i;
        for (i = 0, cfg = Nios_IIConfigTable, prm = list;
             cfg->name;
             i++, cfg++, prm++) {
            prm->name = cfg->name;
            prm->value = i;
        }
    }
    return list;
}

//
// First time through, fill the formals table
//
static vmiParameterP getFormals(void) {
    static Bool first = True;
    if(first) {
        first = False;
        formals[0].u.enumParam.legalValues = getVariantList();
    }
    return formals;
}


//
// Function to iterate the parameter specs
//
VMI_PROC_PARAM_SPECS_FN(Nios_IIParamSpecs) {
    if(!prev) {
        return getFormals();
    } else {
        prev++;
        if (prev->name)
            return prev;
        else
            return 0;
    }
}

//
// Get the size of the parameter values table
//
VMI_PROC_PARAM_TABLE_SIZE_FN(Nios_IIParamValueSize) {
    return sizeof(Nios_IIParamValues);
}

