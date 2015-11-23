#Make sure to place this file in the platform directory in the scripts folder!


tcl source scripts/icount.tcl
proc cpu0
b *0x1854
b *0x18c4

tclcallback 1 myCallback
tclcallback 2 myCallback

continue
