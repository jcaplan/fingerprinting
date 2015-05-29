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

#include <string.h>
#include "vmi/vmiAttrs.h"
#include "vmi/vmiMessage.h"
#include "vmi/vmiRt.h"
#include "Nios_IIPorts.h"
#include "Nios_IIBus.h"
#include "Nios_IIConfig.h"
#include "Nios_IIDecode.h"
#include "Nios_IIDoc.h"
#include "Nios_IIExceptions.h"
#include "Nios_IIFunctions.h"
#include "Nios_IIStructure.h"
#include "Nios_IIParameters.h"
#include "Nios_IITLB.h"
#include "Nios_IIMPU.h"


static VMIRT_COMMAND_FN(dumpTLB) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_IIPrintTLB(Nios_II);
    return "1";
}

VMI_CONSTRUCTOR_FN(Nios_IIConstructor) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_IIParamValuesP params = (Nios_IIParamValuesP)parameterValues;

    Nios_II->flags = vmirtProcessorFlags(processor);
    Nios_II->configInfo = Nios_IIConfigTable[params->variant];
    if (Nios_II->configInfo.name == NULL) {
        // Assign value as First Variant
        Nios_II->configInfo = Nios_IIConfigTable[0];
    }
    Int32 iter;
    for (iter=0; iter<32; iter++) {
        Nios_II->gpr[iter] = 0;
    }
    for (iter=0; iter<8; iter++) {
        Nios_II->TEMP[iter] = 0;
    }

    Nios_II->iendian = SETBIT(params->endian) ? params->endian : MEM_ENDIAN_LITTLE;
    Nios_II->dendian = SETBIT(params->endian) ? params->endian : MEM_ENDIAN_LITTLE;
    Nios_II->exception = EXC_NONE;

#define COPY_PARAM(_name)  \
    Nios_II->params._name        = params->_name; \
    Nios_II->params._name ## set = params->SETBIT(_name);

    // Store the parameters for later retrieval
    COPY_PARAM(HW_MULTIPLY)
    COPY_PARAM(HW_MULX)
    COPY_PARAM(TEST_MODE_EXIT)
    COPY_PARAM(TEST_HALT_EXIT)
    COPY_PARAM(INST_ADDR_WIDTH)
    COPY_PARAM(DATA_ADDR_WIDTH)
    COPY_PARAM(HW_DIVIDE)
    COPY_PARAM(RESET_VECTOR)
    COPY_PARAM(EXCEPTION_VECTOR)
    COPY_PARAM(BREAK_VECTOR)
    COPY_PARAM(INCLUDE_MMU)
    COPY_PARAM(FAST_TLB_MISS_EXCEPTION_VECTOR)
    COPY_PARAM(INCLUDE_MPU)
    COPY_PARAM(INCLUDE_CPURESETREQUEST)
    COPY_PARAM(INCLUDE_CPURESETTAKEN)
    COPY_PARAM(CPUID_CONTROL_VALUE)
    COPY_PARAM(EXCEPTION_ILLEGAL_INSTRUCTION)
    COPY_PARAM(EXCEPTION_DIVISION_ERROR)
    COPY_PARAM(EXCEPTION_MISALIGNED_MEMORY_ACCESS)
    COPY_PARAM(EXCEPTION_EXTRA_INFORMATION)
    COPY_PARAM(INTERRUPT_CONTROLLER_INTERFACE)
    COPY_PARAM(NUMBER_SHADOW_REGISTER_SETS)
    COPY_PARAM(HARDCOPY_COMPATIBILITY)
    COPY_PARAM(MMU_PID_BITS)
    COPY_PARAM(MMU_OPTIMIZE_NUMBER_OF_TLB_ENTRIES)
    COPY_PARAM(MMU_TLB_ENTRIES)
    COPY_PARAM(MMU_TLB_SET_ASSOCIATIVITY)
    COPY_PARAM(MMU_MICRO_DTLB_ENTRIES)
    COPY_PARAM(MMU_MICRO_ITLB_ENTRIES)
    COPY_PARAM(MPU_USE_LIMIT_FOR_REGION_RANGE)
    COPY_PARAM(MPU_NUMBER_DATA_REGIONS)
    COPY_PARAM(MPU_MINIMUM_DATA_REGION_SIZE)
    COPY_PARAM(MPU_NUMBER_INSTRUCTION_REGIONS)
    COPY_PARAM(MPU_MINIMUM_INSTRUCTION_REGION_SIZE)
    COPY_PARAM(CUSTOM_FP_DIVISION)
    COPY_PARAM(CUSTOM_BITSWAP)
    COPY_PARAM(CUSTOM_ENDIAN_CONVERT)
    COPY_PARAM(CUSTOM_INTERRUPT_VECTOR)

    //
    // Initialize the MMU (if present)
    //
    if (Nios_II->params.INCLUDE_MMU) {
        Nios_IIConstructMMU(Nios_II);
    }

    //
    // Initialize the MPU (if present)
    //
    if (Nios_II->params.INCLUDE_MPU) {
        Nios_IIConstructMPU(Nios_II);
    }

    // Allocate net port descriptions
    Nios_IINewNetPorts(Nios_II);

    // Allocate Bus Port Descriptors
    Nios_IINewBusPorts(Nios_II);

    // Call the Doc Generator
    Nios_IIDoc(processor, params);

    // Link the Decoder CallBack
    Nios_II->decoder = Nios_IIDecode;

    // Install Commands
    vmirtAddCommand(processor, "dumpTLB", "- no arguments", dumpTLB, VMI_CT_QUERY|VMI_CO_TLB|VMI_CA_QUERY);

    // Cause a reset exception
    Nios_IIProcessReset(Nios_II);
}

VMI_DESTRUCTOR_FN(Nios_IIDestructor) {
    Nios_IIP Nios_II = (Nios_IIP)processor;

    // Free net port descriptions
    Nios_IIFreeNetPorts(Nios_II);

    // Free Bus Port Descriptors
    Nios_IIFreeBusPorts(Nios_II);

    // Free The MMU
    Nios_IIDestructMMU(Nios_II);
}
