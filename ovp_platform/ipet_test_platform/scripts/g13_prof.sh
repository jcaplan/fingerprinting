#Make sure to place this file in the platform directory in the scripts folder!


tcl source scripts/icount.tcl
proc cpu0
b *0x17d4
b *0x1850

tclcallback 1 myCallback
tclcallback 2 myCallback

continue
