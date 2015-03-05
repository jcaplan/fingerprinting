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

#include "vmi/vmiCxt.h"
#include "Nios_IIExceptions.h"
#include "Nios_IIFunctions.h"
#include "Nios_IIDecode.h"

VMI_ENDIAN_FN(Nios_IIGetEndian) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    if (isFetch) {
        return Nios_II->iendian;
    } else {
        return Nios_II->dendian;
    }
}

VMI_NEXT_PC_FN(Nios_IInextPC) {
    Nios_IIInstructionInfo info = {0};
    Nios_IIDecode((Nios_IIP)processor, thisPC, &info);
    Nios_IIAddr nextAddress = info.nextPC;
    return nextAddress;
}

VMI_IASSWITCH_FN(Nios_IISwitch) {
}

//
// Table of processor mode descriptions
//
static const vmiModeInfo modes[] = {
    { "VM_MODE_KERNEL",      VM_MODE_KERNEL,      "Supervisor Mode" },
    { "VM_MODE_USER",        VM_MODE_USER,        "User Mode" },
    { "VM_MODE_KERNEL_MPU",  VM_MODE_KERNEL_MPU,  "Supervisor Mode MPU" },
    { "VM_MODE_USER_MPU",    VM_MODE_USER_MPU,    "User Mode MPU" },
    { 0, 0, 0 }
};


VMI_GET_MODE_FN(Nios_IIGetMode) {
    return &modes[Nios_IIGetVMMode((Nios_IIP)processor)];
}

VMI_MODE_INFO_FN(Nios_IIModeInfo) {

    // on the first call, start with the first member of the table
    if(!prev) {
        prev = modes-1;
    }

    // get the next member
    vmiModeInfoCP this = prev+1;

    // return the next member, or NULL if at the end of the list
    return (this->name) ? this : 0;
}
