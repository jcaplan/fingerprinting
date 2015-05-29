
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Tue May 12 14:24:08 2015
//
////////////////////////////////////////////////////////////////////////////////

#ifndef FPRINT_MACROS_IGEN_H
#define FPRINT_MACROS_IGEN_H
////////////////////////////////// Description /////////////////////////////////

// Fingerprint peripheral model

// Before including this file in the application, define the indicated macros
// to fix the base address of each slave port.
// Set the macro 'FPRINT_CSR_SLAVE' to the base of port 'FPRINT_CSR_SLAVE'
#ifndef FPRINT_CSR_SLAVE
    #error FPRINT_CSR_SLAVE is undefined.It needs to be set to the port base address
#endif
#define FPRINT_CSR_SLAVE_AB32_CURRENTSTATE    (FPRINT_CSR_SLAVE + 0x0)

#define FPRINT_CSR_SLAVE_AB32_CURRENTSTATE_KEY   0xf
#define FPRINT_CSR_SLAVE_AB32_CURRENTSTATE_ENABLE   (0x1 << 4)
#define FPRINT_CSR_SLAVE_AB32_PAUSEREG    (FPRINT_CSR_SLAVE + 0x4)

#define FPRINT_CSR_SLAVE_AB32_PAUSESTROBE    (FPRINT_CSR_SLAVE + 0x8)

#define FPRINT_CSR_SLAVE_AB32_UNPAUSESTROBE    (FPRINT_CSR_SLAVE + 0xc)

#define FPRINT_CSR_SLAVE_AB32_COUNTERMAX    (FPRINT_CSR_SLAVE + 0x10)


// Set the macro 'FPRINT_TLB_SLAVE' to the base of port 'FPRINT_TLB_SLAVE'
#ifndef FPRINT_TLB_SLAVE
    #error FPRINT_TLB_SLAVE is undefined.It needs to be set to the port base address
#endif


#endif
