
imodelnewperipheral  \
    -name        sw_reset \
    -vendor      mcgill.ca  \
    -library     peripheral \
    -version     1.0 \
    -constructor constructor \
    -destructor  destructor

iadddocumentation -name Description -text "SW Reset peripheral model"

#mutex csr slave port
imodeladdbusslaveport -name SW_RESET_SLAVE -size 0x2400 -mustbeconnected
iadddocumentation     -name Description -text "SW Reset slave port"

imodeladdaddressblock -name ab32  -width 32  -offset 0x0 -size 0x2400


# Each processor has an offset
imodeladdmmregister -name cpum  -offset 0x0800 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu0  -offset 0x0000 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu1  -offset 0x0400 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu2  -offset 0x0C00 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu3  -offset 0x1000 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu4  -offset 0x1400 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu5  -offset 0x1800 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu6  -offset 0x1C00 -access rw -readfunction regRd32 -writefunction regWr32
imodeladdmmregister -name cpu7  -offset 0x2000 -access rw -readfunction regRd32 -writefunction regWr32

#reset outputs
imodeladdnetport -name cpum_reset -type output
imodeladdnetport -name cpu0_reset -type output
imodeladdnetport -name cpu1_reset -type output
imodeladdnetport -name cpu2_reset -type output
imodeladdnetport -name cpu3_reset -type output
imodeladdnetport -name cpu4_reset -type output
imodeladdnetport -name cpu5_reset -type output
imodeladdnetport -name cpu6_reset -type output
imodeladdnetport -name cpu7_reset -type output