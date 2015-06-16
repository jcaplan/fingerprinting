
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Tue Jun  9 12:42:44 2015
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TLB_MACROS_IGEN_H
#define TLB_MACROS_IGEN_H
// Before including this file in the application, define the indicated macros
// to fix the base address of each slave port.
// Set the macro 'TLB_CSR' to the base of port 'TLB_CSR'
#ifndef TLB_CSR
    #error TLB_CSR is undefined.It needs to be set to the port base address
#endif
#define TLB_CSR_AB32_LINEENABLEREG    (TLB_CSR + 0x0)

#define TLB_CSR_AB32_LINEVALIDREG    (TLB_CSR + 0x20)

#define TLB_CSR_AB32_TLBACTIVATEREG    (TLB_CSR + 0x200)

#define TLB_CSR_AB32_PHYSICAL_TABLE (TLB_CSR + 0x180)
#define TLB_CSR_AB32_PHYSICAL_TABLE_BYTES 0x80

#define TLB_CSR_AB32_VIRTUAL_TABLE (TLB_CSR + 0x100)
#define TLB_CSR_AB32_VIRTUAL_TABLE_BYTES 0x80



#endif
