
# proc cpum
# b sendDMA
# b handleComp
# proc cpu1
# b handleCPU
# continue

proc cpu1
b handleCPU
continue
continue
continue
set itrace change
diagnostics cpu1_fprint 3
proc cpum
b handleComp
continue


# proc cpu0
# b handleCPU
# continue
# continue
# continue
# b alt_exception
# set itrace change
# continue
