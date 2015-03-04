def calculateCRC(gp,crcwidth,dataWidth,data):
    x = data;
    x <<= crcwidth
    gp += 2**crcwidth
    for i in range(dataWidth + crcwidth-1,crcwidth-1,-1):
        if (x & (1 <<i)):
            x ^= (gp << (i - crcwidth))
    return x

with open("out.log") as f:
    content = f.readlines()

gp = 0x1da97
crcwidth = 32
dataWidth = 59


    

crc = 0
for i in xrange(2,len(content)):
    data = int(content[i][0:len(content[i])-1],16)

    crc = calculateCRC(gp,crcwidth,dataWidth, (crc << 27) ^ data)
    print "intermediate CRC: \n" + hex(crc)

print "final crc: \n" + hex(crc)
print len(bin(int(content[0][0:len(content[i])-1],16)))    



