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

#ifndef NIOS_II_TLB_H
#define NIOS_II_TLB_H
#include "vmi/vmiTypes.h"
#include "Nios_IIStructure.h"

void Nios_IIConstructMMU(Nios_IIP Nios_II);
void Nios_IIDestructMMU(Nios_IIP Nios_II);
void Nios_IIPrintTLB(Nios_IIP Nios_II);
void Nios_IIWriteTLB(Nios_IIP Nios_II, TLBentry entry, Uns32 way);
TLBentryP Nios_IIReadTLB(Nios_IIP Nios_II, Uns32 way, Uns32 VA);
void Nios_IISearchTLB(Nios_IIP Nios_II);
Bool Nios_IITLBMiss(Nios_IIP Nios_II, memPriv action, Uns32 VA, memAccessAttrs attrs);

#endif // NIOS_II_TLB_H
