#Write crc_rtla
from generateCRC_RTLA import *


for i in range(8,264,8):
    s = 'circuits\\crc_rtla_8_' + str(i) + '.vhd'
    print s
    writeCRC_RTLA(s,i,8,0x8e)

for i in range(12,264,8):
    s = 'circuits\\crc_rtla_12_' + str(i) + '.vhd'
    print s
    writeCRC_RTLA(s,i,12,0xB75)

for i in range(16,264,8):
    s = 'circuits\\crc_rtla_32_' + str(i) + '.vhd'
    print s
    writeCRC_RTLA(s,i,16,0xCBE5)

for i in range(24,264,8):
    s = 'circuits\\crc_rtla_24_' + str(i) + '.vhd'
    print s
    writeCRC_RTLA(s,i,24,0x5D6DCB)

for i in range(32,264,8):
    s = 'circuits\\crc_rtla_32_' + str(i) + '.vhd'
    print s
    writeCRC_RTLA(s,i,32,0xF4ACFB13)
