
imodelnewperipheral  \
    -name        fprint \
    -vendor      mcgill.ca  \
    -library     peripheral \
    -version     2.0 \
    -constructor constructor \
    -destructor  destructor

iadddocumentation -name Description -text "Fingerprint peripheral model"

#fingerprinting master bus port
#imodeladdbusmasterport -name "FPRINT_MASTER"  -addresswidth 32 
#-mustbeconnected
#iadddocumentation -name Description -text "Fingerprint master bus port"


imodeladdbusmasterport -name FPRINT_MASTER -addresswidth 32 
#-mustbeconnected
iadddocumentation     -name Description -text "Fingerprint master port"

#fingerprinting csr slave port
imodeladdbusslaveport -name FPRINT_CSR_SLAVE -size 0x140 -mustbeconnected
iadddocumentation     -name Description -text "Fingerprint control register slave port"

imodeladdaddressblock -name ab32  -width 32  -offset 0x0 -size 0x20


#Fingerprinting nets
imodeladdnetport -name FPRINT_WRITE -type input -updatefunction do_store
iadddocumentation     -name Description -text "bus write signal"

imodeladdnetport -name "FPRINT_WRITE_ADDRESS" -type input
iadddocumentation     -name Description -text "bus address signal"

imodeladdnetport -name "FPRINT_WRITE_DATA" -type input
iadddocumentation     -name Description -text "bus address signal"


# 32 bit registers
imodeladdmmregister -name currentState  -offset 0x0 -access rw -readfunction regRd32 -writefunction regWr32

imodeladdfield      -name key    -bitoffset 0  -width 4 
#CHANGE THE OFFSET BACK TO 4 AFTER!
imodeladdfield      -name enable -bitoffset 4  -width 1 

imodeladdmmregister -name pauseReg      -offset 0x4 -access rw  -readfunction regRd32 -writefunction regWr32 
imodeladdmmregister -name pauseStrobe   -offset 0x8 -access r   -readfunction pause 
imodeladdmmregister -name unpauseStrobe -offset 0xC -access r   -readfunction unpause
imodeladdmmregister -name counterMax    -offset 0x10 -access rw -readfunction regRd32 -writefunction regWr32



iadddocumentation -name Description -text "current state register"   -handle FPRINT_CSR_SLAVE/ab32/currentState
iadddocumentation -name Description -text "pause strobe"   -handle FPRINT_CSR_SLAVE/ab32/pauseStrobe
iadddocumentation -name Description -text "unpause strobe"            -handle FPRINT_CSR_SLAVE/ab32/unpauseStrobe
iadddocumentation -name Description -text "counter max"        -handle FPRINT_CSR_SLAVE/ab32/counterMax

imodeladdformal   -name coreID -type uns32 -max 15 

imodeladdbusslaveport -name FPRINT_TLB_SLAVE -size 0x400 
iadddocumentation     -name Description -text "Get translations from TLB"
imodeladdaddressblock -name ab32 -port FPRINT_TLB_SLAVE    -width 32  -offset 0x0 -size 0x400
imodeladdlocalmemory -addressblock FPRINT_TLB_SLAVE/ab32 -name fprint_table  -offset 0x00 -size 0x84 -writefunction memregWr
