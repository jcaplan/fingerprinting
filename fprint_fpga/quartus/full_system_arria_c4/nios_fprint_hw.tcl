package require -exact qsys 13.1

# module properties
set_module_property NAME {nios_fprint_export}
set_module_property DISPLAY_NAME {nios_fprint_export_display}

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

    add_instance sys_id altera_avalon_sysid_qsys 13.1
    set_instance_parameter_value sys_id {id} {0}

    add_instance shared_memory altera_avalon_onchip_memory2 13.1
    set_instance_parameter_value shared_memory {allowInSystemMemoryContentEditor} {0}
    set_instance_parameter_value shared_memory {blockType} {AUTO}
    set_instance_parameter_value shared_memory {dataWidth} {32}
    set_instance_parameter_value shared_memory {dualPort} {0}
    set_instance_parameter_value shared_memory {initMemContent} {1}
    set_instance_parameter_value shared_memory {initializationFileName} {onchip_mem.hex}
    set_instance_parameter_value shared_memory {instanceID} {NONE}
    set_instance_parameter_value shared_memory {memorySize} {1024.0}
    set_instance_parameter_value shared_memory {readDuringWriteMode} {DONT_CARE}
    set_instance_parameter_value shared_memory {simAllowMRAMContentsFile} {0}
    set_instance_parameter_value shared_memory {simMemInitOnlyFilename} {0}
    set_instance_parameter_value shared_memory {singleClockOperation} {0}
    set_instance_parameter_value shared_memory {slave1Latency} {1}
    set_instance_parameter_value shared_memory {slave2Latency} {1}
    set_instance_parameter_value shared_memory {useNonDefaultInitFile} {0}
    set_instance_parameter_value shared_memory {useShallowMemBlocks} {0}
    set_instance_parameter_value shared_memory {writable} {1}
    set_instance_parameter_value shared_memory {ecc_enabled} {0}

    add_instance mutex_0 altera_avalon_mutex 13.1
    set_instance_parameter_value mutex_0 {initialValue} {0}
    set_instance_parameter_value mutex_0 {initialOwner} {0}

    add_instance cfpu_0 cfpu 1.0

    add_instance memory_0 memory 1.0

    add_instance mm_bridge_0 altera_avalon_mm_bridge 13.1
    set_instance_parameter_value mm_bridge_0 {DATA_WIDTH} {32}
    set_instance_parameter_value mm_bridge_0 {SYMBOL_WIDTH} {8}
    set_instance_parameter_value mm_bridge_0 {ADDRESS_WIDTH} {25}
    set_instance_parameter_value mm_bridge_0 {ADDRESS_UNITS} {SYMBOLS}
    set_instance_parameter_value mm_bridge_0 {MAX_BURST_SIZE} {1}
    set_instance_parameter_value mm_bridge_0 {MAX_PENDING_RESPONSES} {4}
    set_instance_parameter_value mm_bridge_0 {LINEWRAPBURSTS} {0}
    set_instance_parameter_value mm_bridge_0 {PIPELINE_COMMAND} {1}
    set_instance_parameter_value mm_bridge_0 {PIPELINE_RESPONSE} {1}

    add_instance processor1_0 processor1 1.0

    add_instance processor0_0 processor0 1.0

    add_instance processorM_0 processorM 1.0

    add_instance processor2_0 processor2 1.0

    add_instance processor3_0 processor3 1.0

    # connections and connection parameters
    add_connection clk_0.clk_reset sys_id.reset reset

    add_connection clk_0.clk_reset mutex_0.reset reset

    add_connection clk_0.clk_reset cfpu_0.reset reset

    add_connection clk_0.clk sys_id.clk clock

    add_connection clk_0.clk shared_memory.clk1 clock

    add_connection clk_0.clk mutex_0.clk clock

    add_connection clk_0.clk cfpu_0.clock clock

    add_connection clk_0.clk memory_0.clk clock

    add_connection clk_0.clk_reset memory_0.reset reset

    add_connection clk_0.clk mm_bridge_0.clk clock

    add_connection clk_0.clk_reset mm_bridge_0.reset reset

    add_connection mm_bridge_0.m0 memory_0.mm_bridge_0_s0 avalon
    set_connection_parameter_value mm_bridge_0.m0/memory_0.mm_bridge_0_s0 arbitrationPriority {1}
    set_connection_parameter_value mm_bridge_0.m0/memory_0.mm_bridge_0_s0 baseAddress {0x0000}
    set_connection_parameter_value mm_bridge_0.m0/memory_0.mm_bridge_0_s0 defaultConnection {0}

    add_connection mm_bridge_0.m0 cfpu_0.fprint avalon
    set_connection_parameter_value mm_bridge_0.m0/cfpu_0.fprint arbitrationPriority {1}
    set_connection_parameter_value mm_bridge_0.m0/cfpu_0.fprint baseAddress {0x01000000}
    set_connection_parameter_value mm_bridge_0.m0/cfpu_0.fprint defaultConnection {0}

    add_connection clk_0.clk_reset shared_memory.reset1 reset

    add_connection mm_bridge_0.m0 sys_id.control_slave avalon
    set_connection_parameter_value mm_bridge_0.m0/sys_id.control_slave arbitrationPriority {1}
    set_connection_parameter_value mm_bridge_0.m0/sys_id.control_slave baseAddress {0x01001020}
    set_connection_parameter_value mm_bridge_0.m0/sys_id.control_slave defaultConnection {0}

    add_connection clk_0.clk processor1_0.philosopher_clk_in clock

    add_connection clk_0.clk_reset processor1_0.reset reset

    add_connection processor1_0.outgoing_master mutex_0.s1 avalon
    set_connection_parameter_value processor1_0.outgoing_master/mutex_0.s1 arbitrationPriority {1}
    set_connection_parameter_value processor1_0.outgoing_master/mutex_0.s1 baseAddress {0x02300000}
    set_connection_parameter_value processor1_0.outgoing_master/mutex_0.s1 defaultConnection {0}

    add_connection processor1_0.outgoing_master processor1_0.cpu1_irq_0_s0 avalon
    set_connection_parameter_value processor1_0.outgoing_master/processor1_0.cpu1_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor1_0.outgoing_master/processor1_0.cpu1_irq_0_s0 baseAddress {0x02200400}
    set_connection_parameter_value processor1_0.outgoing_master/processor1_0.cpu1_irq_0_s0 defaultConnection {0}

    add_connection processor1_0.outgoing_master shared_memory.s1 avalon
    set_connection_parameter_value processor1_0.outgoing_master/shared_memory.s1 arbitrationPriority {1}
    set_connection_parameter_value processor1_0.outgoing_master/shared_memory.s1 baseAddress {0x02500000}
    set_connection_parameter_value processor1_0.outgoing_master/shared_memory.s1 defaultConnection {0}

    add_connection processor1_0.outgoing_master mm_bridge_0.s0 avalon
    set_connection_parameter_value processor1_0.outgoing_master/mm_bridge_0.s0 arbitrationPriority {1}
    set_connection_parameter_value processor1_0.outgoing_master/mm_bridge_0.s0 baseAddress {0x0000}
    set_connection_parameter_value processor1_0.outgoing_master/mm_bridge_0.s0 defaultConnection {0}

    add_connection processor1_0.fingerprint_0_avalon_master mm_bridge_0.s0 avalon
    set_connection_parameter_value processor1_0.fingerprint_0_avalon_master/mm_bridge_0.s0 arbitrationPriority {1}
    set_connection_parameter_value processor1_0.fingerprint_0_avalon_master/mm_bridge_0.s0 baseAddress {0x0000}
    set_connection_parameter_value processor1_0.fingerprint_0_avalon_master/mm_bridge_0.s0 defaultConnection {0}

    add_connection clk_0.clk processor0_0.philosopher_clk_in clock

    add_connection clk_0.clk_reset processor0_0.reset reset

    add_connection processor0_0.out_system_bridge_m0 mutex_0.s1 avalon
    set_connection_parameter_value processor0_0.out_system_bridge_m0/mutex_0.s1 arbitrationPriority {1}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/mutex_0.s1 baseAddress {0x02300000}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/mutex_0.s1 defaultConnection {0}

    add_connection processor0_0.out_system_bridge_m0 processor0_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processor0_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processor0_0.cpu_irq_0_s0 baseAddress {0x02200000}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processor0_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor1_0.outgoing_master processor0_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor1_0.outgoing_master/processor0_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor1_0.outgoing_master/processor0_0.cpu_irq_0_s0 baseAddress {0x02200000}
    set_connection_parameter_value processor1_0.outgoing_master/processor0_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor0_0.out_system_bridge_m0 processor1_0.cpu1_irq_0_s0 avalon
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processor1_0.cpu1_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processor1_0.cpu1_irq_0_s0 baseAddress {0x02200400}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processor1_0.cpu1_irq_0_s0 defaultConnection {0}

    add_connection processor0_0.out_system_bridge_m0 shared_memory.s1 avalon
    set_connection_parameter_value processor0_0.out_system_bridge_m0/shared_memory.s1 arbitrationPriority {1}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/shared_memory.s1 baseAddress {0x02500000}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/shared_memory.s1 defaultConnection {0}

    add_connection processor0_0.out_system_bridge_m0 mm_bridge_0.s0 avalon
    set_connection_parameter_value processor0_0.out_system_bridge_m0/mm_bridge_0.s0 arbitrationPriority {1}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/mm_bridge_0.s0 baseAddress {0x0000}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/mm_bridge_0.s0 defaultConnection {0}

    add_connection processor0_0.fingerprint_0_avalon_master mm_bridge_0.s0 avalon
    set_connection_parameter_value processor0_0.fingerprint_0_avalon_master/mm_bridge_0.s0 arbitrationPriority {1}
    set_connection_parameter_value processor0_0.fingerprint_0_avalon_master/mm_bridge_0.s0 baseAddress {0x0000}
    set_connection_parameter_value processor0_0.fingerprint_0_avalon_master/mm_bridge_0.s0 defaultConnection {0}

    add_connection clk_0.clk_reset processorM_0.reset reset

    add_connection processorM_0.outgoing_master mutex_0.s1 avalon
    set_connection_parameter_value processorM_0.outgoing_master/mutex_0.s1 arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/mutex_0.s1 baseAddress {0x02300000}
    set_connection_parameter_value processorM_0.outgoing_master/mutex_0.s1 defaultConnection {0}

    add_connection processorM_0.outgoing_master processor0_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processorM_0.outgoing_master/processor0_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/processor0_0.cpu_irq_0_s0 baseAddress {0x02200000}
    set_connection_parameter_value processorM_0.outgoing_master/processor0_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processorM_0.outgoing_master processor1_0.cpu1_irq_0_s0 avalon
    set_connection_parameter_value processorM_0.outgoing_master/processor1_0.cpu1_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/processor1_0.cpu1_irq_0_s0 baseAddress {0x02200400}
    set_connection_parameter_value processorM_0.outgoing_master/processor1_0.cpu1_irq_0_s0 defaultConnection {0}

    add_connection processor1_0.outgoing_master processorM_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor1_0.outgoing_master/processorM_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor1_0.outgoing_master/processorM_0.cpu_irq_0_s0 baseAddress {0x02200800}
    set_connection_parameter_value processor1_0.outgoing_master/processorM_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor0_0.out_system_bridge_m0 processorM_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processorM_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processorM_0.cpu_irq_0_s0 baseAddress {0x02200800}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processorM_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processorM_0.dma_0_irq_receiver_irq processor0_0.dma_0_irq interrupt
    set_connection_parameter_value processorM_0.dma_0_irq_receiver_irq/processor0_0.dma_0_irq irqNumber {0}

    add_connection cfpu_0.irq processorM_0.fprint_irq_0_irq conduit
    set_connection_parameter_value cfpu_0.irq/processorM_0.fprint_irq_0_irq endPort {}
    set_connection_parameter_value cfpu_0.irq/processorM_0.fprint_irq_0_irq endPortLSB {0}
    set_connection_parameter_value cfpu_0.irq/processorM_0.fprint_irq_0_irq startPort {}
    set_connection_parameter_value cfpu_0.irq/processorM_0.fprint_irq_0_irq startPortLSB {0}
    set_connection_parameter_value cfpu_0.irq/processorM_0.fprint_irq_0_irq width {0}

    add_connection processorM_0.outgoing_master processorM_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processorM_0.outgoing_master/processorM_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/processorM_0.cpu_irq_0_s0 baseAddress {0x02200800}
    set_connection_parameter_value processorM_0.outgoing_master/processorM_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processorM_0.outgoing_master shared_memory.s1 avalon
    set_connection_parameter_value processorM_0.outgoing_master/shared_memory.s1 arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/shared_memory.s1 baseAddress {0x02500000}
    set_connection_parameter_value processorM_0.outgoing_master/shared_memory.s1 defaultConnection {0}

    add_connection processorM_0.outgoing_master mm_bridge_0.s0 avalon
    set_connection_parameter_value processorM_0.outgoing_master/mm_bridge_0.s0 arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/mm_bridge_0.s0 baseAddress {0x0000}
    set_connection_parameter_value processorM_0.outgoing_master/mm_bridge_0.s0 defaultConnection {0}

    add_connection processorM_0.outgoing_master cfpu_0.csr avalon
    set_connection_parameter_value processorM_0.outgoing_master/cfpu_0.csr arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/cfpu_0.csr baseAddress {0x02400000}
    set_connection_parameter_value processorM_0.outgoing_master/cfpu_0.csr defaultConnection {0}

    add_connection processorM_0.outgoing_master processor1_0.dma_0_control_port_slave avalon
    set_connection_parameter_value processorM_0.outgoing_master/processor1_0.dma_0_control_port_slave arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/processor1_0.dma_0_control_port_slave baseAddress {0x02110000}
    set_connection_parameter_value processorM_0.outgoing_master/processor1_0.dma_0_control_port_slave defaultConnection {0}

    add_connection processorM_0.outgoing_master processor0_0.dma_0_control_port_slave avalon
    set_connection_parameter_value processorM_0.outgoing_master/processor0_0.dma_0_control_port_slave arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/processor0_0.dma_0_control_port_slave baseAddress {0x02100000}
    set_connection_parameter_value processorM_0.outgoing_master/processor0_0.dma_0_control_port_slave defaultConnection {0}

    add_connection processorM_0.outgoing_master processor1_0.sw_reset_0_s0 avalon
    set_connection_parameter_value processorM_0.outgoing_master/processor1_0.sw_reset_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/processor1_0.sw_reset_0_s0 baseAddress {0x03000400}
    set_connection_parameter_value processorM_0.outgoing_master/processor1_0.sw_reset_0_s0 defaultConnection {0}

    add_connection processorM_0.outgoing_master processor0_0.sw_reset_0_s0_1 avalon
    set_connection_parameter_value processorM_0.outgoing_master/processor0_0.sw_reset_0_s0_1 arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/processor0_0.sw_reset_0_s0_1 baseAddress {0x03000000}
    set_connection_parameter_value processorM_0.outgoing_master/processor0_0.sw_reset_0_s0_1 defaultConnection {0}

    add_connection processor0_0.sw_reset_0_reset_source processor0_0.reset reset

    add_connection processor1_0.sw_reset_0_reset_source processor1_0.reset reset

    add_connection processor0_0.out_system_bridge_m0 processorM_0.reset_monitor_0_s0 avalon
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processorM_0.reset_monitor_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processorM_0.reset_monitor_0_s0 baseAddress {0x02800000}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processorM_0.reset_monitor_0_s0 defaultConnection {0}

    add_connection processor1_0.outgoing_master processorM_0.reset_monitor_0_s0 avalon
    set_connection_parameter_value processor1_0.outgoing_master/processorM_0.reset_monitor_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor1_0.outgoing_master/processorM_0.reset_monitor_0_s0 baseAddress {0x02800000}
    set_connection_parameter_value processor1_0.outgoing_master/processorM_0.reset_monitor_0_s0 defaultConnection {0}

    add_connection processorM_0.outgoing_master processorM_0.reset_monitor_0_s0 avalon
    set_connection_parameter_value processorM_0.outgoing_master/processorM_0.reset_monitor_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/processorM_0.reset_monitor_0_s0 baseAddress {0x02800000}
    set_connection_parameter_value processorM_0.outgoing_master/processorM_0.reset_monitor_0_s0 defaultConnection {0}

    add_connection clk_0.clk processorM_0.philosopher_clk_in clock

    add_connection processorM_0.dma_0_irq_receiver_irq processor1_0.dma_0_irq interrupt
    set_connection_parameter_value processorM_0.dma_0_irq_receiver_irq/processor1_0.dma_0_irq irqNumber {1}

    add_connection clk_0.clk processor2_0.philosopher_clk_in clock

    add_connection clk_0.clk_reset processor2_0.reset reset

    add_connection clk_0.clk processor3_0.philosopher_clk_in clock

    add_connection clk_0.clk_reset processor3_0.reset reset

    add_connection processor2_0.fingerprint_0_avalon_master mm_bridge_0.s0 avalon
    set_connection_parameter_value processor2_0.fingerprint_0_avalon_master/mm_bridge_0.s0 arbitrationPriority {1}
    set_connection_parameter_value processor2_0.fingerprint_0_avalon_master/mm_bridge_0.s0 baseAddress {0x0000}
    set_connection_parameter_value processor2_0.fingerprint_0_avalon_master/mm_bridge_0.s0 defaultConnection {0}

    add_connection processor3_0.fingerprint_0_avalon_master mm_bridge_0.s0 avalon
    set_connection_parameter_value processor3_0.fingerprint_0_avalon_master/mm_bridge_0.s0 arbitrationPriority {1}
    set_connection_parameter_value processor3_0.fingerprint_0_avalon_master/mm_bridge_0.s0 baseAddress {0x0000}
    set_connection_parameter_value processor3_0.fingerprint_0_avalon_master/mm_bridge_0.s0 defaultConnection {0}

    add_connection processorM_0.dma_0_irq_receiver_irq processor2_0.dma_0_irq interrupt
    set_connection_parameter_value processorM_0.dma_0_irq_receiver_irq/processor2_0.dma_0_irq irqNumber {2}

    add_connection processorM_0.dma_0_irq_receiver_irq processor3_0.dma_0_irq interrupt
    set_connection_parameter_value processorM_0.dma_0_irq_receiver_irq/processor3_0.dma_0_irq irqNumber {3}

    add_connection processor2_0.out_system_bridge_m0 processor2_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processor2_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processor2_0.cpu_irq_0_s0 baseAddress {0x02200c00}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processor2_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor2_0.out_system_bridge_m0 processor1_0.cpu1_irq_0_s0 avalon
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processor1_0.cpu1_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processor1_0.cpu1_irq_0_s0 baseAddress {0x02200400}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processor1_0.cpu1_irq_0_s0 defaultConnection {0}

    add_connection processor2_0.out_system_bridge_m0 processor0_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processor0_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processor0_0.cpu_irq_0_s0 baseAddress {0x02200000}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processor0_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor2_0.out_system_bridge_m0 processor3_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processor3_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processor3_0.cpu_irq_0_s0 baseAddress {0x02201000}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processor3_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor2_0.out_system_bridge_m0 processorM_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processorM_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processorM_0.cpu_irq_0_s0 baseAddress {0x02200800}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processorM_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor3_0.out_system_bridge_m0 processorM_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor3_0.out_system_bridge_m0/processorM_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor3_0.out_system_bridge_m0/processorM_0.cpu_irq_0_s0 baseAddress {0x02200800}
    set_connection_parameter_value processor3_0.out_system_bridge_m0/processorM_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor3_0.out_system_bridge_m0 processor3_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor3_0.out_system_bridge_m0/processor3_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor3_0.out_system_bridge_m0/processor3_0.cpu_irq_0_s0 baseAddress {0x02201000}
    set_connection_parameter_value processor3_0.out_system_bridge_m0/processor3_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor3_0.fingerprint_0_avalon_master processor2_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor3_0.fingerprint_0_avalon_master/processor2_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor3_0.fingerprint_0_avalon_master/processor2_0.cpu_irq_0_s0 baseAddress {0x02200c00}
    set_connection_parameter_value processor3_0.fingerprint_0_avalon_master/processor2_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor3_0.fingerprint_0_avalon_master processor1_0.cpu1_irq_0_s0 avalon
    set_connection_parameter_value processor3_0.fingerprint_0_avalon_master/processor1_0.cpu1_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor3_0.fingerprint_0_avalon_master/processor1_0.cpu1_irq_0_s0 baseAddress {0x02200400}
    set_connection_parameter_value processor3_0.fingerprint_0_avalon_master/processor1_0.cpu1_irq_0_s0 defaultConnection {0}

    add_connection processor3_0.fingerprint_0_avalon_master processor0_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor3_0.fingerprint_0_avalon_master/processor0_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor3_0.fingerprint_0_avalon_master/processor0_0.cpu_irq_0_s0 baseAddress {0x02200000}
    set_connection_parameter_value processor3_0.fingerprint_0_avalon_master/processor0_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor1_0.outgoing_master processor2_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor1_0.outgoing_master/processor2_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor1_0.outgoing_master/processor2_0.cpu_irq_0_s0 baseAddress {0x02200c00}
    set_connection_parameter_value processor1_0.outgoing_master/processor2_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor1_0.outgoing_master processor3_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor1_0.outgoing_master/processor3_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor1_0.outgoing_master/processor3_0.cpu_irq_0_s0 baseAddress {0x02201000}
    set_connection_parameter_value processor1_0.outgoing_master/processor3_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor0_0.out_system_bridge_m0 processor3_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processor3_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processor3_0.cpu_irq_0_s0 baseAddress {0x02201000}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processor3_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor0_0.out_system_bridge_m0 processor2_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processor2_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processor2_0.cpu_irq_0_s0 baseAddress {0x02200c00}
    set_connection_parameter_value processor0_0.out_system_bridge_m0/processor2_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processor2_0.out_system_bridge_m0 processorM_0.reset_monitor_0_s0 avalon
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processorM_0.reset_monitor_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processorM_0.reset_monitor_0_s0 baseAddress {0x02800000}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/processorM_0.reset_monitor_0_s0 defaultConnection {0}

    add_connection processor3_0.out_system_bridge_m0 processorM_0.reset_monitor_0_s0 avalon
    set_connection_parameter_value processor3_0.out_system_bridge_m0/processorM_0.reset_monitor_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processor3_0.out_system_bridge_m0/processorM_0.reset_monitor_0_s0 baseAddress {0x02800000}
    set_connection_parameter_value processor3_0.out_system_bridge_m0/processorM_0.reset_monitor_0_s0 defaultConnection {0}

    add_connection processor3_0.out_system_bridge_m0 mutex_0.s1 avalon
    set_connection_parameter_value processor3_0.out_system_bridge_m0/mutex_0.s1 arbitrationPriority {1}
    set_connection_parameter_value processor3_0.out_system_bridge_m0/mutex_0.s1 baseAddress {0x02300000}
    set_connection_parameter_value processor3_0.out_system_bridge_m0/mutex_0.s1 defaultConnection {0}

    add_connection processor2_0.out_system_bridge_m0 mutex_0.s1 avalon
    set_connection_parameter_value processor2_0.out_system_bridge_m0/mutex_0.s1 arbitrationPriority {1}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/mutex_0.s1 baseAddress {0x02300000}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/mutex_0.s1 defaultConnection {0}

    add_connection processor2_0.out_system_bridge_m0 shared_memory.s1 avalon
    set_connection_parameter_value processor2_0.out_system_bridge_m0/shared_memory.s1 arbitrationPriority {1}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/shared_memory.s1 baseAddress {0x02500000}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/shared_memory.s1 defaultConnection {0}

    add_connection processor3_0.out_system_bridge_m0 shared_memory.s1 avalon
    set_connection_parameter_value processor3_0.out_system_bridge_m0/shared_memory.s1 arbitrationPriority {1}
    set_connection_parameter_value processor3_0.out_system_bridge_m0/shared_memory.s1 baseAddress {0x02500000}
    set_connection_parameter_value processor3_0.out_system_bridge_m0/shared_memory.s1 defaultConnection {0}

    add_connection processor2_0.out_system_bridge_m0 mm_bridge_0.s0 avalon
    set_connection_parameter_value processor2_0.out_system_bridge_m0/mm_bridge_0.s0 arbitrationPriority {1}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/mm_bridge_0.s0 baseAddress {0x0000}
    set_connection_parameter_value processor2_0.out_system_bridge_m0/mm_bridge_0.s0 defaultConnection {0}

    add_connection processor3_0.out_system_bridge_m0 mm_bridge_0.s0 avalon
    set_connection_parameter_value processor3_0.out_system_bridge_m0/mm_bridge_0.s0 arbitrationPriority {1}
    set_connection_parameter_value processor3_0.out_system_bridge_m0/mm_bridge_0.s0 baseAddress {0x0000}
    set_connection_parameter_value processor3_0.out_system_bridge_m0/mm_bridge_0.s0 defaultConnection {0}

    add_connection processorM_0.outgoing_master processor2_0.sw_reset_0_s0_1 avalon
    set_connection_parameter_value processorM_0.outgoing_master/processor2_0.sw_reset_0_s0_1 arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/processor2_0.sw_reset_0_s0_1 baseAddress {0x03000800}
    set_connection_parameter_value processorM_0.outgoing_master/processor2_0.sw_reset_0_s0_1 defaultConnection {0}

    add_connection processorM_0.outgoing_master processor2_0.dma_0_control_port_slave avalon
    set_connection_parameter_value processorM_0.outgoing_master/processor2_0.dma_0_control_port_slave arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/processor2_0.dma_0_control_port_slave baseAddress {0x02120000}
    set_connection_parameter_value processorM_0.outgoing_master/processor2_0.dma_0_control_port_slave defaultConnection {0}

    add_connection processorM_0.outgoing_master processor3_0.sw_reset_0_s0_1 avalon
    set_connection_parameter_value processorM_0.outgoing_master/processor3_0.sw_reset_0_s0_1 arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/processor3_0.sw_reset_0_s0_1 baseAddress {0x03000c00}
    set_connection_parameter_value processorM_0.outgoing_master/processor3_0.sw_reset_0_s0_1 defaultConnection {0}

    add_connection processorM_0.outgoing_master processor3_0.dma_0_control_port_slave avalon
    set_connection_parameter_value processorM_0.outgoing_master/processor3_0.dma_0_control_port_slave arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/processor3_0.dma_0_control_port_slave baseAddress {0x02130000}
    set_connection_parameter_value processorM_0.outgoing_master/processor3_0.dma_0_control_port_slave defaultConnection {0}

    add_connection processor2_0.sw_reset_0_reset_source processor2_0.reset reset

    add_connection processor3_0.sw_reset_0_reset_source processor3_0.reset reset

    add_connection processorM_0.outgoing_master processor2_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processorM_0.outgoing_master/processor2_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/processor2_0.cpu_irq_0_s0 baseAddress {0x02200c00}
    set_connection_parameter_value processorM_0.outgoing_master/processor2_0.cpu_irq_0_s0 defaultConnection {0}

    add_connection processorM_0.outgoing_master processor3_0.cpu_irq_0_s0 avalon
    set_connection_parameter_value processorM_0.outgoing_master/processor3_0.cpu_irq_0_s0 arbitrationPriority {1}
    set_connection_parameter_value processorM_0.outgoing_master/processor3_0.cpu_irq_0_s0 baseAddress {0x02201000}
    set_connection_parameter_value processorM_0.outgoing_master/processor3_0.cpu_irq_0_s0 defaultConnection {0}

    # exported interfaces
    add_interface clk clock sink
    set_interface_property clk EXPORT_OF clk_0.clk_in
    add_interface reset reset sink
    set_interface_property reset EXPORT_OF clk_0.clk_in_reset
}
