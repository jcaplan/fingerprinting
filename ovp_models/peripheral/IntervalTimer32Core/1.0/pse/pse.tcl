#
# Copyright (c) 2005-2014 Imperas Software Ltd. All Rights Reserved.
#
# THIS SOFTWARE CONTAINS CONFIDENTIAL INFORMATION AND TRADE SECRETS
# OF IMPERAS SOFTWARE LTD. USE, DISCLOSURE, OR REPRODUCTION IS PROHIBITED
# EXCEPT AS MAY BE PROVIDED FOR IN A WRITTEN AGREEMENT WITH IMPERAS SOFTWARE LTD.
#

set vendor  mcgill.ca
set library peripheral
set name    IntervalTimer32Core
set version 1.0

imodelnewperipheral -name $name -imagefile pse.pse -library $library -vendor $vendor -version $version -constructor constructor

iadddocumentation -name Licensing   -text "Open Source Apache 2.0"
iadddocumentation -name Description -text "Altera Avalon Interval Timer32 Core"
iadddocumentation -name Limitations -text "No Support for pin level transitions"

imodeladdformal -name timeoutPeriod  -type integer
imodeladdformal -name timerFrequency -type integer
imodeladdformal -name timeoutConfig  -type enumeration -defaultValue Simple
  imodeladdenumeration -formal timeoutConfig -name Simple
  imodeladdenumeration -formal timeoutConfig -name Full
  imodeladdenumeration -formal timeoutConfig -name Watchdog
imodeladdformal -name writeablePeriod  -type bool -defaultValue 0
imodeladdformal -name readableSnapshot -type bool -defaultValue 0
imodeladdformal -name startStopControlBits -type bool -defaultValue 0
imodeladdformal -name timeoutPulse -type bool -defaultValue 0
imodeladdformal -name systemResetOnTimeout -type bool -defaultValue 0

imodeladdnetport -name irq           -type output
imodeladdnetport -name resetrequest  -type output
imodeladdnetport -name timeout_pulse -type output

#
# Slave port
#
imodeladdbusslaveport -name sp1 -size 24

#
# Registers
#
imodeladdaddressblock -name reg0 -port sp1 -offset 0 -size 24 -width 16

#
# Registers
#
imodeladdmmregister -addressblock sp1/reg0 -name status  -width 16 -offset 0  -access rw -writefunction write_status  -readfunction read_status
imodeladdmmregister -addressblock sp1/reg0 -name control -width 16 -offset 4  -access rw -writefunction write_control -readfunction read_control
imodeladdmmregister -addressblock sp1/reg0 -name periodl -width 16 -offset 8  -access rw -writefunction write_period  -readfunction read_period  -userdata 0
imodeladdmmregister -addressblock sp1/reg0 -name periodh -width 16 -offset 12 -access rw -writefunction write_period  -readfunction read_period  -userdata 1
imodeladdmmregister -addressblock sp1/reg0 -name snapl   -width 16 -offset 16 -access rw -writefunction write_snap    -readfunction read_snap    -userdata 0
imodeladdmmregister -addressblock sp1/reg0 -name snaph   -width 16 -offset 20 -access rw -writefunction write_snap    -readfunction read_snap    -userdata 1

#
# Resets
#
imodeladdreset -mmregister sp1/reg0/status  -name IRESET -value 0x0000
imodeladdreset -mmregister sp1/reg0/control -name IRESET -value 0x0000
imodeladdreset -mmregister sp1/reg0/periodl -name IRESET -value 0x0000
imodeladdreset -mmregister sp1/reg0/periodh -name IRESET -value 0x0000
imodeladdreset -mmregister sp1/reg0/snapl   -name IRESET -value 0x0000
imodeladdreset -mmregister sp1/reg0/snaph   -name IRESET -value 0x0000

#
# Bit Fields
#
imodeladdfield -mmregister sp1/reg0/status  -name TO    -bitoffset 0 -width 1
imodeladdfield -mmregister sp1/reg0/status  -name RUN   -bitoffset 1 -width 1
imodeladdfield -mmregister sp1/reg0/control -name ITO   -bitoffset 0 -width 1
imodeladdfield -mmregister sp1/reg0/control -name CONT  -bitoffset 1 -width 1
imodeladdfield -mmregister sp1/reg0/control -name START -bitoffset 2 -width 1
imodeladdfield -mmregister sp1/reg0/control -name STOP  -bitoffset 3 -width 1
