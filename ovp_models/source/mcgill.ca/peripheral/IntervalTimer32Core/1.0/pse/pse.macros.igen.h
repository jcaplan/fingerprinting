
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Mon Sep 22 11:24:39 2014
//
////////////////////////////////////////////////////////////////////////////////

#ifndef PSE_MACROS_IGEN_H
#define PSE_MACROS_IGEN_H
/////////////////////////////////// Licensing //////////////////////////////////

// Open Source Apache 2.0

////////////////////////////////// Description /////////////////////////////////

// Altera Avalon Interval Timer32 Core

////////////////////////////////// Limitations /////////////////////////////////

// No Support for pin level transitions

// Before including this file in the application, define the indicated macros
// to fix the base address of each slave port.
// Set the macro 'SP1' to the base of port 'sp1'
#ifndef SP1
    #error SP1 is undefined.It needs to be set to the port base address
#endif
#define SP1_REG0_STATUS    (SP1 + 0x0)

#define SP1_REG0_STATUS_TO   0x1
#define SP1_REG0_STATUS_RUN   (0x1 << 1)
#define SP1_REG0_CONTROL    (SP1 + 0x4)

#define SP1_REG0_CONTROL_ITO   0x1
#define SP1_REG0_CONTROL_CONT   (0x1 << 1)
#define SP1_REG0_CONTROL_START   (0x1 << 2)
#define SP1_REG0_CONTROL_STOP   (0x1 << 3)
#define SP1_REG0_PERIODL    (SP1 + 0x8)

#define SP1_REG0_PERIODH    (SP1 + 0xc)

#define SP1_REG0_SNAPL    (SP1 + 0x10)

#define SP1_REG0_SNAPH    (SP1 + 0x14)



#endif
