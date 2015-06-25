
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Tue Jun 23 14:43:06 2015
//
////////////////////////////////////////////////////////////////////////////////

#ifndef RESET_MONITOR_MACROS_IGEN_H
#define RESET_MONITOR_MACROS_IGEN_H
// Before including this file in the application, define the indicated macros
// to fix the base address of each slave port.
// Set the macro 'RESET_MONITOR_SLAVE' to the base of port 'RESET_MONITOR_SLAVE'
#ifndef RESET_MONITOR_SLAVE
    #error RESET_MONITOR_SLAVE is undefined.It needs to be set to the port base address
#endif
#define RESET_MONITOR_SLAVE_AB32_ENABLEREG    (RESET_MONITOR_SLAVE + 0x0)

#define RESET_MONITOR_SLAVE_AB32_MONITORREG    (RESET_MONITOR_SLAVE + 0x4)

#define RESET_MONITOR_SLAVE_AB32_CORESTATUSREG    (RESET_MONITOR_SLAVE + 0x8)



#endif
