
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Sat May 16 13:41:13 2015
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
} COMP_CSR_SLAVE_ab32_dataT, *COMP_CSR_SLAVE_ab32_dataTP;

typedef struct COMP_FPRINT_SLAVE_ab32_dataS { 
} COMP_FPRINT_SLAVE_ab32_dataT, *COMP_FPRINT_SLAVE_ab32_dataTP;

/////////////////////////////// Port Declarations //////////////////////////////

extern COMP_CSR_SLAVE_ab32_dataT COMP_CSR_SLAVE_ab32_data;

extern COMP_FPRINT_SLAVE_ab32_dataT COMP_FPRINT_SLAVE_ab32_data;

#ifdef _PSE_
///////////////////////////////// Port handles /////////////////////////////////

typedef struct handlesS {
    void                 *COMP_CSR_SLAVE;
    void                 *COMP_FPRINT_SLAVE;
    ppmNetHandle          COMP_IRQ;
} handlesT, *handlesTP;

extern handlesT handles;

////////////////////////////// Callback prototypes /////////////////////////////

PPM_WRITE_CB(fprintWr);
PPM_READ_CB(memRd);
PPM_WRITE_CB(memWr);
PPM_CONSTRUCTOR_CB(periphConstructor);
PPM_DESTRUCTOR_CB(periphDestructor);
PPM_CONSTRUCTOR_CB(constructor);
PPM_DESTRUCTOR_CB(destructor);

#endif

#endif
