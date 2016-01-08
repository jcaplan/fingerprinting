#Make sure to place this file in the platform directory in the scripts folder!


tcl source scripts/icount.tcl
proc cpu0
b *0x12f4
b *0x1380

tclcallback 1 myCallback
tclcallback 2 myCallback

continue
