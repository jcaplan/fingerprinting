#
# Copyright (c) 2005-2014 Imperas Software Ltd. All Rights Reserved.
#
# THIS SOFTWARE CONTAINS CONFIDENTIAL INFORMATION AND TRADE SECRETS
# OF IMPERAS SOFTWARE LTD. USE, DISCLOSURE, OR REPRODUCTION IS PROHIBITED
# EXCEPT AS MAY BE PROVIDED FOR IN A WRITTEN AGREEMENT WITH IMPERAS SOFTWARE LTD.
#

set desc "This platform models the Altera Cyclone III 3c120.

The processor is an Altera Nios_II procesor, Nios_II_F."

set limit "Peripherals are modeled only to the extent required to boot and run Operating Systems such as Linux."

proc define {name {value 1}} {
    upvar $name l_name
    set l_name $value
}

proc mkram { name bus lo hi } {
    ihwaddmemory -instancename ${name} -type ram
    ihwconnect -bus $bus -instancename ${name} -busslaveport sp1 -loaddress ${lo} -hiaddress ${hi}
}



# Setup variables for platform info
set vendor  mcgill.ca
set library platform
set name    FprintPlatform
set version 1.0

#
# Start new platform creation
#
ihwnew -name $name -vendor $vendor -library $library -version $version -stoponctrlc -verbose

#
# Documentation
#
iadddocumentation -name Licensing -text "Open Source Apache 2.0"

iadddocumentation -name Description -text $desc
iadddocumentation -name Limitations -text $limit

#
# command line parser
#
ihwaddclp -allargs

ihwaddbus -instancename smbus -addresswidth 32
mkram shared_memory     smbus          0x00000000 0x000003FF

ihwaddbus -instancename mmbus -addresswidth 32
mkram main_memory     mmbus          0x00000000 0x00128000

ihwaddbus -instancename flashbus -addresswidth 32
mkram flash     flashbus          0x00000000 0x003FFFFF

ihwaddbus -instancename mutexbus -addresswidth 32
ihwaddbus -instancename cpuirqbus -addresswidth 32
#
# MUTEX
#
ihwaddperipheral -instancename mutex -vendor mcgill.ca -type mutex -diagnosticlevel 0
ihwconnect       -instancename mutex  -bus mutexbus -busslaveport MUTEX_SLAVE -loaddress 0x0000000 -hiaddress 0x7

ihwaddperipheral -instancename cpu_irq -vendor mcgill.ca -type cpu_irq -diagnosticlevel 0
ihwconnect       -instancename cpu_irq  -bus cpuirqbus -busslaveport CPU_IRQ_SLAVE -loaddress 0x0000000 -hiaddress 0x000000023FF

###############################################################################
# Processor 0
###############################################################################
### grep define system.h | sed 's/#//' > system.tcl
### source system.tcl
###
define CPU0_BIG_ENDIAN 0
define CPU0_BREAK_ADDR 0x08000820
define CPU0_CPU_FREQ 50000000u
define CPU0_CPU_ID_SIZE 1
define CPU0_CPU_ID_VALUE 0x00000001
define CPU0_CPU_IMPLEMENTATION "small"
define CPU0_DATA_ADDR_WIDTH 0x1c
define CPU0_DCACHE_LINE_SIZE 0
define CPU0_DCACHE_LINE_SIZE_LOG2 0
define CPU0_DCACHE_SIZE 0
define CPU0_EXCEPTION_ADDR 0x00464020
define CPU0_FLUSHDA_SUPPORTED
define CPU0_HARDWARE_DIVIDE_PRESENT 0
define CPU0_HARDWARE_MULTIPLY_PRESENT 0
define CPU0_HARDWARE_MULX_PRESENT 0
define CPU0_HAS_DEBUG_CORE 1
define CPU0_HAS_DEBUG_STUB
define CPU0_HAS_JMPI_INSTRUCTION
define CPU0_ICACHE_LINE_SIZE 32
define CPU0_ICACHE_LINE_SIZE_LOG2 5
define CPU0_ICACHE_SIZE 1024
define CPU0_INST_ADDR_WIDTH 0x1c
define CPU0_RESET_ADDR 0x00464000

###
###
###
ihwaddprocessor \
	-instancename cpu0 \
	-type nios_ii \
	-vendor mcgill.ca \
	-version 1.0 \
	-endian little \
	-simulateexceptions -mips 125.0
isetattribute -handle cpu0 -name variant                         -value Nios_II_S
isetattribute -handle cpu0 -name BREAK_VECTOR                    -value ${CPU0_BREAK_ADDR}
isetattribute -handle cpu0 -name EXCEPTION_VECTOR                -value ${CPU0_EXCEPTION_ADDR}
isetattribute -handle cpu0 -name RESET_VECTOR                    -value ${CPU0_RESET_ADDR}
isetattribute -handle cpu0 -name HW_DIVIDE                       -value ${CPU0_HARDWARE_DIVIDE_PRESENT}
isetattribute -handle cpu0 -name HW_MULTIPLY                     -value ${CPU0_HARDWARE_MULTIPLY_PRESENT}
isetattribute -handle cpu0 -name HW_MULX                         -value ${CPU0_HARDWARE_MULX_PRESENT}
isetattribute -handle cpu0 -name INCLUDE_MMU                     -value 0
isetattribute -handle cpu0 -name DATA_ADDR_WIDTH                 -value ${CPU0_DATA_ADDR_WIDTH}
isetattribute -handle cpu0 -name INST_ADDR_WIDTH                 -value ${CPU0_INST_ADDR_WIDTH}
isetattribute -handle cpu0 -name TEST_HALT_EXIT                  -value 1



ihwaddbus -instancename cpu0_iobus -addresswidth 32


#
# Individual I & D Bus
#
ihwaddbus -instancename cpu0_dbus  -addresswidth 32
ihwaddbus -instancename cpu0_ibus  -addresswidth 32

#
# Connect processor to bus
#
ihwconnect -bus cpu0_ibus -instancename cpu0 -busmasterport INSTRUCTION -loaddress 0 -hiaddress 0x0FFFFFFF
ihwconnect -bus cpu0_dbus -instancename cpu0 -busmasterport DATA        -loaddress 0 -hiaddress 0x0FFFFFFF


ihwaddbridge -instancename cpu0_out_bridge_d
ihwconnect   -instancename cpu0_out_bridge_d -busslaveport  sp1 -bus cpu0_dbus  -loaddress 0x03000000 -hiaddress 0x030003FF
ihwconnect   -instancename cpu0_out_bridge_d -busmasterport mp1 -bus smbus -loaddress 0x00000000 -hiaddress 0x000003FF

ihwaddbridge -instancename cpu0_mm_out_bridge_i
ihwconnect 	 -instancename cpu0_mm_out_bridge_i -busslaveport  sp1 -bus cpu0_ibus  -loaddress 0x00400000 -hiaddress 0x00527FFF
ihwconnect   -instancename cpu0_mm_out_bridge_i -busmasterport mp1 -bus mmbus      -loaddress 0x00000000 -hiaddress 0x00127FFF

ihwaddbridge -instancename cpu0_mm_out_bridge_d
ihwconnect 	 -instancename cpu0_mm_out_bridge_d -busslaveport  sp1 -bus cpu0_dbus  -loaddress 0x00400000 -hiaddress 0x00527FFF
ihwconnect   -instancename cpu0_mm_out_bridge_d -busmasterport mp1 -bus mmbus      -loaddress 0x00000000 -hiaddress 0x00127FFF



ihwaddbridge -instancename cpu0_mutex
ihwconnect 	 -instancename cpu0_mutex -busslaveport  sp1 -bus cpu0_dbus  -loaddress 0x04500000 -hiaddress 0x04500007
ihwconnect   -instancename cpu0_mutex -busmasterport mp1 -bus mutexbus   -loaddress 0x00000000 -hiaddress 0x00000007

ihwaddbridge -instancename cpu0_irq_bridge
ihwconnect 	 -instancename cpu0_irq_bridge -busslaveport  sp1 -bus cpu0_dbus   -loaddress 0x04000000 -hiaddress 0x040023FF
ihwconnect   -instancename cpu0_irq_bridge -busmasterport mp1 -bus cpuirqbus   -loaddress 0x00000000 -hiaddress 0x000023FF

ihwaddbridge -instancename cpu0_flash_bridge
ihwconnect 	 -instancename cpu0_flash_bridge -busslaveport  sp1 -bus cpu0_dbus   -loaddress 0x00000000 -hiaddress 0x003FFFFF
ihwconnect   -instancename cpu0_flash_bridge -busmasterport mp1 -bus flashbus    -loaddress 0x00000000 -hiaddress 0x003FFFFF


#
#
# Memory
#
#proc mkram { name bus lo hi }
mkram cpu0_breakmem     cpu0_ibus      0x08008020 0x08008100
mkram cpu0_scratchpad   cpu0_iobus     0x00200000 0x00203FFF


# IO bus bridgepb_cpu_to_io 0x0800_0000 0x087F_FFFF
#
ihwaddbridge -instancename cpu0_local_bridge
ihwconnect   -instancename cpu0_local_bridge -busslaveport  sp1 -bus cpu0_dbus  -loaddress 0x08000000 -hiaddress 0x087FFFFF
ihwconnect   -instancename cpu0_local_bridge -busmasterport mp1 -bus cpu0_iobus -loaddress 0x00000000 -hiaddress 0x007FFFFF 


# 0x4D50 - 0x4D57 jtag_uart 
ihwaddperipheral -instancename cpu0_jtag_uart -vendor altera.ovpworld.org -type JtagUart -diagnosticlevel 0
ihwconnect       -instancename cpu0_jtag_uart -bus cpu0_iobus -busslaveport sp1 -loaddress 0x0001040 -hiaddress 0x0001047
isetattribute    -handle       cpu0_jtag_uart -name writeIRQThreshold  -value 8
isetattribute    -handle       cpu0_jtag_uart -name readIRQThreshold   -value 8
isetattribute    -handle       cpu0_jtag_uart -name writeBufferDepth   -value 64
isetattribute    -handle       cpu0_jtag_uart -name readBufferDepth    -value 64
isetattribute    -handle       cpu0_jtag_uart -name console            -value 1
isetattribute    -handle       cpu0_jtag_uart -name finishOnDisconnect -value 1
ihwconnect       -instancename cpu0_jtag_uart -netport irq     -net cpu0_irq4
ihwconnect       -instancename cpu0           -netport d_irq4  -net cpu0_irq4

# 0x00400000 - 0x00400017 timer_10ms
ihwaddperipheral -instancename cpu0_timer_10ms -vendor mcgill.ca -type IntervalTimer32Core -diagnosticlevel 0
ihwconnect       -instancename cpu0_timer_10ms -bus cpu0_iobus -busslaveport sp1 -loaddress 0x00001020 -hiaddress 0x00001037
isetattribute    -handle       cpu0_timer_10ms -name timerFrequency  -value 50000000
isetattribute    -handle       cpu0_timer_10ms -name timeoutPeriod   -value 500000 
ihwconnect       -instancename cpu0_timer_10ms -netport irq     -net cpu0_irq0
ihwconnect       -instancename cpu0            -netport d_irq0 -net cpu0_irq0

# 0x00400000 - 0x00400017 timestamp
ihwaddperipheral -instancename cpu0_timestamp -vendor mcgill.ca -type IntervalTimer32Core -diagnosticlevel 0
ihwconnect       -instancename cpu0_timestamp -bus cpu0_iobus -busslaveport sp1 -loaddress 0x00000000 -hiaddress 0x00000017
isetattribute    -handle       cpu0_timestamp -name timerFrequency  -value 50000000
isetattribute    -handle       cpu0_timestamp -name timeoutPeriod   -value 500000

#INTER CPU irq
ihwconnect       -instancename cpu_irq         -netport cpu0_irq     -net cpu0_irq1
ihwconnect       -instancename cpu0            -netport d_irq1       -net cpu0_irq1

#
# FPRINT
#
ihwaddperipheral -instancename cpu0_fprint -vendor mcgill.ca -version 1.0 -type fprint -diagnosticlevel 0
ihwconnect       -instancename cpu0_fprint    -bus cpu0_iobus -busslaveport FPRINT_CSR_SLAVE -loaddress 0x00100000 -hiaddress 0x0010013F
isetattribute    -handle       cpu0_fprint -name coreID  -value 0


ihwconnect       -instancename cpu0_fprint    -netport FPRINT_WRITE              -net fprint_write_0
ihwconnect       -instancename cpu0           -netport fprint_write_out          -net fprint_write_0

ihwconnect       -instancename cpu0_fprint    -netport FPRINT_WRITE_ADDRESS      -net fprint_write_address_0
ihwconnect       -instancename cpu0           -netport fprint_write_out_address  -net fprint_write_address_0

ihwconnect       -instancename cpu0_fprint    -netport FPRINT_WRITE_DATA         -net fprint_write_data_0
ihwconnect       -instancename cpu0           -netport fprint_write_out_data     -net fprint_write_data_0


###############################################################################
# Processor 1
###############################################################################
### grep define system.h | sed 's/#//' > system.tcl
### source system.tcl
###

define CPU1_BIG_ENDIAN 0
define CPU1_BREAK_ADDR 0x08008020
define CPU1_CPU_FREQ 50000000u
define CPU1_CPU_ID_SIZE 1
define CPU1_CPU_ID_VALUE 0x00000002
define CPU1_CPU_IMPLEMENTATION "small"
define CPU1_DATA_ADDR_WIDTH 0x1c
define CPU1_DCACHE_LINE_SIZE 0
define CPU1_DCACHE_LINE_SIZE_LOG2 0
define CPU1_DCACHE_SIZE 0
define CPU1_EXCEPTION_ADDR 0x00432020
define CPU1_FLUSHDA_SUPPORTED
define CPU1_HARDWARE_DIVIDE_PRESENT 0
define CPU1_HARDWARE_MULTIPLY_PRESENT 0
define CPU1_HARDWARE_MULX_PRESENT 0
define CPU1_HAS_DEBUG_CORE 1
define CPU1_HAS_DEBUG_STUB
define CPU1_HAS_JMPI_INSTRUCTION
define CPU1_ICACHE_LINE_SIZE 32
define CPU1_ICACHE_LINE_SIZE_LOG2 5
define CPU1_ICACHE_SIZE 1024
define CPU1_INST_ADDR_WIDTH 0x1c
define CPU1_RESET_ADDR 0x00432000



###
###
###
ihwaddprocessor \
	-instancename cpu1 \
	-type nios_ii \
	-vendor mcgill.ca \
	-version 1.0 \
	-endian little \
	-simulateexceptions -mips 125.0
isetattribute -handle cpu1 -name variant                         -value Nios_II_S
isetattribute -handle cpu1 -name BREAK_VECTOR                    -value ${CPU1_BREAK_ADDR}
isetattribute -handle cpu1 -name EXCEPTION_VECTOR                -value ${CPU1_EXCEPTION_ADDR}
isetattribute -handle cpu1 -name RESET_VECTOR                    -value ${CPU1_RESET_ADDR}
isetattribute -handle cpu1 -name HW_DIVIDE                       -value ${CPU1_HARDWARE_DIVIDE_PRESENT}
isetattribute -handle cpu1 -name HW_MULTIPLY                     -value ${CPU1_HARDWARE_MULTIPLY_PRESENT}
isetattribute -handle cpu1 -name HW_MULX                         -value ${CPU1_HARDWARE_MULX_PRESENT}
isetattribute -handle cpu1 -name INCLUDE_MMU                     -value 0
isetattribute -handle cpu1 -name DATA_ADDR_WIDTH                 -value ${CPU1_DATA_ADDR_WIDTH}
isetattribute -handle cpu1 -name INST_ADDR_WIDTH                 -value ${CPU1_INST_ADDR_WIDTH}
isetattribute -handle cpu1 -name TEST_HALT_EXIT                  -value 1



ihwaddbus -instancename cpu1_iobus -addresswidth 32


#
# Individual I & D Bus
#
ihwaddbus -instancename cpu1_dbus  -addresswidth 32
ihwaddbus -instancename cpu1_ibus  -addresswidth 32



#
# Connect processor to bus
#
ihwconnect -bus cpu1_ibus -instancename cpu1 -busmasterport INSTRUCTION -loaddress 0 -hiaddress 0x0FFFFFFF
ihwconnect -bus cpu1_dbus -instancename cpu1 -busmasterport DATA        -loaddress 0 -hiaddress 0x0FFFFFFF


ihwaddbridge -instancename cpu1_out_bridge_d
ihwconnect   -instancename cpu1_out_bridge_d -busslaveport  sp1 -bus cpu1_dbus  -loaddress 0x03000000 -hiaddress 0x030003FF
ihwconnect   -instancename cpu1_out_bridge_d -busmasterport mp1 -bus smbus -loaddress 0x00000000 -hiaddress 0x000003FF


ihwaddbridge -instancename cpu1_mm_out_bridge_i
ihwconnect 	 -instancename cpu1_mm_out_bridge_i -busslaveport  sp1 -bus cpu1_ibus  -loaddress 0x00400000 -hiaddress 0x00527FFF
ihwconnect   -instancename cpu1_mm_out_bridge_i -busmasterport mp1 -bus mmbus      -loaddress 0x00000000 -hiaddress 0x00127FFF

ihwaddbridge -instancename cpu1_mm_out_bridge_d
ihwconnect 	 -instancename cpu1_mm_out_bridge_d -busslaveport  sp1 -bus cpu1_dbus  -loaddress 0x00400000 -hiaddress 0x00527FFF
ihwconnect   -instancename cpu1_mm_out_bridge_d -busmasterport mp1 -bus mmbus      -loaddress 0x00000000 -hiaddress 0x00127FFF


ihwaddbridge -instancename cpu1_mutex
ihwconnect 	 -instancename cpu1_mutex -busslaveport  sp1 -bus cpu1_dbus  -loaddress 0x04500000 -hiaddress 0x04500007
ihwconnect   -instancename cpu1_mutex -busmasterport mp1 -bus mutexbus   -loaddress 0x00000000 -hiaddress 0x00000007

ihwaddbridge -instancename cpu1_irq_bridge
ihwconnect 	 -instancename cpu1_irq_bridge -busslaveport  sp1 -bus cpu1_dbus   -loaddress 0x04000000 -hiaddress 0x040023FF
ihwconnect   -instancename cpu1_irq_bridge -busmasterport mp1 -bus cpuirqbus   -loaddress 0x00000000 -hiaddress 0x000023FF

ihwaddbridge -instancename cpu1_flash_bridge
ihwconnect 	 -instancename cpu1_flash_bridge -busslaveport  sp1 -bus cpu1_dbus   -loaddress 0x00000000 -hiaddress 0x003FFFFF
ihwconnect   -instancename cpu1_flash_bridge -busmasterport mp1 -bus flashbus    -loaddress 0x00000000 -hiaddress 0x003FFFFF


#
# Memory
#
#proc mkram { name bus lo hi }
mkram cpu1_breakmem     cpu1_ibus       0x08008020 0x08008100
mkram cpu1_scratchpad   cpu1_iobus      0x00200000 0x00203FFF

# IO bus bridgepb_cpu_to_io 0x0800_0000 0x087F_FFFF
#
ihwaddbridge -instancename cpu1_local_bridge
ihwconnect   -instancename cpu1_local_bridge -busslaveport  sp1 -bus cpu1_dbus  -loaddress 0x08000000 -hiaddress 0x087FFFFF
ihwconnect   -instancename cpu1_local_bridge -busmasterport mp1 -bus cpu1_iobus -loaddress 0x00000000 -hiaddress 0x007FFFFF 




# 0x4D50 - 0x4D57 jtag_uart 
ihwaddperipheral -instancename cpu1_jtag_uart -vendor altera.ovpworld.org -type JtagUart -diagnosticlevel 0
ihwconnect       -instancename cpu1_jtag_uart -bus cpu1_iobus -busslaveport sp1 -loaddress 0x0008a00 -hiaddress 0x0008a07
isetattribute    -handle       cpu1_jtag_uart -name writeIRQThreshold  -value 8
isetattribute    -handle       cpu1_jtag_uart -name readIRQThreshold   -value 8
isetattribute    -handle       cpu1_jtag_uart -name writeBufferDepth   -value 64
isetattribute    -handle       cpu1_jtag_uart -name readBufferDepth    -value 64
isetattribute    -handle       cpu1_jtag_uart -name console            -value 1
isetattribute    -handle       cpu1_jtag_uart -name finishOnDisconnect -value 1
ihwconnect       -instancename cpu1_jtag_uart -netport irq     -net cpu1_irq3
ihwconnect       -instancename cpu1           -netport d_irq3  -net cpu1_irq3

# 0x00400000 - 0x00400017 timer_10ms
ihwaddperipheral -instancename cpu1_timer_10ms -vendor mcgill.ca -type IntervalTimer32Core -diagnosticlevel 0
ihwconnect       -instancename cpu1_timer_10ms -bus cpu1_iobus -busslaveport sp1 -loaddress 0x00008900 -hiaddress 0x00008917
isetattribute    -handle       cpu1_timer_10ms -name timerFrequency  -value 50000000
isetattribute    -handle       cpu1_timer_10ms -name timeoutPeriod   -value 500000 
ihwconnect       -instancename cpu1_timer_10ms -netport irq     -net cpu1_irq0
ihwconnect       -instancename cpu1            -netport d_irq0 -net cpu1_irq0

# 0x00400000 - 0x00400017 timestamp
ihwaddperipheral -instancename cpu1_timestamp -vendor mcgill.ca -type IntervalTimer32Core -diagnosticlevel 0
ihwconnect       -instancename cpu1_timestamp -bus cpu1_iobus -busslaveport sp1 -loaddress 0x00000000 -hiaddress 0x00000017
isetattribute    -handle       cpu1_timestamp -name timerFrequency  -value 50000000
isetattribute    -handle       cpu1_timestamp -name timeoutPeriod   -value 500000

#INTER CPU irq
ihwconnect       -instancename cpu_irq         -netport cpu1_irq     -net cpu1_irq1
ihwconnect       -instancename cpu1            -netport d_irq1       -net cpu1_irq1

#
# FPRINT
#
ihwaddperipheral -instancename cpu1_fprint -vendor mcgill.ca -type fprint -version 1.0 -diagnosticlevel 0
ihwconnect       -instancename cpu1_fprint    -bus cpu1_iobus -busslaveport FPRINT_CSR_SLAVE -loaddress 0x00100000 -hiaddress 0x0010013F
isetattribute    -handle       cpu1_fprint -name coreID  -value 1

ihwconnect       -instancename cpu1_fprint    -netport FPRINT_WRITE              -net fprint_write_1
ihwconnect       -instancename cpu1           -netport fprint_write_out          -net fprint_write_1

ihwconnect       -instancename cpu1_fprint    -netport FPRINT_WRITE_ADDRESS      -net fprint_write_address_1
ihwconnect       -instancename cpu1           -netport fprint_write_out_address  -net fprint_write_address_1

ihwconnect       -instancename cpu1_fprint    -netport FPRINT_WRITE_DATA         -net fprint_write_data_1
ihwconnect       -instancename cpu1           -netport fprint_write_out_data     -net fprint_write_data_1


###############################################################################
# Processor 2
###############################################################################
### grep define system.h | sed 's/#//' > system.tcl
### source system.tcl
###
define CPU2_BIG_ENDIAN 0
define CPU2_BREAK_ADDR 0x08000820
define CPU2_CPU_FREQ 50000000u
define CPU2_CPU_ID_SIZE 2
define CPU2_CPU_ID_VALUE 0x00000003
define CPU2_CPU_IMPLEMENTATION "small"
define CPU2_DATA_ADDR_WIDTH 0x1c
define CPU2_DCACHE_LINE_SIZE 0
define CPU2_DCACHE_LINE_SIZE_LOG2 0
define CPU2_DCACHE_SIZE 0
define CPU2_EXCEPTION_ADDR 0x00496020
define CPU2_FLUSHDA_SUPPORTED
define CPU2_HARDWARE_DIVIDE_PRESENT 0
define CPU2_HARDWARE_MULTIPLY_PRESENT 0
define CPU2_HARDWARE_MULX_PRESENT 0
define CPU2_HAS_DEBUG_CORE 1
define CPU2_HAS_DEBUG_STUB
define CPU2_HAS_JMPI_INSTRUCTION
define CPU2_ICACHE_LINE_SIZE 32
define CPU2_ICACHE_LINE_SIZE_LOG2 5
define CPU2_ICACHE_SIZE 1024
define CPU2_INST_ADDR_WIDTH 0x1c
define CPU2_RESET_ADDR 0x00496000
###
###
###
ihwaddprocessor \
	-instancename cpu2 \
	-type nios_ii \
	-vendor mcgill.ca \
	-version 1.0 \
	-endian little \
	-simulateexceptions -mips 125.0
isetattribute -handle cpu2 -name variant                         -value Nios_II_S
isetattribute -handle cpu2 -name BREAK_VECTOR                    -value ${CPU2_BREAK_ADDR}
isetattribute -handle cpu2 -name EXCEPTION_VECTOR                -value ${CPU2_EXCEPTION_ADDR}
isetattribute -handle cpu2 -name RESET_VECTOR                    -value ${CPU2_RESET_ADDR}
isetattribute -handle cpu2 -name HW_DIVIDE                       -value ${CPU2_HARDWARE_DIVIDE_PRESENT}
isetattribute -handle cpu2 -name HW_MULTIPLY                     -value ${CPU2_HARDWARE_MULTIPLY_PRESENT}
isetattribute -handle cpu2 -name HW_MULX                         -value ${CPU2_HARDWARE_MULX_PRESENT}
isetattribute -handle cpu2 -name INCLUDE_MMU                     -value 0
isetattribute -handle cpu2 -name DATA_ADDR_WIDTH                 -value ${CPU2_DATA_ADDR_WIDTH}
isetattribute -handle cpu2 -name INST_ADDR_WIDTH                 -value ${CPU2_INST_ADDR_WIDTH}
isetattribute -handle cpu2 -name TEST_HALT_EXIT                  -value 1



ihwaddbus -instancename cpu2_iobus -addresswidth 32


#
# Individual I & D Bus
#
ihwaddbus -instancename cpu2_dbus  -addresswidth 32
ihwaddbus -instancename cpu2_ibus  -addresswidth 32

#
# Connect processor to bus
#
ihwconnect -bus cpu2_ibus -instancename cpu2 -busmasterport INSTRUCTION -loaddress 0 -hiaddress 0x0FFFFFFF
ihwconnect -bus cpu2_dbus -instancename cpu2 -busmasterport DATA        -loaddress 0 -hiaddress 0x0FFFFFFF


ihwaddbridge -instancename cpu2_out_bridge_d
ihwconnect   -instancename cpu2_out_bridge_d -busslaveport  sp1 -bus cpu2_dbus  -loaddress 0x03000000 -hiaddress 0x030003FF
ihwconnect   -instancename cpu2_out_bridge_d -busmasterport mp1 -bus smbus -loaddress 0x00000000 -hiaddress 0x000003FF

ihwaddbridge -instancename cpu2_mm_out_bridge_i
ihwconnect 	 -instancename cpu2_mm_out_bridge_i -busslaveport  sp1 -bus cpu2_ibus  -loaddress 0x00400000 -hiaddress 0x00527FFF
ihwconnect   -instancename cpu2_mm_out_bridge_i -busmasterport mp1 -bus mmbus      -loaddress 0x00000000 -hiaddress 0x00127FFF

ihwaddbridge -instancename cpu2_mm_out_bridge_d
ihwconnect 	 -instancename cpu2_mm_out_bridge_d -busslaveport  sp1 -bus cpu2_dbus  -loaddress 0x00400000 -hiaddress 0x00527FFF
ihwconnect   -instancename cpu2_mm_out_bridge_d -busmasterport mp1 -bus mmbus      -loaddress 0x00000000 -hiaddress 0x00127FFF



ihwaddbridge -instancename cpu2_mutex
ihwconnect 	 -instancename cpu2_mutex -busslaveport  sp1 -bus cpu2_dbus  -loaddress 0x04500000 -hiaddress 0x04500007
ihwconnect   -instancename cpu2_mutex -busmasterport mp1 -bus mutexbus   -loaddress 0x00000000 -hiaddress 0x00000007

ihwaddbridge -instancename cpu2_irq_bridge
ihwconnect 	 -instancename cpu2_irq_bridge -busslaveport  sp1 -bus cpu2_dbus   -loaddress 0x04000000 -hiaddress 0x040023FF
ihwconnect   -instancename cpu2_irq_bridge -busmasterport mp1 -bus cpuirqbus   -loaddress 0x00000000 -hiaddress 0x000023FF

ihwaddbridge -instancename cpu2_flash_bridge
ihwconnect 	 -instancename cpu2_flash_bridge -busslaveport  sp1 -bus cpu2_dbus   -loaddress 0x00000000 -hiaddress 0x003FFFFF
ihwconnect   -instancename cpu2_flash_bridge -busmasterport mp1 -bus flashbus    -loaddress 0x00000000 -hiaddress 0x003FFFFF


#
#
# Memory
#
#proc mkram { name bus lo hi }
mkram cpu2_breakmem     cpu2_ibus      0x08008020 0x08008100
mkram cpu2_scratchpad   cpu2_iobus     0x00200000 0x00203FFF


# IO bus bridgepb_cpu_to_io 0x0800_0000 0x087F_FFFF
#
ihwaddbridge -instancename cpu2_local_bridge
ihwconnect   -instancename cpu2_local_bridge -busslaveport  sp1 -bus cpu2_dbus  -loaddress 0x08000000 -hiaddress 0x087FFFFF
ihwconnect   -instancename cpu2_local_bridge -busmasterport mp1 -bus cpu2_iobus -loaddress 0x00000000 -hiaddress 0x007FFFFF 


# 0x4D50 - 0x4D57 jtag_uart 
ihwaddperipheral -instancename cpu2_jtag_uart -vendor altera.ovpworld.org -type JtagUart -diagnosticlevel 0
ihwconnect       -instancename cpu2_jtag_uart -bus cpu2_iobus -busslaveport sp1 -loaddress 0x0001040 -hiaddress 0x0001047
isetattribute    -handle       cpu2_jtag_uart -name writeIRQThreshold  -value 8
isetattribute    -handle       cpu2_jtag_uart -name readIRQThreshold   -value 8
isetattribute    -handle       cpu2_jtag_uart -name writeBufferDepth   -value 64
isetattribute    -handle       cpu2_jtag_uart -name readBufferDepth    -value 64
isetattribute    -handle       cpu2_jtag_uart -name console            -value 1
isetattribute    -handle       cpu2_jtag_uart -name finishOnDisconnect -value 1
ihwconnect       -instancename cpu2_jtag_uart -netport irq     -net cpu2_irq4
ihwconnect       -instancename cpu2           -netport d_irq4  -net cpu2_irq4

# 0x00400000 - 0x00400017 timer_10ms
ihwaddperipheral -instancename cpu2_timer_10ms -vendor mcgill.ca -type IntervalTimer32Core -diagnosticlevel 0
ihwconnect       -instancename cpu2_timer_10ms -bus cpu2_iobus -busslaveport sp1 -loaddress 0x00001020 -hiaddress 0x00001037
isetattribute    -handle       cpu2_timer_10ms -name timerFrequency  -value 50000000
isetattribute    -handle       cpu2_timer_10ms -name timeoutPeriod   -value 500000 
ihwconnect       -instancename cpu2_timer_10ms -netport irq     -net cpu2_irq0
ihwconnect       -instancename cpu2            -netport d_irq0 -net cpu2_irq0

# 0x00400000 - 0x00400017 timestamp
ihwaddperipheral -instancename cpu2_timestamp -vendor mcgill.ca -type IntervalTimer32Core -diagnosticlevel 0
ihwconnect       -instancename cpu2_timestamp -bus cpu2_iobus -busslaveport sp1 -loaddress 0x00000000 -hiaddress 0x00000017
isetattribute    -handle       cpu2_timestamp -name timerFrequency  -value 50000000
isetattribute    -handle       cpu2_timestamp -name timeoutPeriod   -value 500000

#INTER CPU irq
ihwconnect       -instancename cpu_irq         -netport cpu2_irq     -net cpu2_irq1
ihwconnect       -instancename cpu2            -netport d_irq1       -net cpu2_irq1

#
# FPRINT
#
ihwaddperipheral -instancename cpu2_fprint -vendor mcgill.ca -type fprint -version 1.0 -diagnosticlevel 0
ihwconnect       -instancename cpu2_fprint    -bus cpu2_iobus -busslaveport FPRINT_CSR_SLAVE -loaddress 0x00100000 -hiaddress 0x0010013F
isetattribute    -handle       cpu2_fprint -name coreID  -value 2


ihwconnect       -instancename cpu2_fprint    -netport FPRINT_WRITE              -net fprint_write_2
ihwconnect       -instancename cpu2           -netport fprint_write_out          -net fprint_write_2

ihwconnect       -instancename cpu2_fprint    -netport FPRINT_WRITE_ADDRESS      -net fprint_write_address_2
ihwconnect       -instancename cpu2           -netport fprint_write_out_address  -net fprint_write_address_2

ihwconnect       -instancename cpu2_fprint    -netport FPRINT_WRITE_DATA         -net fprint_write_data_2
ihwconnect       -instancename cpu2           -netport fprint_write_out_data     -net fprint_write_data_2



###############################################################################
# Processor 3
###############################################################################
### grep define system.h | sed 's/#//' > system.tcl
### source system.tcl
###
define CPU3_BIG_ENDIAN 0
define CPU3_BREAK_ADDR 0x08000820
define CPU3_CPU_FREQ 50000000u
define CPU3_CPU_ID_SIZE 2
define CPU3_CPU_ID_VALUE 0x00000004
define CPU3_CPU_IMPLEMENTATION "small"
define CPU3_DATA_ADDR_WIDTH 0x1c
define CPU3_DCACHE_LINE_SIZE 0
define CPU3_DCACHE_LINE_SIZE_LOG2 0
define CPU3_DCACHE_SIZE 0
define CPU3_EXCEPTION_ADDR 0x004c8020
define CPU3_FLUSHDA_SUPPORTED
define CPU3_HARDWARE_DIVIDE_PRESENT 0
define CPU3_HARDWARE_MULTIPLY_PRESENT 0
define CPU3_HARDWARE_MULX_PRESENT 0
define CPU3_HAS_DEBUG_CORE 1
define CPU3_HAS_DEBUG_STUB
define CPU3_HAS_JMPI_INSTRUCTION
define CPU3_ICACHE_LINE_SIZE 32
define CPU3_ICACHE_LINE_SIZE_LOG2 5
define CPU3_ICACHE_SIZE 1024
define CPU3_INST_ADDR_WIDTH 0x1c
define CPU3_RESET_ADDR 0x004c8000

###
###
###
ihwaddprocessor \
	-instancename cpu3 \
	-type nios_ii \
	-vendor altera.ovpworld.org \
	-version 1.0 \
	-endian little \
	-simulateexceptions -mips 125.0
isetattribute -handle cpu3 -name variant                         -value Nios_II_S
isetattribute -handle cpu3 -name BREAK_VECTOR                    -value ${CPU3_BREAK_ADDR}
isetattribute -handle cpu3 -name EXCEPTION_VECTOR                -value ${CPU3_EXCEPTION_ADDR}
isetattribute -handle cpu3 -name RESET_VECTOR                    -value ${CPU3_RESET_ADDR}
isetattribute -handle cpu3 -name HW_DIVIDE                       -value ${CPU3_HARDWARE_DIVIDE_PRESENT}
isetattribute -handle cpu3 -name HW_MULTIPLY                     -value ${CPU3_HARDWARE_MULTIPLY_PRESENT}
isetattribute -handle cpu3 -name HW_MULX                         -value ${CPU3_HARDWARE_MULX_PRESENT}
isetattribute -handle cpu3 -name INCLUDE_MMU                     -value 0
isetattribute -handle cpu3 -name DATA_ADDR_WIDTH                 -value ${CPU3_DATA_ADDR_WIDTH}
isetattribute -handle cpu3 -name INST_ADDR_WIDTH                 -value ${CPU3_INST_ADDR_WIDTH}
isetattribute -handle cpu3 -name TEST_HALT_EXIT                  -value 1



ihwaddbus -instancename cpu3_iobus -addresswidth 32


#
# Individual I & D Bus
#
ihwaddbus -instancename cpu3_dbus  -addresswidth 32
ihwaddbus -instancename cpu3_ibus  -addresswidth 32

#
# Connect processor to bus
#
ihwconnect -bus cpu3_ibus -instancename cpu3 -busmasterport INSTRUCTION -loaddress 0 -hiaddress 0x0FFFFFFF
ihwconnect -bus cpu3_dbus -instancename cpu3 -busmasterport DATA        -loaddress 0 -hiaddress 0x0FFFFFFF


ihwaddbridge -instancename cpu3_out_bridge_d
ihwconnect   -instancename cpu3_out_bridge_d -busslaveport  sp1 -bus cpu3_dbus  -loaddress 0x03000000 -hiaddress 0x030003FF
ihwconnect   -instancename cpu3_out_bridge_d -busmasterport mp1 -bus smbus -loaddress 0x00000000 -hiaddress 0x000003FF

ihwaddbridge -instancename cpu3_mm_out_bridge_i
ihwconnect 	 -instancename cpu3_mm_out_bridge_i -busslaveport  sp1 -bus cpu3_ibus  -loaddress 0x00400000 -hiaddress 0x00527FFF
ihwconnect   -instancename cpu3_mm_out_bridge_i -busmasterport mp1 -bus mmbus      -loaddress 0x00000000 -hiaddress 0x00127FFF

ihwaddbridge -instancename cpu3_mm_out_bridge_d
ihwconnect 	 -instancename cpu3_mm_out_bridge_d -busslaveport  sp1 -bus cpu3_dbus  -loaddress 0x00400000 -hiaddress 0x00527FFF
ihwconnect   -instancename cpu3_mm_out_bridge_d -busmasterport mp1 -bus mmbus      -loaddress 0x00000000 -hiaddress 0x00127FFF



ihwaddbridge -instancename cpu3_mutex
ihwconnect 	 -instancename cpu3_mutex -busslaveport  sp1 -bus cpu3_dbus  -loaddress 0x04500000 -hiaddress 0x04500007
ihwconnect   -instancename cpu3_mutex -busmasterport mp1 -bus mutexbus   -loaddress 0x00000000 -hiaddress 0x00000007

ihwaddbridge -instancename cpu3_irq_bridge
ihwconnect 	 -instancename cpu3_irq_bridge -busslaveport  sp1 -bus cpu3_dbus   -loaddress 0x04000000 -hiaddress 0x040023FF
ihwconnect   -instancename cpu3_irq_bridge -busmasterport mp1 -bus cpuirqbus   -loaddress 0x00000000 -hiaddress 0x000023FF

ihwaddbridge -instancename cpu3_flash_bridge
ihwconnect 	 -instancename cpu3_flash_bridge -busslaveport  sp1 -bus cpu3_dbus   -loaddress 0x00000000 -hiaddress 0x003FFFFF
ihwconnect   -instancename cpu3_flash_bridge -busmasterport mp1 -bus flashbus    -loaddress 0x00000000 -hiaddress 0x003FFFFF


#
#
# Memory
#
#proc mkram { name bus lo hi }
mkram cpu3_breakmem     cpu3_ibus      0x08008020 0x08008100
mkram cpu3_scratchpad   cpu3_iobus     0x00200000 0x00203FFF


# IO bus bridgepb_cpu_to_io 0x0800_0000 0x087F_FFFF
#
ihwaddbridge -instancename cpu3_local_bridge
ihwconnect   -instancename cpu3_local_bridge -busslaveport  sp1 -bus cpu3_dbus  -loaddress 0x08000000 -hiaddress 0x087FFFFF
ihwconnect   -instancename cpu3_local_bridge -busmasterport mp1 -bus cpu3_iobus -loaddress 0x00000000 -hiaddress 0x007FFFFF 


# 0x4D50 - 0x4D57 jtag_uart 
ihwaddperipheral -instancename cpu3_jtag_uart -vendor altera.ovpworld.org -type JtagUart -diagnosticlevel 0
ihwconnect       -instancename cpu3_jtag_uart -bus cpu3_iobus -busslaveport sp1 -loaddress 0x0001040 -hiaddress 0x0001047
isetattribute    -handle       cpu3_jtag_uart -name writeIRQThreshold  -value 8
isetattribute    -handle       cpu3_jtag_uart -name readIRQThreshold   -value 8
isetattribute    -handle       cpu3_jtag_uart -name writeBufferDepth   -value 64
isetattribute    -handle       cpu3_jtag_uart -name readBufferDepth    -value 64
isetattribute    -handle       cpu3_jtag_uart -name console            -value 1
isetattribute    -handle       cpu3_jtag_uart -name finishOnDisconnect -value 1
ihwconnect       -instancename cpu3_jtag_uart -netport irq     -net cpu3_irq4
ihwconnect       -instancename cpu3           -netport d_irq4  -net cpu3_irq4

# 0x00400000 - 0x00400017 timer_10ms
ihwaddperipheral -instancename cpu3_timer_10ms -vendor mcgill.ca -type IntervalTimer32Core -diagnosticlevel 0
ihwconnect       -instancename cpu3_timer_10ms -bus cpu3_iobus -busslaveport sp1 -loaddress 0x00001020 -hiaddress 0x00001037
isetattribute    -handle       cpu3_timer_10ms -name timerFrequency  -value 50000000
isetattribute    -handle       cpu3_timer_10ms -name timeoutPeriod   -value 500000 
ihwconnect       -instancename cpu3_timer_10ms -netport irq     -net cpu3_irq0
ihwconnect       -instancename cpu3            -netport d_irq0 -net cpu3_irq0

# 0x00400000 - 0x00400017 timestamp
ihwaddperipheral -instancename cpu3_timestamp -vendor mcgill.ca -type IntervalTimer32Core -diagnosticlevel 0
ihwconnect       -instancename cpu3_timestamp -bus cpu3_iobus -busslaveport sp1 -loaddress 0x00000000 -hiaddress 0x00000017
isetattribute    -handle       cpu3_timestamp -name timerFrequency  -value 50000000
isetattribute    -handle       cpu3_timestamp -name timeoutPeriod   -value 500000

#INTER CPU irq
ihwconnect       -instancename cpu_irq         -netport cpu3_irq     -net cpu3_irq1
ihwconnect       -instancename cpu3            -netport d_irq1       -net cpu3_irq1

#
# FPRINT
#
ihwaddperipheral -instancename cpu3_fprint -vendor mcgill.ca -version 1.0 -type fprint -diagnosticlevel 0
ihwconnect       -instancename cpu3_fprint    -bus cpu3_iobus -busslaveport FPRINT_CSR_SLAVE -loaddress 0x00100000 -hiaddress 0x0010013F
isetattribute    -handle       cpu3_fprint -name coreID  -value 3


ihwconnect       -instancename cpu3_fprint    -netport FPRINT_WRITE              -net fprint_write_3
ihwconnect       -instancename cpu3           -netport fprint_write_out          -net fprint_write_3

ihwconnect       -instancename cpu3_fprint    -netport FPRINT_WRITE_ADDRESS      -net fprint_write_address_3
ihwconnect       -instancename cpu3           -netport fprint_write_out_address  -net fprint_write_address_3

ihwconnect       -instancename cpu3_fprint    -netport FPRINT_WRITE_DATA         -net fprint_write_data_3
ihwconnect       -instancename cpu3           -netport fprint_write_out_data     -net fprint_write_data_3


###############################################################################
# Processor Monitor
###############################################################################
### grep define system.h | sed 's/#//' > system.tcl
### source system.tcl
###
define CPUM_BIG_ENDIAN 0
define CPUM_BREAK_ADDR 0x08000820
define CPUM_CPU_FREQ 50000000u
define CPUM_CPU_ID_SIZE 4
define CPUM_CPU_ID_VALUE 0x00000009
define CPUM_CPU_IMPLEMENTATION "small"
define CPUM_DATA_ADDR_WIDTH 0x1c
define CPUM_DCACHE_LINE_SIZE 0
define CPUM_DCACHE_LINE_SIZE_LOG2 0
define CPUM_DCACHE_SIZE 0
define CPUM_EXCEPTION_ADDR 0x00400020
define CPUM_FLUSHDA_SUPPORTED
define CPUM_HARDWARE_DIVIDE_PRESENT 0
define CPUM_HARDWARE_MULTIPLY_PRESENT 0
define CPUM_HARDWARE_MULX_PRESENT 0
define CPUM_HAS_DEBUG_CORE 1
define CPUM_HAS_DEBUG_STUB
define CPUM_HAS_JMPI_INSTRUCTION
define CPUM_ICACHE_LINE_SIZE 32
define CPUM_ICACHE_LINE_SIZE_LOG2 5
define CPUM_ICACHE_SIZE 1024
define CPUM_INST_ADDR_WIDTH 0x1c
define CPUM_RESET_ADDR 0x00400000


###
###
###
ihwaddprocessor \
	-instancename cpum \
	-type nios_ii \
	-vendor altera.ovpworld.org \
	-version 1.0 \
	-endian little \
	-simulateexceptions -mips 125.0
isetattribute -handle cpum -name variant                         -value Nios_II_S
isetattribute -handle cpum -name BREAK_VECTOR                    -value ${CPUM_BREAK_ADDR}
isetattribute -handle cpum -name EXCEPTION_VECTOR                -value ${CPUM_EXCEPTION_ADDR}
isetattribute -handle cpum -name RESET_VECTOR                    -value ${CPUM_RESET_ADDR}
isetattribute -handle cpum -name HW_DIVIDE                       -value ${CPUM_HARDWARE_DIVIDE_PRESENT}
isetattribute -handle cpum -name HW_MULTIPLY                     -value ${CPUM_HARDWARE_MULTIPLY_PRESENT}
isetattribute -handle cpum -name HW_MULX                         -value ${CPUM_HARDWARE_MULX_PRESENT}
isetattribute -handle cpum -name INCLUDE_MMU                     -value 0
isetattribute -handle cpum -name DATA_ADDR_WIDTH                 -value ${CPUM_DATA_ADDR_WIDTH}
isetattribute -handle cpum -name INST_ADDR_WIDTH                 -value ${CPUM_INST_ADDR_WIDTH}
isetattribute -handle cpum -name TEST_HALT_EXIT                  -value 1



ihwaddbus -instancename cpum_iobus -addresswidth 32


#
# Individual I & D Bus
#
ihwaddbus -instancename cpum_dbus  -addresswidth 32
ihwaddbus -instancename cpum_ibus  -addresswidth 32

#
# Connect processor to bus
#
ihwconnect -bus cpum_ibus -instancename cpum -busmasterport INSTRUCTION -loaddress 0 -hiaddress 0x0FFFFFFF
ihwconnect -bus cpum_dbus -instancename cpum -busmasterport DATA        -loaddress 0 -hiaddress 0x0FFFFFFF


ihwaddbridge -instancename cpum_out_bridge_d
ihwconnect   -instancename cpum_out_bridge_d -busslaveport  sp1 -bus cpum_dbus  -loaddress 0x03000000 -hiaddress 0x030003FF
ihwconnect   -instancename cpum_out_bridge_d -busmasterport mp1 -bus smbus -loaddress 0x00000000 -hiaddress 0x000003FF


ihwaddbridge -instancename cpum_mm_out_bridge_i
ihwconnect 	 -instancename cpum_mm_out_bridge_i -busslaveport  sp1 -bus cpum_ibus  -loaddress 0x00400000 -hiaddress 0x00527FFF
ihwconnect   -instancename cpum_mm_out_bridge_i -busmasterport mp1 -bus mmbus      -loaddress 0x00000000 -hiaddress 0x00127FFF

ihwaddbridge -instancename cpum_mm_out_bridge_d
ihwconnect 	 -instancename cpum_mm_out_bridge_d -busslaveport  sp1 -bus cpum_dbus  -loaddress 0x00400000 -hiaddress 0x00527FFF
ihwconnect   -instancename cpum_mm_out_bridge_d -busmasterport mp1 -bus mmbus      -loaddress 0x00000000 -hiaddress 0x00127FFF

ihwaddbridge -instancename cpum_mutex
ihwconnect 	 -instancename cpum_mutex -busslaveport  sp1 -bus cpum_dbus  -loaddress 0x04500000 -hiaddress 0x04500007
ihwconnect   -instancename cpum_mutex -busmasterport mp1 -bus mutexbus   -loaddress 0x00000000 -hiaddress 0x00000007

ihwaddbridge -instancename cpum_irq_bridge
ihwconnect 	 -instancename cpum_irq_bridge -busslaveport  sp1 -bus cpum_dbus   -loaddress 0x04000000 -hiaddress 0x040023FF
ihwconnect   -instancename cpum_irq_bridge -busmasterport mp1 -bus cpuirqbus   -loaddress 0x00000000 -hiaddress 0x000023FF

ihwaddbridge -instancename cpum_flash_bridge
ihwconnect 	 -instancename cpum_flash_bridge -busslaveport  sp1 -bus cpum_dbus   -loaddress 0x00000000 -hiaddress 0x003FFFFF
ihwconnect   -instancename cpum_flash_bridge -busmasterport mp1 -bus flashbus    -loaddress 0x00000000 -hiaddress 0x003FFFFF

#
# Memory
#
#proc mkram { name bus lo hi } 
mkram cpum_breakmem     cpum_ibus       0x08008820 0x08008900
mkram cpum_scratchpad   cpum_iobus      0x00200000 0x00203FFF

# IO bus bridgepb_cpu_to_io 0x0800_0000 0x087F_FFFF
#
ihwaddbridge -instancename cpum_local_bridge
ihwconnect   -instancename cpum_local_bridge -busslaveport  sp1 -bus cpum_dbus  -loaddress 0x08000000 -hiaddress 0x087FFFFF
ihwconnect   -instancename cpum_local_bridge -busmasterport mp1 -bus cpum_iobus -loaddress 0x00000000 -hiaddress 0x007FFFFF 




# 0x4D50 - 0x4D57 jtag_uart 
ihwaddperipheral -instancename cpum_jtag_uart -vendor altera.ovpworld.org -type JtagUart -diagnosticlevel 0
ihwconnect       -instancename cpum_jtag_uart -bus cpum_iobus -busslaveport sp1 -loaddress 0x0001040 -hiaddress 0x0001047
isetattribute    -handle       cpum_jtag_uart -name writeIRQThreshold  -value 8
isetattribute    -handle       cpum_jtag_uart -name readIRQThreshold   -value 8
isetattribute    -handle       cpum_jtag_uart -name writeBufferDepth   -value 64
isetattribute    -handle       cpum_jtag_uart -name readBufferDepth    -value 64
isetattribute    -handle       cpum_jtag_uart -name console            -value 1
isetattribute    -handle       cpum_jtag_uart -name finishOnDisconnect -value 1
ihwconnect       -instancename cpum_jtag_uart -netport irq     -net cpum_irq4
ihwconnect       -instancename cpum           -netport d_irq4  -net cpum_irq4

# 0x00400000 - 0x00400017 timer_10ms
ihwaddperipheral -instancename cpum_timer_10ms -vendor mcgill.ca -type IntervalTimer32Core -diagnosticlevel 0
ihwconnect       -instancename cpum_timer_10ms -bus cpum_iobus -busslaveport sp1 -loaddress 0x00001020 -hiaddress 0x00001037
isetattribute    -handle       cpum_timer_10ms -name timerFrequency  -value 50000000
isetattribute    -handle       cpum_timer_10ms -name timeoutPeriod   -value 500000 
ihwconnect       -instancename cpum_timer_10ms -netport irq     -net cpum_irq0
ihwconnect       -instancename cpum            -netport d_irq0 -net cpum_irq0


# 0x00400000 - 0x00400017 timestamp
ihwaddperipheral -instancename cpum_timestamp -vendor mcgill.ca -type IntervalTimer32Core -diagnosticlevel 0
ihwconnect       -instancename cpum_timestamp -bus cpum_iobus -busslaveport sp1 -loaddress 0x00000000 -hiaddress 0x00000017
isetattribute    -handle       cpum_timestamp -name timerFrequency  -value 50000000
isetattribute    -handle       cpum_timestamp -name timeoutPeriod   -value 500000

#INTER CPU irq
ihwconnect       -instancename cpu_irq         -netport cpum_irq     -net cpum_irq1
ihwconnect       -instancename cpum            -netport d_irq1       -net cpum_irq1



###############################################################################
# DMA
###############################################################################

###############
# DMA 0
###############
ihwaddbus -instancename cpu0_dmabus  -addresswidth 32
ihwaddperipheral -instancename cpu0_dma -vendor mcgill.ca -type dma -diagnosticlevel 3
ihwconnect       -instancename cpu0_dma -bus cpu0_dmabus -busmasterport MWRITE -loaddress 0 -hiaddress 0XFFFFFFFF
ihwconnect 		 -instancename cpu0_dma -bus cpu0_dmabus -busmasterport MREAD  -loaddress 0 -hiaddress 0XFFFFFFFF
ihwconnect       -instancename cpu0_dma -bus cpum_dbus   -busslaveport  DMACSP -loaddress 0x5100000 -hiaddress 0x510001F

ihwaddbridge -instancename cpu0_dmasp_bridge
ihwconnect 	 -instancename cpu0_dmasp_bridge -busslaveport  sp1 -bus cpu0_dmabus   -loaddress 0x08000000 -hiaddress 0x087FFFFF
ihwconnect   -instancename cpu0_dmasp_bridge -busmasterport mp1 -bus cpu0_iobus    -loaddress 0x00000000 -hiaddress 0x007FFFFF

ihwaddbridge -instancename cpu0_dmamm_bridge
ihwconnect 	 -instancename cpu0_dmamm_bridge -busslaveport  sp1 -bus cpu0_dmabus   -loaddress 0x00400000 -hiaddress 0x004F9FFF
ihwconnect   -instancename cpu0_dmamm_bridge -busmasterport mp1 -bus mmbus    	   -loaddress 0x00000000 -hiaddress 0x000F9FFF

ihwconnect       -instancename cpu0_dma        -netport DMA_IRQ      -net cpum_irq5
ihwconnect       -instancename cpum            -netport d_irq5       -net cpum_irq5


###############
# DMA 1
###############
ihwaddbus -instancename cpu1_dmabus  -addresswidth 32
ihwaddperipheral -instancename cpu1_dma -vendor mcgill.ca -type dma -diagnosticlevel 3
ihwconnect       -instancename cpu1_dma -bus cpu1_dmabus -busmasterport MWRITE -loaddress 0 -hiaddress 0XFFFFFFFF
ihwconnect 		 -instancename cpu1_dma -bus cpu1_dmabus -busmasterport MREAD  -loaddress 0 -hiaddress 0XFFFFFFFF
ihwconnect       -instancename cpu1_dma -bus cpum_dbus   -busslaveport  DMACSP -loaddress 0x5110000 -hiaddress 0x511001F

ihwaddbridge -instancename cpu1_dmasp_bridge
ihwconnect 	 -instancename cpu1_dmasp_bridge -busslaveport  sp1 -bus cpu1_dmabus   -loaddress 0x08000000 -hiaddress 0x087FFFFF
ihwconnect   -instancename cpu1_dmasp_bridge -busmasterport mp1 -bus cpu1_iobus    -loaddress 0x00000000 -hiaddress 0x007FFFFF

ihwaddbridge -instancename cpu1_dmamm_bridge
ihwconnect 	 -instancename cpu1_dmamm_bridge -busslaveport  sp1 -bus cpu1_dmabus   -loaddress 0x00400000 -hiaddress 0x004F9FFF
ihwconnect   -instancename cpu1_dmamm_bridge -busmasterport mp1 -bus mmbus         -loaddress 0x00000000 -hiaddress 0x000F9FFF

ihwconnect       -instancename cpu1_dma        -netport DMA_IRQ      -net cpum_irq6
ihwconnect       -instancename cpum            -netport d_irq6       -net cpum_irq6

###############
# DMA 2
###############
ihwaddbus -instancename cpu2_dmabus  -addresswidth 32
ihwaddperipheral -instancename cpu2_dma -vendor mcgill.ca -type dma -diagnosticlevel 3
ihwconnect       -instancename cpu2_dma -bus cpu2_dmabus -busmasterport MWRITE -loaddress 0 -hiaddress 0XFFFFFFFF
ihwconnect 		 -instancename cpu2_dma -bus cpu2_dmabus -busmasterport MREAD  -loaddress 0 -hiaddress 0XFFFFFFFF
ihwconnect       -instancename cpu2_dma -bus cpum_dbus   -busslaveport  DMACSP -loaddress 0x5120000 -hiaddress 0x512001F

ihwaddbridge -instancename cpu2_dmasp_bridge
ihwconnect 	 -instancename cpu2_dmasp_bridge -busslaveport  sp1 -bus cpu2_dmabus   -loaddress 0x08000000 -hiaddress 0x087FFFFF
ihwconnect   -instancename cpu2_dmasp_bridge -busmasterport mp1 -bus cpu2_iobus    -loaddress 0x00000000 -hiaddress 0x007FFFFF

ihwaddbridge -instancename cpu2_dmamm_bridge
ihwconnect 	 -instancename cpu2_dmamm_bridge -busslaveport  sp1 -bus cpu2_dmabus   -loaddress 0x00400000 -hiaddress 0x004F9FFF
ihwconnect   -instancename cpu2_dmamm_bridge -busmasterport mp1 -bus mmbus         -loaddress 0x00000000 -hiaddress 0x000F9FFF

ihwconnect       -instancename cpu2_dma        -netport DMA_IRQ      -net cpum_irq7
ihwconnect       -instancename cpum            -netport d_irq7       -net cpum_irq7

###############
# DMA 3
###############
ihwaddbus -instancename cpu3_dmabus  -addresswidth 32
ihwaddperipheral -instancename cpu3_dma -vendor mcgill.ca -type dma -diagnosticlevel 3
ihwconnect       -instancename cpu3_dma -bus cpu3_dmabus -busmasterport MWRITE -loaddress 0 -hiaddress 0XFFFFFFFF
ihwconnect 		 -instancename cpu3_dma -bus cpu3_dmabus -busmasterport MREAD  -loaddress 0 -hiaddress 0XFFFFFFFF
ihwconnect       -instancename cpu3_dma -bus cpum_dbus   -busslaveport  DMACSP -loaddress 0x5130000 -hiaddress 0x513001F

ihwaddbridge -instancename cpu3_dmasp_bridge
ihwconnect 	 -instancename cpu3_dmasp_bridge -busslaveport  sp1 -bus cpu3_dmabus   -loaddress 0x08000000 -hiaddress 0x087FFFFF
ihwconnect   -instancename cpu3_dmasp_bridge -busmasterport mp1 -bus cpu3_iobus    -loaddress 0x00000000 -hiaddress 0x007FFFFF

ihwaddbridge -instancename cpu3_dmamm_bridge
ihwconnect 	 -instancename cpu3_dmamm_bridge -busslaveport  sp1 -bus cpu3_dmabus   -loaddress 0x00400000 -hiaddress 0x004F9FFF
ihwconnect   -instancename cpu3_dmamm_bridge -busmasterport mp1 -bus mmbus         -loaddress 0x00000000 -hiaddress 0x000F9FFF

ihwconnect       -instancename cpu3_dma        -netport DMA_IRQ      -net cpum_irq8
ihwconnect       -instancename cpum            -netport d_irq8       -net cpum_irq8

###############################################################################
# Comparator
###############################################################################

ihwaddbus -instancename compbus            -addresswidth 32

ihwaddperipheral -instancename comparator  -vendor mcgill.ca -type comparator -diagnosticlevel 3
ihwconnect       -instancename comparator  -bus compbus   -busslaveport COMP_FPRINT_SLAVE -loaddress 0x01000000 -hiaddress 0x01000FFF
ihwconnect       -instancename comparator  -bus cpum_dbus -busslaveport COMP_CSR_SLAVE -loaddress 0x5000000 -hiaddress 0x5000FFF
ihwconnect       -instancename cpu0_fprint -bus compbus -busmasterport FPRINT_MASTER -loaddress 0x00000000 -hiaddress 0x0FFFFFFF 
ihwconnect       -instancename cpu1_fprint -bus compbus -busmasterport FPRINT_MASTER -loaddress 0x00000000 -hiaddress 0x0FFFFFFF 
ihwconnect       -instancename cpu2_fprint -bus compbus -busmasterport FPRINT_MASTER -loaddress 0x00000000 -hiaddress 0x0FFFFFFF 
ihwconnect       -instancename cpu3_fprint -bus compbus -busmasterport FPRINT_MASTER -loaddress 0x00000000 -hiaddress 0x0FFFFFFF 

#INTER CPU irq
ihwconnect       -instancename comparator      -netport COMP_IRQ     -net cpum_irq3
ihwconnect       -instancename cpum            -netport d_irq3       -net cpum_irq3




define elf0 ../../fprint_fpga/quartus/full_system_arria_4/software/ucos0/ucos0.elf
ihwaddimagefile -handle cpu0 -filename ${elf0} -loadphysical false -useobjectentry true -name "fprint0"

define elf1 ../../fprint_fpga/quartus/full_system_arria_4/software/ucos1/ucos1.elf
ihwaddimagefile -handle cpu1 -filename ${elf1} -loadphysical false -useobjectentry true -name "fprint1"

define elf2 ../../fprint_fpga/quartus/full_system_arria_4/software/ucos2/ucos2.elf
ihwaddimagefile -handle cpu2 -filename ${elf2} -loadphysical false -useobjectentry true -name "fprint2"

define elf3 ../../fprint_fpga/quartus/full_system_arria_4/software/ucos3/ucos3.elf
ihwaddimagefile -handle cpu3 -filename ${elf3} -loadphysical false -useobjectentry true -name "fprint3"

define elfm ../../fprint_fpga/quartus/full_system_arria_4/software/ucosMon/ucosMon.elf
ihwaddimagefile -handle cpum -filename ${elfm} -loadphysical false -useobjectentry true -name "Monitor"
