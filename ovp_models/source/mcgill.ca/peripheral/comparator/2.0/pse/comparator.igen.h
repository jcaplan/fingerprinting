
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Thu Jul 23 18:36:09 2015
//
////////////////////////////////////////////////////////////////////////////////

#ifndef COMPARATOR_IGEN_H
#define COMPARATOR_IGEN_H

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

typedef struct COMP_CSR_SLAVE_ab32_dataS { 
    union { 
        Uns32 value;
    } exceptionReg;
    union { 
        Uns32 value;
    } statusReg;
    union { 
        Uns32 value;
    } successReg;
    union { 
        Uns32 value;
    } failedReg;
    union { 
        Uns32 value;
        struct {
            unsigned data : 16;
            unsigned taskID : 8;
            unsigned coreID : 8;
        } bits;
    } catReg;
    union { 
        Uns32 value;
        struct {
            unsigned data : 16;
            unsigned taskID : 8;
        } bits;
    } nmrReg;
    union { 
        Uns32 value;
        struct {
            unsigned data : 16;
            unsigned taskID : 8;
        } bits;
    } successCounterReg;
} COMP_CSR_SLAVE_ab32_dataT, *COMP_CSR_SLAVE_ab32_dataTP;

typedef struct COMP_FPRINT_SLAVE_ab32_dataS { 
    union { 
        Uns32 value;
        struct {
            unsigned taskID : 4;
            unsigned enable : 1;
            unsigned coreID : 4;
        } bits;
    } currentState;
    union { 
        Uns32 value;
        struct {
            unsigned taskID : 4;
            unsigned coreID : 4;
            unsigned messageID : 1;
            unsigned __pad9 : 7;
            unsigned fprint : 16;
        } bits;
    } fprint;
} COMP_FPRINT_SLAVE_ab32_dataT, *COMP_FPRINT_SLAVE_ab32_dataTP;

/////////////////////////////// Port Declarations //////////////////////////////

extern COMP_CSR_SLAVE_ab32_dataT COMP_CSR_SLAVE_ab32_data;

extern COMP_FPRINT_SLAVE_ab32_dataT COMP_FPRINT_SLAVE_ab32_data;

#ifdef _PSE_
///////////////////////////////// Port handles /////////////////////////////////

typedef struct handlesS {
    ppmAddressSpaceHandle COMP_CHECKPOINT_IRQ_MASTER;
    void                 *COMP_CSR_SLAVE;
    void                 *COMP_FPRINT_SLAVE;
    ppmNetHandle          COMP_IRQ;
} handlesT, *handlesTP;

extern handlesT handles;

////////////////////////////// Callback prototypes /////////////////////////////

PPM_REG_WRITE_CB(catRegWrite);
PPM_REG_READ_CB(csrRd32);
PPM_REG_WRITE_CB(currentStateWrite);
PPM_REG_WRITE_CB(exceptionRegWrite);
PPM_REG_WRITE_CB(fprintWrite);
PPM_REG_WRITE_CB(nmrRegWrite);
PPM_REG_WRITE_CB(successCounterRegWrite);
PPM_CONSTRUCTOR_CB(periphConstructor);
PPM_DESTRUCTOR_CB(periphDestructor);
PPM_CONSTRUCTOR_CB(constructor);
PPM_DESTRUCTOR_CB(destructor);

#endif

#endif
