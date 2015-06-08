
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Mon Jun  8 13:57:07 2015
//
////////////////////////////////////////////////////////////////////////////////

// MODEL IO:
//    Slave Port sp1
//    Net output  irq;
//    Net output  resetrequest;
//    Net output  timeout_pulse;

#ifndef MCGILL_CA_PERIPHERAL_INTERVALTIMER32CORE__1_0
#define MCGILL_CA_PERIPHERAL_INTERVALTIMER32CORE__1_0
#include "ovpworld.org/modelSupport/tlmPeripheral/1.0/tlm2.0/tlmPeripheral.hpp"
using namespace sc_core;

class IntervalTimer32Core : public icmPeripheral
{
  private:
    const char *getModel() {
        return icmGetVlnvString (NULL, "mcgill.ca", "peripheral", "IntervalTimer32Core", "1.0", "pse");
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
