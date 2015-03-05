
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Mon Sep 22 11:24:39 2014
//
////////////////////////////////////////////////////////////////////////////////

#ifndef PSE_IGEN_H
#define PSE_IGEN_H

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

typedef struct sp1_reg0_dataS { 
    union { 
        Uns16 value;
        struct {
            unsigned TO : 1;
            unsigned RUN : 1;
        } bits;
    } status;
    union { 
        Uns16 value;
        struct {
            unsigned ITO : 1;
            unsigned CONT : 1;
            unsigned START : 1;
            unsigned STOP : 1;
        } bits;
    } control;
    union { 
        Uns16 value;
    } periodl;
    union { 
        Uns16 value;
    } periodh;
    union { 
        Uns16 value;
    } snapl;
    union { 
        Uns16 value;
    } snaph;
} sp1_reg0_dataT, *sp1_reg0_dataTP;

/////////////////////////////// Port Declarations //////////////////////////////

extern sp1_reg0_dataT sp1_reg0_data;

#ifdef _PSE_
///////////////////////////////// Port handles /////////////////////////////////

typedef struct handlesS {
    void                 *sp1;
    ppmNetHandle          irq;
    ppmNetHandle          resetrequest;
    ppmNetHandle          timeout_pulse;
} handlesT, *handlesTP;

extern handlesT handles;

////////////////////////////// Callback prototypes /////////////////////////////

PPM_REG_READ_CB(read_control);
PPM_REG_READ_CB(read_period);
PPM_REG_READ_CB(read_snap);
PPM_REG_READ_CB(read_status);
PPM_REG_WRITE_CB(write_control);
PPM_REG_WRITE_CB(write_period);
PPM_REG_WRITE_CB(write_snap);
PPM_REG_WRITE_CB(write_status);
PPM_NET_CB(reset_IRESET);
PPM_CONSTRUCTOR_CB(periphConstructor);
PPM_CONSTRUCTOR_CB(constructor);

#endif

#endif
