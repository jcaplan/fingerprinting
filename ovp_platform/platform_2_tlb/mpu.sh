
# proc cpum
# b sendDMA
# b handleComp
# proc cpu1
# b handleCPU
# continue

# proc cpu1
# b handleCPU
# continue
# continue
# continue
# set itrace change
# diagnostics cpu1_fprint 3
# b alt_exception
# continue

proc cpu0 
b runtimeMonitor.c:14
proc cpu0_timestamp
b clocksRemaining
b timerStart
continue
# proc cpu0
# b handleCPU
# continue
# continue
# continue
# b alt_exception
# set itrace change
# continue
