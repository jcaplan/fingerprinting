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

    add_connection clk_0.clk onchip_memoryMain.clk1 clock

    add_connection clk_0.clk mm_bridge_0.clk clock

    add_connection clk_0.clk_reset mm_bridge_0.reset reset

    add_connection mm_bridge_0.m0 onchip_memoryMain.s1 avalon
    set_connection_parameter_value mm_bridge_0.m0/onchip_memoryMain.s1 arbitrationPriority {1}
    set_connection_parameter_value mm_bridge_0.m0/onchip_memoryMain.s1 baseAddress {0x00400000}
    set_connection_parameter_value mm_bridge_0.m0/onchip_memoryMain.s1 defaultConnection {0}

    # exported interfaces
    add_interface clk clock sink
    set_interface_property clk EXPORT_OF clk_0.clk_in
    add_interface reset reset sink
    set_interface_property reset EXPORT_OF clk_0.clk_in_reset
    add_interface mm_bridge_0_s0 avalon slave
    set_interface_property mm_bridge_0_s0 EXPORT_OF mm_bridge_0.s0
}
