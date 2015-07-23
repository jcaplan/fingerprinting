
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Thu Jul 23 18:36:09 2015
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
#define COMP_CSR_SLAVE_AB32_EXCEPTIONREG    (COMP_CSR_SLAVE + 0x0)

#define COMP_CSR_SLAVE_AB32_STATUSREG    (COMP_CSR_SLAVE + 0x4)

#define COMP_CSR_SLAVE_AB32_SUCCESSREG    (COMP_CSR_SLAVE + 0x8)

#define COMP_CSR_SLAVE_AB32_FAILEDREG    (COMP_CSR_SLAVE + 0xc)

#define COMP_CSR_SLAVE_AB32_CATREG    (COMP_CSR_SLAVE + 0x10)

#define COMP_CSR_SLAVE_AB32_CATREG_DATA   0xffff
#define COMP_CSR_SLAVE_AB32_CATREG_TASKID   (0xff << 16)
#define COMP_CSR_SLAVE_AB32_CATREG_COREID   (0xff << 24)
#define COMP_CSR_SLAVE_AB32_NMRREG    (COMP_CSR_SLAVE + 0x14)

#define COMP_CSR_SLAVE_AB32_NMRREG_DATA   0xffff
#define COMP_CSR_SLAVE_AB32_NMRREG_TASKID   (0xff << 16)
#define COMP_CSR_SLAVE_AB32_SUCCESSCOUNTERREG    (COMP_CSR_SLAVE + 0x18)

#define COMP_CSR_SLAVE_AB32_SUCCESSCOUNTERREG_DATA   0xffff
#define COMP_CSR_SLAVE_AB32_SUCCESSCOUNTERREG_TASKID   (0xff << 16)

// Set the macro 'COMP_FPRINT_SLAVE' to the base of port 'COMP_FPRINT_SLAVE'
#ifndef COMP_FPRINT_SLAVE
    #error COMP_FPRINT_SLAVE is undefined.It needs to be set to the port base address
#endif
#define COMP_FPRINT_SLAVE_AB32_CURRENTSTATE    (COMP_FPRINT_SLAVE + 0x0)

#define COMP_FPRINT_SLAVE_AB32_CURRENTSTATE_TASKID   0xf
#define COMP_FPRINT_SLAVE_AB32_CURRENTSTATE_ENABLE   (0x1 << 4)
#define COMP_FPRINT_SLAVE_AB32_CURRENTSTATE_COREID   (0xf << 5)
#define COMP_FPRINT_SLAVE_AB32_FPRINT    (COMP_FPRINT_SLAVE + 0x4)

#define COMP_FPRINT_SLAVE_AB32_FPRINT_TASKID   0xf
#define COMP_FPRINT_SLAVE_AB32_FPRINT_COREID   (0xf << 4)
#define COMP_FPRINT_SLAVE_AB32_FPRINT_MESSAGEID   (0x1 << 8)
#define COMP_FPRINT_SLAVE_AB32_FPRINT_FPRINT   (0xffff << 16)


#endif
