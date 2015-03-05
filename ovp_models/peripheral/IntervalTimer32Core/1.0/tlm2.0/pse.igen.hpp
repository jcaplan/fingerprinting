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
//                          Thu May  1 20:05:07 2014
//
////////////////////////////////////////////////////////////////////////////////

// MODEL IO:
//    Slave Port sp1
//    Net output  irq;
//    Net output  resetrequest;
//    Net output  timeout_pulse;

#ifndef ALTERA_OVPWORLD_ORG_PERIPHERAL_INTERVALTIMER32CORE__1_0
#define ALTERA_OVPWORLD_ORG_PERIPHERAL_INTERVALTIMER32CORE__1_0
#include "ovpworld.org/modelSupport/tlmPeripheral/1.0/tlm2.0/tlmPeripheral.hpp"
using namespace sc_core;

class IntervalTimer32Core : public icmPeripheral
{
  private:
    const char *getModel() {
        return icmGetVlnvString (NULL, "altera.ovpworld.org", "peripheral", "IntervalTimer32Core", "1.0", "pse");
    }

  public:
    icmSlavePort        sp1;
    icmOutputNetPort irq;
    icmOutputNetPort resetrequest;
    icmOutputNetPort timeout_pulse;

    IntervalTimer32Core(sc_module_name name, icmAttrListObject *initialAttrs = 0 )
        : icmPeripheral(name, getModel(), 0, initialAttrs)
        , sp1(this, "sp1", 0x18) // static
        , irq(this, "irq")
        , resetrequest(this, "resetrequest")
        , timeout_pulse(this, "timeout_pulse")
    {
    }

}; /* class IntervalTimer32Core */

#endif
