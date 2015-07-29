
# proc cpum
# b sendDMA
# b handleComp
# proc cpu1
# b handleCPU
# continue

proc cpu0
b handleCPU
continue
set itrace change
diagnostics cpu0_fprint 3
b alt_exception
continue


# proc cpu0
# b handleCPU
# continue
# continue
# continue
# b alt_exception
# set itrace change
# continue
