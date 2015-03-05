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

#include "hostapi/impAlloc.h"
#include "vmi/vmiAttrs.h"
#include "vmi/vmiMessage.h"
#include "vmi/vmiRt.h"
#include "vmi/vmiTypes.h"
#include "Nios_IIMPU.h"
#include "Nios_IIStructure.h"
#include "Nios_IIExceptions.h"

#define TRACEMPU 0

static void allocTableMPU(MPUtableP table, Uns32 size) {
    table->data = STYPE_CALLOC_N(MPUentry, size);
    table->code = STYPE_CALLOC_N(MPUentry, size);
    table->size = size;
}

static void freeTableMPU(MPUtableP table) {
    STYPE_FREE(table->data);
    STYPE_FREE(table->code);
}

static void updateProtectionEntryRange(
    Nios_IIP     Nios_II,
    Uns32        VA,
    MPUentryP   *match,
    MPUentryP    try,
    Uns32       *low,
    Uns32       *high
) {
    Uns32 rlow  = try->base;
    Uns32 rhigh = rlow + (try->size - 1);

    if((rlow<=VA) && (rhigh>=VA)) {
        // match in this region
        *match = try;
        *low   = rlow;
        *high  = rhigh;
    } else if(!*match) {
        // no further action unless a match found
    } else if((rlow>VA) && (rlow<*high)) {
        // remove part of region ABOVE matching address
        *high = rlow-1;
    } else if((rhigh<VA) && (rhigh>*low)) {
        // remove part of region BELOW matching address
        *low = rhigh+1;
    }
}

void Nios_IIConstructMPU(Nios_IIP Nios_II) {
    //
    // Do we have an MPU ?
    //
    if (Nios_II->params.INCLUDE_MPU) {
        Uns32 dsize = Nios_II->params.MPU_NUMBER_DATA_REGIONS;
        Uns32 isize = Nios_II->params.MPU_NUMBER_INSTRUCTION_REGIONS;
        Uns32 size  = (dsize > isize) ? dsize : isize;

        allocTableMPU(&Nios_II->mpu, size);
    }
}

void Nios_IIDestructMPU(Nios_IIP Nios_II) {
    //
    // Do we have an MMU ?
    //
    if (Nios_II->params.INCLUDE_MPU) {
        freeTableMPU(&Nios_II->mpu);
    }
}

//
// Determine if the access using the passed virtual address misses the MPU,
// updating the processor state to take an appropriate exception if so.
//
Bool Nios_IIMPUMiss(
    Nios_IIP       Nios_II,
    memPriv        action,
    Uns32          VA,
    memAccessAttrs attrs
) {

    MPUtableP      mpu = &Nios_II->mpu;
    Nios_IIVMMode mode = Nios_IIGetVMMode(Nios_II);
    Uns32         size = mpu->size;

    Uns32         low=0, high=~0;

    // scan common regions searching for a match
    MPUentryP match = 0;
    MPUentryP entry;
    if (action == MEM_PRIV_X) {
        entry = &(mpu->code[0]);
    } else {
        entry = &(mpu->data[0]);
    }

    //
    // Lowest priority are at the end of the array - so search in reverse order
    //
    Int32 idx;
    for(idx=(size-1); idx>=0; idx--) {
        updateProtectionEntryRange(Nios_II, VA, &match, &entry[idx], &low, &high);
    }

    Bool isMapped = False;
    if (match) {
        memPriv priv = match->priv[mode];
        if ((action & MEM_PRIV_RW) && (action & priv)) {
            //
            // Data Domain Load/Store
            //
            vmirtProtectMemory(Nios_II->virtDomainD[mode], low, high, priv, MEM_PRIV_SET);
            isMapped = True;

        } else if ((action & MEM_PRIV_X) && (action & priv)) {
            //
            // Code Domain Execute
            //
            vmirtProtectMemory(Nios_II->virtDomainC[mode], low, high, priv, MEM_PRIV_SET);
            isMapped = True;

        }

#if(TRACEMPU==1)
        if (isMapped) {
            char R = (priv & MEM_PRIV_R) ? 'r' : '-';
            char W = (priv & MEM_PRIV_W) ? 'w' : '-';
            char X = (priv & MEM_PRIV_X) ? 'x' : '-';

            vmiPrintf("MPU Miss (Action) VA=0x%08x low=0x%08x high=0x%08x %c%c%c\n",
                    VA, low, high, R, W, X);
            Nios_IIPrintMPU(Nios_II);
        } else {
            vmiPrintf("MPU Miss (Action) VA=0x%08x - Not Mapped\n", VA);
        }
#endif
    } else {
#if(TRACEMPU==1)
        vmiPrintf("MPU Miss (Action) VA=0x%08x - No Match\n", VA);
#endif
    }


    if (isMapped) {
        return False;
    } else {
        //
        // If this is a real access
        //
        if(attrs) {
            Nios_IIMPUMissException(Nios_II, action, VA);
        }
    }

    return True;
}

void Nios_IIWriteMPU(Nios_IIP Nios_II) {
    memPriv user=MEM_PRIV_NONE, kernel=MEM_PRIV_NONE;
    MPUentryP entry;
    Uns32 low, high;

    Uns32 idx   = Nios_II->mpubase.bits.INDEX;
    Uns8 perm   = Nios_II->mpuacc.bits.PERM;
    Bool isData = Nios_II->mpubase.bits.D;

    if (isData) {
        //
        // Assert that the index is in range
        //
        if (idx >= Nios_II->params.MPU_NUMBER_DATA_REGIONS) {
            vmiMessage("F", "NIOS_MPU", "Index out of range = %d", idx);
        }
        entry = &(Nios_II->mpu.data[idx]);
        //
        // Remove Existing mapping from USER_MPU & KERNEL_MPU
        //
        low  = entry->base;
        high = low + (entry->size - 1);
        vmirtProtectMemory(Nios_II->virtDomainD[VM_MODE_USER_MPU],   low, high, MEM_PRIV_NONE, MEM_PRIV_SET);
        vmirtProtectMemory(Nios_II->virtDomainD[VM_MODE_KERNEL_MPU], low, high, MEM_PRIV_NONE, MEM_PRIV_SET);
#if(TRACEMPU==1)
        vmiPrintf("MPU Protect Data MEM_PRIV_NONE low=0x%08x : high=0x%08x\n", low, high);
#endif
    } else {
        //
        // Assert that the index is in range
        //
        if (idx >= Nios_II->params.MPU_NUMBER_INSTRUCTION_REGIONS) {
            vmiMessage("F", "NIOS_MPU", "Index out of range = %d", idx);
        }
        entry = &(Nios_II->mpu.code[idx]);
        //
        // Remove Existing mapping from USER_MPU & KERNEL_MPU
        //
        low  = entry->base;
        high = low + (entry->size - 1);
        vmirtProtectMemory(Nios_II->virtDomainC[VM_MODE_USER_MPU],   low, high, MEM_PRIV_NONE, MEM_PRIV_SET);
        vmirtProtectMemory(Nios_II->virtDomainC[VM_MODE_KERNEL_MPU], low, high, MEM_PRIV_NONE, MEM_PRIV_SET);
#if(TRACEMPU==1)
        vmiPrintf("MPU Protect Code MEM_PRIV_NONE low=0x%08x : high=0x%08x\n", low, high);
#endif

    }

    //
    // Store for later retrieval on a read
    //
    entry->mpuacc  = Nios_II->mpuacc;
    entry->mpubase = Nios_II->mpubase;

    //  PERM  KERNEL    USER
    // 0 000       -       -
    // 1 001      RX       -
    // 2 010      RX       R
    // 4 100      RW       -
    // 5 101      RW       R
    // 6 110      RW      RW
    if        (perm == 0) {
        kernel = MEM_PRIV_NONE;
        user   = MEM_PRIV_NONE;

    } else if (perm == 1) {
        kernel = MEM_PRIV_RX;
        user   = MEM_PRIV_NONE;

    } else if (perm == 2) {
        kernel = MEM_PRIV_RX;
        user   = MEM_PRIV_RX;

    } else if (perm == 4) {
        kernel = MEM_PRIV_RW;
        user   = MEM_PRIV_NONE;

    } else if (perm == 5) {
        kernel = MEM_PRIV_RW;
        user   = MEM_PRIV_R;

    } else if (perm == 6) {
        kernel = MEM_PRIV_RW;
        user   = MEM_PRIV_RW;

    } else {
    }

    entry->base = Nios_II->mpubase.bits.BASE << 6;
    if (Nios_II->params.MPU_USE_LIMIT_FOR_REGION_RANGE) {
        vmiMessage("F", "Nios_IIWriteMPU","MPU_USE_LIMIT_FOR_REGION_RANGE");
    } else {
        //
        // and add leading 1s for the address size
        //
        Uns32 bits   = Nios_II->sizeInstDataBus;
        Uns32 orBits = (bits==32) ? 0 : (~0 << bits);
        entry->size  = -(orBits | (Nios_II->mpuacc.bits.MASK_LIMIT << 6));
    }
    entry->C = Nios_II->mpuacc.bits.C;
    entry->priv[VM_MODE_USER_MPU]   = user;
    entry->priv[VM_MODE_KERNEL_MPU] = kernel;

    //
    // Remove New mapping from USER & KERNEL (even if it does not yet exist)
    //
    low  = entry->base;
    high = low + (entry->size - 1);
    if (isData) {
        vmirtProtectMemory(Nios_II->virtDomainD[VM_MODE_USER_MPU],   low, high, MEM_PRIV_NONE, MEM_PRIV_SET);
        vmirtProtectMemory(Nios_II->virtDomainD[VM_MODE_KERNEL_MPU], low, high, MEM_PRIV_NONE, MEM_PRIV_SET);
    } else {
        vmirtProtectMemory(Nios_II->virtDomainC[VM_MODE_USER_MPU],   low, high, MEM_PRIV_NONE, MEM_PRIV_SET);
        vmirtProtectMemory(Nios_II->virtDomainC[VM_MODE_KERNEL_MPU], low, high, MEM_PRIV_NONE, MEM_PRIV_SET);
    }

#if(TRACEMPU==1)
    char kR = (kernel & MEM_PRIV_R) ? 'r' : '-';
    char kW = (kernel & MEM_PRIV_W) ? 'w' : '-';
    char kX = (kernel & MEM_PRIV_X) ? 'x' : '-';
    char uR = (user   & MEM_PRIV_R) ? 'r' : '-';
    char uW = (user   & MEM_PRIV_W) ? 'w' : '-';
    char uX = (user   & MEM_PRIV_X) ? 'x' : '-';
    char *bus = isData ? "Data" : "Code";

    vmiPrintf("MPU Write (Pending) %s low=0x%08x high=0x%08x K=%c%c%c U=%c%c%c\n",
            bus, low, high,
            kR,kW,kX,
            uR,uW,uX);
    Nios_IIPrintMPU(Nios_II);
#endif
}

#if(TRACEMPU==1)
void Nios_IIPrintMPU(Nios_IIP Nios_II) {
    //
    // Print info
    //
    int i;
    for (i=0; i<Nios_II->mpu.size; i++) {
        MPUentryP entry = &(Nios_II->mpu.code[i]);
        Uns8 kernel      = entry->priv[VM_MODE_KERNEL_MPU];
        Uns8 user        = entry->priv[VM_MODE_USER_MPU];

        char kR = (kernel & MEM_PRIV_R) ? 'r' : '-';
        char kW = (kernel & MEM_PRIV_W) ? 'w' : '-';
        char kX = (kernel & MEM_PRIV_X) ? 'x' : '-';
        char uR = (user   & MEM_PRIV_R) ? 'r' : '-';
        char uW = (user   & MEM_PRIV_W) ? 'w' : '-';
        char uX = (user   & MEM_PRIV_X) ? 'x' : '-';

        vmiPrintf("MPU[%d] Code K:%c%c%c U:%c%c%c BASE=0x%08x SIZE=0x%08x C=%d\n",
                i,
                kR,kW,kX,
                uR,uW,uX,
                entry->base, (Uns32)entry->size,
                entry->C);
    }
    vmirtDebugDomain(Nios_II->virtDomainC[VM_MODE_KERNEL_MPU]);
    vmirtDebugDomain(Nios_II->virtDomainC[VM_MODE_USER_MPU]);

    for (i=0; i<Nios_II->mpu.size; i++) {
        MPUentryP entryP = &(Nios_II->mpu.data[i]);
        Uns8 kernel      = entryP->priv[VM_MODE_KERNEL_MPU];
        Uns8 user        = entryP->priv[VM_MODE_USER_MPU];

        char kR = (kernel & MEM_PRIV_R) ? 'r' : '-';
        char kW = (kernel & MEM_PRIV_W) ? 'w' : '-';
        char kX = (kernel & MEM_PRIV_X) ? 'x' : '-';
        char uR = (user   & MEM_PRIV_R) ? 'r' : '-';
        char uW = (user   & MEM_PRIV_W) ? 'w' : '-';
        char uX = (user   & MEM_PRIV_X) ? 'x' : '-';

        vmiPrintf("MPU[%d] Data K:%c%c%c U:%c%c%c BASE=0x%08x SIZE=0x%08x C=%d\n",
                i,
                kR,kW,kX,
                uR,uW,uX,
                entryP->base, entryP->size,
                entryP->C);
    }
    vmirtDebugDomain(Nios_II->virtDomainD[VM_MODE_KERNEL_MPU]);
    vmirtDebugDomain(Nios_II->virtDomainD[VM_MODE_USER_MPU]);

}
#endif

//
// Fill contents of mpuacc with index from mpubase
//
void Nios_IIReadMPU(Nios_IIP Nios_II) {
    MPUentryP entry;

    Bool  isData = Nios_II->mpubase.bits.D;
    Uns32 idx    = Nios_II->mpubase.bits.INDEX;

    if (isData) {
        //
        // Read from Data region
        //
        entry = &(Nios_II->mpu.data[idx]);

    } else {
        //
        // Read from Code region
        //
        entry = &(Nios_II->mpu.code[idx]);

    }

    Nios_II->mpuacc  = entry->mpuacc;
    Nios_II->mpubase = entry->mpubase;

}
