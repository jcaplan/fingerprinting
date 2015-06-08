
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Mon Jun  8 15:20:35 2015
//
////////////////////////////////////////////////////////////////////////////////

#ifndef SW_RESET_MACROS_IGEN_H
#define SW_RESET_MACROS_IGEN_H
// Before including this file in the application, define the indicated macros
// to fix the base address of each slave port.
// Set the macro 'SW_RESET_SLAVE' to the base of port 'SW_RESET_SLAVE'
#ifndef SW_RESET_SLAVE
    #error SW_RESET_SLAVE is undefined.It needs to be set to the port base address
#endif
#define SW_RESET_SLAVE_AB32_CPUM    (SW_RESET_SLAVE + 0x800)

#define SW_RESET_SLAVE_AB32_CPU0    (SW_RESET_SLAVE + 0x0)

#define SW_RESET_SLAVE_AB32_CPU1    (SW_RESET_SLAVE + 0x400)

#define SW_RESET_SLAVE_AB32_CPU2    (SW_RESET_SLAVE + 0xc00)

#define SW_RESET_SLAVE_AB32_CPU3    (SW_RESET_SLAVE + 0x1000)

#define SW_RESET_SLAVE_AB32_CPU4    (SW_RESET_SLAVE + 0x1400)

#define SW_RESET_SLAVE_AB32_CPU5    (SW_RESET_SLAVE + 0x1800)

#define SW_RESET_SLAVE_AB32_CPU6    (SW_RESET_SLAVE + 0x1c00)

#define SW_RESET_SLAVE_AB32_CPU7    (SW_RESET_SLAVE + 0x2000)



#endif
