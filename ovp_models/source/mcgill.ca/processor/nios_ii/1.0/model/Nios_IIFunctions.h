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

#ifndef NIOS_II_FUNCTIONS_H
#define NIOS_II_FUNCTIONS_H
#include "vmi/vmiAttrs.h"

// constructor & destructor
VMI_CONSTRUCTOR_FN(Nios_IIConstructor);
VMI_VMINIT_FN(Nios_IIVMInit);
VMI_DESTRUCTOR_FN(Nios_IIDestructor);

// morph function
VMI_MORPH_FN(Nios_IIMorph);
VMI_FETCH_SNAP_FN(Nios_IIFetchSnap);
VMI_RD_WR_SNAP_FN(Nios_IIRdSnap);
VMI_RD_WR_SNAP_FN(Nios_IIWrSnap);

// simulation support functions
VMI_ENDIAN_FN(Nios_IIGetEndian);
VMI_NEXT_PC_FN(Nios_IInextPC);
VMI_DISASSEMBLE_FN(Nios_IIDis);
VMI_IASSWITCH_FN(Nios_IISwitch);

// debugger integration support routines
VMI_REG_GROUP_FN(Nios_IIRegGroup);
VMI_REG_INFO_FN(Nios_IIRegInfo);
VMI_EXCEPTION_INFO_FN(Nios_IIExceptionInfo);
VMI_MODE_INFO_FN(Nios_IIModeInfo);
VMI_GET_EXCEPTION_FN(Nios_IIGetException);
VMI_GET_MODE_FN(Nios_IIGetMode);
VMI_DEBUG_FN(Nios_IIDebug);

// exception functions
VMI_RD_PRIV_EXCEPT_FN(Nios_IIRdPrivExcept);
VMI_WR_PRIV_EXCEPT_FN(Nios_IIWrPrivExcept);
VMI_RD_ALIGN_EXCEPT_FN(Nios_IIRdAlignExcept);
VMI_WR_ALIGN_EXCEPT_FN(Nios_IIWrAlignExcept);
VMI_RD_ABORT_EXCEPT_FN(Nios_IIRdAbortExcept);
VMI_WR_ABORT_EXCEPT_FN(Nios_IIWrAbortExcept);
VMI_IFETCH_FN(Nios_IIIfetchExcept);
VMI_ARITH_EXCEPT_FN(Nios_IIArithExcept);
VMI_FP_ARITH_EXCEPT_FN(Nios_IIFPArithExcept);
VMI_ICOUNT_FN(Nios_IIIcountExcept);

// parameter support functions
VMI_PROC_PARAM_SPECS_FN(Nios_IIParamSpecs);
VMI_PROC_PARAM_TABLE_SIZE_FN(Nios_IIParamValueSize);

// port/net functions
VMI_BUS_PORT_SPECS_FN(Nios_IIBusPortSpecs);
VMI_NET_PORT_SPECS_FN(Nios_IINetPortSpecs);

// Imperas intercepted function support
VMI_INT_RETURN_FN(Nios_IIIntReturn);
VMI_INT_RESULT_FN(Nios_IIIntResult);
VMI_INT_PAR_FN(Nios_IIIntPar);

// Processor information support
VMI_PROC_INFO_FN(Nios_IIProcInfo);
#endif // NIOS_II_FUNCTIONS_H
