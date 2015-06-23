
imodelnewperipheral  \
    -name        reset_monitor \
    -vendor      mcgill.ca  \
    -library     peripheral \
    -version     1.0 \
    -constructor constructor \
    -destructor  destructor

iadddocumentation -name Description -text "reset monitor peripheral model"

#mutex csr slave port
imodeladdbusslaveport -name RESET_MONITOR_SLAVE -size 0x400 -mustbeconnected
iadddocumentation     -name Description -text "SW Reset slave port"

imodeladdaddressblock -name ab32  -width 32  -offset 0x0 -size 0x400


# Each processor has an offset
imodeladdmmregister -name enableReg  	-offset 0x000 -access rw -readfunction regRd32 -writefunction writeEnableReg
imodeladdmmregister -name monitorReg  	-offset 0x004 -access rw -readfunction regRd32 -writefunction writeMonitorReg
imodeladdmmregister -name coreStatusReg -offset 0x008 -access rw -readfunction regRd32 -writefunction writeCoreReg

#reset outputs
imodeladdnetport -name cpum_notify -type output