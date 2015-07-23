#include "vmi/vmiMessage.h"
#include "vmi/vmiOSAttrs.h"
#include "vmi/vmiRt.h"
#include "vmi/vmiPSE.h"
#include "vmi/vmiVersion.h"
#include "vmi/vmiTypes.h"
#include <string.h>

#define POLYNOMIAL 0x0001da97  /* 11011 followed by 0's */

typedef Uns32 crc;

#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))

extern crc  crcTable[256];

void crcInit(void);
crc crcFast(Uns32 storeAddress, Uns32 storeData, crc remainder);
