
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Mon Aug 18 14:59:50 2014
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MUTEX_MACROS_IGEN_H
#define MUTEX_MACROS_IGEN_H
////////////////////////////////// Description /////////////////////////////////

// Mutex peripheral model

// Before including this file in the application, define the indicated macros
// to fix the base address of each slave port.
// Set the macro 'MUTEX_SLAVE' to the base of port 'MUTEX_SLAVE'
#ifndef MUTEX_SLAVE
    #error MUTEX_SLAVE is undefined.It needs to be set to the port base address
#endif
#define MUTEX_SLAVE_AB32_MUTEX    (MUTEX_SLAVE + 0x0)

#define MUTEX_SLAVE_AB32_MUTEX_OWNER   (0xffff << 16)
#define MUTEX_SLAVE_AB32_MUTEX_VALUE   0xffff
#define MUTEX_SLAVE_AB32_RESET    (MUTEX_SLAVE + 0x4)

#define MUTEX_SLAVE_AB32_RESET_RESET   0x1
#define MUTEX_SLAVE_AB32_RESET_RESERVED   (0x7fffffff << 1)


#endif
