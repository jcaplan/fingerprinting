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

#ifndef NIOS_II_PORTS_H
#define NIOS_II_PORTS_H

#include "Nios_IITypeRefs.h"

void Nios_IINewNetPorts(Nios_IIP Nios_II);
void Nios_IIFreeNetPorts(Nios_IIP Nios_II);
VMI_NET_PORT_SPECS_FN(Nios_IINetPortSpecs);
#endif /* NIOS_II_PORTS_H */
