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
#include "vmi/vmiModelInfo.h"
#include "hostapi/impAlloc.h"

#include "Nios_IIFunctions.h"

VMI_PROC_INFO_FN(Nios_IIProcInfo) {

    static const vmiProcessorInfo info = {

        .vlnv.vendor      = "altera.ovpworld.org",
        .vlnv.library     = "processor",
        .vlnv.name        = "nios_ii",
        .vlnv.version     = "1.0",

        .semihost.vendor  = "altera.ovpworld.org",
        .semihost.library = "semihosting",
        .semihost.name    = "nios_iiNewlib",
        .semihost.version = "1.0",

        .helper.vendor    = "imperas.com",
        .helper.library   = "intercept",
        .helper.name      = "nios_iiCpuHelper",
        .helper.version   = "1.0",

        .elfCode          = 113,
        .endianFixed      = True,
        .endian           = MEM_ENDIAN_LITTLE,
        .gdbPath          = "$IMPERAS_HOME/lib/$IMPERAS_ARCH/gdb/nios2-elf-gdb" VMI_EXE_SUFFIX,
        .family           = "ALTERA",
        .QLQualified      = True
    };

    return &info;
}

