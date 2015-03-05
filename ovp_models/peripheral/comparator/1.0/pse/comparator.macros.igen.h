
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Fri Oct 31 11:43:01 2014
//
////////////////////////////////////////////////////////////////////////////////

#ifndef COMPARATOR_MACROS_IGEN_H
#define COMPARATOR_MACROS_IGEN_H
////////////////////////////////// Description /////////////////////////////////

// Comparator peripheral model

// Before including this file in the application, define the indicated macros
// to fix the base address of each slave port.
// Set the macro 'COMP_CSR_SLAVE' to the base of port 'COMP_CSR_SLAVE'
#ifndef COMP_CSR_SLAVE
    #error COMP_CSR_SLAVE is undefined.It needs to be set to the port base address
#endif

// Set the macro 'COMP_FPRINT_SLAVE' to the base of port 'COMP_FPRINT_SLAVE'
#ifndef COMP_FPRINT_SLAVE
    #error COMP_FPRINT_SLAVE is undefined.It needs to be set to the port base address
#endif


#endif
