
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Thu Sep  4 17:29:59 2014
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CPU_IRQ_IGEN_H
#define CPU_IRQ_IGEN_H

#ifdef _PSE_
#    include "peripheral/impTypes.h"
#    include "peripheral/bhm.h"
#    include "peripheral/ppm.h"
#else
#    include "hostapi/impTypes.h"
#endif

//////////////////////////////////// Externs ///////////////////////////////////

    extern Uns32 diagnosticLevel;

/////////////////////////// Register data declaration //////////////////////////

typedef struct CPU_IRQ_SLAVE_ab32_dataS { 
    union { 
        Uns32 value;
    } cpum;
    union { 
        Uns32 value;
    } cpu0;
    union { 
        Uns32 value;
    } cpu1;
    union { 
        Uns32 value;
    } cpu2;
    union { 
        Uns32 value;
    } cpu3;
    union { 
        Uns32 value;
    } cpu4;
    union { 
        Uns32 value;
    } cpu5;
    union { 
        Uns32 value;
    } cpu6;
    union { 
        Uns32 value;
        struct {
            unsigned RESET : 1;
            unsigned RESERVED : 31;
        } bits;
    } cpu7;
} CPU_IRQ_SLAVE_ab32_dataT, *CPU_IRQ_SLAVE_ab32_dataTP;

/////////////////////////////// Port Declarations //////////////////////////////

extern CPU_IRQ_SLAVE_ab32_dataT CPU_IRQ_SLAVE_ab32_data;

#ifdef _PSE_
///////////////////////////////// Port handles /////////////////////////////////

typedef struct handlesS {
    void                 *CPU_IRQ_SLAVE;
    ppmNetHandle          cpum_irq;
    ppmNetHandle          cpu0_irq;
    ppmNetHandle          cpu1_irq;
    ppmNetHandle          cpu2_irq;
    ppmNetHandle          cpu3_irq;
    ppmNetHandle          cpu4_irq;
    ppmNetHandle          cpu5_irq;
    ppmNetHandle          cpu6_irq;
    ppmNetHandle          cpu7_irq;
} handlesT, *handlesTP;

extern handlesT handles;

////////////////////////////// Callback prototypes /////////////////////////////

PPM_REG_READ_CB(regRd32);
PPM_REG_WRITE_CB(regWr32);
PPM_CONSTRUCTOR_CB(periphConstructor);
PPM_DESTRUCTOR_CB(periphDestructor);
PPM_CONSTRUCTOR_CB(constructor);
PPM_DESTRUCTOR_CB(destructor);

#endif

#endif
