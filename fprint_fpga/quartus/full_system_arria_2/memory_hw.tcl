package require -exact qsys 13.1

# module properties
set_module_property NAME {memory_export}
set_module_property DISPLAY_NAME {memory_export_display}

# default module properties
set_module_property VERSION {1.0}
set_module_property GROUP {default group}
set_module_property DESCRIPTION {default description}
set_module_property AUTHOR {author}

set_module_property COMPOSITION_CALLBACK compose
set_module_property opaque_address_map false

proc compose { } {
    # Instances and instance parameters
    # (disabled instances are intentionally culled)
    add_instance clk_0 clock_source 13.1
    set_instance_parameter_value clk_0 {clockFrequency} {50000000.0}
    set_instance_parameter_value clk_0 {clockFrequencyKnown} {1}
    set_instance_parameter_value clk_0 {resetSynchronousEdges} {NONE}

    add_instance onchip_memoryMain altera_avalon_onchip_memory2 13.1
    set_instance_parameter_value onchip_memoryMain {allowInSystemMemoryContentEditor} {0}
    set_instance_parameter_value onchip_memoryMain {blockType} {AUTO}
    set_instance_parameter_value onchip_memoryMain {dataWidth} {32}
    set_instance_parameter_value onchip_memoryMain {dualPort} {0}
    set_instance_parameter_value onchip_memoryMain {initMemContent} {1}
    set_instance_parameter_value onchip_memoryMain {initializationFileName} {onchip_mem.hex}
    set_instance_parameter_value onchip_memoryMain {instanceID} {NONE}
    set_instance_parameter_value onchip_memoryMain {memorySize} {1024000.0}
    set_instance_parameter_value onchip_memoryMain {readDuringWriteMode} {DONT_CARE}
    set_instance_parameter_value onchip_memoryMain {simAllowMRAMContentsFile} {0}
    set_instance_parameter_value onchip_memoryMain {simMemInitOnlyFilename} {0}
    set_instance_parameter_value onchip_memoryMain {singleClockOperation} {0}
    set_instance_parameter_value onchip_memoryMain {slave1Latency} {1}
    set_instance_parameter_value onchip_memoryMain {slave2Latency} {1}
    set_instance_parameter_value onchip_memoryMain {useNonDefaultInitFile} {0}
    set_instance_parameter_value onchip_memoryMain {useShallowMemBlocks} {0}
    set_instance_parameter_value onchip_memoryMain {writable} {1}
    set_instance_parameter_value onchip_memoryMain {ecc_enabled} {0}

    add_instance tristate_conduit_bridge_0 altera_tristate_conduit_bridge 13.1

    add_instance generic_tristate_controller_0 altera_generic_tristate_controller 13.1
    set_instance_parameter_value generic_tristate_controller_0 {TCM_ADDRESS_W} {22}
    set_instance_parameter_value generic_tristate_controller_0 {TCM_DATA_W} {16}
    set_instance_parameter_value generic_tristate_controller_0 {TCM_BYTEENABLE_W} {2}
    set_instance_parameter_value generic_tristate_controller_0 {TCM_READ_WAIT} {100}
    set_instance_parameter_value generic_tristate_controller_0 {TCM_WRITE_WAIT} {100}
    set_instance_parameter_value generic_tristate_controller_0 {TCM_SETUP_WAIT} {25}
    set_instance_parameter_value generic_tristate_controller_0 {TCM_DATA_HOLD} {20}
    set_instance_parameter_value generic_tristate_controller_0 {TCM_MAX_PENDING_READ_TRANSACTIONS} {3}
    set_instance_parameter_value generic_tristate_controller_0 {TCM_TURNAROUND_TIME} {2}
    set_instance_parameter_value generic_tristate_controller_0 {TCM_TIMING_UNITS} {0}
    set_instance_parameter_value generic_tristate_controller_0 {TCM_READLATENCY} {2}
    set_instance_parameter_value generic_tristate_controller_0 {TCM_SYMBOLS_PER_WORD} {2}
    set_instance_parameter_value generic_tristate_controller_0 {USE_READDATA} {1}
    set_instance_parameter_value generic_tristate_controller_0 {USE_WRITEDATA} {1}
    set_instance_parameter_value generic_tristate_controller_0 {USE_READ} {1}
    set_instance_parameter_value generic_tristate_controller_0 {USE_WRITE} {1}
    set_instance_parameter_value generic_tristate_controller_0 {USE_BEGINTRANSFER} {0}
    set_instance_parameter_value generic_tristate_controller_0 {USE_BYTEENABLE} {0}
    set_instance_parameter_value generic_tristate_controller_0 {USE_CHIPSELECT} {1}
    set_instance_parameter_value generic_tristate_controller_0 {USE_LOCK} {0}
    set_instance_parameter_value generic_tristate_controller_0 {USE_ADDRESS} {1}
    set_instance_parameter_value generic_tristate_controller_0 {USE_WAITREQUEST} {0}
    set_instance_parameter_value generic_tristate_controller_0 {USE_WRITEBYTEENABLE} {0}
    set_instance_parameter_value generic_tristate_controller_0 {USE_OUTPUTENABLE} {0}
    set_instance_parameter_value generic_tristate_controller_0 {USE_RESETREQUEST} {0}
    set_instance_parameter_value generic_tristate_controller_0 {USE_IRQ} {0}
    set_instance_parameter_value generic_tristate_controller_0 {USE_RESET_OUTPUT} {0}
    set_instance_parameter_value generic_tristate_controller_0 {ACTIVE_LOW_READ} {1}
    set_instance_parameter_value generic_tristate_controller_0 {ACTIVE_LOW_LOCK} {0}
    set_instance_parameter_value generic_tristate_controller_0 {ACTIVE_LOW_WRITE} {1}
    set_instance_parameter_value generic_tristate_controller_0 {ACTIVE_LOW_CHIPSELECT} {1}
    set_instance_parameter_value generic_tristate_controller_0 {ACTIVE_LOW_BYTEENABLE} {0}
    set_instance_parameter_value generic_tristate_controller_0 {ACTIVE_LOW_OUTPUTENABLE} {0}
    set_instance_parameter_value generic_tristate_controller_0 {ACTIVE_LOW_WRITEBYTEENABLE} {0}
    set_instance_parameter_value generic_tristate_controller_0 {ACTIVE_LOW_WAITREQUEST} {0}
    set_instance_parameter_value generic_tristate_controller_0 {ACTIVE_LOW_BEGINTRANSFER} {0}
    set_instance_parameter_value generic_tristate_controller_0 {ACTIVE_LOW_RESETREQUEST} {0}
    set_instance_parameter_value generic_tristate_controller_0 {ACTIVE_LOW_IRQ} {0}
    set_instance_parameter_value generic_tristate_controller_0 {ACTIVE_LOW_RESET_OUTPUT} {0}
    set_instance_parameter_value generic_tristate_controller_0 {CHIPSELECT_THROUGH_READLATENCY} {0}
    set_instance_parameter_value generic_tristate_controller_0 {IS_MEMORY_DEVICE} {1}
    set_instance_parameter_value generic_tristate_controller_0 {MODULE_ASSIGNMENT_KEYS} {embeddedsw.configuration.hwClassnameDriverSupportList embeddedsw.configuration.hwClassnameDriverSupportDefault embeddedsw.CMacro.SETUP_VALUE embeddedsw.CMacro.WAIT_VALUE embeddedsw.CMacro.HOLD_VALUE embeddedsw.CMacro.TIMING_UNITS embeddedsw.CMacro.SIZE embeddedsw.memoryInfo.MEM_INIT_DATA_WIDTH embeddedsw.memoryInfo.HAS_BYTE_LANE embeddedsw.memoryInfo.IS_FLASH embeddedsw.memoryInfo.GENERATE_DAT_SYM embeddedsw.memoryInfo.GENERATE_FLASH embeddedsw.memoryInfo.DAT_SYM_INSTALL_DIR embeddedsw.memoryInfo.FLASH_INSTALL_DIR}
    set_instance_parameter_value generic_tristate_controller_0 {MODULE_ASSIGNMENT_VALUES} {altera_avalon_lan91c111:altera_avalon_cfi_flash altera_avalon_cfi_flash 25 100 20 ns 33554432u 16 1 1 1 1 SIM_DIR APP_DIR}
    set_instance_parameter_value generic_tristate_controller_0 {INTERFACE_ASSIGNMENT_KEYS} {embeddedsw.configuration.isFlash embeddedsw.configuration.isMemoryDevice embeddedsw.configuration.isNonVolatileStorage}
    set_instance_parameter_value generic_tristate_controller_0 {INTERFACE_ASSIGNMENT_VALUES} {1 1 1}

    add_instance mm_bridge_0 altera_avalon_mm_bridge 13.1
    set_instance_parameter_value mm_bridge_0 {DATA_WIDTH} {32}
    set_instance_parameter_value mm_bridge_0 {SYMBOL_WIDTH} {8}
    set_instance_parameter_value mm_bridge_0 {ADDRESS_WIDTH} {23}
    set_instance_parameter_value mm_bridge_0 {ADDRESS_UNITS} {SYMBOLS}
    set_instance_parameter_value mm_bridge_0 {MAX_BURST_SIZE} {1}
    set_instance_parameter_value mm_bridge_0 {MAX_PENDING_RESPONSES} {4}
    set_instance_parameter_value mm_bridge_0 {LINEWRAPBURSTS} {0}
    set_instance_parameter_value mm_bridge_0 {PIPELINE_COMMAND} {1}
    set_instance_parameter_value mm_bridge_0 {PIPELINE_RESPONSE} {1}

    # connections and connection parameters
    add_connection clk_0.clk_reset onchip_memoryMain.reset1 reset

    add_connection clk_0.clk tristate_conduit_bridge_0.clk clock

    add_connection clk_0.clk_reset generic_tristate_controller_0.reset reset

    add_connection clk_0.clk_reset tristate_conduit_bridge_0.reset reset

    add_connection clk_0.clk generic_tristate_controller_0.clk clock

    add_connection generic_tristate_controller_0.tcm tristate_conduit_bridge_0.tcs tristate_conduit

    add_connection clk_0.clk onchip_memoryMain.clk1 clock

    add_connection clk_0.clk mm_bridge_0.clk clock

    add_connection clk_0.clk_reset mm_bridge_0.reset reset

    add_connection mm_bridge_0.m0 onchip_memoryMain.s1 avalon
    set_connection_parameter_value mm_bridge_0.m0/onchip_memoryMain.s1 arbitrationPriority {1}
    set_connection_parameter_value mm_bridge_0.m0/onchip_memoryMain.s1 baseAddress {0x00400000}
    set_connection_parameter_value mm_bridge_0.m0/onchip_memoryMain.s1 defaultConnection {0}

    add_connection mm_bridge_0.m0 generic_tristate_controller_0.uas avalon
    set_connection_parameter_value mm_bridge_0.m0/generic_tristate_controller_0.uas arbitrationPriority {1}
    set_connection_parameter_value mm_bridge_0.m0/generic_tristate_controller_0.uas baseAddress {0x0000}
    set_connection_parameter_value mm_bridge_0.m0/generic_tristate_controller_0.uas defaultConnection {0}

    # exported interfaces
    add_interface clk clock sink
    set_interface_property clk EXPORT_OF clk_0.clk_in
    add_interface reset reset sink
    set_interface_property reset EXPORT_OF clk_0.clk_in_reset
    add_interface tristate_conduit_bridge conduit end
    set_interface_property tristate_conduit_bridge EXPORT_OF tristate_conduit_bridge_0.out
    add_interface mm_bridge_0_s0 avalon slave
    set_interface_property mm_bridge_0_s0 EXPORT_OF mm_bridge_0.s0
}
