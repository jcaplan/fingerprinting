
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Tue Oct  7 11:22:25 2014
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TLB_IGEN_H
#define TLB_IGEN_H

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

typedef struct TLB_CSR_ab32_dataS { 
    union { 
        Uns32 value;
    } lineEnableReg;
    union { 
        Uns32 value;
    } lineValidReg;
    union { 
        Uns32 value;
    } tlbActivateReg;
} TLB_CSR_ab32_dataT, *TLB_CSR_ab32_dataTP;

/////////////////////////////// Port Declarations //////////////////////////////

extern TLB_CSR_ab32_dataT TLB_CSR_ab32_data;

#ifdef _PSE_
///////////////////////////////// Port handles /////////////////////////////////

typedef struct handlesS {
    ppmAddressSpaceHandle TLB_MASTER;
    ppmAddressSpaceHandle TLB_FPRINT_MASTER;
    void                 *TLB_SLAVE;
    void                 *TLB_CSR;
} handlesT, *handlesTP;

extern handlesT handles;

////////////////////////////// Callback prototypes /////////////////////////////

PPM_REG_READ_CB(regRd32);
PPM_REG_WRITE_CB(regWr32);
PPM_WRITE_CB(memregWr);
PPM_CONSTRUCTOR_CB(periphConstructor);
PPM_CONSTRUCTOR_CB(constructor);

#endif

#endif
