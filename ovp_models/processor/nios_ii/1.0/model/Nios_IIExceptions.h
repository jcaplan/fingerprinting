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

#ifndef NIOS_II_EXCEPTIONS_H
#define NIOS_II_EXCEPTIONS_H
#include "vmi/vmiAttrs.h"
#include "Nios_IIStructure.h"

void Nios_IIRegisterInit (Nios_IIP Nios_II);
void Nios_IIExceptionCB (Nios_IIP Nios_II, Nios_IIException exc);
void Nios_IIMemoryExceptCommon (Nios_IIP Nios_II, Nios_IIException exc, Uns32 VA);
Uns8 Nios_IIGetExceptionIndex(Uns32 val);
void Nios_IICheckExceptions(Nios_IIP Nios_II);
void Nios_IIWriteStatus(Nios_IIP Nios_II, TYPE_status status);
TYPE_status Nios_IIReadStatus(Nios_IIP Nios_II);
void Nios_IIWriteConfig(Nios_IIP Nios_II, TYPE_config config);
TYPE_config Nios_IIReadConfig(Nios_IIP Nios_II);
void Nios_IITLBPrivException(Nios_IIP Nios_II, memPriv action, Uns32 VA);
void Nios_IITLBMissException(Nios_IIP Nios_II, memPriv action, Uns32 VA);
void Nios_IIPartitionException(Nios_IIP Nios_II, memPriv action, Uns32 VA);
void Nios_IIMPUMissException(Nios_IIP Nios_II, memPriv action, Uns32 VA);
Nios_IIVMMode Nios_IIGetVMMode(Nios_IIP Nios_II);

#endif // NIOS_II_EXCEPTIONS_H
