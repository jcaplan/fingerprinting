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

// VMI header files
#include "vmi/vmiAttrs.h"
#include "vmi/vmiModelInfo.h"
#include "vmi/vmiDoc.h"

#include "Nios_IIDoc.h"
#include "Nios_IIVariant.h"
#include "Nios_IIStructure.h"
void Nios_IIDoc(vmiProcessorP processor, Nios_IIParamValuesP parameters) {

    vmiDocNodeP Root = vmidocAddSection(0, "Root");
    vmiDocNodeP Description = vmidocAddSection(Root, "Description");
    vmiDocNodeP Licensing = vmidocAddSection(Root, "Licensing");

    vmidocAddText(Description, "Nios_II Family Processor Model.");
    vmidocAddText(Licensing, "Open Source Apache 2.0");

    Nios_IIP Nios_II = (Nios_IIP)processor;

    vmiDocNodeP Limitations  = vmidocAddSection(Root, "Limitations");
    vmiDocNodeP Verification = vmidocAddSection(Root, "Verification");
    vmiDocNodeP Features     = vmidocAddSection(Root, "Features");

    vmidocAddText(
        Verification,
        "Models have been extensively tested by Imperas, and validated against tests from Altera."
    );

    vmidocAddText(Features, "Barrel Shifter.");

    vmidocAddText(Limitations, "No Custom instructions.");
    vmidocAddText(Limitations, "No Cache model.");
    vmidocAddText(Limitations, "No JTAG.");

    if (Nios_II->configInfo.arch == NIOS_II_F) {
        vmidocAddText(Limitations, "");

        vmidocAddText(Features, "Configurable MPU.");
        vmidocAddText(Features, "Configurable MMU.");
        vmidocAddText(Features, "Shadow Register Sets.");
        vmidocAddText(Features, "Hardware Multiply.");
        vmidocAddText(Features, "Hardware Divide.");
        vmidocAddText(Features, "Hardware Extended Multiply.");
    }

    if (Nios_II->configInfo.arch == NIOS_II_S) {
        vmidocAddText(Limitations, "");
        vmidocAddText(Features, "Hardware Multiply.");
        vmidocAddText(Features, "Hardware Divide.");
    }

    if (Nios_II->configInfo.arch == NIOS_II_E) {
        vmidocAddText(Limitations, "");
        vmidocAddText(Features, "");
    }
    vmidocProcessor(processor, Root);
}

