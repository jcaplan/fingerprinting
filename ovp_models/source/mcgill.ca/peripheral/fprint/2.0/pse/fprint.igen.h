
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Tue Jun  9 12:42:55 2015
//
////////////////////////////////////////////////////////////////////////////////

#ifndef FPRINT_IGEN_H
#define FPRINT_IGEN_H

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

typedef struct FPRINT_CSR_SLAVE_ab32_dataS { 
    union { 
        Uns32 value;
        struct {
            unsigned key : 4;
            unsigned enable : 1;
        } bits;
    } currentState;
    union { 
        Uns32 value;
    } pauseReg;
    union { 
        Uns32 value;
    } pauseStrobe;
    union { 
        Uns32 value;
    } unpauseStrobe;
    union { 
        Uns32 value;
    } counterMax;
} FPRINT_CSR_SLAVE_ab32_dataT, *FPRINT_CSR_SLAVE_ab32_dataTP;

typedef struct FPRINT_TLB_SLAVE_ab32_dataS { 
} FPRINT_TLB_SLAVE_ab32_dataT, *FPRINT_TLB_SLAVE_ab32_dataTP;

/////////////////////////////// Port Declarations //////////////////////////////

extern FPRINT_CSR_SLAVE_ab32_dataT FPRINT_CSR_SLAVE_ab32_data;

extern FPRINT_TLB_SLAVE_ab32_dataT FPRINT_TLB_SLAVE_ab32_data;

#ifdef _PSE_
///////////////////////////////// Port handles /////////////////////////////////

typedef struct handlesS {
    ppmAddressSpaceHandle FPRINT_MASTER;
    void                 *FPRINT_CSR_SLAVE;
    void                 *FPRINT_TLB_SLAVE;
    ppmNetHandle          FPRINT_WRITE;
    ppmNetHandle          FPRINT_WRITE_ADDRESS;
    ppmNetHandle          FPRINT_WRITE_DATA;
    ppmNetHandle          FPRINT_RESET;
} handlesT, *handlesTP;

extern handlesT handles;

////////////////////////////// Callback prototypes /////////////////////////////

PPM_NET_CB(do_reset);
PPM_NET_CB(do_store);
PPM_REG_READ_CB(pause);
PPM_REG_READ_CB(regRd32);
PPM_REG_WRITE_CB(regWr32);
PPM_REG_READ_CB(unpause);
PPM_WRITE_CB(memregWr);
PPM_CONSTRUCTOR_CB(periphConstructor);
PPM_DESTRUCTOR_CB(periphDestructor);
PPM_CONSTRUCTOR_CB(constructor);
PPM_DESTRUCTOR_CB(destructor);

#endif

#endif
