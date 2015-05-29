
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Fri Oct  3 15:35:36 2014
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DMA_MACROS_IGEN_H
#define DMA_MACROS_IGEN_H
////////////////////////////////// Description /////////////////////////////////

// DMA peripheral model

// Before including this file in the application, define the indicated macros
// to fix the base address of each slave port.
// Set the macro 'DMACSP' to the base of port 'DMACSP'
#ifndef DMACSP
    #error DMACSP is undefined.It needs to be set to the port base address
#endif
#define DMACSP_AB8_STATUS    (DMACSP + 0x0)

#define DMACSP_AB8_STATUS_DONE   0x1
#define DMACSP_AB8_STATUS_BUSY   (0x1 << 1)
#define DMACSP_AB8_STATUS_REOP   (0x1 << 2)
#define DMACSP_AB8_STATUS_WEOP   (0x1 << 3)
#define DMACSP_AB8_STATUS_LEN   (0x1 << 4)
#define DMACSP_AB8_READADDRESS    (DMACSP + 0x4)

#define DMACSP_AB8_WRITEADDRESS    (DMACSP + 0x8)

#define DMACSP_AB8_LENGTH    (DMACSP + 0xc)

#define DMACSP_AB8_CONTROL    (DMACSP + 0x18)

#define DMACSP_AB8_CONTROL_BYTE   0x1
#define DMACSP_AB8_CONTROL_HW   (0x1 << 1)
#define DMACSP_AB8_CONTROL_WORD   (0x1 << 2)
#define DMACSP_AB8_CONTROL_GO   (0x1 << 3)
#define DMACSP_AB8_CONTROL_I_EN   (0x1 << 4)
#define DMACSP_AB8_CONTROL_REEN   (0x1 << 5)
#define DMACSP_AB8_CONTROL_WEEN   (0x1 << 6)
#define DMACSP_AB8_CONTROL_LEEN   (0x1 << 7)
#define DMACSP_AB8_CONTROL_RCON   (0x1 << 8)
#define DMACSP_AB8_CONTROL_WCON   (0x1 << 9)
#define DMACSP_AB8_CONTROL_DOUBLEWORD   (0x1 << 10)
#define DMACSP_AB8_CONTROL_QUADWORD   (0x1 << 11)
#define DMACSP_AB8_CONTROL_SOFTWARERESET   (0x1 << 12)


#endif
