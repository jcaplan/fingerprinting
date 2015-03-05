
imodelnewperipheral  \
    -name        cpu_irq \
    -vendor      mcgill.ca  \
    -library     peripheral \
    -version     1.0 \
    -constructor constructor \
    -destructor  destructor

iadddocumentation -name Description -text "CPU IRQ peripheral model"

#mutex csr slave port
imodeladdbusslaveport -name CPU_IRQ_SLAVE -size 0x2400 -mustbeconnected
iadddocumentation     -name Description -text "CPU IRQ slave port"

imodeladdaddressblock -name ab32  -width 32  -offset 0x0 -size 0x2400


# 32 bit registers
imodeladdmmregister -name cpum  -offset 0x0800 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu0  -offset 0x0000 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu1  -offset 0x0400 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu2  -offset 0x0C00 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu3  -offset 0x1000 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu4  -offset 0x1400 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu5  -offset 0x1800 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu6  -offset 0x1C00 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu7  -offset 0x2000 -access rw -readfunction regRd32 -writefunction regWr32


imodeladdfield      -name RESET    -bitoffset 0  -width 1 
#CHANGE THE OFFSET BACK TO 4 AFTER!
imodeladdfield      -name RESERVED -bitoffset 1  -width 31 


imodeladdformal   -name InitialValue -type uns32 
imodeladdformal   -name InitialOwner -type uns32 

imodeladdnetport -name cpum_irq -type output
imodeladdnetport -name cpu0_irq -type output
imodeladdnetport -name cpu1_irq -type output
imodeladdnetport -name cpu2_irq -type output
imodeladdnetport -name cpu3_irq -type output
imodeladdnetport -name cpu4_irq -type output
imodeladdnetport -name cpu5_irq -type output
imodeladdnetport -name cpu6_irq -type output
imodeladdnetport -name cpu7_irq -type output