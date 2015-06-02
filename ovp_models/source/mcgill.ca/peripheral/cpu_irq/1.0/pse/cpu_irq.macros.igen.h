
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Thu Sep  4 17:29:59 2014
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CPU_IRQ_MACROS_IGEN_H
#define CPU_IRQ_MACROS_IGEN_H
////////////////////////////////// Description /////////////////////////////////

// CPU IRQ peripheral model

// Before including this file in the application, define the indicated macros
// to fix the base address of each slave port.
// Set the macro 'CPU_IRQ_SLAVE' to the base of port 'CPU_IRQ_SLAVE'
#ifndef CPU_IRQ_SLAVE
    #error CPU_IRQ_SLAVE is undefined.It needs to be set to the port base address
#endif
#define CPU_IRQ_SLAVE_AB32_CPUM    (CPU_IRQ_SLAVE + 0x800)

#define CPU_IRQ_SLAVE_AB32_CPU0    (CPU_IRQ_SLAVE + 0x0)

#define CPU_IRQ_SLAVE_AB32_CPU1    (CPU_IRQ_SLAVE + 0x400)

#define CPU_IRQ_SLAVE_AB32_CPU2    (CPU_IRQ_SLAVE + 0xc00)

#define CPU_IRQ_SLAVE_AB32_CPU3    (CPU_IRQ_SLAVE + 0x1000)

#define CPU_IRQ_SLAVE_AB32_CPU4    (CPU_IRQ_SLAVE + 0x1400)

#define CPU_IRQ_SLAVE_AB32_CPU5    (CPU_IRQ_SLAVE + 0x1800)

#define CPU_IRQ_SLAVE_AB32_CPU6    (CPU_IRQ_SLAVE + 0x1c00)

#define CPU_IRQ_SLAVE_AB32_CPU7    (CPU_IRQ_SLAVE + 0x2000)

#define CPU_IRQ_SLAVE_AB32_CPU7_RESET   0x1
#define CPU_IRQ_SLAVE_AB32_CPU7_RESERVED   (0x7fffffff << 1)


#endif
