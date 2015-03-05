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

#include "vmi/vmiAttrs.h"
#include "vmi/vmiMessage.h"
#include "vmi/vmiRt.h"
#include "vmi/vmiTypes.h"
#include "Nios_IIVM.h"
#include "Nios_IIStructure.h"

VMI_VMINIT_FN(Nios_IIVMInit) {
    Nios_IIP   Nios_II     = (Nios_IIP)processor;
    Uns32      bits        = NIOS_II_GPR_BITS;
    memDomainP physDomainC = codeDomains[0];
    memDomainP physDomainD = dataDomains[0];

    Nios_II->physDomainC = physDomainC;
    Nios_II->physDomainD = physDomainD;

    if (Nios_II->params.INCLUDE_MMU) {

        memDomainP virtDomainKC = vmirtNewDomain("virtDomainKC", bits);
        memDomainP virtDomainKD = vmirtNewDomain("virtDomainKD", bits);
        memDomainP virtDomainUC = vmirtNewDomain("virtDomainUC", bits);
        memDomainP virtDomainUD = vmirtNewDomain("virtDomainUD", bits);

        Nios_II->virtDomainC[VM_MODE_KERNEL] = virtDomainKC;
        Nios_II->virtDomainD[VM_MODE_KERNEL] = virtDomainKD;
        Nios_II->virtDomainC[VM_MODE_USER]   = virtDomainUC;
        Nios_II->virtDomainD[VM_MODE_USER]   = virtDomainUD;

        vmirtAliasMemory(physDomainC, virtDomainKC, 0x00000000, 0x1fffffff, 0xc0000000, 0);
        vmirtAliasMemory(physDomainC, virtDomainKC, 0x00000000, 0x1fffffff, 0xe0000000, 0);
        vmirtAliasMemory(physDomainD, virtDomainKD, 0x00000000, 0x1fffffff, 0xc0000000, 0);
        vmirtAliasMemory(physDomainD, virtDomainKD, 0x00000000, 0x1fffffff, 0xe0000000, 0);

        codeDomains[VM_MODE_KERNEL] = virtDomainKC;
        dataDomains[VM_MODE_KERNEL] = virtDomainKD;
        codeDomains[VM_MODE_USER]   = virtDomainUC;
        dataDomains[VM_MODE_USER]   = virtDomainUD;

    } else if (Nios_II->params.INCLUDE_MPU) {

        memDomainP virtDomainKC     = vmirtNewDomain("virtDomainKC",     bits);
        memDomainP virtDomainKD     = vmirtNewDomain("virtDomainKD",     bits);
        memDomainP virtDomainKC_MPU = vmirtNewDomain("virtDomainKC_MPU", bits);
        memDomainP virtDomainKD_MPU = vmirtNewDomain("virtDomainKD_MPU", bits);
        memDomainP virtDomainUC_MPU = vmirtNewDomain("virtDomainUC_MPU", bits);
        memDomainP virtDomainUD_MPU = vmirtNewDomain("virtDomainUD_MPU", bits);

        Nios_II->virtDomainC[VM_MODE_KERNEL]     = virtDomainKC;
        Nios_II->virtDomainD[VM_MODE_KERNEL]     = virtDomainKD;
        Nios_II->virtDomainC[VM_MODE_KERNEL_MPU] = virtDomainKC_MPU;
        Nios_II->virtDomainD[VM_MODE_KERNEL_MPU] = virtDomainKD_MPU;
        Nios_II->virtDomainC[VM_MODE_USER_MPU]   = virtDomainUC_MPU;
        Nios_II->virtDomainD[VM_MODE_USER_MPU]   = virtDomainUD_MPU;

        //
        // Bit 31 cache DATA bypass
        //
        Addr lowPA   = 0x00000000;
        Addr highPAD = 0x7fffffff;  // This is to accomodate bit 31 cache bypass
        Addr highPAC = 0xffffffff;
        vmirtAliasMemory(physDomainC, virtDomainKC, lowPA, highPAC, 0x00000000, 0);
        vmirtAliasMemory(physDomainD, virtDomainKD, lowPA, highPAD, 0x00000000, 0);
        vmirtAliasMemory(physDomainD, virtDomainKD, lowPA, highPAD, 0x80000000, 0);

        vmirtAliasMemory(physDomainC, virtDomainKC_MPU, lowPA, highPAC, 0x00000000, 0);
        vmirtAliasMemory(physDomainD, virtDomainKD_MPU, lowPA, highPAD, 0x00000000, 0);
        vmirtAliasMemory(physDomainD, virtDomainKD_MPU, lowPA, highPAD, 0x80000000, 0);

        vmirtAliasMemory(physDomainC, virtDomainUC_MPU, lowPA, highPAC, 0x00000000, 0);
        vmirtAliasMemory(physDomainD, virtDomainUD_MPU, lowPA, highPAD, 0x00000000, 0);
        vmirtAliasMemory(physDomainD, virtDomainUD_MPU, lowPA, highPAD, 0x80000000, 0);

        vmirtProtectMemory(virtDomainKC_MPU, 0x00000000, 0xffffffff, MEM_PRIV_NONE, MEM_PRIV_SET);
        vmirtProtectMemory(virtDomainKD_MPU, 0x00000000, 0xffffffff, MEM_PRIV_NONE, MEM_PRIV_SET);
        vmirtProtectMemory(virtDomainUC_MPU, 0x00000000, 0xffffffff, MEM_PRIV_NONE, MEM_PRIV_SET);
        vmirtProtectMemory(virtDomainUD_MPU, 0x00000000, 0xffffffff, MEM_PRIV_NONE, MEM_PRIV_SET);

        codeDomains[VM_MODE_KERNEL]     = virtDomainKC;
        dataDomains[VM_MODE_KERNEL]     = virtDomainKD;
        codeDomains[VM_MODE_KERNEL_MPU] = virtDomainKC_MPU;
        dataDomains[VM_MODE_KERNEL_MPU] = virtDomainKD_MPU;
        codeDomains[VM_MODE_USER_MPU]   = virtDomainUC_MPU;
        dataDomains[VM_MODE_USER_MPU]   = virtDomainUD_MPU;

    } else {

        memDomainP virtDomainKC = vmirtNewDomain("virtDomainKC", bits);
        memDomainP virtDomainKD = vmirtNewDomain("virtDomainKD", bits);

        //
        // If there is no MMU, then address bit31 is used as a DATA cache
        // bypass - so alias DATA memory as 0x00000000 up to 0x80000000
        //
        Addr lowPA   = 0x00000000;
        Addr highPAD = 0x7fffffff;  // This is to accomodate bit 31 cache bypass
        Addr highPAC = 0xffffffff;
        vmirtAliasMemory(physDomainC, virtDomainKC, lowPA, highPAC, 0x00000000, 0);
        vmirtAliasMemory(physDomainD, virtDomainKD, lowPA, highPAD, 0x00000000, 0);
        vmirtAliasMemory(physDomainD, virtDomainKD, lowPA, highPAD, 0x80000000, 0);

        codeDomains[VM_MODE_KERNEL] = virtDomainKC;
        dataDomains[VM_MODE_KERNEL] = virtDomainKD;
    }
}

