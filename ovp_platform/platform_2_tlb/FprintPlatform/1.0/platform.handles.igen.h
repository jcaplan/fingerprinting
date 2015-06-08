
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Mon Jun  8 15:15:39 2015
//
////////////////////////////////////////////////////////////////////////////////

// This file declares handles to platform objects.
// Access to these are required by parts of the ICM API.
// This file should NOT need to be edited.

struct handlesS {
    icmBusP              smbus_b;
    icmBusP              mmbus_b;
    icmBusP              flashbus_b;
    icmBusP              mutexbus_b;
    icmBusP              cpuirqbus_b;
    icmBusP              swresetbus_b;
    icmBusP              cpu0_iobus_b;
    icmBusP              cpu0_tlbbus_b;
    icmBusP              cpu0_tlbbus_in_b;
    icmBusP              cpu0_dbus_b;
    icmBusP              cpu0_ibus_b;
    icmBusP              cpu0_tlb_fprint_bus_b;
    icmBusP              cpu1_iobus_b;
    icmBusP              cpu1_dbus_b;
    icmBusP              cpu1_ibus_b;
    icmBusP              cpu1_tlbbus_b;
    icmBusP              cpu1_tlbbus_in_b;
    icmBusP              cpu1_tlb_fprint_bus_b;
    icmBusP              cpum_iobus_b;
    icmBusP              cpum_dbus_b;
    icmBusP              cpum_ibus_b;
    icmBusP              cpu0_dmabus_b;
    icmBusP              cpu1_dmabus_b;
    icmBusP              compbus_b;
    icmProcessorP        cpu0_c;
    icmProcessorP        cpu1_c;
    icmProcessorP        cpum_c;
    icmMemoryP           shared_memory_m;
    icmMemoryP           main_memory_m;
    icmMemoryP           flash_m;
    icmMemoryP           cpu0_breakmem_m;
    icmMemoryP           cpu0_scratchpad_m;
    icmMemoryP           cpu1_breakmem_m;
    icmMemoryP           cpu1_scratchpad_m;
    icmMemoryP           cpum_breakmem_m;
    icmMemoryP           cpum_scratchpad_m;
    icmPseP              mutex_p;
    icmPseP              cpu_irq_p;
    icmPseP              sw_reset_p;
    icmPseP              cpu0_tlb_p;
    icmPseP              cpu0_jtag_uart_p;
    icmPseP              cpu0_timer_10ms_p;
    icmPseP              cpu0_timestamp_p;
    icmPseP              cpu0_fprint_p;
    icmPseP              cpu1_tlb_p;
    icmPseP              cpu1_jtag_uart_p;
    icmPseP              cpu1_timer_10ms_p;
    icmPseP              cpu1_timestamp_p;
    icmPseP              cpu1_fprint_p;
    icmPseP              cpum_jtag_uart_p;
    icmPseP              cpum_timer_10ms_p;
    icmPseP              cpum_timestamp_p;
    icmPseP              cpu0_dma_p;
    icmPseP              cpu1_dma_p;
    icmPseP              comparator_p;
    icmNetP              cpu0_irq4_n;
    icmNetP              cpu0_irq0_n;
    icmNetP              cpu0_irq1_n;
    icmNetP              cpu0_reset1_n;
    icmNetP              fprint_write_0_n;
    icmNetP              fprint_write_address_0_n;
    icmNetP              fprint_write_data_0_n;
    icmNetP              cpu1_irq3_n;
    icmNetP              cpu1_irq0_n;
    icmNetP              cpu1_irq1_n;
    icmNetP              cpu1_reset1_n;
    icmNetP              fprint_write_1_n;
    icmNetP              fprint_write_address_1_n;
    icmNetP              fprint_write_data_1_n;
    icmNetP              cpum_irq4_n;
    icmNetP              cpum_irq0_n;
    icmNetP              cpum_irq5_n;
    icmNetP              cpum_irq6_n;
    icmNetP              cpum_irq3_n;
    icmNetP              cpum_irq1_n;
} handles;
