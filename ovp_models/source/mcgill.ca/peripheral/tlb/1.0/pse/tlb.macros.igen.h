
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Tue Oct  7 11:16:09 2014
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TLB_MACROS_IGEN_H
#define TLB_MACROS_IGEN_H
////////////////////////////////// Description /////////////////////////////////

// TLB.

/////////////////////////////////// Licensing //////////////////////////////////

// Open Source Apache 2.0

////////////////////////////////// Limitations /////////////////////////////////

// The range of the input slave port must not conflict with any exiting port conn
// ected to the bus.
// The output bus width is hard coded to be 32 bits.

// Before including this file in the application, define the indicated macros
// to fix the base address of each slave port.
// Set the macro 'TLB_CSR' to the base of port 'TLB_CSR'
#ifndef TLB_CSR
    #error TLB_CSR is undefined.It needs to be set to the port base address
#endif
#define TLB_CSR_AB32_LINEENABLEREG    (TLB_CSR + 0x0)

#define TLB_CSR_AB32_LINEVALIDREG    (TLB_CSR + 0x20)

#define TLB_CSR_AB32_TLBACTIVATEREG    (TLB_CSR + 0x200)



#endif
