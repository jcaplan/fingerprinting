
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Tue Jun  9 12:42:55 2015
//
////////////////////////////////////////////////////////////////////////////////

#ifndef FPRINT_MACROS_IGEN_H
#define FPRINT_MACROS_IGEN_H
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
#define FPRINT_TLB_SLAVE_AB32_FPRINT_TABLE (FPRINT_TLB_SLAVE + 0x0)
#define FPRINT_TLB_SLAVE_AB32_FPRINT_TABLE_BYTES 0x84



#endif
