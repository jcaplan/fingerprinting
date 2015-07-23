imodelnewperipheral  \
    -name        comparator \
    -vendor      mcgill.ca  \
    -library     peripheral \
    -version     2.0 \
    -constructor constructor \
    -destructor  destructor

iadddocumentation -name Description -text "Comparator peripheral model"


#fingerprinting csr slave port
imodeladdbusslaveport -name COMP_CSR_SLAVE -size 0x1000 
iadddocumentation     -name Description -text "Comparator control register slave port"

#fingerprinting csr slave port
imodeladdbusslaveport -name COMP_FPRINT_SLAVE -size 0x1000 
iadddocumentation     -name Description -text "Comparator fprint slave port"

imodeladdbusmasterport -name COMP_CHECKPOINT_IRQ_MASTER -addresswidth 32


#COMP_FPRINT_SLAVE
imodeladdaddressblock -name ab32 -port COMP_FPRINT_SLAVE  -width 32  -offset 0x0 -size 0x1000

#currentState reg
imodeladdmmregister -name currentState  -offset 0x0 -access w -writefunction currentStateWrite
imodeladdfield      -name taskID -bitoffset 0  -width 4 
imodeladdfield      -name enable -bitoffset 4  -width 1 
imodeladdfield      -name coreID -bitoffset 5  -width 4

#fprint reg
imodeladdmmregister -name fprint        -offset 0x4 -access w  -writefunction fprintWrite
imodeladdfield 	-name taskID    -bitoffset  0 -width  4
imodeladdfield  -name coreID    -bitoffset  4 -width  4
imodeladdfield  -name messageID -bitoffset  8 -width  1
imodeladdfield  -name fprint    -bitoffset 16 -width 16

#COMP_CSR_SLAVE
imodeladdaddressblock -name ab32 -port COMP_CSR_SLAVE  -width 32  -offset 0x0 -size 0x1000

#Exception reg
imodeladdmmregister -name exceptionReg      -offset  0x0 -access rw -readfunction csrRd32 -writefunction exceptionRegWrite
imodeladdmmregister -name statusReg         -offset  0x4 -access r  -readfunction csrRd32
imodeladdmmregister -name successReg        -offset  0x8 -access r  -readfunction csrRd32
imodeladdmmregister -name failedReg         -offset  0xc -access r  -readfunction csrRd32
imodeladdmmregister -name catReg            -offset 0x10 -access  w -writefunction catRegWrite
imodeladdfield -name data   -bitoffset 0   -width 16
imodeladdfield -name taskID -bitoffset 16  -width 8
imodeladdfield -name coreID -bitoffset 24  -width 8
imodeladdmmregister -name nmrReg            -offset 0x14 -access  w -writefunction nmrRegWrite
imodeladdfield -name data   -bitoffset 0   -width 16
imodeladdfield -name taskID -bitoffset 16  -width 8
imodeladdmmregister -name successCounterReg -offset 0x18 -access  w -writefunction successCounterRegWrite
imodeladdfield -name data   -bitoffset 0   -width 16
imodeladdfield -name taskID -bitoffset 16  -width 8


#Interrupt net
imodeladdnetport -name COMP_IRQ -type output
iadddocumentation     -name Description -text "comparator interrupt"

