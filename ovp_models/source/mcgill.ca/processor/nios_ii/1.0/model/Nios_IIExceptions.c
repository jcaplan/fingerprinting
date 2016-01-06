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

#include "vmi/vmiMessage.h"
#include "vmi/vmiRt.h"
#include "Nios_IIExceptions.h"
#include "Nios_IIFunctions.h"
#include "Nios_IIStructure.h"
#include "Nios_IITLB.h"
#include "Nios_IIMPU.h"

static inline Uns32 getPC(Nios_IIP Nios_II) {
    return (vmirtGetPC((vmiProcessorP)Nios_II));
}
static inline void setPCException(Nios_IIP Nios_II, Uns32 pc) {
    vmirtSetPCException((vmiProcessorP)Nios_II, pc);
}

Nios_IIVMMode Nios_IIGetVMMode(Nios_IIP Nios_II) {
    Nios_IIVMMode newMode;
    if (Nios_II->params.INCLUDE_MMU) {
        //
        // MMU Mode
        //
        newMode = Nios_II->status.bits.U ? VM_MODE_USER : VM_MODE_KERNEL;

    } else if (Nios_II->params.INCLUDE_MPU) {
        //
        // MPU Mode
        //
        if (Nios_II->config.bits.PE) {
            newMode = Nios_II->status.bits.U ? VM_MODE_USER_MPU : VM_MODE_KERNEL_MPU;
        } else {
            newMode = VM_MODE_KERNEL;
        }
    } else {
        //
        // Non MPU/MMU
        //
        newMode = VM_MODE_KERNEL;
    }

    return newMode;
}

static void doModeSwitch (Nios_IIP Nios_II, Nios_IIVMMode mode) {
    vmirtSetMode((vmiProcessorP)Nios_II, mode);
}

//
// All config writes through this function
//
void Nios_IIWriteConfig(Nios_IIP Nios_II, TYPE_config nxt_config) {
    //
    // Detect which bits are changing state
    //
    TYPE_config change;
    change.reg = Nios_II->config.reg ^ nxt_config.reg;

    Nios_II->config    = nxt_config;
    Nios_IIVMMode mode = Nios_IIGetVMMode(Nios_II);

    //
    // A change in PE will cause a mode switch
    //
    if (change.bits.PE) {
        doModeSwitch(Nios_II, mode);
    }
}

TYPE_config Nios_IIReadConfig(Nios_IIP Nios_II) {
    TYPE_config config = Nios_II->config;
    return config;
}

//
// All status writes through this function
//
void Nios_IIWriteStatus(Nios_IIP Nios_II, TYPE_status nxt_status) {
    //
    // Detect which bits are changing state
    //
    TYPE_status change;
    change.reg = Nios_II->status.reg ^ nxt_status.reg;

    Nios_II->status = nxt_status;
    Nios_IIVMMode mode = Nios_IIGetVMMode(Nios_II);

    //
    // A change in PIE to 1 could enable an interrupt thorugh
    //
    if (change.bits.PIE && Nios_II->status.bits.PIE) {
        Nios_IICheckExceptions(Nios_II);
    }

    //
    // A change in U will cause a mode switch
    //
    if (change.bits.U) {
        doModeSwitch(Nios_II, mode);
    }
}

TYPE_status Nios_IIReadStatus(Nios_IIP Nios_II) {
    TYPE_status status = Nios_II->status;
    return status;
}

static Bool isIntActive(Nios_IIP Nios_II) {

    if (Nios_II->ResetPending) {
        // Held in reset
        return False;
    }

    TYPE_status status = Nios_IIReadStatus(Nios_II);

    //
    // Interrupts are active high, masked by the interrupt enable register
    // Global disable in PIE (page 76)
    //
    Nios_II->ipending = Nios_II->d_irq & Nios_II->ienable;

    return (status.bits.PIE && Nios_II->ipending);
}

Uns8 Nios_IIGetExceptionIndex(Uns32 val) {
    Uns8 idx;
    Uns32 v = val;
    for (idx=1; idx<=31; idx++) {
        if (v & 0x1) {
            return idx;
        }
        v>>=1;
    }
    return 0;
}

static void excEntry(Nios_IIP Nios_II, Nios_IIException Exc, Bool cond, Uns8 Cause, Addr Vector) {
    Uns8 idx = Nios_IIGetExceptionIndex(Exc);
    Nios_II->exceptionTable[idx].Exc       = Exc;
    Nios_II->exceptionTable[idx].Available = cond ? Exc : 0;
    Nios_II->exceptionTable[idx].Cause     = Cause;
    Nios_II->exceptionTable[idx].Vector    = Vector;
}

//
// Attempt to access kernel area in user mode
//
inline static Bool isPartitionException(Nios_IIP Nios_II, Uns32 VA) {
    Nios_IIVMMode mode = Nios_IIGetVMMode(Nios_II);
    return ((mode==VM_MODE_USER) && (VA>=0x80000000));
}

static void exceptionAction(Nios_IIP Nios_II, Nios_IIException exc, Uns32 ea) {
    //
    // Ref: Nios II Processor Status After Taking Exception
    // Table 3-35
    //
    Nios_II->exception = exc;

//    Addr RESET     = Nios_II->params.RESET_VECTOR;
//    Addr BREAK     = Nios_II->params.BREAK_VECTOR;
    Addr EXCEPTION = Nios_II->params.EXCEPTION_VECTOR;
    Addr FAST_TLB  = Nios_II->params.FAST_TLB_MISS_EXCEPTION_VECTOR;

//    Addr HANDLER   = 0xbadc0de;
    Addr RHA       = 0xbadc0de;

    Addr pc;    // Exception Hander Address

    //
    // TBD
    //
    Uns8  RRS  = 0;
    Uns8  RNMI = 0;
    Uns8  RIL  = 0;

    //
    // Set the cause register if supported
    //
    if (Nios_II->params.EXCEPTION_EXTRA_INFORMATION) {
        Uns8 idx = Nios_IIGetExceptionIndex(exc);
        Nios_II->except.bits.CAUSE = Nios_II->exceptionTable[idx].Cause;
    }

    //
    // Next state registers
    //
    TYPE_status  status      = Nios_IIReadStatus(Nios_II);
    TYPE_sstatus sstatus     = Nios_II->sstatus;
    TYPE_estatus estatus     = Nios_II->estatus;

    TYPE_status  nxt_status  = status;
    TYPE_sstatus nxt_sstatus = sstatus;
    TYPE_estatus nxt_estatus = estatus;

    nxt_status.bits.U = 0;
    if (exc & EXC_EXT_INTERRUPT_ASSERTED) {
        //
        // if External Interrupt Asserted
        //
        pc                   = RHA;
        nxt_status.bits.PIE  = Nios_II->config.bits.ANI;
        nxt_status.bits.IH   = 1;
        nxt_status.bits.NMI  = RNMI;
        nxt_status.bits.IL   = RIL;
        nxt_status.bits.RSIE = 0;
        nxt_status.bits.CRS  = RRS;

        if (status.bits.EH == 1) {
            //
            // status.EH==1
            //
            if (RRS == 0) {
                //
                // RRS == 0
                //
            } else {
                //
                // RRS != 0
                //
            }
        } else {
            //
            // status.EH==0
            //
            nxt_status.bits.PRS = status.bits.CRS;
            Nios_II->gpr[29] = ea;

            if (RRS == 0) {
                nxt_estatus.reg  = status.reg;
            } else {
                nxt_sstatus.reg  = status.reg;
            }
        }
    } else {
        //
        // if Internal Interrupt Asserted || Noninterrupt exception
        //
        nxt_status.bits.PIE = 0;

        if (Nios_II->params.INCLUDE_MMU) {
            nxt_status.bits.EH  = 1;
        }

        if (status.bits.EH == 1) {
            //
            // status.EH==1
            // (Double TLB miss)
            //
            pc = EXCEPTION;
        } else {
            //
            // status.EH==0
            //
            Nios_II->gpr[29] = ea;

            if (exc & EXC_TLB_MISS) {
                pc = FAST_TLB;
            } else {
                pc = EXCEPTION;
            }
            nxt_estatus.reg  = status.reg;
        }
    }

    if (Nios_II->params.INCLUDE_MMU) {
        //
        // tlbmisc : DBL / BAD / PERM
        //
        Nios_II->tlbmisc.bits.DBL  = (exc & EXC_DOUBLE_TLB_MISS)          ? 1 : 0;
        Nios_II->tlbmisc.bits.BAD  = (exc & EXC_SET_BAD_FLAG)             ? 1 : 0;
        Nios_II->tlbmisc.bits.PERM = (exc & EXC_TLB_PERMISSION_VIOLATION) ? 1 : 0;

        //
        // D Flag
        //
        if (
                (exc & EXC_TLB_MISS_DATA) ||
                (exc & EXC_TLB_PERMISSION_VIOLATION_READ) ||
                (exc & EXC_TLB_PERMISSION_VIOLATION_WRITE) ||
                (exc & EXC_MISALIGNED_DATA_ADDRESS) ||
                (exc & EXC_SUPERVISOR_ONLY_DATA_ADDRESS)
            ) {
            Nios_II->tlbmisc.bits.D = 1;
        } else {
            Nios_II->tlbmisc.bits.D = 0;
        }
    }

    //
    // Copy the registers back out
    //
    Nios_IIWriteStatus(Nios_II, nxt_status);

    Nios_II->sstatus = nxt_sstatus;
    Nios_II->estatus = nxt_estatus;

    //
    // Set the PC for the next instruction
    //
    setPCException(Nios_II, pc);
}

void Nios_IIExceptionCB(Nios_IIP Nios_II, Nios_IIException exc) {
    exceptionAction(Nios_II, exc, getPC(Nios_II)+4);
}

void Nios_IIMemoryExceptCommon(Nios_IIP Nios_II, Nios_IIException exc, Uns32 VA) {
    TYPE_status status = Nios_IIReadStatus(Nios_II);

    if (Nios_II->params.EXCEPTION_EXTRA_INFORMATION) {
        Nios_II->badaddr = VA;
    }
    if (status.bits.EH == 0 && (exc & EXC_TLB)) {
        Nios_II->pteaddr.bits.VPN = VA >> NIOS_II_VPN_SHIFT;
    }

    exceptionAction(Nios_II, exc, getPC(Nios_II)+4);
}

void Nios_IITLBMissException(Nios_IIP Nios_II, memPriv action, Uns32 VA) {
    Nios_IIException exc;

    if (Nios_II->status.bits.EH == 1) {
        exc = action==MEM_PRIV_X ? EXC_DOUBLE_TLB_MISS_INSTRUCTION : EXC_DOUBLE_TLB_MISS_DATA;
    } else {
        exc = action==MEM_PRIV_X ? EXC_FAST_TLB_MISS_INSTRUCTION   : EXC_FAST_TLB_MISS_DATA;
        Nios_II->tlbmisc.bits.WE = 1;
    }

    Nios_IIMemoryExceptCommon(Nios_II, exc, VA);
}

void Nios_IITLBPrivException(Nios_IIP Nios_II, memPriv action, Uns32 VA) {
    Nios_IIException exc;

    if (action==MEM_PRIV_R) {
        exc = EXC_TLB_PERMISSION_VIOLATION_READ;

    } else if (action==MEM_PRIV_W) {
        exc = EXC_TLB_PERMISSION_VIOLATION_WRITE;

    } else /* MEM_PRIV_X */ {
        exc = EXC_TLB_PERMISSION_VIOLATION_EXECUTE;
    }

    Nios_II->tlbmisc.bits.WE = 1;
    Nios_IIMemoryExceptCommon(Nios_II, exc, VA);
}

static void mpuExceptCommon(Nios_IIP Nios_II, Nios_IIException exc, Uns32 VA) {
    //TYPE_status status = Nios_IIReadStatus(Nios_II);

    if (Nios_II->params.EXCEPTION_EXTRA_INFORMATION) {
        Nios_II->badaddr = VA;
    }

    exceptionAction(Nios_II, exc, getPC(Nios_II)+4);
}

void Nios_IIMPUMissException(Nios_IIP Nios_II, memPriv action, Uns32 VA) {
    Nios_IIException exc;

    if (action==MEM_PRIV_X) {
        exc = EXC_MPU_REGION_VIOLATION_INSTRUCTION;
    } else {
        exc = EXC_MPU_REGION_VIOLATION_DATA;
    }

    mpuExceptCommon(Nios_II, exc, VA);
}

//
// Attempt to access kernel area in user mode
//
inline static Bool isKernelDirectMapped(Nios_IIP Nios_II, Uns32 VA) {
    Nios_IIVMMode mode = Nios_IIGetVMMode(Nios_II);
    return ((mode==VM_MODE_KERNEL) && (VA>=0xc0000000));
}

VMI_RD_PRIV_EXCEPT_FN(Nios_IIRdPrivExcept) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_II->exception = EXC_NONE;
    Uns32 VA = (Uns32)address;

    if (Nios_II->params.INCLUDE_MMU) {
        if (isPartitionException(Nios_II, VA)) {
            //
            // User Mode access to Kernel space
            //
            if (attrs) {
                Nios_IIMemoryExceptCommon(Nios_II, EXC_SUPERVISOR_ONLY_DATA_ADDRESS, VA);
            }
            *action = VMI_LOAD_STORE_TERMINATE;

        } else if(isKernelDirectMapped(Nios_II, VA)) {
                *action = VMI_LOAD_STORE_CONTINUE;

        } else if(!Nios_IITLBMiss(Nios_II, MEM_PRIV_R, (Uns32)address, attrs)) {
            *action = VMI_LOAD_STORE_CONTINUE;
        }

    } else if (Nios_II->params.INCLUDE_MPU) {
        //
        // Handling for an MPU Present
        //
        if (!Nios_IIMPUMiss(Nios_II, MEM_PRIV_R, (Uns32)address, attrs)) {
            *action = VMI_LOAD_STORE_CONTINUE;
        }
    } else {
        //vmiMessage("F", "PRIV", "Nios_IIRdPrivExcept / (!MPU && !MMU)");

    }
}

VMI_WR_PRIV_EXCEPT_FN(Nios_IIWrPrivExcept) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_II->exception = EXC_NONE;
    Uns32 VA = (Uns32)address;

    if (Nios_II->params.INCLUDE_MMU) {
        if (isPartitionException(Nios_II, VA)) {
            //
            // User Mode access to Kernel space
            //
            if (attrs) {
                Nios_IIMemoryExceptCommon(Nios_II, EXC_SUPERVISOR_ONLY_DATA_ADDRESS, VA);
            }
            *action = VMI_LOAD_STORE_TERMINATE; // Remove ?

        } else if(isKernelDirectMapped(Nios_II, VA)) {
                *action = VMI_LOAD_STORE_CONTINUE;

        } else if(!Nios_IITLBMiss(Nios_II, MEM_PRIV_W, (Uns32)address, attrs)) {
            *action = VMI_LOAD_STORE_CONTINUE;
        }

    } else if (Nios_II->params.INCLUDE_MPU) {
        //
        // Handling for an MPU Present
        //
        if (!Nios_IIMPUMiss(Nios_II, MEM_PRIV_W, (Uns32)address, attrs)) {
            *action = VMI_LOAD_STORE_CONTINUE;
        }

    } else {
        //vmiMessage("F", "PRIV", "Nios_IIWrPrivExcept / (!MPU && !MMU)");

    }
}

//
// EXC_MISALIGNED_DATA_ADDRESS
//
VMI_RD_ALIGN_EXCEPT_FN(Nios_IIRdAlignExcept) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_II->exception = EXC_NONE;

    //
    // Is the alignment check enabled ?
    //
    Uns8 idx = Nios_IIGetExceptionIndex(EXC_MISALIGNED_DATA_ADDRESS);
    if (Nios_II->exceptionTable[idx].Available == EXC_MISALIGNED_DATA_ADDRESS) {

        if (Nios_II->params.EXCEPTION_EXTRA_INFORMATION) {
            Nios_II->badaddr = (Uns32)address;
        }
        exceptionAction(Nios_II, EXC_MISALIGNED_DATA_ADDRESS, getPC(Nios_II)+4);

    } else {
        //
        // This behavior is undefined, which returns 0xbadc0de for the read
        //
    }

    return 0;
}

VMI_WR_ALIGN_EXCEPT_FN(Nios_IIWrAlignExcept) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_II->exception = EXC_NONE;

    //
    // Is the alignment check enabled ?
    //
    Uns8 idx = Nios_IIGetExceptionIndex(EXC_MISALIGNED_DATA_ADDRESS);
    if (Nios_II->exceptionTable[idx].Available == EXC_MISALIGNED_DATA_ADDRESS) {

        if (Nios_II->params.EXCEPTION_EXTRA_INFORMATION) {
            Nios_II->badaddr = (Uns32)address;
        }
        exceptionAction(Nios_II, EXC_MISALIGNED_DATA_ADDRESS, getPC(Nios_II)+4);

    } else {
        //
        // This behavior is undefined, which returns 0xbadc0de for the read
        //
    }

    return 0;
}

VMI_RD_ABORT_EXCEPT_FN(Nios_IIRdAbortExcept) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_II->exception = EXC_NONE;

    if (isFetch) {
        vmiMessage("W", "ABORT", "VMI_RD_ABORT_EXCEPT_FN Fetch, PC=0x%08x", (Uns32)address);
    } else {
        vmiMessage("W", "ABORT", "VMI_RD_ABORT_EXCEPT_FN (0x%08x)Load, address=0x%08x",
                (Uns32)vmirtGetPC(processor), (Uns32)address);
    }
    vmirtExit(processor);
}

VMI_WR_ABORT_EXCEPT_FN(Nios_IIWrAbortExcept) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_II->exception = EXC_NONE;

    vmiMessage("W", "ABORT", "VMI_WR_ABORT_EXCEPT_FN (0x%08x)Store, address=0x%08x",
           (Uns32)vmirtGetPC(processor), (Uns32)address);
    vmirtExit(processor);
}

VMI_ARITH_EXCEPT_FN(Nios_IIArithExcept) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_II->exception = EXC_NONE;

    Uns8 idx = Nios_IIGetExceptionIndex(EXC_DIVISION_ERROR);

    switch(exceptionType) {
    case VMI_INTEGER_DIVIDE_BY_ZERO:
    case VMI_INTEGER_OVERFLOW:
        if (Nios_II->exceptionTable[idx].Available == EXC_DIVISION_ERROR) {
            //
            // Take exception xception checking enabled
            //
            exceptionAction(Nios_II, EXC_DIVISION_ERROR, getPC(Nios_II)+4);
            return VMI_INTEGER_ABORT;
        } else {
            //
            // Return as handled - target value is undefined
            //
            return VMI_INTEGER_CONTINUE;
        }
        break;

    default:
        vmiPrintf("Exception UNKNOWN\n");
        break;
    }

    //
    // Unhandled exception
    //
    return VMI_INTEGER_UNHANDLED;
}

VMI_FP_ARITH_EXCEPT_FN(Nios_IIFPArithExcept) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_II->exception = EXC_NONE;

//    vmiPrintf("VMI_FP_ARITH_EXCEPT_FN\n");

    return VMI_FLOAT_UNHANDLED;
}

void Nios_IIProcessInterrupt(vmiProcessorP processor) {

    Nios_IIP Nios_II = (Nios_IIP)processor;

    Uns32 ea = getPC(Nios_II)+4;
    if (Nios_II->params.INTERRUPT_CONTROLLER_INTERFACE == 0) {
        exceptionAction(Nios_II, EXC_INTERNAL_INTERRUPT, ea);
    } else {
        exceptionAction(Nios_II, EXC_EXTERNAL_MASKABLE_INTERRUPT, ea);
    }
}

//
// Validate that the passed address is a mapped fetch address
//
static Bool validateFetchAddress(Nios_IIP Nios_II, Uns32 simPC, Bool complete) {

    memAccessAttrs attrs = complete ? MEM_AA_TRUE : MEM_AA_FALSE;

    if(vmirtIsExecutable((vmiProcessorP)Nios_II, simPC)) {
        // no exception pending
        return True;
    }

    //
    // Handling for an MMU Present
    //
    if (Nios_II->params.INCLUDE_MMU) {
        if (isPartitionException(Nios_II, simPC)) {
            if(complete) {
                Nios_IIMemoryExceptCommon(Nios_II, EXC_SUPERVISOR_ONLY_INSTRUCTION_ADDRESS, simPC);
            }
            return False;

        } else if(isKernelDirectMapped(Nios_II, simPC)) {
           return False;

        } else if(Nios_IITLBMiss(Nios_II, MEM_PRIV_X, simPC, attrs)) {
            // TLB miss exception of some kind, handled by TLBMiss, so no
            // further action required here.
            return False;

        } else {
            // no exception pending
            return True;
        }

    } else if (Nios_II->params.INCLUDE_MPU) {
        //
        // Handling for an MPU Present
        //
        if (Nios_IIMPUMiss(Nios_II, MEM_PRIV_X, simPC, attrs)) {
            vmiPrintf("NiosException,%d::problem!!!!\n",vmirtCPUId((vmiProcessorP)Nios_II));
            // MPU miss exception of some kind, handled by MPUMiss, so no
            // further action required here.
            return False;
        } else {
            // no exception pending
            return True;
        }
        return False;
    }

    // no exception pending
    return True;
}

VMI_IFETCH_FN(Nios_IIIfetchExcept) {
    Uns32 simPC      = address;
    Nios_IIP Nios_II = (Nios_IIP)processor;
    Nios_II->exception = EXC_NONE;

    Bool  fetchOK = False;

    //
    // Perform test conditions here.
    //
    if(isIntActive(Nios_II)) {

        // update registers to complete interrupt exception if required
        if(complete) {
            Nios_IIProcessInterrupt(processor);
        }

    } else if(!validateFetchAddress(Nios_II, simPC, complete)) {

        // mapping error - (handled in validateFetchAddress)

    } else {

        // no exception pending
        fetchOK = True;

    }

    // return appropriate result
    if(fetchOK) {
        return VMI_FETCH_NONE;
    } else if(complete) {
        return VMI_FETCH_EXCEPTION_COMPLETE;
    } else {
        return VMI_FETCH_EXCEPTION_PENDING;
    }
}

VMI_ICOUNT_FN(Nios_IIIcountExcept) {
    vmiPrintf("VMI_ICOUNT_FN\n");
}

//
// Table of exception mode descriptors
//
static const vmiExceptionInfo exceptions[] = {
    { "NONE",                                EXC_NONE                                },
    { "RESET",                               EXC_RESET                               },
    { "HARDWARE_BREAK",                      EXC_HARDWARE_BREAK                      },
    { "PROCESSOR_ONLY_RESET_REQUEST",        EXC_PROCESSOR_ONLY_RESET_REQUEST        },
    { "INTERNAL_INTERRUPT",                  EXC_INTERNAL_INTERRUPT                  },
    { "EXTERNAL_NONMASKABLE_INTERRUPT",      EXC_EXTERNAL_NONMASKABLE_INTERRUPT      },
    { "EXTERNAL_MASKABLE_INTERRUPT",         EXC_EXTERNAL_MASKABLE_INTERRUPT         },
    { "SUPERVISOR_ONLY_INSTRUCTION_ADDRESS", EXC_SUPERVISOR_ONLY_INSTRUCTION_ADDRESS },
    { "FAST_TLB_MISS_INSTRUCTION",           EXC_FAST_TLB_MISS_INSTRUCTION           },
    { "DOUBLE_TLB_MISS_INSTRUCTION",         EXC_DOUBLE_TLB_MISS_INSTRUCTION         },
    { "TLB_PERMISSION_VIOLATION_EXECUTE",    EXC_TLB_PERMISSION_VIOLATION_EXECUTE    },
    { "MPU_REGION_VIOLATION_INSTRUCTION",    EXC_MPU_REGION_VIOLATION_INSTRUCTION    },
    { "SUPERVISOR_ONLY_INSTRUCTION",         EXC_SUPERVISOR_ONLY_INSTRUCTION         },
    { "TRAP_INSTRUCTION",                    EXC_TRAP_INSTRUCTION                    },
    { "ILLEGAL_INSTRUCTION",                 EXC_ILLEGAL_INSTRUCTION                 },
    { "UNIMPLEMENTED_INSTRUCTION",           EXC_UNIMPLEMENTED_INSTRUCTION           },
    { "BREAK_INSTRUCTION",                   EXC_BREAK_INSTRUCTION                   },
    { "SUPERVISOR_ONLY_DATA_ADDRESS",        EXC_SUPERVISOR_ONLY_DATA_ADDRESS        },
    { "MISALIGNED_DATA_ADDRESS",             EXC_MISALIGNED_DATA_ADDRESS             },
    { "MISALIGNED_DESTINATION_ADDRESS",      EXC_MISALIGNED_DESTINATION_ADDRESS      },
    { "DIVISION_ERROR",                      EXC_DIVISION_ERROR                      },
    { "FAST_TLB_MISS_DATA",                  EXC_FAST_TLB_MISS_DATA                  },
    { "DOUBLE_TLB_MISS_DATA",                EXC_DOUBLE_TLB_MISS_DATA                },
    { "TLB_PERMISSION_VIOLATION_READ",       EXC_TLB_PERMISSION_VIOLATION_READ       },
    { "TLB_PERMISSION_VIOLATION_WRITE",      EXC_TLB_PERMISSION_VIOLATION_WRITE      },
    { "MPU_REGION_VIOLATION_DATA",           EXC_MPU_REGION_VIOLATION_DATA           },
    { 0 }
};

VMI_GET_EXCEPTION_FN(Nios_IIGetException) {
    Nios_IIP Nios_II = (Nios_IIP)processor;
    return &exceptions[Nios_II->exception];
}

VMI_EXCEPTION_INFO_FN(Nios_IIExceptionInfo) {

    // on the first call, start with the first member of the table
    if(!prev) {
        return exceptions;
    }
    prev++;
    return prev->name ? prev : 0;
}

static void checkVectorAlign4(Nios_IIP Nios_II, Addr vector, const char *paramName) {
    const char *name = vmirtProcessorName((vmiProcessorP) Nios_II);
    if (vector & 0x3) {
        vmiMessage("F", "ALIGN4_VEC", "Misaligned %s/%s = 0x%08x", name, paramName, (Uns32)vector);
    }
}

#define LOCKED(_name) ._name ## locked = True

// NiosII/e
//     CORE
//         32 bit RISC
//     Reset Vector
//         ResetVector       VALUE=config
//     Exception Vector
//         ExceptionVector   VALUE=config
//     Caches & Memory IF
//     Advanced Features
//         General
//             Interrupt Controller (Internal)
//             include cpu_resetrequest & cpu_resettaken
//             cpuid           VALUE=config
//         Exception Checking
//             Illegal Instructions (True|False)
//         HardCopy Compatibility
//             Compatible (True|False)
//     MMU & MPU Settings
//     JTAG (ignored)
const Nios_IIParameters NIOS_II_E_Param_Locked = {
        LOCKED(HW_MULTIPLY),
        LOCKED(HW_MULX),
        LOCKED(HW_DIVIDE),
        LOCKED(INCLUDE_MMU),
        LOCKED(FAST_TLB_MISS_EXCEPTION_VECTOR),
        LOCKED(INCLUDE_MPU),
        LOCKED(EXCEPTION_DIVISION_ERROR),
        LOCKED(EXCEPTION_MISALIGNED_MEMORY_ACCESS),
        LOCKED(EXCEPTION_EXTRA_INFORMATION),
        LOCKED(NUMBER_SHADOW_REGISTER_SETS),
        LOCKED(HARDCOPY_COMPATIBILITY),
        LOCKED(MMU_PID_BITS),
        LOCKED(MMU_OPTIMIZE_NUMBER_OF_TLB_ENTRIES),
        LOCKED(MMU_TLB_ENTRIES),
        LOCKED(MMU_TLB_SET_ASSOCIATIVITY),
        LOCKED(MMU_MICRO_DTLB_ENTRIES),
        LOCKED(MMU_MICRO_ITLB_ENTRIES),
        LOCKED(MPU_USE_LIMIT_FOR_REGION_RANGE),
        LOCKED(MPU_NUMBER_DATA_REGIONS),
        LOCKED(MPU_MINIMUM_DATA_REGION_SIZE),
        LOCKED(MPU_NUMBER_INSTRUCTION_REGIONS),
        LOCKED(MPU_MINIMUM_INSTRUCTION_REGION_SIZE),
        LOCKED(CUSTOM_FP_DIVISION),
        LOCKED(CUSTOM_BITSWAP),
        LOCKED(CUSTOM_ENDIAN_CONVERT),
        LOCKED(CUSTOM_INTERRUPT_VECTOR)
};

// NiosII/s
//     +NiosII/e
//     CORE
//         Hardware Arithmetic Operation
//             Hardware Multiply (DSP, LOGIC, NONE)
//             Hardware Divide (True|False)
//
const Nios_IIParameters NIOS_II_S_Param_Locked = {
        LOCKED(INCLUDE_MMU),
        LOCKED(FAST_TLB_MISS_EXCEPTION_VECTOR),
        LOCKED(INCLUDE_MPU),
        LOCKED(EXCEPTION_DIVISION_ERROR),
        LOCKED(EXCEPTION_MISALIGNED_MEMORY_ACCESS),
        LOCKED(EXCEPTION_EXTRA_INFORMATION),
        LOCKED(NUMBER_SHADOW_REGISTER_SETS),
        LOCKED(HARDCOPY_COMPATIBILITY),
        LOCKED(MMU_PID_BITS),
        LOCKED(MMU_OPTIMIZE_NUMBER_OF_TLB_ENTRIES),
        LOCKED(MMU_TLB_ENTRIES),
        LOCKED(MMU_TLB_SET_ASSOCIATIVITY),
        LOCKED(MMU_MICRO_DTLB_ENTRIES),
        LOCKED(MMU_MICRO_ITLB_ENTRIES),
        LOCKED(MPU_USE_LIMIT_FOR_REGION_RANGE),
        LOCKED(MPU_NUMBER_DATA_REGIONS),
        LOCKED(MPU_MINIMUM_DATA_REGION_SIZE),
        LOCKED(MPU_NUMBER_INSTRUCTION_REGIONS),
        LOCKED(MPU_MINIMUM_INSTRUCTION_REGION_SIZE),
        LOCKED(CUSTOM_FP_DIVISION),
        LOCKED(CUSTOM_BITSWAP),
        LOCKED(CUSTOM_ENDIAN_CONVERT),
        LOCKED(CUSTOM_INTERRUPT_VECTOR)
};

// NiosII/f
//     +NiosII/s
//     CORE
//         Barrel Shifter (True)
//     MMU & MPU Settings (mutually exclusive)
//         Include MMU (MPU==FALSE)
//             FAST TLB Exception Vector   VALUE=config
//         Include MPU (MMU==FALSE)
//     Advanced Features
//         General
//             Interrupt Controller (Internal | External)
//             Shadow register Sets (0-63)
//         Exception Checking
//             Division Error          (True|False) (? MMU)
//             Misaligned memory access    (True|False) (? MMU)
//             Extra exception information (True|False) (? MMU)
//     MMU & MPU Settings
//         MMU
//             PID BITS (8-14)
//             TLB Entries (128,256,512,1024)
//             TLB Set-Associativity (8,16) ways
//         MPU
//             Use Limit For Region Range (True|False)
//             Number data regions (2-32)
//             minimum data regions size(64B,128,256,1K,2,4,8,16,32,64,128,256,512,1M)
//             Number instruction regions (2-32)
//             minimum instruction regions size(64B,128,256,1K,2,4,8,16,32,64,128,256,512,1M)
const Nios_IIParameters NIOS_II_F_Param_Locked = {
};

//
// Report error if default is different to requested, and the parameter is locked
// .u.boolParam.defaultValue
//
#define TESTMEMBER(_name, _arch) \
    if (Nios_II->params._name ## set && _arch ## _Param_Locked._name ## locked) { \
        vmiMessage("I", "LOCKED_PARAM", "Parameter %s in Variant %s is Locked\n", # _name, # _arch); \
    }

#define TESTLOCKS(_arch)                                    \
    TESTMEMBER(HW_MULTIPLY, _arch)                          \
    TESTMEMBER(HW_MULX, _arch)                              \
    TESTMEMBER(HW_DIVIDE, _arch)                            \
    TESTMEMBER(RESET_VECTOR, _arch)                         \
    TESTMEMBER(INST_ADDR_WIDTH, _arch)                      \
    TESTMEMBER(DATA_ADDR_WIDTH, _arch)                      \
    TESTMEMBER(EXCEPTION_VECTOR, _arch)                     \
    TESTMEMBER(BREAK_VECTOR, _arch)                         \
    TESTMEMBER(INCLUDE_MMU, _arch)                          \
    TESTMEMBER(FAST_TLB_MISS_EXCEPTION_VECTOR, _arch)       \
    TESTMEMBER(INCLUDE_MPU, _arch)                          \
    TESTMEMBER(INCLUDE_CPURESETREQUEST, _arch)              \
    TESTMEMBER(INCLUDE_CPURESETTAKEN, _arch)                \
    TESTMEMBER(CPUID_CONTROL_VALUE, _arch)                  \
    TESTMEMBER(EXCEPTION_ILLEGAL_INSTRUCTION, _arch)        \
    TESTMEMBER(EXCEPTION_DIVISION_ERROR, _arch)             \
    TESTMEMBER(EXCEPTION_MISALIGNED_MEMORY_ACCESS, _arch)   \
    TESTMEMBER(EXCEPTION_EXTRA_INFORMATION, _arch)          \
    TESTMEMBER(INTERRUPT_CONTROLLER_INTERFACE, _arch)       \
    TESTMEMBER(NUMBER_SHADOW_REGISTER_SETS, _arch)          \
    TESTMEMBER(HARDCOPY_COMPATIBILITY, _arch)               \
    TESTMEMBER(MMU_PID_BITS, _arch)                         \
    TESTMEMBER(MMU_OPTIMIZE_NUMBER_OF_TLB_ENTRIES, _arch)   \
    TESTMEMBER(MMU_TLB_ENTRIES, _arch)                      \
    TESTMEMBER(MMU_TLB_SET_ASSOCIATIVITY, _arch)            \
    TESTMEMBER(MMU_MICRO_DTLB_ENTRIES, _arch)               \
    TESTMEMBER(MMU_MICRO_ITLB_ENTRIES, _arch)               \
    TESTMEMBER(MPU_USE_LIMIT_FOR_REGION_RANGE, _arch)       \
    TESTMEMBER(MPU_NUMBER_DATA_REGIONS, _arch)              \
    TESTMEMBER(MPU_MINIMUM_DATA_REGION_SIZE, _arch)         \
    TESTMEMBER(MPU_NUMBER_INSTRUCTION_REGIONS, _arch)       \
    TESTMEMBER(MPU_MINIMUM_INSTRUCTION_REGION_SIZE, _arch)  \
    TESTMEMBER(CUSTOM_FP_DIVISION, _arch)                   \
    TESTMEMBER(CUSTOM_BITSWAP, _arch)                       \
    TESTMEMBER(CUSTOM_ENDIAN_CONVERT, _arch)                \
    TESTMEMBER(CUSTOM_INTERRUPT_VECTOR, _arch)

static void Nios_IIRegisterInit (Nios_IIP Nios_II) {
    //
    // Get the variant
    //
    if (Nios_II->configInfo.arch == NIOS_II_E) {
        TESTLOCKS(NIOS_II_E)

    } else if (Nios_II->configInfo.arch == NIOS_II_S) {
        TESTLOCKS(NIOS_II_S)

    } else if (Nios_II->configInfo.arch == NIOS_II_F) {
        TESTLOCKS(NIOS_II_F)

    } else {
        //
        // ???
        //
    }

    //
    // Shadow Sets
    //
    Nios_II->sstatus.reg = 0x00000000;
    Int8 i, j;
    for(i=0; i <= Nios_II->params.NUMBER_SHADOW_REGISTER_SETS; i++) {
        for(j=0; j<32; j++) {
            // Iter the shadow sets
        }
    }

    //
    // Which Exceptions are available ?
    //
    Bool INTI = Nios_II->params.INTERRUPT_CONTROLLER_INTERFACE == 0;
    Bool EXTI = Nios_II->params.INTERRUPT_CONTROLLER_INTERFACE == 1;
    Bool MMU  = Nios_II->params.INCLUDE_MMU;
    Bool MPU  = Nios_II->params.INCLUDE_MPU;
    Bool ILL  = Nios_II->params.EXCEPTION_ILLEGAL_INSTRUCTION;
    Bool MEM  = Nios_II->params.EXCEPTION_MISALIGNED_MEMORY_ACCESS;
    Bool DIV  = Nios_II->params.EXCEPTION_DIVISION_ERROR;

    //
    // It is illegal to have an MMU and an MPU
    //
    if (MPU && MMU) {
        const char *name = vmirtProcessorName((vmiProcessorP) Nios_II);
        vmiMessage("F", "ILLEGAL", "%s/INCLUDE_MMU=1 && %s/INCLUDE_MPU=1", name, name);
    }

    //
    // Vector Addresses
    //
    Addr RESET     = Nios_II->params.RESET_VECTOR;
    Addr BREAK     = Nios_II->params.BREAK_VECTOR;
    Addr EXCEPTION = Nios_II->params.EXCEPTION_VECTOR;
    Addr FAST_TLB  = Nios_II->params.FAST_TLB_MISS_EXCEPTION_VECTOR;
    Addr HANDLER   = 0x00000000;

    //
    // Check vector alignment
    //
    checkVectorAlign4(Nios_II, RESET,     "RESET_VECTOR");
    checkVectorAlign4(Nios_II, BREAK,     "BREAK_VECTOR");
    checkVectorAlign4(Nios_II, EXCEPTION, "EXCEPTION_VECTOR");
    checkVectorAlign4(Nios_II, FAST_TLB,  "FAST_TLB_MISS_EXCEPTION_VECTOR");

    //                Excep                                             Cond  Cause       Vector
    excEntry(Nios_II, EXC_NONE,                                        False,     0,         0x0);
    excEntry(Nios_II, EXC_RESET,                                        True,     0,       RESET);
    excEntry(Nios_II, EXC_HARDWARE_BREAK,                               True,     0,       BREAK);
    excEntry(Nios_II, EXC_PROCESSOR_ONLY_RESET_REQUEST,                 True,     1,       RESET);
    excEntry(Nios_II, EXC_INTERNAL_INTERRUPT,                           INTI,     2,   EXCEPTION);
    excEntry(Nios_II, EXC_EXTERNAL_NONMASKABLE_INTERRUPT,               EXTI,     0,     HANDLER);
    excEntry(Nios_II, EXC_EXTERNAL_MASKABLE_INTERRUPT,                  EXTI,     2,     HANDLER);
    excEntry(Nios_II, EXC_SUPERVISOR_ONLY_INSTRUCTION_ADDRESS,           MMU,     9,   EXCEPTION);
    excEntry(Nios_II, EXC_FAST_TLB_MISS_INSTRUCTION,                     MMU,    12,    FAST_TLB);
    excEntry(Nios_II, EXC_DOUBLE_TLB_MISS_INSTRUCTION,                   MMU,    12,   EXCEPTION);
    excEntry(Nios_II, EXC_TLB_PERMISSION_VIOLATION_EXECUTE,              MMU,    13,   EXCEPTION);
    excEntry(Nios_II, EXC_MPU_REGION_VIOLATION_INSTRUCTION,              MPU,    16,   EXCEPTION);
    excEntry(Nios_II, EXC_SUPERVISOR_ONLY_INSTRUCTION,                   MMU,    10,   EXCEPTION);
    excEntry(Nios_II, EXC_TRAP_INSTRUCTION,                             True,     3,   EXCEPTION);
    excEntry(Nios_II, EXC_ILLEGAL_INSTRUCTION,                 (ILL|MMU|MPU),     5,   EXCEPTION);
    excEntry(Nios_II, EXC_UNIMPLEMENTED_INSTRUCTION,                    True,     4,   EXCEPTION);
    excEntry(Nios_II, EXC_BREAK_INSTRUCTION,                            True,     0,       BREAK);
    excEntry(Nios_II, EXC_SUPERVISOR_ONLY_DATA_ADDRESS,                  MMU,    11,   EXCEPTION);
    excEntry(Nios_II, EXC_MISALIGNED_DATA_ADDRESS,             (MEM|MMU|MPU),     6,   EXCEPTION);
    excEntry(Nios_II, EXC_MISALIGNED_DESTINATION_ADDRESS,      (MEM|MMU|MPU),     7,   EXCEPTION);
    excEntry(Nios_II, EXC_DIVISION_ERROR,                                DIV,     8,   EXCEPTION);
    excEntry(Nios_II, EXC_FAST_TLB_MISS_DATA,                            MMU,    12,    FAST_TLB);
    excEntry(Nios_II, EXC_DOUBLE_TLB_MISS_DATA,                          MMU,    12,   EXCEPTION);
    excEntry(Nios_II, EXC_TLB_PERMISSION_VIOLATION_READ,                 MMU,    14,   EXCEPTION);
    excEntry(Nios_II, EXC_TLB_PERMISSION_VIOLATION_WRITE,                MMU,    15,   EXCEPTION);
    excEntry(Nios_II, EXC_MPU_REGION_VIOLATION_DATA,                     MPU,    17,   EXCEPTION);

    Uns32 dbus=32, ibus=32;
    if (Nios_II->params.DATA_ADDR_WIDTHset) {
        dbus = Nios_II->params.DATA_ADDR_WIDTH;
    }
    if (Nios_II->params.INST_ADDR_WIDTHset) {
        ibus = Nios_II->params.INST_ADDR_WIDTH;
    }
    Nios_II->sizeInstDataBus = (dbus > ibus) ? dbus : ibus;

    //
    // Reset Values
    //
    TYPE_status nxt_status = {.reg = 0x00000000};
    TYPE_config nxt_config = {.reg = 0x00000000};

    //
    // Contains shadow register sets and external interrupt controller
    //
    if (Nios_II->params.NUMBER_SHADOW_REGISTER_SETS && EXTI) {
        nxt_status.bits.RSIE = 1;
    } else {
        nxt_status.bits.RSIE = 0;
    }
    Nios_IIWriteStatus(Nios_II, nxt_status);

    Nios_II->estatus.reg      = 0x00000000;
    Nios_II->bstatus.reg      = 0x00000000;
    Nios_II->ienable          = 0x00000000;
    Nios_II->ipending         = 0x00000000; // Read Only
    Nios_II->cpuid            = Nios_II->params.CPUID_CONTROL_VALUE; // Read Only
    Nios_II->except.reg       = 0x00000000;
    Nios_II->pteaddr.reg      = 0x00000000;
    Nios_II->tlbacc.reg       = 0x00000000;
    Nios_II->tlbmisc.reg      = 0x00000000;
    Nios_II->badaddr          = 0x00000000; // Read Only
    Nios_II->mpubase.reg      = 0x00000000;
    Nios_II->mpuacc.reg       = 0x00000000;

    Nios_IIWriteConfig(Nios_II, nxt_config);

    //
    // Read Write Attributes
    //
    Nios_II->status_wmask.reg = 0;
    Nios_II->status_rmask.reg = 0;
    if (Nios_II->params.NUMBER_SHADOW_REGISTER_SETS && EXTI) {
        Nios_II->status_wmask.bits.RSIE = 1;
        Nios_II->status_rmask.bits.RSIE = 1;
    }
    if (Nios_II->params.NUMBER_SHADOW_REGISTER_SETS > 0) {
        Nios_II->status_wmask.bits.PRS = ~0;

        Nios_II->status_rmask.bits.PRS = ~0;
        Nios_II->status_rmask.bits.CRS = ~0;    // This field is read !write
    }
    // NMI is only readable
    // Test for External Interrupt controller
//    if (EIC) {
//        Nios_II->status_wmask.bits.IL = ~0;
//        Nios_II->status_wmask.bits.IH = 1;
//        Nios_II->status_rmask.bits.IL = ~0;
//        Nios_II->status_rmask.bits.IH = 1;
//
//    }
    if (MMU) {
        Nios_II->status_wmask.bits.EH = 1;
        Nios_II->status_rmask.bits.EH = 1;
    }
    if (MMU || MPU) {
        Nios_II->status_wmask.bits.U = 1;
        Nios_II->status_rmask.bits.U = 1;
    }
    Nios_II->status_wmask.bits.PIE = 1;
    Nios_II->status_rmask.bits.PIE = 1;

    //
    // estatus and bstatus all writable
    // status + (if(NUMBER_SHADOW_REGISTER_SETS) CRS)
    //
    Nios_II->estatus_wmask.reg = Nios_II->status_wmask.reg;
    Nios_II->bstatus_wmask.reg = Nios_II->status_wmask.reg;
    if (Nios_II->params.NUMBER_SHADOW_REGISTER_SETS > 0) {
        Nios_II->estatus_wmask.bits.CRS = ~0;
        Nios_II->sstatus_wmask.bits.CRS = ~0;
    }
    Nios_II->estatus_rmask.reg = Nios_II->status_rmask.reg;
    Nios_II->bstatus_rmask.reg = Nios_II->status_rmask.reg;


    Nios_II->except_wmask.reg = ~0;
    Nios_II->except_wmask.bits.CAUSE = 0;
    Nios_II->except_wmask.bits.RSV0 = 0;
    Nios_II->except_wmask.bits.RSV1 = 0;
    Nios_II->except_rmask.reg = ~0;

    if (!MMU) {
        Nios_II->pteaddr_wmask.reg = 0;
        Nios_II->pteaddr_rmask.reg = 0;
        Nios_II->tlbacc_wmask.reg = 0;
        Nios_II->tlbacc_rmask.reg = 0;
        Nios_II->tlbmisc_wmask.reg = 0;
        Nios_II->tlbmisc_rmask.reg = 0;
    } else {
        Nios_II->pteaddr_wmask.reg = ~0;
        Nios_II->pteaddr_wmask.bits.RSV0 = 0;
        Nios_II->pteaddr_rmask.reg = ~0;

        Nios_II->tlbacc_wmask.reg = ~0;
        //
        // PFN size = MAX(DATA_ADDR_WIDTH,INST_ADDR_WIDTH) - 12
        //
        Nios_II->tlbacc_wmask.bits.PFN = (1 << (Nios_II->sizeInstDataBus - 12)) - 1;
        Nios_II->tlbacc_rmask.reg = ~0;
        Nios_II->tlbacc_rmask.bits.IG  = 0;

        Nios_II->tlbmisc_wmask.reg = ~0;
        Nios_II->tlbmisc_wmask.bits.RSV0 = 0;
        Nios_II->tlbmisc_wmask.bits.DBL  = 0;
        Nios_II->tlbmisc_wmask.bits.BAD  = 0;
        Nios_II->tlbmisc_wmask.bits.PERM = 0;
        Nios_II->tlbmisc_wmask.bits.D    = 0;
        Uns32 pidbits = 14;
        if (Nios_II->params.MMU_PID_BITSset) {
            pidbits = Nios_II->params.MMU_PID_BITS;
        }
        Nios_II->tlbmisc_wmask.bits.PID  = (1 << pidbits) - 1;
        Nios_II->tlbmisc_rmask.reg = ~0;
    }

    if (!MPU) {
        Nios_II->mpubase_wmask.reg = 0;
        Nios_II->mpubase_rmask.reg = 0;
        Nios_II->mpuacc_wmask.reg  = 0;
        Nios_II->mpuacc_rmask.reg  = 0;
        Nios_II->sstatus_wmask.reg = 0;
        Nios_II->sstatus_rmask.reg = 0;

    } else {
        Nios_II->config_wmask.reg = ~0;
        Nios_II->config_wmask.bits.RSV0 = 0;
        Nios_II->config_rmask.reg = ~0;

        Nios_II->mpubase_wmask.bits.BASE  = ~0;
        Nios_II->mpubase_rmask.bits.BASE  = ~0;
        Nios_II->mpubase_wmask.bits.INDEX = ~0;
        Nios_II->mpubase_rmask.bits.INDEX = ~0;
        Nios_II->mpubase_wmask.bits.D     = 1;
        Nios_II->mpubase_rmask.bits.D     = 1;
        if (Nios_II->params.MPU_USE_LIMIT_FOR_REGION_RANGE) {
            //
            // MASK_LIMIT = LIMIT
            //
            Nios_II->mpuacc_wmask.bits.MASK_LIMIT = ~0;
            Nios_II->mpuacc_rmask.bits.MASK_LIMIT = ~0;
        } else {
            //
            // MASK_LIMIT = MASK, setting (same as mpubase MASK)
            //
//            Nios_II->mpuacc_wmask.bits.MASK_LIMIT = mpubaseMask;
//            Nios_II->mpuacc_rmask.bits.MASK_LIMIT = mpubaseMask;

            Nios_II->mpuacc_wmask.bits.MASK_LIMIT = ~0;
            Nios_II->mpuacc_rmask.bits.MASK_LIMIT = ~0;
        }

        Nios_II->mpuacc_wmask.bits.C          = 1;
        Nios_II->mpuacc_rmask.bits.C          = 1;
        Nios_II->mpuacc_wmask.bits.PERM       = 0x7;
        Nios_II->mpuacc_rmask.bits.PERM       = 0x7;
        Nios_II->mpuacc_wmask.bits.RD         = 1;
        Nios_II->mpuacc_rmask.bits.RD         = 0;
        Nios_II->mpuacc_wmask.bits.WR         = 1;
        Nios_II->mpuacc_rmask.bits.WR         = 0;
    }

    Nios_II->sstatus_wmask.reg = ~0;
    Nios_II->sstatus_wmask.bits.RSV0 = 0;
    Nios_II->sstatus_rmask.reg = ~0;
}

void Nios_IIDoException(Nios_IIP Nios_II) {
    vmiProcessorP processor = (vmiProcessorP)Nios_II;

    vmirtRestartNext(processor);
    vmirtDoSynchronousInterrupt(processor);
}

void Nios_IICheckExceptions(Nios_IIP Nios_II) {

    if(isIntActive(Nios_II)) {
        Nios_IIDoException(Nios_II);
    }
}

void Nios_IIProcessReset(Nios_IIP Nios_II) {
    //
    // Perform reset actions, then jump to reset vector
    //
    Nios_II->exception = EXC_RESET;

    Nios_IIRegisterInit(Nios_II);

    // jump to reset vector
    vmirtSetPCException((vmiProcessorP)Nios_II, Nios_II->params.RESET_VECTOR);
}
