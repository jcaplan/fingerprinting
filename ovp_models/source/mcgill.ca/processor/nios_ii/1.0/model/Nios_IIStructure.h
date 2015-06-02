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

#ifndef NIOS_II_STRUCTURE_H
#define NIOS_II_STRUCTURE_H

#include "vmi/vmiTypes.h"
#include "Nios_IITypeRefs.h"
#include "Nios_IIVariant.h"
#include "Nios_IIConfig.h"
#define NUM_MEMBERS(_A) (sizeof(_A)/sizeof((_A)[0]))
#define NIOS_II_DISASSEMBLE_MASK 0x00000001
#define NIOS_II_DISASSEMBLE(_P) ((_P)->flags & NIOS_II_DISASSEMBLE_MASK)

// decoder callback function to decode instruction at the passed address
#define NIOS_II_DECODER_FN(_NAME) void _NAME ( \
    Nios_IIP Nios_II, \
    Uns32 thisPC, \
    Nios_IIInstructionInfoP info \
)
typedef NIOS_II_DECODER_FN((*Nios_IIDecoderFn));

#define NIOS_PARAM(_type, _name) \
    _type _name;                 \
    Bool _name ## set;           \
    Bool _name ## locked;

typedef struct Nios_IIParametersS {
    NIOS_PARAM(Bool  , TEST_MODE_EXIT                     )
    NIOS_PARAM(Bool  , TEST_HALT_EXIT                     )
    NIOS_PARAM(Uns32 , INST_ADDR_WIDTH                    )
    NIOS_PARAM(Uns32 , DATA_ADDR_WIDTH                    )
    NIOS_PARAM(Bool  , HW_MULTIPLY                        )
    NIOS_PARAM(Bool  , HW_MULX                            )
    NIOS_PARAM(Bool  , HW_DIVIDE                          )
    NIOS_PARAM(Uns32 , RESET_VECTOR                       )
    NIOS_PARAM(Uns32 , EXCEPTION_VECTOR                   )
    NIOS_PARAM(Uns32 , BREAK_VECTOR                       )
    NIOS_PARAM(Bool  , INCLUDE_MMU                        )
    NIOS_PARAM(Uns32 , FAST_TLB_MISS_EXCEPTION_VECTOR     )
    NIOS_PARAM(Bool  , INCLUDE_MPU                        )
    NIOS_PARAM(Bool  , INCLUDE_CPURESETREQUEST            )
    NIOS_PARAM(Bool  , INCLUDE_CPURESETTAKEN              )
    NIOS_PARAM(Uns32 , CPUID_CONTROL_VALUE                )
    NIOS_PARAM(Bool  , EXCEPTION_ILLEGAL_INSTRUCTION      )
    NIOS_PARAM(Bool  , EXCEPTION_DIVISION_ERROR           )
    NIOS_PARAM(Bool  , EXCEPTION_MISALIGNED_MEMORY_ACCESS )
    NIOS_PARAM(Bool  , EXCEPTION_EXTRA_INFORMATION        )
    NIOS_PARAM(Uns32 , INTERRUPT_CONTROLLER_INTERFACE     )   // Enumeration
    NIOS_PARAM(Uns32 , NUMBER_SHADOW_REGISTER_SETS        )
    NIOS_PARAM(Bool  , HARDCOPY_COMPATIBILITY             )
    NIOS_PARAM(Uns32 , MMU_PID_BITS                       )
    NIOS_PARAM(Bool  , MMU_OPTIMIZE_NUMBER_OF_TLB_ENTRIES )
    NIOS_PARAM(Uns32 , MMU_TLB_ENTRIES                    )
    NIOS_PARAM(Uns32 , MMU_TLB_SET_ASSOCIATIVITY          )
    NIOS_PARAM(Uns32 , MMU_MICRO_DTLB_ENTRIES             )
    NIOS_PARAM(Uns32 , MMU_MICRO_ITLB_ENTRIES             )
    NIOS_PARAM(Bool  , MPU_USE_LIMIT_FOR_REGION_RANGE     )
    NIOS_PARAM(Uns32 , MPU_NUMBER_DATA_REGIONS            )
    NIOS_PARAM(Uns32 , MPU_MINIMUM_DATA_REGION_SIZE       )
    NIOS_PARAM(Uns32 , MPU_NUMBER_INSTRUCTION_REGIONS     )
    NIOS_PARAM(Uns32 , MPU_MINIMUM_INSTRUCTION_REGION_SIZE)
    NIOS_PARAM(Bool  , CUSTOM_FP_DIVISION                 )
    NIOS_PARAM(Bool  , CUSTOM_BITSWAP                     )
    NIOS_PARAM(Bool  , CUSTOM_ENDIAN_CONVERT              )
    NIOS_PARAM(Bool  , CUSTOM_INTERRUPT_VECTOR            )
} Nios_IIParameters;

typedef enum Nios_IIVMModeE {
    VM_MODE_KERNEL,
    VM_MODE_USER,
    VM_MODE_KERNEL_MPU,
    VM_MODE_USER_MPU,
    VM_MODE_LAST
} Nios_IIVMMode;

#define NIOS_II_VPN_SHIFT 12
#define NIOS_II_PAGE_SIZE (1 << NIOS_II_VPN_SHIFT)

typedef enum Nios_IIExceptionsS {
    EXC_NONE                                = 0x00000000,
    EXC_RESET                               = 0x00000001,
    EXC_HARDWARE_BREAK                      = 0x00000002,
    EXC_PROCESSOR_ONLY_RESET_REQUEST        = 0x00000004,
    EXC_INTERNAL_INTERRUPT                  = 0x00000008,
    EXC_EXTERNAL_NONMASKABLE_INTERRUPT      = 0x00000010,
    EXC_EXTERNAL_MASKABLE_INTERRUPT         = 0x00000020,
    EXC_SUPERVISOR_ONLY_INSTRUCTION_ADDRESS = 0x00000040,
    EXC_FAST_TLB_MISS_INSTRUCTION           = 0x00000080,
    EXC_DOUBLE_TLB_MISS_INSTRUCTION         = 0x00000100,
    EXC_TLB_PERMISSION_VIOLATION_EXECUTE    = 0x00000200,
    EXC_MPU_REGION_VIOLATION_INSTRUCTION    = 0x00000400,
    EXC_SUPERVISOR_ONLY_INSTRUCTION         = 0x00000800,
    EXC_TRAP_INSTRUCTION                    = 0x00001000,
    EXC_ILLEGAL_INSTRUCTION                 = 0x00002000,
    EXC_UNIMPLEMENTED_INSTRUCTION           = 0x00004000,
    EXC_BREAK_INSTRUCTION                   = 0x00008000,
    EXC_SUPERVISOR_ONLY_DATA_ADDRESS        = 0x00010000,
    EXC_MISALIGNED_DATA_ADDRESS             = 0x00020000,
    EXC_MISALIGNED_DESTINATION_ADDRESS      = 0x00040000,
    EXC_DIVISION_ERROR                      = 0x00080000,
    EXC_FAST_TLB_MISS_DATA                  = 0x00100000,
    EXC_DOUBLE_TLB_MISS_DATA                = 0x00200000,
    EXC_TLB_PERMISSION_VIOLATION_READ       = 0x00400000,
    EXC_TLB_PERMISSION_VIOLATION_WRITE      = 0x00800000,
    EXC_MPU_REGION_VIOLATION_DATA           = 0x01000000,

    //
    // Compound conditions
    //
    EXC_EXT_INTERRUPT_ASSERTED              = (EXC_EXTERNAL_NONMASKABLE_INTERRUPT   |
                                               EXC_EXTERNAL_MASKABLE_INTERRUPT),

    EXC_TLB_MISS_DATA                       = (EXC_FAST_TLB_MISS_DATA               |
                                               EXC_DOUBLE_TLB_MISS_DATA ),

    EXC_TLB_MISS_INSTRUCTION                = (EXC_FAST_TLB_MISS_INSTRUCTION        |
                                               EXC_DOUBLE_TLB_MISS_INSTRUCTION ),

    EXC_TLB_MISS                            = (EXC_FAST_TLB_MISS_INSTRUCTION        |
                                               EXC_DOUBLE_TLB_MISS_INSTRUCTION      |
                                               EXC_FAST_TLB_MISS_DATA               |
                                               EXC_DOUBLE_TLB_MISS_DATA ),

    EXC_DOUBLE_TLB_MISS                     = (EXC_DOUBLE_TLB_MISS_INSTRUCTION      |
                                               EXC_DOUBLE_TLB_MISS_DATA ),

    EXC_TLB_PERMISSION_VIOLATION            = (EXC_TLB_PERMISSION_VIOLATION_EXECUTE |
                                               EXC_TLB_PERMISSION_VIOLATION_READ    |
                                               EXC_TLB_PERMISSION_VIOLATION_WRITE),

    EXC_SET_BAD_FLAG                        = (EXC_SUPERVISOR_ONLY_INSTRUCTION_ADDRESS |
                                               EXC_SUPERVISOR_ONLY_DATA_ADDRESS        |
                                               EXC_MISALIGNED_DATA_ADDRESS             |
                                               EXC_MISALIGNED_DESTINATION_ADDRESS),

    EXC_TLB                                 = (EXC_TLB_MISS                         |
                                               EXC_TLB_PERMISSION_VIOLATION)
} Nios_IIException;

typedef struct Nios_IIExceptionTableS {
    Nios_IIException Exc;        // Exception bit mask
    Nios_IIException Available;  // This is dependant upon the configuration
    Uns8             Cause;      // 5 bit code in except register
    Addr             Vector;
} Nios_IIExceptionTable, *Nios_IIExceptionTableP;

// this enumeration specifies modes enabling instruction subsets
typedef enum Nios_IIBlockMaskE {
    BM_NONE       = 0x000,  // no requirement
    BM_STATUS_PIE = 0x001,  // Change state of the status.PIE (Processor Interrupt Enable)
    BM_STATUS_U   = 0x002,  // Change state of the status.U   (User Mode)
    BM_STATUS_EH  = 0x004,  // Change state of the status.IH  (Exception Handler - only MMU)
    BM_STATUS_IH  = 0x008,  // Change state of the status.IH  (Interrupt Handler - only EIC)
} Nios_IIBlockMask;

typedef struct Nios_IINetPortS *Nios_IINetPortP, **Nios_IINetPortPP;

typedef union {
    Uns32 reg;
    struct {
        Uns32        PIE : 1;
        Uns32          U : 1;
        Uns32         EH : 1;
        Uns32         IH : 1;
        Uns32         IL : 6;
        Uns32        CRS : 6;
        Uns32        PRS : 6;
        Uns32        NMI : 1;
        Uns32       RSIE : 1;
        Uns32       RSV0 : 8;
    } bits;
} TYPE_status;

typedef union {
    Uns32 reg;
    struct {
        Uns32        PIE : 1;
        Uns32          U : 1;
        Uns32         EH : 1;
        Uns32         IH : 1;
        Uns32         IL : 6;
        Uns32        CRS : 6;
        Uns32        PRS : 6;
        Uns32        NMI : 1;
        Uns32       RSIE : 1;
        Uns32       RSV0 : 8;
    } bits;
} TYPE_estatus;

typedef union {
    Uns32 reg;
    struct {
        Uns32        PIE : 1;
        Uns32          U : 1;
        Uns32         EH : 1;
        Uns32         IH : 1;
        Uns32         IL : 6;
        Uns32        CRS : 6;
        Uns32        PRS : 6;
        Uns32        NMI : 1;
        Uns32       RSIE : 1;
        Uns32       RSV0 : 8;
    } bits;
} TYPE_bstatus;

typedef union {
    Uns32 reg;
    struct {
        Uns32       RSV0 : 2;
        Uns32      CAUSE : 5;
        Uns32       RSV1 : 25;
    } bits;
} TYPE_except;

typedef union {
    Uns32 reg;
    struct {
        Uns32       RSV0 : 2;
        Uns32        VPN : 20;
        Uns32     PTBASE : 10;
    } bits;
} TYPE_pteaddr;

typedef union {
    Uns32 reg;
    struct {
        Uns32        PFN : 20;
        Uns32          G : 1;
        Uns32          X : 1;
        Uns32          W : 1;
        Uns32          R : 1;
        Uns32          C : 1;
        Uns32         IG : 7;
    } bits;
} TYPE_tlbacc;

typedef union {
    Uns32 reg;
    struct {
        Uns32          D : 1;
        Uns32       PERM : 1;
        Uns32        BAD : 1;
        Uns32        DBL : 1;
        Uns32        PID : 14;
        Uns32         WE : 1;
        Uns32         RD : 1;
        Uns32        WAY : 4;
        Uns32       RSV0 : 8;
    } bits;
} TYPE_tlbmisc;

typedef union {
    Uns32 reg;
    struct {
        Uns32         PE : 1;
        Uns32        ANI : 1;
        Uns32       RSV0 : 30;
    } bits;
} TYPE_config;

//
// The documentation indicates that bit31 is always 0, but it appears
// not to be the case in the simulation reference model
//
typedef union {
    Uns32 reg;
    struct {
        Uns32          D : 1;
        Uns32      INDEX : 5;
//        Uns32       BASE : 25;
//        Uns32       RSV0 : 1;
        Uns32       BASE : 26;
    } bits;
} TYPE_mpubase;

typedef union {
    Uns32 reg;
    struct {
        Uns32         WR : 1;
        Uns32         RD : 1;
        Uns32       PERM : 3;
        Uns32          C : 1;
        Uns32 MASK_LIMIT : 26;
    } bits;
} TYPE_mpuacc;

typedef union {
    Uns32 reg;
    struct {
        Uns32        PIE : 1;
        Uns32          U : 1;
        Uns32         EH : 1;
        Uns32         IH : 1;
        Uns32         IL : 6;
        Uns32        CRS : 6;
        Uns32        PRS : 6;
        Uns32        NMI : 1;
        Uns32       RSIE : 1;
        Uns32       RSV0 : 7;
        Uns32        SRS : 1;
    } bits;
} TYPE_sstatus;

typedef struct {
    Uns32   VA;
    Uns32   PA;
    Uns32   PID  : 16; // process ID
    memPriv priv :  8; // region privileges
    Bool    G    :  1; // global entry
    Bool    C    :  1; // cacheable (ignored)

    Bool    simMapped[VM_MODE_LAST];   // modes in which mapped
} TLBentry, *TLBentryP;

typedef struct {
    TLBentryP entry;
    Uns16     size;
    Uns16     ways;
    Uns16     rows;
} TLBtable, *TLBtableP;

typedef enum protRegionCodeE {
    PROT_ENABLED,        // enabled area description
    PROT_DISABLED,       // disabled area description
    PROT_INVALID_SIZE,   // invalid size (less than minimum)
    PROT_INVALID_BASE,   // invalid base (not a multiple of size)
} protRegionCode;

typedef struct {
    Uns32   base;
    Uns32   size;
    Bool    C;
    memPriv priv[VM_MODE_LAST];

    //
    // Register value for retrieval
    //
    TYPE_mpuacc  mpuacc;
    TYPE_mpubase mpubase;
} MPUentry, *MPUentryP;

typedef struct {
    MPUentryP data, code;
    Uns16     size;
} MPUtable, *MPUtableP;

typedef struct Nios_IIS {
    //
    // User Defined and Builtin Registers
    //
    Uns32 gpr[32];
    Uns32 TEMP[8];
    TYPE_status status, status_rmask, status_wmask;
    TYPE_estatus estatus, estatus_rmask, estatus_wmask;
    TYPE_bstatus bstatus, bstatus_rmask, bstatus_wmask;
    Uns32 ienable;
    Uns32 ipending;
    Uns32 cpuid;
    TYPE_except except, except_rmask, except_wmask;
    TYPE_pteaddr pteaddr, pteaddr_rmask, pteaddr_wmask;
    TYPE_tlbacc tlbacc, tlbacc_rmask, tlbacc_wmask;
    TYPE_tlbmisc tlbmisc, tlbmisc_rmask, tlbmisc_wmask;
    Uns32 badaddr;
    TYPE_config config, config_rmask, config_wmask;
    TYPE_mpubase mpubase, mpubase_rmask, mpubase_wmask;
    TYPE_mpuacc mpuacc, mpuacc_rmask, mpuacc_wmask;
    TYPE_sstatus sstatus, sstatus_rmask, sstatus_wmask;
    Bool FLG_CI;
    Bool FLG_CO;
    Bool FLG_PA;
    Bool FLG_ZR;
    Bool FLG_SI;
    Bool FLG_OV;

    Uns32 shadow[64][32];

    //
    // TLB Table Pointers
    //
    TLBtable tlb;     // MMU_TLB_ENTRIES / MMU_TLB_SET_ASSOCIATIVITY

    //
    // MPU Table Pointers
    //
    MPUtable mpu;     // MPU table of entries

    //
    // Model Support Registers
    //
    Uns32 flags;
    Nios_IIConfig configInfo;
    Bool verbose;
    memEndian dendian;
    memEndian iendian;
    Nios_IIException      exception;
    Nios_IIExceptionTable exceptionTable[32];

    // Parameters
    Nios_IIParameters params;
    Uns32 sizeInstDataBus;

    //Fingerprinting
    Uns32 fprint_write_out;
    Uns32 fprint_write_out_address;
    Uns32 fprint_write_out_data;

    //
    // Core Signals
    //
    Bool  ResetPending;
    Uns32 reset_n;
    Uns32 d_irq;

    // Bus ports
    vmiBusPortP busPorts;

    // Net ports
    Nios_IINetPortP netPorts;

    // generic instruction decoder
    Nios_IIDecoderFn decoder;

    // virtual memory domain
    memDomainP physDomainC;
    memDomainP physDomainD;
    memDomainP virtDomainC[VM_MODE_LAST];
    memDomainP virtDomainD[VM_MODE_LAST];

} Nios_II;

#define NIOS_II_CPU_OFFSET(_F) VMI_CPU_OFFSET(Nios_IIP, _F)
#define NIOS_II_CPU_REG(_F) VMI_CPU_REG(Nios_IIP, _F)

#define NIOS_II_GPR_BITS (sizeof(Uns32) * 8)
#define NIOS_II_GPR_RD(_R) NIOS_II_CPU_REG(gpr[_R])
#define NIOS_II_GPR_WR(_R) ((_R==0)) ? VMI_NOREG : NIOS_II_CPU_REG(gpr[_R])

#define NIOS_II_TEMP_BITS (sizeof(Uns32) * 8)
#define NIOS_II_TEMP_RD(_R) NIOS_II_CPU_REG(TEMP[_R])
#define NIOS_II_TEMP_WR(_R) NIOS_II_CPU_REG(TEMP[_R])

#define NIOS_II_FLG_CI_BITS (sizeof(Bool) * 8)
#define NIOS_II_FLG_CI_RD NIOS_II_CPU_REG(FLG_CI)
#define NIOS_II_FLG_CI_WR NIOS_II_CPU_REG(FLG_CI)

#define NIOS_II_FLG_CO_BITS (sizeof(Bool) * 8)
#define NIOS_II_FLG_CO_RD NIOS_II_CPU_REG(FLG_CO)
#define NIOS_II_FLG_CO_WR NIOS_II_CPU_REG(FLG_CO)

#define NIOS_II_FLG_PA_BITS (sizeof(Bool) * 8)
#define NIOS_II_FLG_PA_RD NIOS_II_CPU_REG(FLG_PA)
#define NIOS_II_FLG_PA_WR NIOS_II_CPU_REG(FLG_PA)

#define NIOS_II_FLG_ZR_BITS (sizeof(Bool) * 8)
#define NIOS_II_FLG_ZR_RD NIOS_II_CPU_REG(FLG_ZR)
#define NIOS_II_FLG_ZR_WR NIOS_II_CPU_REG(FLG_ZR)

#define NIOS_II_FLG_SI_BITS (sizeof(Bool) * 8)
#define NIOS_II_FLG_SI_RD NIOS_II_CPU_REG(FLG_SI)
#define NIOS_II_FLG_SI_WR NIOS_II_CPU_REG(FLG_SI)

#define NIOS_II_FLG_OV_BITS (sizeof(Bool) * 8)
#define NIOS_II_FLG_OV_RD NIOS_II_CPU_REG(FLG_OV)
#define NIOS_II_FLG_OV_WR NIOS_II_CPU_REG(FLG_OV)

#define NIOS_II_CPU_REG_CONST(_F) VMI_CPU_REG_CONST(Nios_IIP, _F)
#define NIOS_II_GPR_CONST(_R) NIOS_II_CPU_REG_CONST(gpr[_R])

#define NIOS_II_TEMP_CONST(_R) NIOS_II_CPU_REG_CONST(TEMP[_R])

#define NIOS_II_STATUS_CONST NIOS_II_CPU_REG_CONST(status)

#define NIOS_II_ESTATUS_CONST NIOS_II_CPU_REG_CONST(estatus)

#define NIOS_II_BSTATUS_CONST NIOS_II_CPU_REG_CONST(bstatus)

#define NIOS_II_IENABLE_CONST NIOS_II_CPU_REG_CONST(ienable)

#define NIOS_II_IPENDING_CONST NIOS_II_CPU_REG_CONST(ipending)

#define NIOS_II_CPUID_CONST NIOS_II_CPU_REG_CONST(cpuid)

#define NIOS_II_EXCEPT_CONST NIOS_II_CPU_REG_CONST(except)

#define NIOS_II_PTEADDR_CONST NIOS_II_CPU_REG_CONST(pteaddr)

#define NIOS_II_TLBACC_CONST NIOS_II_CPU_REG_CONST(tlbacc)

#define NIOS_II_TLBMISC_CONST NIOS_II_CPU_REG_CONST(tlbmisc)

#define NIOS_II_BADADDR_CONST NIOS_II_CPU_REG_CONST(badaddr)

#define NIOS_II_CONFIG_CONST NIOS_II_CPU_REG_CONST(config)

#define NIOS_II_MPUBASE_CONST NIOS_II_CPU_REG_CONST(mpubase)

#define NIOS_II_MPUACC_CONST NIOS_II_CPU_REG_CONST(mpuacc)

#define NIOS_II_SSTATUS_CONST NIOS_II_CPU_REG_CONST(sstatus)

#define NIOS_II_FLG_CI_CONST NIOS_II_CPU_REG_CONST(FLG_CI)

#define NIOS_II_FLG_CO_CONST NIOS_II_CPU_REG_CONST(FLG_CO)

#define NIOS_II_FLG_PA_CONST NIOS_II_CPU_REG_CONST(FLG_PA)

#define NIOS_II_FLG_ZR_CONST NIOS_II_CPU_REG_CONST(FLG_ZR)

#define NIOS_II_FLG_SI_CONST NIOS_II_CPU_REG_CONST(FLG_SI)

#define NIOS_II_FLG_OV_CONST NIOS_II_CPU_REG_CONST(FLG_OV)

static inline const char *getstr_CTL(int index) {
    static const char *lookup[] = {
        [0] "STATUS",
        [1] "ESTATUS",
        [2] "BSTATUS",
        [3] "IENABLE",
        [4] "IPENDING",
        [5] "CPUID",
        [6] "CTL6",
        [7] "EXCEPTION",
        [8] "PTEADDR",
        [9] "TLBACC",
        [10] "TLBMISC",
        [11] "ECCINJ",
        [12] "BADADDR",
        [13] "CONFIG",
        [14] "MPUBASE",
        [15] "MPUACC",
    };
    return lookup[index];
}

static inline const char *getstr_GPR(int index) {
    static const char *lookup[] = {
        [0] "ZERO",
        [1] "AT",
        [24] "ET",
        [25] "BT",
        [26] "GP",
        [27] "SP",
        [28] "FP",
        [29] "EA",
        [30] "BA",
        [31] "RA",
    };
    return lookup[index];
}

#define CPU_PREFIX "NIOS_II"

typedef Uns32 Nios_IIAddr;

#endif // NIOS_II_RA_H
