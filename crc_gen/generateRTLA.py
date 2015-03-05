def generateRTLAint(GP,circuitWidth,crcWidth):
    #Assign local variables
    x = []
    msbCheck = 1 << crcWidth;

    #Check for legal polynomial
    if GP & msbCheck:
        print "ERROR!"          
    else:
        #Generate RTLA coefficients
        #First coefficient is always the GP
        x.append(GP)
        for j in range(1, circuitWidth):
            x.append(x[j-1] << 1)
            if (x[j] & msbCheck):
               x[j] ^= GP + (msbCheck)
    return x

def generateRTLAhex(GP,circuitWidth,crcWidth):
    x = generateRTLAint(GP,circuitWidth,crcWidth)
    if x:
        #Conversion of CRC into properly formatted string
        stringLength = crcWidth/4;
        for j in range(0, circuitWidth):
            x[j] = hex(x[j]).replace('L','').replace('0x','')
            if len(x[j]) != stringLength:
                x[j] = '0'*(stringLength-len(x[j])) + x[j]    

        #Return list of hexadecimal string RTLA entries in the form '0x.......'
        return x

def generateRTLAbin(GP,circuitWidth,crcWidth):
    #Assign local variables
    x = generateRTLAint(GP,circuitWidth,crcWidth)
    if x:
        #Conversion of CRC into properly formatted string
        stringLength = crcWidth;
        for j in range(0, circuitWidth):
            x[j] = bin(x[j])[2:]
            if len(x[j]) != stringLength:
                x[j] = '0'*(stringLength-len(x[j])) + x[j]    

        #Return list of binary string RTLA entries in the form '........' (no '0b')
        return x



