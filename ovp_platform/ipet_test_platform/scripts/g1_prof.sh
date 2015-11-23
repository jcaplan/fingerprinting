#Make sure to place this file in the platform directory in the scripts folder!


tcl source scripts/icount.tcl
proc cpu0
b *0x113c
b *0x11a8

tclcallback 1 myCallback
tclcallback 2 myCallback

continue
