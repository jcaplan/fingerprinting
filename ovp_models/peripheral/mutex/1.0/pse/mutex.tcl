
imodelnewperipheral  \
    -name        mutex \
    -vendor      mcgill.ca  \
    -library     peripheral \
    -version     1.0 \
    -constructor constructor \
    -destructor  destructor

iadddocumentation -name Description -text "Mutex peripheral model"

#mutex csr slave port
imodeladdbusslaveport -name MUTEX_SLAVE -size 0x8 -mustbeconnected
iadddocumentation     -name Description -text "Mutex control register slave port"

imodeladdaddressblock -name ab32  -width 32  -offset 0x0 -size 0x8

# 32 bit registers
imodeladdmmregister -name mutex  -offset 0x0 -access rw -readfunction regRd32 -writefunction regWr32

imodeladdfield      -name OWNER    -bitoffset 16  -width 16 
#CHANGE THE OFFSET BACK TO 4 AFTER!
imodeladdfield      -name VALUE -bitoffset 0  -width 16 

imodeladdmmregister -name reset  -offset 0x4 -access rw -readfunction regRd32 -writefunction regWr32

imodeladdfield      -name RESET    -bitoffset 0  -width 1 
#CHANGE THE OFFSET BACK TO 4 AFTER!
imodeladdfield      -name RESERVED -bitoffset 1  -width 31 


imodeladdformal   -name InitialValue -type uns32 
imodeladdformal   -name InitialOwner -type uns32 
