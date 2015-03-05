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


////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Wed Apr 30 17:47:30 2014
//
////////////////////////////////////////////////////////////////////////////////

#ifndef ALTERA_OVPWORLD_ORG_PROCESSOR_NIOS_II__1_0
#define ALTERA_OVPWORLD_ORG_PROCESSOR_NIOS_II__1_0

#include "ovpworld.org/modelSupport/tlmProcessor/1.0/tlm2.0/tlmProcessor.hpp"
using namespace sc_core;

class nios_ii : public icmCpu
{
  private:
    const char *getModel() {
        return icmGetVlnvString (NULL, "altera.ovpworld.org", "processor", "nios_ii", "1.0", "model");
    }

    const char *getSHL() {
        return icmGetVlnvString (NULL, "altera.ovpworld.org", "semihosting", "nios_iiNewlib", "1.0", "model");
    }

  public:
    icmCpuMasterPort     INSTRUCTION;
    icmCpuMasterPort     DATA;
    icmCpuInterrupt      reset_n;
    icmCpuInterrupt      d_irq0;
    icmCpuInterrupt      d_irq1;
    icmCpuInterrupt      d_irq2;
    icmCpuInterrupt      d_irq3;
    icmCpuInterrupt      d_irq4;
    icmCpuInterrupt      d_irq5;
    icmCpuInterrupt      d_irq6;
    icmCpuInterrupt      d_irq7;
    icmCpuInterrupt      d_irq8;
    icmCpuInterrupt      d_irq9;
    icmCpuInterrupt      d_irq10;
    icmCpuInterrupt      d_irq11;
    icmCpuInterrupt      d_irq12;
    icmCpuInterrupt      d_irq13;
    icmCpuInterrupt      d_irq14;
    icmCpuInterrupt      d_irq15;
    icmCpuInterrupt      d_irq16;
    icmCpuInterrupt      d_irq17;
    icmCpuInterrupt      d_irq18;
    icmCpuInterrupt      d_irq19;
    icmCpuInterrupt      d_irq20;
    icmCpuInterrupt      d_irq21;
    icmCpuInterrupt      d_irq22;
    icmCpuInterrupt      d_irq23;
    icmCpuInterrupt      d_irq24;
    icmCpuInterrupt      d_irq25;
    icmCpuInterrupt      d_irq26;
    icmCpuInterrupt      d_irq27;
    icmCpuInterrupt      d_irq28;
    icmCpuInterrupt      d_irq29;
    icmCpuInterrupt      d_irq30;
    icmCpuInterrupt      d_irq31;

    nios_ii(
        sc_module_name        name,
        const unsigned int    ID,
        icmNewProcAttrs       attrs        = ICM_ATTR_DEFAULT,
        icmAttrListObject    *attrList    = NULL,
        const char           *semiHost    = NULL,
        Uns32                 addressBits = 32,
        bool                  dmi         = true,
        Uns32                 cpuFlags    = 0
     )
    : icmCpu(name, ID, "nios_ii", getModel(), 0, semiHost ? ((0 == strlen(semiHost)) ? 0 : semiHost) : getSHL(), attrs, attrList, addressBits, dmi, cpuFlags)
    , INSTRUCTION (this, "INSTRUCTION", 32)
    , DATA (this, "DATA", 32)
    , reset_n("reset_n", this)
    , d_irq0("d_irq0", this)
    , d_irq1("d_irq1", this)
    , d_irq2("d_irq2", this)
    , d_irq3("d_irq3", this)
    , d_irq4("d_irq4", this)
    , d_irq5("d_irq5", this)
    , d_irq6("d_irq6", this)
    , d_irq7("d_irq7", this)
    , d_irq8("d_irq8", this)
    , d_irq9("d_irq9", this)
    , d_irq10("d_irq10", this)
    , d_irq11("d_irq11", this)
    , d_irq12("d_irq12", this)
    , d_irq13("d_irq13", this)
    , d_irq14("d_irq14", this)
    , d_irq15("d_irq15", this)
    , d_irq16("d_irq16", this)
    , d_irq17("d_irq17", this)
    , d_irq18("d_irq18", this)
    , d_irq19("d_irq19", this)
    , d_irq20("d_irq20", this)
    , d_irq21("d_irq21", this)
    , d_irq22("d_irq22", this)
    , d_irq23("d_irq23", this)
    , d_irq24("d_irq24", this)
    , d_irq25("d_irq25", this)
    , d_irq26("d_irq26", this)
    , d_irq27("d_irq27", this)
    , d_irq28("d_irq28", this)
    , d_irq29("d_irq29", this)
    , d_irq30("d_irq30", this)
    , d_irq31("d_irq31", this)
    {
    }

    void before_end_of_elaboration() {
        DATA.bindIfNotBound();
    }
}; /* class nios_ii */

#endif
