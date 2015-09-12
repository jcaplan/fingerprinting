package require -exact qsys 13.1

# module properties
set_module_property NAME {processor4_export}
set_module_property DISPLAY_NAME {processor4_export_display}

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
    add_instance clk clock_source 13.1
    set_instance_parameter_value clk {clockFrequency} {50000000.0}
    set_instance_parameter_value clk {clockFrequencyKnown} {1}
    set_instance_parameter_value clk {resetSynchronousEdges} {NONE}

    add_instance cpu4 altera_nios2_qsys 13.1
    set_instance_parameter_value cpu4 {setting_showUnpublishedSettings} {0}
    set_instance_parameter_value cpu4 {setting_showInternalSettings} {0}
    set_instance_parameter_value cpu4 {setting_preciseSlaveAccessErrorException} {0}
    set_instance_parameter_value cpu4 {setting_preciseIllegalMemAccessException} {0}
    set_instance_parameter_value cpu4 {setting_preciseDivisionErrorException} {0}
    set_instance_parameter_value cpu4 {setting_performanceCounter} {0}
    set_instance_parameter_value cpu4 {setting_illegalMemAccessDetection} {0}
    set_instance_parameter_value cpu4 {setting_illegalInstructionsTrap} {0}
    set_instance_parameter_value cpu4 {setting_fullWaveformSignals} {0}
    set_instance_parameter_value cpu4 {setting_extraExceptionInfo} {0}
    set_instance_parameter_value cpu4 {setting_exportPCB} {0}
    set_instance_parameter_value cpu4 {setting_debugSimGen} {0}
    set_instance_parameter_value cpu4 {setting_clearXBitsLDNonBypass} {1}
    set_instance_parameter_value cpu4 {setting_bit31BypassDCache} {1}
    set_instance_parameter_value cpu4 {setting_bigEndian} {0}
    set_instance_parameter_value cpu4 {setting_export_large_RAMs} {0}
    set_instance_parameter_value cpu4 {setting_asic_enabled} {0}
    set_instance_parameter_value cpu4 {setting_asic_synopsys_translate_on_off} {0}
    set_instance_parameter_value cpu4 {setting_oci_export_jtag_signals} {0}
    set_instance_parameter_value cpu4 {setting_bhtIndexPcOnly} {0}
    set_instance_parameter_value cpu4 {setting_avalonDebugPortPresent} {0}
    set_instance_parameter_value cpu4 {setting_alwaysEncrypt} {1}
    set_instance_parameter_value cpu4 {setting_allowFullAddressRange} {0}
    set_instance_parameter_value cpu4 {setting_activateTrace} {1}
    set_instance_parameter_value cpu4 {setting_activateTrace_user} {0}
    set_instance_parameter_value cpu4 {setting_activateTestEndChecker} {0}
    set_instance_parameter_value cpu4 {setting_ecc_sim_test_ports} {0}
    set_instance_parameter_value cpu4 {setting_activateMonitors} {1}
    set_instance_parameter_value cpu4 {setting_activateModelChecker} {0}
    set_instance_parameter_value cpu4 {setting_HDLSimCachesCleared} {1}
    set_instance_parameter_value cpu4 {setting_HBreakTest} {0}
    set_instance_parameter_value cpu4 {setting_breakslaveoveride} {0}
    set_instance_parameter_value cpu4 {muldiv_divider} {0}
    set_instance_parameter_value cpu4 {mpu_useLimit} {0}
    set_instance_parameter_value cpu4 {mpu_enabled} {1}
    set_instance_parameter_value cpu4 {mmu_enabled} {0}
    set_instance_parameter_value cpu4 {mmu_autoAssignTlbPtrSz} {1}
    set_instance_parameter_value cpu4 {manuallyAssignCpuID} {1}
    set_instance_parameter_value cpu4 {debug_triggerArming} {1}
    set_instance_parameter_value cpu4 {debug_embeddedPLL} {1}
    set_instance_parameter_value cpu4 {debug_debugReqSignals} {0}
    set_instance_parameter_value cpu4 {debug_assignJtagInstanceID} {0}
    set_instance_parameter_value cpu4 {dcache_omitDataMaster} {0}
    set_instance_parameter_value cpu4 {cpuReset} {0}
    set_instance_parameter_value cpu4 {is_hardcopy_compatible} {0}
    set_instance_parameter_value cpu4 {setting_shadowRegisterSets} {0}
    set_instance_parameter_value cpu4 {mpu_numOfInstRegion} {8}
    set_instance_parameter_value cpu4 {mpu_numOfDataRegion} {16}
    set_instance_parameter_value cpu4 {mmu_TLBMissExcOffset} {0}
    set_instance_parameter_value cpu4 {debug_jtagInstanceID} {0}
    set_instance_parameter_value cpu4 {resetOffset} {4718592}
    set_instance_parameter_value cpu4 {exceptionOffset} {4718624}
    set_instance_parameter_value cpu4 {cpuID} {5}
    set_instance_parameter_value cpu4 {cpuID_stored} {5}
    set_instance_parameter_value cpu4 {breakOffset} {32}
    set_instance_parameter_value cpu4 {userDefinedSettings} {}
    set_instance_parameter_value cpu4 {resetSlave} {Absolute}
    set_instance_parameter_value cpu4 {mmu_TLBMissExcSlave} {}
    set_instance_parameter_value cpu4 {exceptionSlave} {Absolute}
    set_instance_parameter_value cpu4 {breakSlave} {cpu4.jtag_debug_module}
    set_instance_parameter_value cpu4 {setting_perfCounterWidth} {32}
    set_instance_parameter_value cpu4 {setting_interruptControllerType} {Internal}
    set_instance_parameter_value cpu4 {setting_branchPredictionType} {Automatic}
    set_instance_parameter_value cpu4 {setting_bhtPtrSz} {8}
    set_instance_parameter_value cpu4 {muldiv_multiplierType} {NoneSmall}
    set_instance_parameter_value cpu4 {mpu_minInstRegionSize} {12}
    set_instance_parameter_value cpu4 {mpu_minDataRegionSize} {10}
    set_instance_parameter_value cpu4 {mmu_uitlbNumEntries} {4}
    set_instance_parameter_value cpu4 {mmu_udtlbNumEntries} {6}
    set_instance_parameter_value cpu4 {mmu_tlbPtrSz} {7}
    set_instance_parameter_value cpu4 {mmu_tlbNumWays} {16}
    set_instance_parameter_value cpu4 {mmu_processIDNumBits} {8}
    set_instance_parameter_value cpu4 {impl} {Fast}
    set_instance_parameter_value cpu4 {icache_size} {1024}
    set_instance_parameter_value cpu4 {icache_tagramBlockType} {Automatic}
    set_instance_parameter_value cpu4 {icache_ramBlockType} {Automatic}
    set_instance_parameter_value cpu4 {icache_numTCIM} {0}
    set_instance_parameter_value cpu4 {icache_burstType} {None}
    set_instance_parameter_value cpu4 {dcache_bursts} {false}
    set_instance_parameter_value cpu4 {dcache_victim_buf_impl} {ram}
    set_instance_parameter_value cpu4 {debug_level} {Level1}
    set_instance_parameter_value cpu4 {debug_OCIOnchipTrace} {_128}
    set_instance_parameter_value cpu4 {dcache_size} {0}
    set_instance_parameter_value cpu4 {dcache_tagramBlockType} {Automatic}
    set_instance_parameter_value cpu4 {dcache_ramBlockType} {Automatic}
    set_instance_parameter_value cpu4 {dcache_numTCDM} {0}
    set_instance_parameter_value cpu4 {dcache_lineSize} {32}
    set_instance_parameter_value cpu4 {setting_exportvectors} {0}
    set_instance_parameter_value cpu4 {setting_ecc_present} {0}
    set_instance_parameter_value cpu4 {setting_ic_ecc_present} {1}
    set_instance_parameter_value cpu4 {setting_rf_ecc_present} {1}
    set_instance_parameter_value cpu4 {setting_mmu_ecc_present} {1}
    set_instance_parameter_value cpu4 {setting_dc_ecc_present} {0}
    set_instance_parameter_value cpu4 {setting_itcm_ecc_present} {0}
    set_instance_parameter_value cpu4 {setting_dtcm_ecc_present} {0}
    set_instance_parameter_value cpu4 {regfile_ramBlockType} {Automatic}
    set_instance_parameter_value cpu4 {ocimem_ramBlockType} {Automatic}
    set_instance_parameter_value cpu4 {mmu_ramBlockType} {Automatic}
    set_instance_parameter_value cpu4 {bht_ramBlockType} {Automatic}

    add_instance timer altera_avalon_timer 13.1
    set_instance_parameter_value timer {alwaysRun} {0}
    set_instance_parameter_value timer {counterSize} {32}
    set_instance_parameter_value timer {fixedPeriod} {0}
    set_instance_parameter_value timer {period} {10}
    set_instance_parameter_value timer {periodUnits} {MSEC}
    set_instance_parameter_value timer {resetOutput} {0}
    set_instance_parameter_value timer {snapshot} {1}
    set_instance_parameter_value timer {timeoutPulseOutput} {0}

    add_instance jtag_uart altera_avalon_jtag_uart 13.1
    set_instance_parameter_value jtag_uart {allowMultipleConnections} {0}
    set_instance_parameter_value jtag_uart {hubInstanceID} {0}
    set_instance_parameter_value jtag_uart {readBufferDepth} {64}
    set_instance_parameter_value jtag_uart {readIRQThreshold} {8}
    set_instance_parameter_value jtag_uart {simInputCharacterStream} {}
    set_instance_parameter_value jtag_uart {simInteractiveOptions} {INTERACTIVE_ASCII_OUTPUT}
    set_instance_parameter_value jtag_uart {useRegistersForReadBuffer} {0}
    set_instance_parameter_value jtag_uart {useRegistersForWriteBuffer} {0}
    set_instance_parameter_value jtag_uart {useRelativePathForSimFile} {0}
    set_instance_parameter_value jtag_uart {writeBufferDepth} {64}
    set_instance_parameter_value jtag_uart {writeIRQThreshold} {8}

    add_instance button_pio altera_avalon_pio 13.1
    set_instance_parameter_value button_pio {bitClearingEdgeCapReg} {1}
    set_instance_parameter_value button_pio {bitModifyingOutReg} {0}
    set_instance_parameter_value button_pio {captureEdge} {1}
    set_instance_parameter_value button_pio {direction} {Input}
    set_instance_parameter_value button_pio {edgeType} {RISING}
    set_instance_parameter_value button_pio {generateIRQ} {1}
    set_instance_parameter_value button_pio {irqType} {EDGE}
    set_instance_parameter_value button_pio {resetValue} {0.0}
    set_instance_parameter_value button_pio {simDoTestBenchWiring} {0}
    set_instance_parameter_value button_pio {simDrivenValue} {0.0}
    set_instance_parameter_value button_pio {width} {1}

    add_instance cpu_irq_0 cpu_irq 1.0

    add_instance timestamp altera_avalon_timer 13.1
    set_instance_parameter_value timestamp {alwaysRun} {0}
    set_instance_parameter_value timestamp {counterSize} {64}
    set_instance_parameter_value timestamp {fixedPeriod} {0}
    set_instance_parameter_value timestamp {period} {1}
    set_instance_parameter_value timestamp {periodUnits} {MSEC}
    set_instance_parameter_value timestamp {resetOutput} {0}
    set_instance_parameter_value timestamp {snapshot} {1}
    set_instance_parameter_value timestamp {timeoutPulseOutput} {0}

    add_instance Fingerprint_0 Fingerprint 1.0
    set_instance_parameter_value Fingerprint_0 {CORE_ID} {4}

    add_instance scratchpad altera_avalon_onchip_memory2 13.1
    set_instance_parameter_value scratchpad {allowInSystemMemoryContentEditor} {0}
    set_instance_parameter_value scratchpad {blockType} {AUTO}
    set_instance_parameter_value scratchpad {dataWidth} {32}
    set_instance_parameter_value scratchpad {dualPort} {0}
    set_instance_parameter_value scratchpad {initMemContent} {1}
    set_instance_parameter_value scratchpad {initializationFileName} {onchip_mem.hex}
    set_instance_parameter_value scratchpad {instanceID} {NONE}
    set_instance_parameter_value scratchpad {memorySize} {16384.0}
    set_instance_parameter_value scratchpad {readDuringWriteMode} {DONT_CARE}
    set_instance_parameter_value scratchpad {simAllowMRAMContentsFile} {0}
    set_instance_parameter_value scratchpad {simMemInitOnlyFilename} {0}
    set_instance_parameter_value scratchpad {singleClockOperation} {0}
    set_instance_parameter_value scratchpad {slave1Latency} {1}
    set_instance_parameter_value scratchpad {slave2Latency} {1}
    set_instance_parameter_value scratchpad {useNonDefaultInitFile} {0}
    set_instance_parameter_value scratchpad {useShallowMemBlocks} {0}
    set_instance_parameter_value scratchpad {writable} {1}
    set_instance_parameter_value scratchpad {ecc_enabled} {0}

    add_instance dma_0 altera_avalon_dma 13.1
    set_instance_parameter_value dma_0 {allowByteTransactions} {1}
    set_instance_parameter_value dma_0 {allowDoubleWordTransactions} {1}
    set_instance_parameter_value dma_0 {allowHalfWordTransactions} {1}
    set_instance_parameter_value dma_0 {allowQuadWordTransactions} {1}
    set_instance_parameter_value dma_0 {allowWordTransactions} {1}
    set_instance_parameter_value dma_0 {bigEndian} {0}
    set_instance_parameter_value dma_0 {burstEnable} {0}
    set_instance_parameter_value dma_0 {fifoDepth} {32}
    set_instance_parameter_value dma_0 {maxBurstSize} {128}
    set_instance_parameter_value dma_0 {minimumDmaTransactionRegisterWidth} {13}
    set_instance_parameter_value dma_0 {useRegistersForFIFO} {0}

    add_instance out_system_bridge altera_avalon_mm_bridge 13.1
    set_instance_parameter_value out_system_bridge {DATA_WIDTH} {32}
    set_instance_parameter_value out_system_bridge {SYMBOL_WIDTH} {8}
    set_instance_parameter_value out_system_bridge {ADDRESS_WIDTH} {27}
    set_instance_parameter_value out_system_bridge {ADDRESS_UNITS} {SYMBOLS}
    set_instance_parameter_value out_system_bridge {MAX_BURST_SIZE} {1}
    set_instance_parameter_value out_system_bridge {MAX_PENDING_RESPONSES} {4}
    set_instance_parameter_value out_system_bridge {LINEWRAPBURSTS} {0}
    set_instance_parameter_value out_system_bridge {PIPELINE_COMMAND} {1}
    set_instance_parameter_value out_system_bridge {PIPELINE_RESPONSE} {1}

    # connections and connection parameters
    add_connection clk.clk cpu4.clk clock

    add_connection clk.clk_reset cpu4.reset_n reset

    add_connection cpu4.instruction_master cpu4.jtag_debug_module avalon
    set_connection_parameter_value cpu4.instruction_master/cpu4.jtag_debug_module arbitrationPriority {1}
    set_connection_parameter_value cpu4.instruction_master/cpu4.jtag_debug_module baseAddress {0x08000800}
    set_connection_parameter_value cpu4.instruction_master/cpu4.jtag_debug_module defaultConnection {0}

    add_connection cpu4.data_master cpu4.jtag_debug_module avalon
    set_connection_parameter_value cpu4.data_master/cpu4.jtag_debug_module arbitrationPriority {1}
    set_connection_parameter_value cpu4.data_master/cpu4.jtag_debug_module baseAddress {0x08000800}
    set_connection_parameter_value cpu4.data_master/cpu4.jtag_debug_module defaultConnection {0}

    add_connection clk.clk_reset timer.reset reset

    add_connection cpu4.data_master timer.s1 avalon
    set_connection_parameter_value cpu4.data_master/timer.s1 arbitrationPriority {1}
    set_connection_parameter_value cpu4.data_master/timer.s1 baseAddress {0x08001020}
    set_connection_parameter_value cpu4.data_master/timer.s1 defaultConnection {0}

    add_connection cpu4.instruction_master timer.s1 avalon
    set_connection_parameter_value cpu4.instruction_master/timer.s1 arbitrationPriority {1}
    set_connection_parameter_value cpu4.instruction_master/timer.s1 baseAddress {0x08001020}
    set_connection_parameter_value cpu4.instruction_master/timer.s1 defaultConnection {0}

    add_connection cpu4.d_irq timer.irq interrupt
    set_connection_parameter_value cpu4.d_irq/timer.irq irqNumber {0}

    add_connection clk.clk timer.clk clock

    add_connection clk.clk jtag_uart.clk clock

    add_connection clk.clk_reset jtag_uart.reset reset

    add_connection cpu4.d_irq jtag_uart.irq interrupt
    set_connection_parameter_value cpu4.d_irq/jtag_uart.irq irqNumber {4}

    add_connection cpu4.data_master jtag_uart.avalon_jtag_slave avalon
    set_connection_parameter_value cpu4.data_master/jtag_uart.avalon_jtag_slave arbitrationPriority {1}
    set_connection_parameter_value cpu4.data_master/jtag_uart.avalon_jtag_slave baseAddress {0x08001040}
    set_connection_parameter_value cpu4.data_master/jtag_uart.avalon_jtag_slave defaultConnection {0}

    add_connection clk.clk button_pio.clk clock

    add_connection clk.clk_reset button_pio.reset reset

    add_connection cpu4.data_master button_pio.s1 avalon
    set_connection_parameter_value cpu4.data_master/button_pio.s1 arbitrationPriority {1}
    set_connection_parameter_value cpu4.data_master/button_pio.s1 baseAddress {0x08002000}
    set_connection_parameter_value cpu4.data_master/button_pio.s1 defaultConnection {0}

    add_connection cpu4.d_irq button_pio.irq interrupt
    set_connection_parameter_value cpu4.d_irq/button_pio.irq irqNumber {2}

    add_connection clk.clk cpu_irq_0.clock clock

    add_connection clk.clk_reset cpu_irq_0.reset reset

    add_connection cpu4.d_irq cpu_irq_0.irq0 interrupt
    set_connection_parameter_value cpu4.d_irq/cpu_irq_0.irq0 irqNumber {1}

    add_connection clk.clk_reset timestamp.reset reset

    add_connection clk.clk timestamp.clk clock

    add_connection cpu4.data_master timestamp.s1 avalon
    set_connection_parameter_value cpu4.data_master/timestamp.s1 arbitrationPriority {1}
    set_connection_parameter_value cpu4.data_master/timestamp.s1 baseAddress {0x08000000}
    set_connection_parameter_value cpu4.data_master/timestamp.s1 defaultConnection {0}

    add_connection clk.clk Fingerprint_0.clock clock

    add_connection clk.clk_reset Fingerprint_0.reset_sink reset

    add_connection cpu4.data_master Fingerprint_0.avalon_slave_0 avalon
    set_connection_parameter_value cpu4.data_master/Fingerprint_0.avalon_slave_0 arbitrationPriority {1}
    set_connection_parameter_value cpu4.data_master/Fingerprint_0.avalon_slave_0 baseAddress {0x08100000}
    set_connection_parameter_value cpu4.data_master/Fingerprint_0.avalon_slave_0 defaultConnection {0}

    add_connection clk.clk scratchpad.clk1 clock

    add_connection clk.clk_reset scratchpad.reset1 reset

    add_connection cpu4.data_master scratchpad.s1 avalon
    set_connection_parameter_value cpu4.data_master/scratchpad.s1 arbitrationPriority {1}
    set_connection_parameter_value cpu4.data_master/scratchpad.s1 baseAddress {0x08200000}
    set_connection_parameter_value cpu4.data_master/scratchpad.s1 defaultConnection {0}

    add_connection dma_0.read_master scratchpad.s1 avalon
    set_connection_parameter_value dma_0.read_master/scratchpad.s1 arbitrationPriority {1}
    set_connection_parameter_value dma_0.read_master/scratchpad.s1 baseAddress {0x08200000}
    set_connection_parameter_value dma_0.read_master/scratchpad.s1 defaultConnection {0}

    add_connection dma_0.write_master scratchpad.s1 avalon
    set_connection_parameter_value dma_0.write_master/scratchpad.s1 arbitrationPriority {1}
    set_connection_parameter_value dma_0.write_master/scratchpad.s1 baseAddress {0x08200000}
    set_connection_parameter_value dma_0.write_master/scratchpad.s1 defaultConnection {0}

    add_connection clk.clk_reset dma_0.reset reset

    add_connection clk.clk dma_0.clk clock

    add_connection clk.clk out_system_bridge.clk clock

    add_connection clk.clk_reset out_system_bridge.reset reset

    add_connection cpu4.data_master out_system_bridge.s0 avalon
    set_connection_parameter_value cpu4.data_master/out_system_bridge.s0 arbitrationPriority {1}
    set_connection_parameter_value cpu4.data_master/out_system_bridge.s0 baseAddress {0x0000}
    set_connection_parameter_value cpu4.data_master/out_system_bridge.s0 defaultConnection {0}

    add_connection dma_0.read_master out_system_bridge.s0 avalon
    set_connection_parameter_value dma_0.read_master/out_system_bridge.s0 arbitrationPriority {1}
    set_connection_parameter_value dma_0.read_master/out_system_bridge.s0 baseAddress {0x0000}
    set_connection_parameter_value dma_0.read_master/out_system_bridge.s0 defaultConnection {0}

    add_connection dma_0.write_master out_system_bridge.s0 avalon
    set_connection_parameter_value dma_0.write_master/out_system_bridge.s0 arbitrationPriority {1}
    set_connection_parameter_value dma_0.write_master/out_system_bridge.s0 baseAddress {0x0000}
    set_connection_parameter_value dma_0.write_master/out_system_bridge.s0 defaultConnection {0}

    add_connection cpu4.instruction_master out_system_bridge.s0 avalon
    set_connection_parameter_value cpu4.instruction_master/out_system_bridge.s0 arbitrationPriority {1}
    set_connection_parameter_value cpu4.instruction_master/out_system_bridge.s0 baseAddress {0x0000}
    set_connection_parameter_value cpu4.instruction_master/out_system_bridge.s0 defaultConnection {0}

    # exported interfaces
    add_interface philosopher_clk_in clock sink
    set_interface_property philosopher_clk_in EXPORT_OF clk.clk_in
    add_interface button_pio_external_connection conduit end
    set_interface_property button_pio_external_connection EXPORT_OF button_pio.external_connection
    add_interface cpu0_jtag_debug_module_reset reset source
    set_interface_property cpu0_jtag_debug_module_reset EXPORT_OF cpu4.jtag_debug_module_reset
    add_interface reset reset sink
    set_interface_property reset EXPORT_OF clk.clk_in_reset
    add_interface cpu_irq_0_s0 avalon slave
    set_interface_property cpu_irq_0_s0 EXPORT_OF cpu_irq_0.s0
    add_interface fingerprint_0_avalon_master avalon master
    set_interface_property fingerprint_0_avalon_master EXPORT_OF Fingerprint_0.avalon_master
    add_interface dma_0_control_port_slave avalon slave
    set_interface_property dma_0_control_port_slave EXPORT_OF dma_0.control_port_slave
    add_interface dma_0_irq interrupt sender
    set_interface_property dma_0_irq EXPORT_OF dma_0.irq
    add_interface out_system_bridge_m0 avalon master
    set_interface_property out_system_bridge_m0 EXPORT_OF out_system_bridge.m0
}
