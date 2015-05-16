
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Sat May 16 13:41:13 2015
//
////////////////////////////////////////////////////////////////////////////////

#ifndef COMPARATOR_MACROS_IGEN_H
#define COMPARATOR_MACROS_IGEN_H
// Before including this file in the application, define the indicated macros
// to fix the base address of each slave port.
// Set the macro 'COMP_CSR_SLAVE' to the base of port 'COMP_CSR_SLAVE'
#ifndef COMP_CSR_SLAVE
    #error COMP_CSR_SLAVE is undefined.It needs to be set to the port base address
#endif
#define COMP_CSR_SLAVE_AB32_CSR (COMP_CSR_SLAVE + 0x0)
#define COMP_CSR_SLAVE_AB32_CSR_BYTES 0x1000


// Set the macro 'COMP_FPRINT_SLAVE' to the base of port 'COMP_FPRINT_SLAVE'
#ifndef COMP_FPRINT_SLAVE
    #error COMP_FPRINT_SLAVE is undefined.It needs to be set to the port base address
#endif
#define COMP_FPRINT_SLAVE_AB32_FPRINTS (COMP_FPRINT_SLAVE + 0x0)
#define COMP_FPRINT_SLAVE_AB32_FPRINTS_BYTES 0x1000



#endif
