proc cpu0
b *0x4014d0
continue
set itrace registers
b alt_exception
continue 