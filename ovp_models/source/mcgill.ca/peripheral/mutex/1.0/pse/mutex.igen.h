
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Mon Aug 18 14:59:50 2014
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MUTEX_IGEN_H
#define MUTEX_IGEN_H

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

typedef struct MUTEX_SLAVE_ab32_dataS { 
    union { 
        Uns32 value;
        struct {
            unsigned VALUE : 16;
            unsigned OWNER : 16;
        } bits;
    } mutex;
    union { 
        Uns32 value;
        struct {
            unsigned RESET : 1;
            unsigned RESERVED : 31;
        } bits;
    } reset;
} MUTEX_SLAVE_ab32_dataT, *MUTEX_SLAVE_ab32_dataTP;

/////////////////////////////// Port Declarations //////////////////////////////

extern MUTEX_SLAVE_ab32_dataT MUTEX_SLAVE_ab32_data;

#ifdef _PSE_
///////////////////////////////// Port handles /////////////////////////////////

typedef struct handlesS {
    void                 *MUTEX_SLAVE;
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
