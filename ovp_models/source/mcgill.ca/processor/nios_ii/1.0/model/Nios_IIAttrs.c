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
#include "Nios_IIStructure.h"
#include "Nios_IIFunctions.h"

static const char *dictNames[] = {"KERNEL", "USER", "KERNEL_MPU", "USER_MPU", 0};

const vmiIASAttr modelAttrs = {
    ////////////////////////////////////////////////////////////////////////
    // VERSION & SIZE ATTRIBUTES
    ////////////////////////////////////////////////////////////////////////

    .versionString       = VMI_VERSION,         // version string (THIS MUST BE 1ST)
    .modelType           = VMI_PROCESSOR_MODEL, // model type (THIS MUST BE 2ND)
    .dictNames           = dictNames,           // null-terminated dictionary name list
    .cpuSize             = sizeof(Nios_II),    // full size of CPU object

    ////////////////////////////////////////////////////////////////////////
    // CONSTRUCTOR/DESTRUCTOR ROUTINES
    ////////////////////////////////////////////////////////////////////////

    .constructorCB      = Nios_IIConstructor, // constructor
    .vmInitCB           = Nios_IIVMInit, // virtual memory initialization
    .destructorCB       = Nios_IIDestructor, // destructor

    ////////////////////////////////////////////////////////////////////////
    // MORPHER CORE ROUTINES
    ////////////////////////////////////////////////////////////////////////

    .morphCB            = Nios_IIMorph,// morph callback
    .fetchSnapCB        = Nios_IIFetchSnap, // fetch address snap callback
    .rdSnapCB           = Nios_IIRdSnap, // read address snap callback
    .wrSnapCB           = Nios_IIWrSnap, // write address snap callback

    ////////////////////////////////////////////////////////////////////////
    // SIMULATION SUPPORT ROUTINES
    ////////////////////////////////////////////////////////////////////////

    .getEndianCB        = Nios_IIGetEndian, // get processor endianness
    .nextPCCB           = Nios_IInextPC, // get next instruction address
    .disCB              = Nios_IIDis, // disassemble instruction
    .switchCB           = Nios_IISwitch, // call on simulator context switch

    ////////////////////////////////////////////////////////////////////////
    // EXCEPTION ROUTINES
    ////////////////////////////////////////////////////////////////////////

    .rdPrivExceptCB     = Nios_IIRdPrivExcept, // read privilege exception
    .wrPrivExceptCB     = Nios_IIWrPrivExcept, // write privilege exception
    .rdAlignExceptCB    = Nios_IIRdAlignExcept, // read alignment exception
    .wrAlignExceptCB    = Nios_IIWrAlignExcept, // write alignment exception
    .rdAbortExceptCB    = Nios_IIRdAbortExcept, // read abort exception
    .wrAbortExceptCB    = Nios_IIWrAbortExcept, // write abort exception
    .ifetchExceptCB     = Nios_IIIfetchExcept, // execution privilege exception
    .arithExceptCB      = Nios_IIArithExcept, // arithmetic exception
    .icountExceptCB     = Nios_IIIcountExcept, // instruction count exception

    ////////////////////////////////////////////////////////////////////////
    // PARAMETER SUPPORT ROUTINES
    ////////////////////////////////////////////////////////////////////////

    .paramSpecsCB     = Nios_IIParamSpecs,
    .paramValueSizeCB = Nios_IIParamValueSize,

    ////////////////////////////////////////////////////////////////////////
    // PORT ROUTINES
    ////////////////////////////////////////////////////////////////////////

    .busPortSpecsCB   = Nios_IIBusPortSpecs,
    .netPortSpecsCB   = Nios_IINetPortSpecs,

    ////////////////////////////////////////////////////////////////////////
    // DEBUGGER INTEGRATION SUPPORT ROUTINES
    ////////////////////////////////////////////////////////////////////////

    .regGroupCB         = Nios_IIRegGroup, // regGroup callback
    .regInfoCB          = Nios_IIRegInfo, // regInfo callback
    .exceptionInfoCB    = Nios_IIExceptionInfo, // exceptionInfo callback
    .modeInfoCB         = Nios_IIModeInfo, // modeInfo callback
    .getExceptionCB     = Nios_IIGetException, // return current processor exception
    .getModeCB          = Nios_IIGetMode, // return current processor mode
    .regReadCB          = 0, // default regRead callback
    .regWriteCB         = 0, // default regWrite callback
    .debugCB            = Nios_IIDebug, // user-defined trace callback

    ////////////////////////////////////////////////////////////////////////
    // IMPERAS INTERCEPTED FUNCTION SUPPORT ROUTINES
    ////////////////////////////////////////////////////////////////////////

    .intReturnCB        = Nios_IIIntReturn, // morph return from opaque intercept
    .intResultCB        = Nios_IIIntResult, // set intercepted function result
    .intParCB           = Nios_IIIntPar, // push function arguments

    ////////////////////////////////////////////////////////////////////////
    // PROCESSOR INFO ROUTINE
    ////////////////////////////////////////////////////////////////////////

    .procInfoCB  = Nios_IIProcInfo

};
