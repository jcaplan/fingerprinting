
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Fri Oct  3 15:35:36 2014
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DMA_IGEN_H
#define DMA_IGEN_H

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

typedef struct DMACSP_ab8_dataS { 
    union { 
        Uns32 value;
        struct {
            unsigned DONE : 1;
            unsigned BUSY : 1;
            unsigned REOP : 1;
            unsigned WEOP : 1;
            unsigned LEN : 1;
        } bits;
    } status;
    union { 
        Uns32 value;
    } readaddress;
    union { 
        Uns32 value;
    } writeaddress;
    union { 
        Uns32 value;
    } length;
    union { 
        Uns32 value;
        struct {
            unsigned BYTE : 1;
            unsigned HW : 1;
            unsigned WORD : 1;
            unsigned GO : 1;
            unsigned I_EN : 1;
            unsigned REEN : 1;
            unsigned WEEN : 1;
            unsigned LEEN : 1;
            unsigned RCON : 1;
            unsigned WCON : 1;
            unsigned DOUBLEWORD : 1;
            unsigned QUADWORD : 1;
            unsigned SOFTWARERESET : 1;
        } bits;
    } control;
} DMACSP_ab8_dataT, *DMACSP_ab8_dataTP;

/////////////////////////////// Port Declarations //////////////////////////////

extern DMACSP_ab8_dataT DMACSP_ab8_data;

#ifdef _PSE_
///////////////////////////////// Port handles /////////////////////////////////

typedef struct handlesS {
    ppmAddressSpaceHandle MREAD;
    ppmAddressSpaceHandle MWRITE;
    void                 *DMACSP;
    ppmNetHandle          DMA_IRQ;
} handlesT, *handlesTP;

extern handlesT handles;

////////////////////////////// Callback prototypes /////////////////////////////

PPM_REG_WRITE_CB(controlWr);
PPM_REG_READ_CB(regRd);
PPM_REG_WRITE_CB(regWr);
PPM_REG_WRITE_CB(statusWr);
PPM_CONSTRUCTOR_CB(periphConstructor);
PPM_DESTRUCTOR_CB(periphDestructor);
PPM_CONSTRUCTOR_CB(constructor);
PPM_DESTRUCTOR_CB(destructor);

#endif

#endif
