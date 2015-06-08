
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Mon Jun  8 15:20:35 2015
//
////////////////////////////////////////////////////////////////////////////////

#ifndef SW_RESET_IGEN_H
#define SW_RESET_IGEN_H

#ifdef _PSE_
#    include "peripheral/impTypes.h"
#    include "peripheral/bhm.h"
#    include "peripheral/ppm.h"
#else
#    include "hostapi/impTypes.h"
#endif

//////////////////////////////////// Externs ///////////////////////////////////

extern Uns32 diagnosticLevel;


/////////////////////////// Dynamic Diagnostic Macros //////////////////////////

// Bottom two bits of word used for PSE diagnostics
#define PSE_DIAG_LOW      (BHM_DIAG_MASK_LOW(diagnosticLevel))
#define PSE_DIAG_MEDIUM   (BHM_DIAG_MASK_MEDIUM(diagnosticLevel))
#define PSE_DIAG_HIGH     (BHM_DIAG_MASK_HIGH(diagnosticLevel))
// Next two bits of word used for PSE semihost/intercept library diagnostics
#define PSE_DIAG_SEMIHOST (BHM_DIAG_MASK_SEMIHOST(diagnosticLevel))

/////////////////////////// Register data declaration //////////////////////////

typedef struct SW_RESET_SLAVE_ab32_dataS { 
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
    } cpu7;
} SW_RESET_SLAVE_ab32_dataT, *SW_RESET_SLAVE_ab32_dataTP;

/////////////////////////////// Port Declarations //////////////////////////////

extern SW_RESET_SLAVE_ab32_dataT SW_RESET_SLAVE_ab32_data;

#ifdef _PSE_
///////////////////////////////// Port handles /////////////////////////////////

typedef struct handlesS {
    void                 *SW_RESET_SLAVE;
    ppmNetHandle          cpum_reset;
    ppmNetHandle          cpu0_reset;
    ppmNetHandle          cpu1_reset;
    ppmNetHandle          cpu2_reset;
    ppmNetHandle          cpu3_reset;
    ppmNetHandle          cpu4_reset;
    ppmNetHandle          cpu5_reset;
    ppmNetHandle          cpu6_reset;
    ppmNetHandle          cpu7_reset;
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
