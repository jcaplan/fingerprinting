#Write crc_rtla
from generateRTLA import *

def writeCRC_RTLA(outputLocation,dataWidth,crcWidth,GP):
    input = open('text\\crc_rtla.vhd','r')
    output = open(outputLocation,'w')
    S= input.read();

    S = S.replace('*data_width',str(dataWidth))
    S = S.replace('*crc_width',str(crcWidth))
    S = S.replace('*rtla',generateRTLA(dataWidth,crcWidth,GP,generateRTLAbin))

    if dataWidth != crcWidth:
        S = S.replace('--*','')
    else:
        S = S.replace('--%','')
    
    output.write(S)

def generateRTLA(dataWidth,crcWidth,GP,f):

    x = f(GP,dataWidth,crcWidth)
    S = ''
    if dataWidth != crcWidth:
        for i in range(0,dataWidth-crcWidth):
            S += 'F2('+ str(i) +') <= "' + x[i] + '";\n'
        for i in range(dataWidth-crcWidth,dataWidth):
            S += 'F1('+ str(i) +') <= "' + x[i] + '";\n'
        return S
    else:
        for i in range(0,dataWidth):
            S += 'F1('+ str(i) +') <= "' + x[i] + '";\n'
        return S


def generateRTLAverilog(dataWidth,crcWidth,GP):
    x = generateRTLAhex(GP,dataWidth,crcWidth)
    S = ''
    if dataWidth != crcWidth:
        for i in range(0,dataWidth):
            S += 'rtla['+ str(i) +"] ="+str(crcWidth)+"'h" + x[i][2:] + ';\n'
        return S
    else:
        for i in range(0,dataWidth):
            S += 'F1('+ str(i) +') <= "' + x[i] + '";\n'
        return S
#writeCRC_RTLA('crc_rtla_0.vhd',24,24,0x5D6DCB)
#writeCRC_RTLA('poly_cost\\crc_rtla_1.vhd',32,32,0x992C1A4C,generateRTLAbin)
#writeCRC_RTLA('poly_cost\\crc_rtla_2.vhd',32,32,0x32583499,generateRTLAbin)
#writeCRC_RTLA('poly_cost\\crc_rtla_3.vhd',32,32,0x0001DA97,generateRTLAbin)
#writeCRC_RTLA('p_compare.vhd',32,64,0x992C1A4C,generateRTLAbin)

#print(generateRTLAverilog(64,32,0x1da97))
