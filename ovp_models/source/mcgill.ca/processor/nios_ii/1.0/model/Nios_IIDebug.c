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

#include "vmi/vmiAttrs.h"
#include "vmi/vmiDbg.h"
#include "vmi/vmiMessage.h"
#include "vmi/vmiOSLib.h"
#include "vmi/vmiRt.h"
#include "Nios_IIStructure.h"
#include "Nios_IIFunctions.h"

#define NIOS_II_RAW_REG_RW(_R) \
    VMI_REG_RAW_READ_CB, \
    VMI_REG_RAW_WRITE_CB, \
    (void *)NIOS_II_CPU_OFFSET(_R)

#define NIOS_II_RAW_REG_R(_R) \
    VMI_REG_RAW_READ_CB, \
    0, \
    (void *)NIOS_II_CPU_OFFSET(_R)

static VMI_REG_READ_FN(readPC) {
    *(Nios_IIAddr*)buffer = (Uns32)vmirtGetPC(processor);
    return True;
}

static VMI_REG_WRITE_FN(writePC) {
    vmirtSetPC(processor, *(Nios_IIAddr*)buffer);
    return True;
}

static VMI_REG_READ_FN(readEmpty) {
    *(Nios_IIAddr*)buffer = 0x00000000;
    return True;
}

static VMI_REG_WRITE_FN(writeEmpty) {
    return True;
}

typedef enum niosRegGroupIdE {
    NIOS2_RG_USER,
    NIOS2_RG_SYSTEM,
    NIOS2_RG_LAST
} niosRegGroupId;

static const vmiRegGroup groups[NIOS2_RG_LAST+1] = {
    [NIOS2_RG_USER]   = {name: "User"   },
    [NIOS2_RG_SYSTEM] = {name: "System" },
};

//
// Return next supported group on this processor
//
static vmiRegGroupCP getNextGroup(vmiRegGroupCP group) {

    if(!group) {
        group = groups;
    } else if((group+1)->name) {
        group = group+1;
    } else {
        group = 0;
    }

    return group;
}

extern const char *GPR[];

static const vmiRegInfo registers[] = {
    {name:"zero",     gdbIndex:0,  usage:vmi_REG_NONE, bits:32, access:vmi_RA_R,  group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[0])},
    {name:"at",       gdbIndex:1,  usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[1])},
    {name:"r2",       gdbIndex:2,  usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[2])},
    {name:"r3",       gdbIndex:3,  usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[3])},
    {name:"r4",       gdbIndex:4,  usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[4])},
    {name:"r5",       gdbIndex:5,  usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[5])},
    {name:"r6",       gdbIndex:6,  usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[6])},
    {name:"r7",       gdbIndex:7,  usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[7])},
    {name:"r8",       gdbIndex:8,  usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[8])},
    {name:"r9",       gdbIndex:9,  usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[9])},
    {name:"r10",      gdbIndex:10, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[10])},
    {name:"r11",      gdbIndex:11, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[11])},
    {name:"r12",      gdbIndex:12, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[12])},
    {name:"r13",      gdbIndex:13, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[13])},
    {name:"r14",      gdbIndex:14, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[14])},
    {name:"r15",      gdbIndex:15, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[15])},
    {name:"r16",      gdbIndex:16, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[16])},
    {name:"r17",      gdbIndex:17, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[17])},
    {name:"r18",      gdbIndex:18, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[18])},
    {name:"r19",      gdbIndex:19, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[19])},
    {name:"r20",      gdbIndex:20, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[20])},
    {name:"r21",      gdbIndex:21, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[21])},
    {name:"r22",      gdbIndex:22, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[22])},
    {name:"r23",      gdbIndex:23, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[23])},
    {name:"et",       gdbIndex:24, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[24])},
    {name:"bt",       gdbIndex:25, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[25])},
    {name:"gp",       gdbIndex:26, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[26])},
    {name:"sp",       gdbIndex:27, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[27])},
    {name:"fp",       gdbIndex:28, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[28])},
    {name:"ea",       gdbIndex:29, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[29])},
    {name:"ba",       gdbIndex:30, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[30])},
    {name:"ra",       gdbIndex:31, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_USER],   raw:NIOS_II_CPU_REG(gpr[31])},
    {name:"PC",       gdbIndex:32, usage:vmi_REG_PC,   bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], readCB:readPC, writeCB:writePC},

    {name:"status",   gdbIndex:33, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], raw:NIOS_II_CPU_REG(status)},
    {name:"estatus",  gdbIndex:34, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], raw:NIOS_II_CPU_REG(estatus)},
    {name:"bstatus",  gdbIndex:35, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], raw:NIOS_II_CPU_REG(bstatus)},
    {name:"ienable",  gdbIndex:36, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], raw:NIOS_II_CPU_REG(ienable)},
    {name:"ipending", gdbIndex:37, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], raw:NIOS_II_CPU_REG(ipending)},
    {name:"cpuid",    gdbIndex:38, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], raw:NIOS_II_CPU_REG(cpuid)},
    // ctl6 - 39
    {name:"ctl6",     gdbIndex:39, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], readCB:readEmpty, writeCB:writeEmpty},

    {name:"except",   gdbIndex:40, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], raw:NIOS_II_CPU_REG(except)},
    {name:"pteaddr",  gdbIndex:41, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], raw:NIOS_II_CPU_REG(pteaddr)},
    {name:"tlbacc",   gdbIndex:42, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], raw:NIOS_II_CPU_REG(tlbacc)},
    {name:"tlbmisc",  gdbIndex:43, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], raw:NIOS_II_CPU_REG(tlbmisc)},
    {name:"eccinj",   gdbIndex:44, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], readCB:readEmpty, writeCB:writeEmpty},
    {name:"badaddr",  gdbIndex:45, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], raw:NIOS_II_CPU_REG(badaddr)},
    {name:"config",   gdbIndex:46, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], raw:NIOS_II_CPU_REG(config)},
    {name:"mpubase",  gdbIndex:47, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], raw:NIOS_II_CPU_REG(mpubase)},
    {name:"mpuacc",   gdbIndex:48, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, group:&groups[NIOS2_RG_SYSTEM], raw:NIOS_II_CPU_REG(mpuacc)},
    //{name:"sstatus", gdbIndex:49, usage:vmi_REG_NONE, bits:32, access:vmi_RA_RW, raw:NIOS_II_CPU_REG(sstatus)},
    {0}
};

//
// Register group iterator
//
VMI_REG_GROUP_FN(Nios_IIRegGroup) {
    return getNextGroup(prev);
}


VMI_REG_INFO_FN(Nios_IIRegInfo) {
    if(!prev) {
        return registers;
    } else if((prev+1)->name) {
        return prev+1;
    } else {
        return 0;
    }
}

VMI_DEBUG_FN(Nios_IIDebug) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Int32 iter;
    for (iter=0; iter<32; iter++) {

        if (GPR[iter]) {
            vmiPrintf("%10s : %08x", GPR[iter], (Uns32)Nios_II->gpr[iter]);
        } else {
            vmiPrintf("%8s%02d : %08x", "r", iter, (Uns32)Nios_II->gpr[iter]);
        }
        if(!((iter+1)&3)) {
            vmiPrintf("\n");
        }
    }
    vmiPrintf("%10s : " "%08x", "status", Nios_II->status.reg);
    vmiPrintf("%10s : " "%08x", "estatus", Nios_II->estatus.reg);
    vmiPrintf("%10s : " "%08x", "bstatus", Nios_II->bstatus.reg);
    vmiPrintf("%10s : " "%08x", "ienable", Nios_II->ienable);
    vmiPrintf("\n");
    vmiPrintf("%10s : " "%08x", "ipending", Nios_II->ipending);
    vmiPrintf("%10s : " "%08x", "cpuid", Nios_II->cpuid);
    vmiPrintf("%10s : " "%08x", "except", Nios_II->except.reg);
    vmiPrintf("%10s : " "%08x", "pteaddr", Nios_II->pteaddr.reg);
    vmiPrintf("\n");
    vmiPrintf("%10s : " "%08x", "tlbacc", Nios_II->tlbacc.reg);
    vmiPrintf("%10s : " "%08x", "tlbmisc", Nios_II->tlbmisc.reg);
    vmiPrintf("%10s : " "%08x", "badaddr", Nios_II->badaddr);
    vmiPrintf("%10s : " "%08x", "config", Nios_II->config.reg);
    vmiPrintf("\n");
    vmiPrintf("%10s : " "%08x", "mpubase", Nios_II->mpubase.reg);
    vmiPrintf("%10s : " "%08x", "mpuacc", Nios_II->mpuacc.reg);
    vmiPrintf("%10s : " "%08x", "sstatus", Nios_II->sstatus.reg);
//    vmiPrintf("%10s : " "       %01x", "FLG_CI", Nios_II->FLG_CI);
//    vmiPrintf("\n");
//    vmiPrintf("%10s : " "       %01x", "FLG_CO", Nios_II->FLG_CO);
//    vmiPrintf("%10s : " "       %01x", "FLG_PA", Nios_II->FLG_PA);
//    vmiPrintf("%10s : " "       %01x", "FLG_ZR", Nios_II->FLG_ZR);
//    vmiPrintf("%10s : " "       %01x", "FLG_SI", Nios_II->FLG_SI);
//    vmiPrintf("\n");
//    vmiPrintf("%10s : " "       %01x", "FLG_OV", Nios_II->FLG_OV);
    vmiPrintf("%10s : %08x\n", "PC", (Nios_IIAddr)vmirtGetPC(processor));
    vmiPrintf("\n");
}

