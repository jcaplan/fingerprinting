
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Tue May 19 15:20:18 2015
//
////////////////////////////////////////////////////////////////////////////////


// This file constructs the platform according to TCL script.
// This file should NOT need to be edited.

////////////////////////////////////////////////////////////////////////////////

void platformConstructor(void) {

    icmInitAttrs FprintPlatform_attrs = ICM_STOP_ON_CTRLC|ICM_VERBOSE;

    icmInitPlatform(ICM_VERSION, FprintPlatform_attrs, 0, 0, "FprintPlatform");

////////////////////////////////////////////////////////////////////////////////
//                                  Bus smbus
////////////////////////////////////////////////////////////////////////////////


    handles.smbus_b = icmNewBus( "smbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                  Bus mmbus
////////////////////////////////////////////////////////////////////////////////


    handles.mmbus_b = icmNewBus( "mmbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus flashbus
////////////////////////////////////////////////////////////////////////////////


    handles.flashbus_b = icmNewBus( "flashbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus mutexbus
////////////////////////////////////////////////////////////////////////////////


    handles.mutexbus_b = icmNewBus( "mutexbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpuirqbus
////////////////////////////////////////////////////////////////////////////////


    handles.cpuirqbus_b = icmNewBus( "cpuirqbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                               Bus cpu0_iobus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu0_iobus_b = icmNewBus( "cpu0_iobus", 32);

////////////////////////////////////////////////////////////////////////////////
//                               Bus cpu0_tlbbus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu0_tlbbus_b = icmNewBus( "cpu0_tlbbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                             Bus cpu0_tlbbus_in
////////////////////////////////////////////////////////////////////////////////


    handles.cpu0_tlbbus_in_b = icmNewBus( "cpu0_tlbbus_in", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpu0_dbus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu0_dbus_b = icmNewBus( "cpu0_dbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpu0_ibus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu0_ibus_b = icmNewBus( "cpu0_ibus", 32);

////////////////////////////////////////////////////////////////////////////////
//                           Bus cpu0_tlb_fprint_bus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu0_tlb_fprint_bus_b = icmNewBus( "cpu0_tlb_fprint_bus", 32);

////////////////////////////////////////////////////////////////////////////////
//                               Bus cpu1_iobus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu1_iobus_b = icmNewBus( "cpu1_iobus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpu1_dbus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu1_dbus_b = icmNewBus( "cpu1_dbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpu1_ibus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu1_ibus_b = icmNewBus( "cpu1_ibus", 32);

////////////////////////////////////////////////////////////////////////////////
//                               Bus cpu1_tlbbus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu1_tlbbus_b = icmNewBus( "cpu1_tlbbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                             Bus cpu1_tlbbus_in
////////////////////////////////////////////////////////////////////////////////


    handles.cpu1_tlbbus_in_b = icmNewBus( "cpu1_tlbbus_in", 32);

////////////////////////////////////////////////////////////////////////////////
//                           Bus cpu1_tlb_fprint_bus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu1_tlb_fprint_bus_b = icmNewBus( "cpu1_tlb_fprint_bus", 32);

////////////////////////////////////////////////////////////////////////////////
//                               Bus cpum_iobus
////////////////////////////////////////////////////////////////////////////////


    handles.cpum_iobus_b = icmNewBus( "cpum_iobus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpum_dbus
////////////////////////////////////////////////////////////////////////////////


    handles.cpum_dbus_b = icmNewBus( "cpum_dbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpum_ibus
////////////////////////////////////////////////////////////////////////////////


    handles.cpum_ibus_b = icmNewBus( "cpum_ibus", 32);

////////////////////////////////////////////////////////////////////////////////
//                               Bus cpu0_dmabus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu0_dmabus_b = icmNewBus( "cpu0_dmabus", 32);

////////////////////////////////////////////////////////////////////////////////
//                               Bus cpu1_dmabus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu1_dmabus_b = icmNewBus( "cpu1_dmabus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                 Bus compbus
////////////////////////////////////////////////////////////////////////////////


    handles.compbus_b = icmNewBus( "compbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                               Processor cpu0
////////////////////////////////////////////////////////////////////////////////


    const char *cpu0_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "nios_ii"           ,    // name
        "1.0"               ,    // version
        "model"                  // model
    );

    icmAttrListP cpu0_attrList = icmNewAttrList();
    icmAddStringAttr(cpu0_attrList, "variant", "Nios_II_S");
    icmAddStringAttr(cpu0_attrList, "TEST_HALT_EXIT", "1");
    icmAddUns64Attr(cpu0_attrList, "INST_ADDR_WIDTH", 0x1c);
    icmAddUns64Attr(cpu0_attrList, "DATA_ADDR_WIDTH", 0x1c);
    icmAddStringAttr(cpu0_attrList, "HW_MULTIPLY", "0");
    icmAddStringAttr(cpu0_attrList, "HW_MULX", "0");
    icmAddStringAttr(cpu0_attrList, "HW_DIVIDE", "0");
    icmAddUns64Attr(cpu0_attrList, "RESET_VECTOR", 0x464000);
    icmAddUns64Attr(cpu0_attrList, "EXCEPTION_VECTOR", 0x464020);
    icmAddUns64Attr(cpu0_attrList, "BREAK_VECTOR", 0x8000820);
    icmAddStringAttr(cpu0_attrList, "INCLUDE_MMU", "0");
    icmAddStringAttr(cpu0_attrList, "INCLUDE_MPU", "1");
    icmAddStringAttr(cpu0_attrList, "MPU_USE_LIMIT_FOR_REGION_RANGE", "0");
    icmAddUns64Attr(cpu0_attrList, "MPU_NUMBER_DATA_REGIONS", 0x10);
    icmAddUns64Attr(cpu0_attrList, "MPU_MINIMUM_DATA_REGION_SIZE", 0x400);
    icmAddUns64Attr(cpu0_attrList, "MPU_NUMBER_INSTRUCTION_REGIONS", 0x8);
    icmAddUns64Attr(cpu0_attrList, "MPU_MINIMUM_INSTRUCTION_REGION_SIZE", 0x1000);
    icmAddDoubleAttr(cpu0_attrList, "mips", 125.000000);
    icmAddStringAttr(cpu0_attrList, "endian", "little");
    icmNewProcAttrs cpu0_attrs = ICM_ATTR_SIMEX;


    handles.cpu0_c = icmNewProcessor(
        "cpu0"              ,   // name
        "nios_ii"           ,   // type
        0                   ,   // cpuId
        0x0000              ,  // flags
        32                  ,   // address bits
        cpu0_path           ,   // model
        0,       // (unused)
        cpu0_attrs          ,   // procAttrs
        cpu0_attrList       ,   // attrlist
        0                   ,   // semihost file
        0        // (unused)
    );


////////////////////////////////////////////////////////////////////////////////
//                               Processor cpu1
////////////////////////////////////////////////////////////////////////////////


    const char *cpu1_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "nios_ii"           ,    // name
        "1.0"               ,    // version
        "model"                  // model
    );

    icmAttrListP cpu1_attrList = icmNewAttrList();
    icmAddStringAttr(cpu1_attrList, "variant", "Nios_II_S");
    icmAddStringAttr(cpu1_attrList, "TEST_HALT_EXIT", "1");
    icmAddUns64Attr(cpu1_attrList, "INST_ADDR_WIDTH", 0x1c);
    icmAddUns64Attr(cpu1_attrList, "DATA_ADDR_WIDTH", 0x1c);
    icmAddStringAttr(cpu1_attrList, "HW_MULTIPLY", "0");
    icmAddStringAttr(cpu1_attrList, "HW_MULX", "0");
    icmAddStringAttr(cpu1_attrList, "HW_DIVIDE", "0");
    icmAddUns64Attr(cpu1_attrList, "RESET_VECTOR", 0x432000);
    icmAddUns64Attr(cpu1_attrList, "EXCEPTION_VECTOR", 0x432020);
    icmAddUns64Attr(cpu1_attrList, "BREAK_VECTOR", 0x8008020);
    icmAddStringAttr(cpu1_attrList, "INCLUDE_MMU", "0");
    icmAddStringAttr(cpu1_attrList, "INCLUDE_MPU", "1");
    icmAddStringAttr(cpu1_attrList, "MPU_USE_LIMIT_FOR_REGION_RANGE", "0");
    icmAddUns64Attr(cpu1_attrList, "MPU_NUMBER_DATA_REGIONS", 0x10);
    icmAddUns64Attr(cpu1_attrList, "MPU_MINIMUM_DATA_REGION_SIZE", 0x400);
    icmAddUns64Attr(cpu1_attrList, "MPU_NUMBER_INSTRUCTION_REGIONS", 0x8);
    icmAddUns64Attr(cpu1_attrList, "MPU_MINIMUM_INSTRUCTION_REGION_SIZE", 0x1000);
    icmAddDoubleAttr(cpu1_attrList, "mips", 125.000000);
    icmAddStringAttr(cpu1_attrList, "endian", "little");
    icmNewProcAttrs cpu1_attrs = ICM_ATTR_SIMEX;


    handles.cpu1_c = icmNewProcessor(
        "cpu1"              ,   // name
        "nios_ii"           ,   // type
        1                   ,   // cpuId
        0x0000              ,  // flags
        32                  ,   // address bits
        cpu1_path           ,   // model
        0,       // (unused)
        cpu1_attrs          ,   // procAttrs
        cpu1_attrList       ,   // attrlist
        0                   ,   // semihost file
        0        // (unused)
    );


////////////////////////////////////////////////////////////////////////////////
//                               Processor cpum
////////////////////////////////////////////////////////////////////////////////


    const char *cpum_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "nios_ii"           ,    // name
        "1.0"               ,    // version
        "model"                  // model
    );

    icmAttrListP cpum_attrList = icmNewAttrList();
    icmAddStringAttr(cpum_attrList, "variant", "Nios_II_S");
    icmAddStringAttr(cpum_attrList, "TEST_HALT_EXIT", "1");
    icmAddUns64Attr(cpum_attrList, "INST_ADDR_WIDTH", 0x1c);
    icmAddUns64Attr(cpum_attrList, "DATA_ADDR_WIDTH", 0x1c);
    icmAddStringAttr(cpum_attrList, "HW_MULTIPLY", "0");
    icmAddStringAttr(cpum_attrList, "HW_MULX", "0");
    icmAddStringAttr(cpum_attrList, "HW_DIVIDE", "0");
    icmAddUns64Attr(cpum_attrList, "RESET_VECTOR", 0x400000);
    icmAddUns64Attr(cpum_attrList, "EXCEPTION_VECTOR", 0x400020);
    icmAddUns64Attr(cpum_attrList, "BREAK_VECTOR", 0x8000820);
    icmAddStringAttr(cpum_attrList, "INCLUDE_MMU", "0");
    icmAddDoubleAttr(cpum_attrList, "mips", 125.000000);
    icmAddStringAttr(cpum_attrList, "endian", "little");
    icmNewProcAttrs cpum_attrs = ICM_ATTR_SIMEX;


    handles.cpum_c = icmNewProcessor(
        "cpum"              ,   // name
        "nios_ii"           ,   // type
        2                   ,   // cpuId
        0x0000              ,  // flags
        32                  ,   // address bits
        cpum_path           ,   // model
        0,       // (unused)
        cpum_attrs          ,   // procAttrs
        cpum_attrList       ,   // attrlist
        0                   ,   // semihost file
        0        // (unused)
    );


////////////////////////////////////////////////////////////////////////////////
//                                  PSE mutex
////////////////////////////////////////////////////////////////////////////////


    const char *mutex_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "mutex"             ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP mutex_attrList = icmNewAttrList();

    handles.mutex_p = icmNewPSE(
        "mutex"             ,   // name
        mutex_path          ,   // model
        mutex_attrList      ,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.mutex_p, handles.mutexbus_b, "MUTEX_SLAVE", 0, 0x0, 0x7);

////////////////////////////////////////////////////////////////////////////////
//                                 PSE cpu_irq
////////////////////////////////////////////////////////////////////////////////


    const char *cpu_irq_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "cpu_irq"           ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu_irq_attrList = icmNewAttrList();

    handles.cpu_irq_p = icmNewPSE(
        "cpu_irq"           ,   // name
        cpu_irq_path        ,   // model
        cpu_irq_attrList    ,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu_irq_p, handles.cpuirqbus_b, "CPU_IRQ_SLAVE", 0, 0x0, 0x23ff);

////////////////////////////////////////////////////////////////////////////////
//                                PSE cpu0_tlb
////////////////////////////////////////////////////////////////////////////////


    const char *cpu0_tlb_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "tlb"               ,    // name
        "1.0"               ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu0_tlb_attrList = icmNewAttrList();

    handles.cpu0_tlb_p = icmNewPSE(
        "cpu0_tlb"          ,   // name
        cpu0_tlb_path       ,   // model
        cpu0_tlb_attrList   ,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu0_tlb_p, handles.cpu0_tlbbus_b, "TLB_MASTER", 1, 0x0, 0xffffffff);

    icmConnectPSEBus( handles.cpu0_tlb_p, handles.cpu0_tlb_fprint_bus_b, "TLB_FPRINT_MASTER", 1, 0x0, 0x7ffffff);

    icmConnectPSEBusDynamic( handles.cpu0_tlb_p, handles.cpu0_tlbbus_in_b, "TLB_SLAVE", 0);

    icmConnectPSEBus( handles.cpu0_tlb_p, handles.cpu0_iobus_b, "TLB_CSR", 0, 0x300000, 0x3003ff);
    icmSetPSEdiagnosticLevel(handles.cpu0_tlb_p, 3);


////////////////////////////////////////////////////////////////////////////////
//                             PSE cpu0_jtag_uart
////////////////////////////////////////////////////////////////////////////////


    const char *cpu0_jtag_uart_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "altera.ovpworld.org",    // vendor
        0                   ,    // library
        "JtagUart"          ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu0_jtag_uart_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu0_jtag_uart_attrList, "readBufferDepth", 0x40);
    icmAddUns64Attr(cpu0_jtag_uart_attrList, "readIRQThreshold", 0x8);
    icmAddUns64Attr(cpu0_jtag_uart_attrList, "writeBufferDepth", 0x40);
    icmAddUns64Attr(cpu0_jtag_uart_attrList, "writeIRQThreshold", 0x8);
    icmAddUns32Attr(cpu0_jtag_uart_attrList, "console", 1);
    icmAddUns32Attr(cpu0_jtag_uart_attrList, "finishOnDisconnect", 1);

    handles.cpu0_jtag_uart_p = icmNewPSE(
        "cpu0_jtag_uart"    ,   // name
        cpu0_jtag_uart_path ,   // model
        cpu0_jtag_uart_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu0_jtag_uart_p, handles.cpu0_iobus_b, "sp1", 0, 0x1040, 0x1047);

////////////////////////////////////////////////////////////////////////////////
//                             PSE cpu0_timer_10ms
////////////////////////////////////////////////////////////////////////////////


    const char *cpu0_timer_10ms_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "IntervalTimer32Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu0_timer_10ms_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu0_timer_10ms_attrList, "timeoutPeriod", 0x7a120);
    icmAddUns64Attr(cpu0_timer_10ms_attrList, "timerFrequency", 0x2faf080);

    handles.cpu0_timer_10ms_p = icmNewPSE(
        "cpu0_timer_10ms"   ,   // name
        cpu0_timer_10ms_path,   // model
        cpu0_timer_10ms_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu0_timer_10ms_p, handles.cpu0_iobus_b, "sp1", 0, 0x1020, 0x1037);

////////////////////////////////////////////////////////////////////////////////
//                             PSE cpu0_timestamp
////////////////////////////////////////////////////////////////////////////////


    const char *cpu0_timestamp_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "IntervalTimer32Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu0_timestamp_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu0_timestamp_attrList, "timeoutPeriod", 0x7a120);
    icmAddUns64Attr(cpu0_timestamp_attrList, "timerFrequency", 0x2faf080);

    handles.cpu0_timestamp_p = icmNewPSE(
        "cpu0_timestamp"    ,   // name
        cpu0_timestamp_path ,   // model
        cpu0_timestamp_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu0_timestamp_p, handles.cpu0_iobus_b, "sp1", 0, 0x0, 0x17);

////////////////////////////////////////////////////////////////////////////////
//                               PSE cpu0_fprint
////////////////////////////////////////////////////////////////////////////////


    const char *cpu0_fprint_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "fprint"            ,    // name
        "2.0"               ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu0_fprint_attrList = icmNewAttrList();
    icmAddUns32Attr(cpu0_fprint_attrList, "coreID", 0);

    handles.cpu0_fprint_p = icmNewPSE(
        "cpu0_fprint"       ,   // name
        cpu0_fprint_path    ,   // model
        cpu0_fprint_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu0_fprint_p, handles.compbus_b, "FPRINT_MASTER", 1, 0x0, 0xfffffff);

    icmConnectPSEBus( handles.cpu0_fprint_p, handles.cpu0_iobus_b, "FPRINT_CSR_SLAVE", 0, 0x100000, 0x10013f);

    icmConnectPSEBus( handles.cpu0_fprint_p, handles.cpu0_tlb_fprint_bus_b, "FPRINT_TLB_SLAVE", 0, 0x0, 0x3ff);

////////////////////////////////////////////////////////////////////////////////
//                                PSE cpu1_tlb
////////////////////////////////////////////////////////////////////////////////


    const char *cpu1_tlb_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "tlb"               ,    // name
        "1.0"               ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu1_tlb_attrList = icmNewAttrList();

    handles.cpu1_tlb_p = icmNewPSE(
        "cpu1_tlb"          ,   // name
        cpu1_tlb_path       ,   // model
        cpu1_tlb_attrList   ,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu1_tlb_p, handles.cpu1_tlbbus_b, "TLB_MASTER", 1, 0x0, 0xffffffff);

    icmConnectPSEBus( handles.cpu1_tlb_p, handles.cpu1_tlb_fprint_bus_b, "TLB_FPRINT_MASTER", 1, 0x0, 0x7ffffff);

    icmConnectPSEBusDynamic( handles.cpu1_tlb_p, handles.cpu1_tlbbus_in_b, "TLB_SLAVE", 0);

    icmConnectPSEBus( handles.cpu1_tlb_p, handles.cpu1_iobus_b, "TLB_CSR", 0, 0x300000, 0x3003ff);
    icmSetPSEdiagnosticLevel(handles.cpu1_tlb_p, 3);


////////////////////////////////////////////////////////////////////////////////
//                             PSE cpu1_jtag_uart
////////////////////////////////////////////////////////////////////////////////


    const char *cpu1_jtag_uart_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "altera.ovpworld.org",    // vendor
        0                   ,    // library
        "JtagUart"          ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu1_jtag_uart_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu1_jtag_uart_attrList, "readBufferDepth", 0x40);
    icmAddUns64Attr(cpu1_jtag_uart_attrList, "readIRQThreshold", 0x8);
    icmAddUns64Attr(cpu1_jtag_uart_attrList, "writeBufferDepth", 0x40);
    icmAddUns64Attr(cpu1_jtag_uart_attrList, "writeIRQThreshold", 0x8);
    icmAddUns32Attr(cpu1_jtag_uart_attrList, "console", 1);
    icmAddUns32Attr(cpu1_jtag_uart_attrList, "finishOnDisconnect", 1);

    handles.cpu1_jtag_uart_p = icmNewPSE(
        "cpu1_jtag_uart"    ,   // name
        cpu1_jtag_uart_path ,   // model
        cpu1_jtag_uart_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu1_jtag_uart_p, handles.cpu1_iobus_b, "sp1", 0, 0x8a00, 0x8a07);

////////////////////////////////////////////////////////////////////////////////
//                             PSE cpu1_timer_10ms
////////////////////////////////////////////////////////////////////////////////


    const char *cpu1_timer_10ms_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "IntervalTimer32Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu1_timer_10ms_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu1_timer_10ms_attrList, "timeoutPeriod", 0x7a120);
    icmAddUns64Attr(cpu1_timer_10ms_attrList, "timerFrequency", 0x2faf080);

    handles.cpu1_timer_10ms_p = icmNewPSE(
        "cpu1_timer_10ms"   ,   // name
        cpu1_timer_10ms_path,   // model
        cpu1_timer_10ms_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu1_timer_10ms_p, handles.cpu1_iobus_b, "sp1", 0, 0x8900, 0x8917);

////////////////////////////////////////////////////////////////////////////////
//                             PSE cpu1_timestamp
////////////////////////////////////////////////////////////////////////////////


    const char *cpu1_timestamp_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "IntervalTimer32Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu1_timestamp_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu1_timestamp_attrList, "timeoutPeriod", 0x7a120);
    icmAddUns64Attr(cpu1_timestamp_attrList, "timerFrequency", 0x2faf080);

    handles.cpu1_timestamp_p = icmNewPSE(
        "cpu1_timestamp"    ,   // name
        cpu1_timestamp_path ,   // model
        cpu1_timestamp_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu1_timestamp_p, handles.cpu1_iobus_b, "sp1", 0, 0x0, 0x17);

////////////////////////////////////////////////////////////////////////////////
//                               PSE cpu1_fprint
////////////////////////////////////////////////////////////////////////////////


    const char *cpu1_fprint_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "fprint"            ,    // name
        "2.0"               ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu1_fprint_attrList = icmNewAttrList();
    icmAddUns32Attr(cpu1_fprint_attrList, "coreID", 1);

    handles.cpu1_fprint_p = icmNewPSE(
        "cpu1_fprint"       ,   // name
        cpu1_fprint_path    ,   // model
        cpu1_fprint_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu1_fprint_p, handles.compbus_b, "FPRINT_MASTER", 1, 0x0, 0xfffffff);

    icmConnectPSEBus( handles.cpu1_fprint_p, handles.cpu1_iobus_b, "FPRINT_CSR_SLAVE", 0, 0x100000, 0x10013f);

    icmConnectPSEBus( handles.cpu1_fprint_p, handles.cpu1_tlb_fprint_bus_b, "FPRINT_TLB_SLAVE", 0, 0x0, 0x3ff);

////////////////////////////////////////////////////////////////////////////////
//                             PSE cpum_jtag_uart
////////////////////////////////////////////////////////////////////////////////


    const char *cpum_jtag_uart_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "altera.ovpworld.org",    // vendor
        0                   ,    // library
        "JtagUart"          ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpum_jtag_uart_attrList = icmNewAttrList();
    icmAddUns64Attr(cpum_jtag_uart_attrList, "readBufferDepth", 0x40);
    icmAddUns64Attr(cpum_jtag_uart_attrList, "readIRQThreshold", 0x8);
    icmAddUns64Attr(cpum_jtag_uart_attrList, "writeBufferDepth", 0x40);
    icmAddUns64Attr(cpum_jtag_uart_attrList, "writeIRQThreshold", 0x8);
    icmAddUns32Attr(cpum_jtag_uart_attrList, "console", 1);
    icmAddUns32Attr(cpum_jtag_uart_attrList, "finishOnDisconnect", 1);

    handles.cpum_jtag_uart_p = icmNewPSE(
        "cpum_jtag_uart"    ,   // name
        cpum_jtag_uart_path ,   // model
        cpum_jtag_uart_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpum_jtag_uart_p, handles.cpum_iobus_b, "sp1", 0, 0x1040, 0x1047);

////////////////////////////////////////////////////////////////////////////////
//                             PSE cpum_timer_10ms
////////////////////////////////////////////////////////////////////////////////


    const char *cpum_timer_10ms_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "IntervalTimer32Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpum_timer_10ms_attrList = icmNewAttrList();
    icmAddUns64Attr(cpum_timer_10ms_attrList, "timeoutPeriod", 0x7a120);
    icmAddUns64Attr(cpum_timer_10ms_attrList, "timerFrequency", 0x2faf080);

    handles.cpum_timer_10ms_p = icmNewPSE(
        "cpum_timer_10ms"   ,   // name
        cpum_timer_10ms_path,   // model
        cpum_timer_10ms_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpum_timer_10ms_p, handles.cpum_iobus_b, "sp1", 0, 0x1020, 0x1037);

////////////////////////////////////////////////////////////////////////////////
//                             PSE cpum_timestamp
////////////////////////////////////////////////////////////////////////////////


    const char *cpum_timestamp_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "IntervalTimer32Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpum_timestamp_attrList = icmNewAttrList();
    icmAddUns64Attr(cpum_timestamp_attrList, "timeoutPeriod", 0x7a120);
    icmAddUns64Attr(cpum_timestamp_attrList, "timerFrequency", 0x2faf080);

    handles.cpum_timestamp_p = icmNewPSE(
        "cpum_timestamp"    ,   // name
        cpum_timestamp_path ,   // model
        cpum_timestamp_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpum_timestamp_p, handles.cpum_iobus_b, "sp1", 0, 0x0, 0x17);

////////////////////////////////////////////////////////////////////////////////
//                                PSE cpu0_dma
////////////////////////////////////////////////////////////////////////////////


    const char *cpu0_dma_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "dma"               ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu0_dma_attrList = icmNewAttrList();

    handles.cpu0_dma_p = icmNewPSE(
        "cpu0_dma"          ,   // name
        cpu0_dma_path       ,   // model
        cpu0_dma_attrList   ,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu0_dma_p, handles.cpu0_dmabus_b, "MWRITE", 1, 0x0, 0xffffffff);

    icmConnectPSEBus( handles.cpu0_dma_p, handles.cpu0_dmabus_b, "MREAD", 1, 0x0, 0xffffffff);

    icmConnectPSEBus( handles.cpu0_dma_p, handles.cpum_dbus_b, "DMACSP", 0, 0x2100000, 0x210001f);

////////////////////////////////////////////////////////////////////////////////
//                                PSE cpu1_dma
////////////////////////////////////////////////////////////////////////////////


    const char *cpu1_dma_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "dma"               ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu1_dma_attrList = icmNewAttrList();

    handles.cpu1_dma_p = icmNewPSE(
        "cpu1_dma"          ,   // name
        cpu1_dma_path       ,   // model
        cpu1_dma_attrList   ,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu1_dma_p, handles.cpu1_dmabus_b, "MWRITE", 1, 0x0, 0xffffffff);

    icmConnectPSEBus( handles.cpu1_dma_p, handles.cpu1_dmabus_b, "MREAD", 1, 0x0, 0xffffffff);

    icmConnectPSEBus( handles.cpu1_dma_p, handles.cpum_dbus_b, "DMACSP", 0, 0x2110000, 0x211001f);
    icmSetPSEdiagnosticLevel(handles.cpu1_dma_p, 3);


////////////////////////////////////////////////////////////////////////////////
//                               PSE comparator
////////////////////////////////////////////////////////////////////////////////


    const char *comparator_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "comparator"        ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP comparator_attrList = icmNewAttrList();

    handles.comparator_p = icmNewPSE(
        "comparator"        ,   // name
        comparator_path     ,   // model
        comparator_attrList ,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.comparator_p, handles.compbus_b, "COMP_FPRINT_SLAVE", 0, 0x1000000, 0x1000fff);

    icmConnectPSEBus( handles.comparator_p, handles.cpum_dbus_b, "COMP_CSR_SLAVE", 0, 0x2400000, 0x2400fff);
    icmSetPSEdiagnosticLevel(handles.comparator_p, 3);


    icmConnectProcessorBusses( handles.cpu0_c, handles.cpu0_ibus_b, handles.cpu0_dbus_b );

    icmConnectProcessorBusses( handles.cpu1_c, handles.cpu1_ibus_b, handles.cpu1_dbus_b );

    icmConnectProcessorBusses( handles.cpum_c, handles.cpum_ibus_b, handles.cpum_dbus_b );

////////////////////////////////////////////////////////////////////////////////
//                            Memory shared_memory
////////////////////////////////////////////////////////////////////////////////

    handles.shared_memory_m = icmNewMemory("shared_memory", 0x7, 0x3ff);


    icmConnectMemoryToBus( handles.smbus_b, "sp1", handles.shared_memory_m, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                             Memory main_memory
////////////////////////////////////////////////////////////////////////////////

    handles.main_memory_m = icmNewMemory("main_memory", 0x7, 0x95fff);


    icmConnectMemoryToBus( handles.mmbus_b, "sp1", handles.main_memory_m, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                                Memory flash
////////////////////////////////////////////////////////////////////////////////

    handles.flash_m = icmNewMemory("flash", 0x7, 0x3fffff);


    icmConnectMemoryToBus( handles.flashbus_b, "sp1", handles.flash_m, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                            Memory cpu0_breakmem
////////////////////////////////////////////////////////////////////////////////

    handles.cpu0_breakmem_m = icmNewMemory("cpu0_breakmem", 0x7, 0xe0);


    icmConnectMemoryToBus( handles.cpu0_iobus_b, "sp1", handles.cpu0_breakmem_m, 0x8020);

////////////////////////////////////////////////////////////////////////////////
//                           Memory cpu0_scratchpad
////////////////////////////////////////////////////////////////////////////////

    handles.cpu0_scratchpad_m = icmNewMemory("cpu0_scratchpad", 0x7, 0x3fff);


    icmConnectMemoryToBus( handles.cpu0_tlbbus_b, "sp1", handles.cpu0_scratchpad_m, 0x4200000);

////////////////////////////////////////////////////////////////////////////////
//                            Memory cpu1_breakmem
////////////////////////////////////////////////////////////////////////////////

    handles.cpu1_breakmem_m = icmNewMemory("cpu1_breakmem", 0x7, 0xe0);


    icmConnectMemoryToBus( handles.cpu1_iobus_b, "sp1", handles.cpu1_breakmem_m, 0x8020);

////////////////////////////////////////////////////////////////////////////////
//                           Memory cpu1_scratchpad
////////////////////////////////////////////////////////////////////////////////

    handles.cpu1_scratchpad_m = icmNewMemory("cpu1_scratchpad", 0x7, 0x3fff);


    icmConnectMemoryToBus( handles.cpu1_tlbbus_b, "sp1", handles.cpu1_scratchpad_m, 0x4200000);

////////////////////////////////////////////////////////////////////////////////
//                            Memory cpum_breakmem
////////////////////////////////////////////////////////////////////////////////

    handles.cpum_breakmem_m = icmNewMemory("cpum_breakmem", 0x7, 0xe0);


    icmConnectMemoryToBus( handles.cpum_ibus_b, "sp1", handles.cpum_breakmem_m, 0x8008820);

////////////////////////////////////////////////////////////////////////////////
//                           Memory cpum_scratchpad
////////////////////////////////////////////////////////////////////////////////

    handles.cpum_scratchpad_m = icmNewMemory("cpum_scratchpad", 0x7, 0x3fff);


    icmConnectMemoryToBus( handles.cpum_iobus_b, "sp1", handles.cpum_scratchpad_m, 0x200000);

////////////////////////////////////////////////////////////////////////////////
//                            Bridge cpu0_d_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_dbus_b, handles.cpu0_tlbbus_in_b, "cpu0_d_bridge", "sp1", "mp1", 0x0, 0xffffffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                            Bridge cpu0_i_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_ibus_b, handles.cpu0_tlbbus_in_b, "cpu0_i_bridge", "sp1", "mp1", 0x0, 0xffffffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpu0_out_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_tlbbus_b, handles.smbus_b, "cpu0_out_bridge", "sp1", "mp1", 0x0, 0x3ff, 0x2500000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu0_mm_out_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_tlbbus_b, handles.mmbus_b, "cpu0_mm_out_bridge", "sp1", "mp1", 0x0, 0x95fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                              Bridge cpu0_mutex
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_tlbbus_b, handles.mutexbus_b, "cpu0_mutex", "sp1", "mp1", 0x0, 0x7, 0x2300000);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpu0_irq_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_tlbbus_b, handles.cpuirqbus_b, "cpu0_irq_bridge", "sp1", "mp1", 0x0, 0x23ff, 0x2200000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu0_flash_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_tlbbus_b, handles.flashbus_b, "cpu0_flash_bridge", "sp1", "mp1", 0x0, 0x3fffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu0_local_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_tlbbus_b, handles.cpu0_iobus_b, "cpu0_local_bridge", "sp1", "mp1", 0x0, 0x7fffff, 0x8000000);

////////////////////////////////////////////////////////////////////////////////
//                            Bridge cpu1_d_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_dbus_b, handles.cpu1_tlbbus_in_b, "cpu1_d_bridge", "sp1", "mp1", 0x0, 0xfffffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                            Bridge cpu1_i_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_ibus_b, handles.cpu1_tlbbus_in_b, "cpu1_i_bridge", "sp1", "mp1", 0x0, 0xfffffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpu1_out_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_tlbbus_b, handles.smbus_b, "cpu1_out_bridge", "sp1", "mp1", 0x0, 0x3ff, 0x2500000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu1_mm_out_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_tlbbus_b, handles.mmbus_b, "cpu1_mm_out_bridge", "sp1", "mp1", 0x0, 0x95fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                              Bridge cpu1_mutex
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_tlbbus_b, handles.mutexbus_b, "cpu1_mutex", "sp1", "mp1", 0x0, 0x7, 0x2300000);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpu1_irq_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_tlbbus_b, handles.cpuirqbus_b, "cpu1_irq_bridge", "sp1", "mp1", 0x0, 0x23ff, 0x2200000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu1_flash_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_tlbbus_b, handles.flashbus_b, "cpu1_flash_bridge", "sp1", "mp1", 0x0, 0x3fffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu1_local_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_tlbbus_b, handles.cpu1_iobus_b, "cpu1_local_bridge", "sp1", "mp1", 0x0, 0x7fffff, 0x8000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpum_out_bridge_d
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.smbus_b, "cpum_out_bridge_d", "sp1", "mp1", 0x0, 0x3ff, 0x2500000);

////////////////////////////////////////////////////////////////////////////////
//                         Bridge cpum_mm_out_bridge_i
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_ibus_b, handles.mmbus_b, "cpum_mm_out_bridge_i", "sp1", "mp1", 0x0, 0x95fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                         Bridge cpum_mm_out_bridge_d
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.mmbus_b, "cpum_mm_out_bridge_d", "sp1", "mp1", 0x0, 0x95fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                              Bridge cpum_mutex
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.mutexbus_b, "cpum_mutex", "sp1", "mp1", 0x0, 0x7, 0x2300000);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpum_irq_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.cpuirqbus_b, "cpum_irq_bridge", "sp1", "mp1", 0x0, 0x23ff, 0x2200000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpum_flash_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.flashbus_b, "cpum_flash_bridge", "sp1", "mp1", 0x0, 0x3fffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpum_local_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.cpum_iobus_b, "cpum_local_bridge", "sp1", "mp1", 0x0, 0x7fffff, 0x8000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu0_dmasp_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_dmabus_b, handles.cpu0_tlbbus_b, "cpu0_dmasp_bridge", "sp1", "mp1", 0x4000000, 0x87fffff, 0x4000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu0_dmamm_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_dmabus_b, handles.mmbus_b, "cpu0_dmamm_bridge", "sp1", "mp1", 0x0, 0xf9fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu1_dmasp_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_dmabus_b, handles.cpu1_tlbbus_b, "cpu1_dmasp_bridge", "sp1", "mp1", 0x4000000, 0x87fffff, 0x4000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu1_dmamm_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_dmabus_b, handles.mmbus_b, "cpu1_dmamm_bridge", "sp1", "mp1", 0x0, 0xf9fff, 0x400000);


////////////////////////////////////////////////////////////////////////////////
//                                 CONNECTIONS
////////////////////////////////////////////////////////////////////////////////

    handles.cpu0_irq4_n = icmNewNet("handles.cpu0_irq4_n" );

    icmConnectProcessorNet( handles.cpu0_c, handles.cpu0_irq4_n, "d_irq4", 0);

    icmConnectPSENet( handles.cpu0_jtag_uart_p, handles.cpu0_irq4_n, "irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpu0_irq0_n = icmNewNet("handles.cpu0_irq0_n" );

    icmConnectProcessorNet( handles.cpu0_c, handles.cpu0_irq0_n, "d_irq0", 0);

    icmConnectPSENet( handles.cpu0_timer_10ms_p, handles.cpu0_irq0_n, "irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpu0_irq1_n = icmNewNet("handles.cpu0_irq1_n" );

    icmConnectProcessorNet( handles.cpu0_c, handles.cpu0_irq1_n, "d_irq1", 0);

    icmConnectPSENet( handles.cpu_irq_p, handles.cpu0_irq1_n, "cpu0_irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.fprint_write_0_n = icmNewNet("handles.fprint_write_0_n" );

    icmConnectProcessorNet( handles.cpu0_c, handles.fprint_write_0_n, "fprint_write_out", 0);

    icmConnectPSENet( handles.cpu0_fprint_p, handles.fprint_write_0_n, "FPRINT_WRITE", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.fprint_write_address_0_n = icmNewNet("handles.fprint_write_address_0_n" );

    icmConnectProcessorNet( handles.cpu0_c, handles.fprint_write_address_0_n, "fprint_write_out_address", 0);

    icmConnectPSENet( handles.cpu0_fprint_p, handles.fprint_write_address_0_n, "FPRINT_WRITE_ADDRESS", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.fprint_write_data_0_n = icmNewNet("handles.fprint_write_data_0_n" );

    icmConnectProcessorNet( handles.cpu0_c, handles.fprint_write_data_0_n, "fprint_write_out_data", 0);

    icmConnectPSENet( handles.cpu0_fprint_p, handles.fprint_write_data_0_n, "FPRINT_WRITE_DATA", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpu1_irq3_n = icmNewNet("handles.cpu1_irq3_n" );

    icmConnectProcessorNet( handles.cpu1_c, handles.cpu1_irq3_n, "d_irq3", 0);

    icmConnectPSENet( handles.cpu1_jtag_uart_p, handles.cpu1_irq3_n, "irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpu1_irq0_n = icmNewNet("handles.cpu1_irq0_n" );

    icmConnectProcessorNet( handles.cpu1_c, handles.cpu1_irq0_n, "d_irq0", 0);

    icmConnectPSENet( handles.cpu1_timer_10ms_p, handles.cpu1_irq0_n, "irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpu1_irq1_n = icmNewNet("handles.cpu1_irq1_n" );

    icmConnectProcessorNet( handles.cpu1_c, handles.cpu1_irq1_n, "d_irq1", 0);

    icmConnectPSENet( handles.cpu_irq_p, handles.cpu1_irq1_n, "cpu1_irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.fprint_write_1_n = icmNewNet("handles.fprint_write_1_n" );

    icmConnectProcessorNet( handles.cpu1_c, handles.fprint_write_1_n, "fprint_write_out", 0);

    icmConnectPSENet( handles.cpu1_fprint_p, handles.fprint_write_1_n, "FPRINT_WRITE", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.fprint_write_address_1_n = icmNewNet("handles.fprint_write_address_1_n" );

    icmConnectProcessorNet( handles.cpu1_c, handles.fprint_write_address_1_n, "fprint_write_out_address", 0);

    icmConnectPSENet( handles.cpu1_fprint_p, handles.fprint_write_address_1_n, "FPRINT_WRITE_ADDRESS", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.fprint_write_data_1_n = icmNewNet("handles.fprint_write_data_1_n" );

    icmConnectProcessorNet( handles.cpu1_c, handles.fprint_write_data_1_n, "fprint_write_out_data", 0);

    icmConnectPSENet( handles.cpu1_fprint_p, handles.fprint_write_data_1_n, "FPRINT_WRITE_DATA", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_irq4_n = icmNewNet("handles.cpum_irq4_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_irq4_n, "d_irq4", 0);

    icmConnectPSENet( handles.cpum_jtag_uart_p, handles.cpum_irq4_n, "irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_irq0_n = icmNewNet("handles.cpum_irq0_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_irq0_n, "d_irq0", 0);

    icmConnectPSENet( handles.cpum_timer_10ms_p, handles.cpum_irq0_n, "irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_irq5_n = icmNewNet("handles.cpum_irq5_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_irq5_n, "d_irq5", 0);

    icmConnectPSENet( handles.cpu0_dma_p, handles.cpum_irq5_n, "DMA_IRQ", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_irq6_n = icmNewNet("handles.cpum_irq6_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_irq6_n, "d_irq6", 0);

    icmConnectPSENet( handles.cpu1_dma_p, handles.cpum_irq6_n, "DMA_IRQ", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_irq3_n = icmNewNet("handles.cpum_irq3_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_irq3_n, "d_irq3", 0);

    icmConnectPSENet( handles.comparator_p, handles.cpum_irq3_n, "COMP_IRQ", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_irq1_n = icmNewNet("handles.cpum_irq1_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_irq1_n, "d_irq1", 0);

    icmConnectPSENet( handles.cpu_irq_p, handles.cpum_irq1_n, "cpum_irq", 0);
}
