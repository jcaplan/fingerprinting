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

#ifndef NIOS_II_CONFIG_H
#define NIOS_II_CONFIG_H
#include "hostapi/impTypes.h"
#include "Nios_IITypeRefs.h"
#include "Nios_IIVariant.h"

typedef struct Nios_IIConfigS {
    const char *name;
    Nios_IIArchitecture arch;
} Nios_IIConfig;

DEFINE_CS(Nios_IIConfig);
extern const struct Nios_IIConfigS Nios_IIConfigTable[];
#endif // NIOS_II_CONFIG_H
