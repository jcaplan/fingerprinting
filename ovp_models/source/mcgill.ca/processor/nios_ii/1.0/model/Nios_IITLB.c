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
#include "vmi/vmiMessage.h"
#include "vmi/vmiRt.h"
#include "vmi/vmiTypes.h"
#include "Nios_IITLB.h"
#include "Nios_IIStructure.h"
#include "Nios_IIExceptions.h"

#define TLBTRACE 0
static void allocTableTLB(TLBtableP table, Uns32 size, Uns32 ways) {
    table->entry = STYPE_CALLOC_N(TLBentry, size);
    table->size  = size;
    table->ways  = ways;
    table->rows  = size / ways;
}

static void freeTableTLB(TLBtableP table) {
    STYPE_FREE(table->entry);
}

inline static Uns32 getRowForVA(TLBtableP tlb, Uns32 VA) {
    return (VA >> NIOS_II_VPN_SHIFT) & (tlb->rows-1);
}

inline static TLBentryP getEntry(TLBtableP tlb, Uns32 row, Uns32 way) {
    return &tlb->entry[row*tlb->ways + way];
}

inline static Uns32 getEntryLowVA(TLBentryP entry) {
    return entry->VA;
}

inline static Uns32 getEntryHighVA(TLBentryP entry) {
    return entry->VA + NIOS_II_PAGE_SIZE - 1;
}

inline static Uns32 getEntryLowPA(TLBentryP entry) {
    return entry->PA;
}

inline static Uns32 getEntryHighPA(TLBentryP entry) {
    return entry->PA + NIOS_II_PAGE_SIZE - 1;
}

static TLBentryP matchEntry(TLBtableP tlb, Uns32 row, Uns32 way, Uns32 VA, Uns32 PID) {

    TLBentryP entry = getEntry(tlb, row, way);

    if((VA<getEntryLowVA(entry)) || (VA>getEntryHighVA(entry))) {
        return NULL;
    } else if (entry->G || (entry->PID == PID)) {
        return entry;
    } else {
        return NULL;
    }
}

static TLBentryP findEntry(TLBtableP tlb, Uns32 VA, Nios_IIVMMode mode, Uns32 PID) {

    if((mode==VM_MODE_USER) && (VA>=0x80000000)) {

        return NULL;

    } else if(VA>=0xc0000000) {

        return NULL;

    } else {

        Uns32     row = getRowForVA(tlb, VA);
        Uns32     way;
        TLBentryP match = 0;

        for(way=0; !match && (way<tlb->ways); way++) {
            match = matchEntry(tlb, row, way, VA, PID);
        }

        return match;
    }
}

static void invalidateEntry(Nios_IIP Nios_II, TLBtableP tlb, Uns32 row, Uns32 way) {

    TLBentryP     entry = getEntry(tlb, row, way);
    Nios_IIVMMode mode;

#if(TLBTRACE==1)
    vmiPrintf("NIOSII_DBG: invalidate entry (old)VA=0x%08x row=%d way=%d\n",
            entry->VA, row, way);
#endif
    for(mode=VM_MODE_KERNEL; mode<VM_MODE_LAST; mode++) {

        if(entry->simMapped[mode]) {
#if(TLBTRACE==1)
            vmiPrintf("vmirtUnaliasMemoryVM virtDomain[%d] VA=0x%08x G=%d PID=%d\n",
                    mode,
                    getEntryLowVA(entry),
                    entry->G, entry->PID
                    );
#endif
            vmirtUnaliasMemoryVM(
                Nios_II->virtDomainC[mode],
                getEntryLowVA(entry),
                getEntryHighVA(entry),
                entry->G,
                entry->PID
            );
            vmirtUnaliasMemoryVM(
                Nios_II->virtDomainD[mode],
                getEntryLowVA(entry),
                getEntryHighVA(entry),
                entry->G,
                entry->PID
            );
            entry->simMapped[mode] = False;

        } else {
        }
    }
}

//
// TLB Size is MMU_TLB_ENTRIES by MMU_TLB_SET_ASSOCIATIVITY
// Entries = 128(0) | 256(1) | 512(2) | 1024(3)
// Ways    =   8(0) |  16(1)
//
void Nios_IIConstructMMU(Nios_IIP Nios_II) {
    //
    // Do we have an MMU ?
    //
    if (Nios_II->params.INCLUDE_MMU) {
        Uns16 entries = Nios_II->params.MMU_TLB_ENTRIES;
        Uns16 ways    = Nios_II->params.MMU_TLB_SET_ASSOCIATIVITY;

        //
        // Ways    =   8(0) |  16(1)
        // Entries = 128(0) | 256(1) | 512(2) | 1024(3)
        //
        if (entries <= 3) {
            //
            // Legal Range
            //
        } else {
            vmiMessage("F", "MMU_TLB_ENTRIES", "ENTRIES=%d (Legal 128 | 256 | 512 | 1024)", entries);
        }
        if (ways <= 1) {
            //
            // Legal Range
            //
        } else {
            vmiMessage("F", "MMU_TLB_SET_ASSOCIATIVITY", "WAYS=%d  (Legal 8 | 16)", ways);
        }

        Uns32 eNum = 128 << entries;
        Uns32 wNum =   8 << ways;
        allocTableTLB(&Nios_II->tlb, eNum, wNum);
    }
}

void Nios_IIDestructMMU(Nios_IIP Nios_II) {
    //
    // Do we have an MMU ?
    //
    if (Nios_II->params.INCLUDE_MMU) {
        freeTableTLB(&Nios_II->tlb);
    }
}

void Nios_IIPrintTLB(Nios_IIP Nios_II) {
    TLBtableP tlb = &Nios_II->tlb;
    Uns32 iter;
    Uns32 size = tlb->size;

    Bool done = False;

    for(iter=0; iter<size; iter++) {
        Uns32   VA   = tlb->entry[iter].VA;
        Uns32   PA   = tlb->entry[iter].PA;
        Uns32   PID  = tlb->entry[iter].PID;
        memPriv priv = tlb->entry[iter].priv;
        Bool    G    = tlb->entry[iter].G;
        Bool    C    = tlb->entry[iter].C;

        Bool    KERN = tlb->entry[iter].simMapped[VM_MODE_KERNEL];
        Bool    USER = tlb->entry[iter].simMapped[VM_MODE_USER];

        Uns32   VPN  = VA >> NIOS_II_VPN_SHIFT;
        Uns32   PFN  = PA >> NIOS_II_VPN_SHIFT;

        char    R    = (priv & MEM_PRIV_R) ? 'r' : '-';
        char    W    = (priv & MEM_PRIV_W) ? 'w' : '-';
        char    X    = (priv & MEM_PRIV_X) ? 'x' : '-';

        if (VPN || PFN) {
            if (!done) {
                vmiPrintf("NIOSII_DBG TLB_____VPN_____PFN__PID_PRV_G_C_KU\n");
                done = True;
            }
            vmiPrintf("NIOSII_DBG %03d 0x%05x 0x%05x %04x %c%c%c %d %d %d%d\n",
                iter, VPN, PFN, PID,
                R, W, X,
                G, C, KERN, USER);
        }
    }
//    vmiPrintf("MemDomain VM_MODE_KERNEL\n");
//    vmirtDebugDomain(Nios_II->virtDomainC[VM_MODE_KERNEL]);
//    vmirtDebugDomain(Nios_II->virtDomainD[VM_MODE_KERNEL]);
}

void Nios_IIWriteTLB(Nios_IIP Nios_II, TLBentry entry, Uns32 way) {

    TLBtableP tlb = &Nios_II->tlb;
    Uns32     row = getRowForVA(tlb, entry.VA);
    Uns32     way2;

    invalidateEntry(Nios_II, tlb, row, way);

    //
    // Additional check for duplicate TLB Entries
    //
    for(way2=0; (way2<tlb->ways); way2++) {
        if((way!=way2) && matchEntry(tlb, row, way2, entry.VA, entry.PID)) {
            vmiMessage("W", "TLB DUP", "Duplicate entries exist in TLB for VA=0x%08x\n",
                    entry.VA);
        }
    }

    TLBentryP entryP = getEntry(tlb, row, way);
    *entryP = entry;

#if(TLBTRACE==1)
    char R = (entry.priv & MEM_PRIV_R) ? 'r' : '-';
    char W = (entry.priv & MEM_PRIV_W) ? 'w' : '-';
    char X = (entry.priv & MEM_PRIV_X) ? 'x' : '-';

    vmiPrintf("NIOSII_DBG: Nios_IIWriteTLB: VA=0x%08x PA=0x%08x PID=%d PRIV=%c%c%c\n",
            entry.VA, entry.PA, entry.PID, R, W, X);

   Nios_IIPrintTLB(Nios_II);
#endif
}

TLBentryP Nios_IIReadTLB(Nios_IIP Nios_II, Uns32 way, Uns32 VA) {

    TLBtableP tlb = &Nios_II->tlb;
    Uns32     row = getRowForVA(tlb, VA);

    TLBentryP entry = getEntry(tlb, row, way);

#if(TLBTRACE==1)
    Nios_IIPrintTLB(Nios_II);
#endif
    return entry;
}

void Nios_IISearchTLB(Nios_IIP Nios_II) {
}

//
// Determine if the access using the passed virtual address misses the TLB,
// updating the processor state to take an appropriate exception if so.
//
Bool Nios_IITLBMiss(
    Nios_IIP       Nios_II,
    memPriv        action,
    Uns32          VA,
    memAccessAttrs attrs
) {

    if(Nios_II->params.INCLUDE_MMU) {
        Uns32         PID   = Nios_II->tlbmisc.bits.PID;
        TLBtableP     tlb   = &Nios_II->tlb;
        Nios_IIVMMode mode  = Nios_IIGetVMMode(Nios_II);
        TLBentryP     entry = findEntry(tlb, VA, mode, PID);

        if(!entry) {
            if(MEM_AA_IS_TRUE_ACCESS(attrs)) {
               Nios_IITLBMissException(Nios_II, action, VA);
            }
            return True;

        } else if(!(entry->priv & action)) {
            if(MEM_AA_IS_TRUE_ACCESS(attrs)) {
                Nios_IITLBPrivException(Nios_II, action, VA);
            }
            return True;

        } else {
            //
            // An entry exists so map it
            //
#if(TLBTRACE==1)
            vmiPrintf("vmirtAliasMemoryVM   virtDomain[%d] VA=0x%08x PA=0x%08x G=%d PRIV=%d PID=%d\n",
                    mode,
                    getEntryLowVA(entry), getEntryLowPA(entry),
                    entry->G, entry->priv, entry->PID
                    );
#endif
            vmirtAliasMemoryVM(
                Nios_II->physDomainC,
                Nios_II->virtDomainC[mode],
                getEntryLowPA(entry),
                getEntryHighPA(entry),
                getEntryLowVA(entry),
                0,
                entry->priv,
                entry->G,
                entry->PID
            );
            vmirtAliasMemoryVM(
                Nios_II->physDomainD,
                Nios_II->virtDomainD[mode],
                getEntryLowPA(entry),
                getEntryHighPA(entry),
                getEntryLowVA(entry),
                0,
                entry->priv,
                entry->G,
                entry->PID
            );
            entry->simMapped[mode] = True;

            return False;
        }

    } else if (Nios_II->params.INCLUDE_MPU){
        if(MEM_AA_IS_TRUE_ACCESS(attrs)) {
            // TODO: exception Abort
            vmiMessage("F", "TLBMISS", "MPU && attrs - bus error");
        }
        return True;

    } else {
        if(MEM_AA_IS_TRUE_ACCESS(attrs)) {
        // TODO: exception Abort
            vmiMessage("F", "TLBMISS", "!MMU && !MPU - bus error");
        }
        return True;
    }
}


