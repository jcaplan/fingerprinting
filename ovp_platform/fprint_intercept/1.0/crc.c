#include "crc.h"

crc crcTable[256];

void crcInit(void)
{
    crc  remainder;
    int dividend;
    Uns8 bit;

    /*
     * Compute the remainder of each possible dividend.
     */
    for (dividend = 0; dividend < 256; ++dividend)
    {
        /*
         * Start with the dividend followed by zeros.
         */
        remainder = dividend << (WIDTH - 8);

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        for (bit = 8; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */         
            if (remainder & TOPBIT)
            {
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }

        /*
         * Store the result into the table.
         */
        crcTable[dividend] = remainder;
    }

}   /* crcInit() */

crc crcFast(Uns32 storeAddress, Uns32 storeData, crc remainder)
{
    Uns8 message[8];
    int byte;
    Uns8 data;
    int i;

    Uns32 mask = 0xff000000;
    Uns32 shift = 24;
    for(i = 0; i < 4; i++){
        message[i] = (storeAddress & mask) >> shift;
        message[i+4] = (storeData & mask) >> shift;
        mask = mask >> 8;
        shift -= 8;
    }
    /*
     * Divide the message by the polynomial, a byte at a time.
     */


    for (byte = 0; byte < 8; ++byte)
    {
        data = message[byte] ^ (remainder >> (WIDTH - 8));
        remainder = crcTable[data] ^ (remainder << 8);

    }

    /*
     * The final remainder is the CRC.
     */
    return (remainder);

}   /* crcFast() */

