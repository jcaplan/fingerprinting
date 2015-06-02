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

#ifndef NIOS_II_VARIANT_H
#define NIOS_II_VARIANT_H

typedef enum Nios_IIArchitectureE {
    //
    // UISA use for User defined Instruction Set combination
    //
    UISA         = 0x00000001,

    //
    // Base Enumerated Architecture types
    //
    ISA_0        = 0x00000002,

    //
    // Compound Enumerated Architecture types
    //
    NIOS_II_F    = 0x00000004 | ISA_0,
    NIOS_II_S    = 0x00000008 | ISA_0,
    NIOS_II_E    = 0x00000010 | ISA_0,
} Nios_IIArchitecture;

//
// Which variants to create port
//
#define PORT_EXIST_RESET \
    (NIOS_II_E | NIOS_II_S | NIOS_II_F)

#define PORT_EXIST_IRQ \
    (NIOS_II_E | NIOS_II_S | NIOS_II_F)

#endif // NIOS_II_VARIANT_H
