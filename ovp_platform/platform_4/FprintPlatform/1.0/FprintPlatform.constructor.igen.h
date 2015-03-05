
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Wed Mar  4 19:09:17 2015
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
//                                Bus cpu0_dbus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu0_dbus_b = icmNewBus( "cpu0_dbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpu0_ibus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu0_ibus_b = icmNewBus( "cpu0_ibus", 32);

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
//                               Bus cpu2_iobus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu2_iobus_b = icmNewBus( "cpu2_iobus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpu2_dbus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu2_dbus_b = icmNewBus( "cpu2_dbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpu2_ibus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu2_ibus_b = icmNewBus( "cpu2_ibus", 32);

////////////////////////////////////////////////////////////////////////////////
//                               Bus cpu3_iobus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu3_iobus_b = icmNewBus( "cpu3_iobus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpu3_dbus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu3_dbus_b = icmNewBus( "cpu3_dbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpu3_ibus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu3_ibus_b = icmNewBus( "cpu3_ibus", 32);

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
//                               Bus cpu2_dmabus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu2_dmabus_b = icmNewBus( "cpu2_dmabus", 32);

////////////////////////////////////////////////////////////////////////////////
//                               Bus cpu3_dmabus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu3_dmabus_b = icmNewBus( "cpu3_dmabus", 32);

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
//                               Processor cpu2
////////////////////////////////////////////////////////////////////////////////


    const char *cpu2_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "nios_ii"           ,    // name
        "1.0"               ,    // version
        "model"                  // model
    );

    icmAttrListP cpu2_attrList = icmNewAttrList();
    icmAddStringAttr(cpu2_attrList, "variant", "Nios_II_S");
    icmAddStringAttr(cpu2_attrList, "TEST_HALT_EXIT", "1");
    icmAddUns64Attr(cpu2_attrList, "INST_ADDR_WIDTH", 0x1c);
    icmAddUns64Attr(cpu2_attrList, "DATA_ADDR_WIDTH", 0x1c);
    icmAddStringAttr(cpu2_attrList, "HW_MULTIPLY", "0");
    icmAddStringAttr(cpu2_attrList, "HW_MULX", "0");
    icmAddStringAttr(cpu2_attrList, "HW_DIVIDE", "0");
    icmAddUns64Attr(cpu2_attrList, "RESET_VECTOR", 0x496000);
    icmAddUns64Attr(cpu2_attrList, "EXCEPTION_VECTOR", 0x496020);
    icmAddUns64Attr(cpu2_attrList, "BREAK_VECTOR", 0x8000820);
    icmAddStringAttr(cpu2_attrList, "INCLUDE_MMU", "0");
    icmAddDoubleAttr(cpu2_attrList, "mips", 125.000000);
    icmAddStringAttr(cpu2_attrList, "endian", "little");
    icmNewProcAttrs cpu2_attrs = ICM_ATTR_SIMEX;


    handles.cpu2_c = icmNewProcessor(
        "cpu2"              ,   // name
        "nios_ii"           ,   // type
        2                   ,   // cpuId
        0x0000              ,  // flags
        32                  ,   // address bits
        cpu2_path           ,   // model
        0,       // (unused)
        cpu2_attrs          ,   // procAttrs
        cpu2_attrList       ,   // attrlist
        0                   ,   // semihost file
        0        // (unused)
    );


////////////////////////////////////////////////////////////////////////////////
//                               Processor cpu3
////////////////////////////////////////////////////////////////////////////////


    const char *cpu3_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "altera.ovpworld.org",    // vendor
        0                   ,    // library
        "nios_ii"           ,    // name
        "1.0"               ,    // version
        "model"                  // model
    );

    icmAttrListP cpu3_attrList = icmNewAttrList();
    icmAddStringAttr(cpu3_attrList, "variant", "Nios_II_S");
    icmAddStringAttr(cpu3_attrList, "TEST_HALT_EXIT", "1");
    icmAddUns64Attr(cpu3_attrList, "INST_ADDR_WIDTH", 0x1c);
    icmAddUns64Attr(cpu3_attrList, "DATA_ADDR_WIDTH", 0x1c);
    icmAddStringAttr(cpu3_attrList, "HW_MULTIPLY", "0");
    icmAddStringAttr(cpu3_attrList, "HW_MULX", "0");
    icmAddStringAttr(cpu3_attrList, "HW_DIVIDE", "0");
    icmAddUns64Attr(cpu3_attrList, "RESET_VECTOR", 0x4c8000);
    icmAddUns64Attr(cpu3_attrList, "EXCEPTION_VECTOR", 0x4c8020);
    icmAddUns64Attr(cpu3_attrList, "BREAK_VECTOR", 0x8000820);
    icmAddStringAttr(cpu3_attrList, "INCLUDE_MMU", "0");
    icmAddDoubleAttr(cpu3_attrList, "mips", 125.000000);
    icmAddStringAttr(cpu3_attrList, "endian", "little");
    icmNewProcAttrs cpu3_attrs = ICM_ATTR_SIMEX;


    handles.cpu3_c = icmNewProcessor(
        "cpu3"              ,   // name
        "nios_ii"           ,   // type
        3                   ,   // cpuId
        0x0000              ,  // flags
        32                  ,   // address bits
        cpu3_path           ,   // model
        0,       // (unused)
        cpu3_attrs          ,   // procAttrs
        cpu3_attrList       ,   // attrlist
        0                   ,   // semihost file
        0        // (unused)
    );


////////////////////////////////////////////////////////////////////////////////
//                               Processor cpum
////////////////////////////////////////////////////////////////////////////////


    const char *cpum_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "altera.ovpworld.org",    // vendor
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
        4                   ,   // cpuId
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
        "1.0"               ,    // version
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
        "1.0"               ,    // version
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

////////////////////////////////////////////////////////////////////////////////
//                             PSE cpu2_jtag_uart
////////////////////////////////////////////////////////////////////////////////


    const char *cpu2_jtag_uart_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "altera.ovpworld.org",    // vendor
        0                   ,    // library
        "JtagUart"          ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu2_jtag_uart_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu2_jtag_uart_attrList, "readBufferDepth", 0x40);
    icmAddUns64Attr(cpu2_jtag_uart_attrList, "readIRQThreshold", 0x8);
    icmAddUns64Attr(cpu2_jtag_uart_attrList, "writeBufferDepth", 0x40);
    icmAddUns64Attr(cpu2_jtag_uart_attrList, "writeIRQThreshold", 0x8);
    icmAddUns32Attr(cpu2_jtag_uart_attrList, "console", 1);
    icmAddUns32Attr(cpu2_jtag_uart_attrList, "finishOnDisconnect", 1);

    handles.cpu2_jtag_uart_p = icmNewPSE(
        "cpu2_jtag_uart"    ,   // name
        cpu2_jtag_uart_path ,   // model
        cpu2_jtag_uart_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu2_jtag_uart_p, handles.cpu2_iobus_b, "sp1", 0, 0x1040, 0x1047);

////////////////////////////////////////////////////////////////////////////////
//                             PSE cpu2_timer_10ms
////////////////////////////////////////////////////////////////////////////////


    const char *cpu2_timer_10ms_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "IntervalTimer32Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu2_timer_10ms_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu2_timer_10ms_attrList, "timeoutPeriod", 0x7a120);
    icmAddUns64Attr(cpu2_timer_10ms_attrList, "timerFrequency", 0x2faf080);

    handles.cpu2_timer_10ms_p = icmNewPSE(
        "cpu2_timer_10ms"   ,   // name
        cpu2_timer_10ms_path,   // model
        cpu2_timer_10ms_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu2_timer_10ms_p, handles.cpu2_iobus_b, "sp1", 0, 0x1020, 0x1037);

////////////////////////////////////////////////////////////////////////////////
//                             PSE cpu2_timestamp
////////////////////////////////////////////////////////////////////////////////


    const char *cpu2_timestamp_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "IntervalTimer32Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu2_timestamp_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu2_timestamp_attrList, "timeoutPeriod", 0x7a120);
    icmAddUns64Attr(cpu2_timestamp_attrList, "timerFrequency", 0x2faf080);

    handles.cpu2_timestamp_p = icmNewPSE(
        "cpu2_timestamp"    ,   // name
        cpu2_timestamp_path ,   // model
        cpu2_timestamp_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu2_timestamp_p, handles.cpu2_iobus_b, "sp1", 0, 0x0, 0x17);

////////////////////////////////////////////////////////////////////////////////
//                               PSE cpu2_fprint
////////////////////////////////////////////////////////////////////////////////


    const char *cpu2_fprint_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "fprint"            ,    // name
        "1.0"               ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu2_fprint_attrList = icmNewAttrList();
    icmAddUns32Attr(cpu2_fprint_attrList, "coreID", 2);

    handles.cpu2_fprint_p = icmNewPSE(
        "cpu2_fprint"       ,   // name
        cpu2_fprint_path    ,   // model
        cpu2_fprint_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu2_fprint_p, handles.compbus_b, "FPRINT_MASTER", 1, 0x0, 0xfffffff);

    icmConnectPSEBus( handles.cpu2_fprint_p, handles.cpu2_iobus_b, "FPRINT_CSR_SLAVE", 0, 0x100000, 0x10013f);

////////////////////////////////////////////////////////////////////////////////
//                             PSE cpu3_jtag_uart
////////////////////////////////////////////////////////////////////////////////


    const char *cpu3_jtag_uart_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "altera.ovpworld.org",    // vendor
        0                   ,    // library
        "JtagUart"          ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu3_jtag_uart_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu3_jtag_uart_attrList, "readBufferDepth", 0x40);
    icmAddUns64Attr(cpu3_jtag_uart_attrList, "readIRQThreshold", 0x8);
    icmAddUns64Attr(cpu3_jtag_uart_attrList, "writeBufferDepth", 0x40);
    icmAddUns64Attr(cpu3_jtag_uart_attrList, "writeIRQThreshold", 0x8);
    icmAddUns32Attr(cpu3_jtag_uart_attrList, "console", 1);
    icmAddUns32Attr(cpu3_jtag_uart_attrList, "finishOnDisconnect", 1);

    handles.cpu3_jtag_uart_p = icmNewPSE(
        "cpu3_jtag_uart"    ,   // name
        cpu3_jtag_uart_path ,   // model
        cpu3_jtag_uart_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu3_jtag_uart_p, handles.cpu3_iobus_b, "sp1", 0, 0x1040, 0x1047);

////////////////////////////////////////////////////////////////////////////////
//                             PSE cpu3_timer_10ms
////////////////////////////////////////////////////////////////////////////////


    const char *cpu3_timer_10ms_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "IntervalTimer32Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu3_timer_10ms_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu3_timer_10ms_attrList, "timeoutPeriod", 0x7a120);
    icmAddUns64Attr(cpu3_timer_10ms_attrList, "timerFrequency", 0x2faf080);

    handles.cpu3_timer_10ms_p = icmNewPSE(
        "cpu3_timer_10ms"   ,   // name
        cpu3_timer_10ms_path,   // model
        cpu3_timer_10ms_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu3_timer_10ms_p, handles.cpu3_iobus_b, "sp1", 0, 0x1020, 0x1037);

////////////////////////////////////////////////////////////////////////////////
//                             PSE cpu3_timestamp
////////////////////////////////////////////////////////////////////////////////


    const char *cpu3_timestamp_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "IntervalTimer32Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu3_timestamp_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu3_timestamp_attrList, "timeoutPeriod", 0x7a120);
    icmAddUns64Attr(cpu3_timestamp_attrList, "timerFrequency", 0x2faf080);

    handles.cpu3_timestamp_p = icmNewPSE(
        "cpu3_timestamp"    ,   // name
        cpu3_timestamp_path ,   // model
        cpu3_timestamp_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu3_timestamp_p, handles.cpu3_iobus_b, "sp1", 0, 0x0, 0x17);

////////////////////////////////////////////////////////////////////////////////
//                               PSE cpu3_fprint
////////////////////////////////////////////////////////////////////////////////


    const char *cpu3_fprint_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "fprint"            ,    // name
        "1.0"               ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu3_fprint_attrList = icmNewAttrList();
    icmAddUns32Attr(cpu3_fprint_attrList, "coreID", 3);

    handles.cpu3_fprint_p = icmNewPSE(
        "cpu3_fprint"       ,   // name
        cpu3_fprint_path    ,   // model
        cpu3_fprint_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu3_fprint_p, handles.compbus_b, "FPRINT_MASTER", 1, 0x0, 0xfffffff);

    icmConnectPSEBus( handles.cpu3_fprint_p, handles.cpu3_iobus_b, "FPRINT_CSR_SLAVE", 0, 0x100000, 0x10013f);

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

    icmConnectPSEBus( handles.cpu0_dma_p, handles.cpum_dbus_b, "DMACSP", 0, 0x5100000, 0x510001f);
    icmSetPSEdiagnosticLevel(handles.cpu0_dma_p, 3);


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

    icmConnectPSEBus( handles.cpu1_dma_p, handles.cpum_dbus_b, "DMACSP", 0, 0x5110000, 0x511001f);
    icmSetPSEdiagnosticLevel(handles.cpu1_dma_p, 3);


////////////////////////////////////////////////////////////////////////////////
//                                PSE cpu2_dma
////////////////////////////////////////////////////////////////////////////////


    const char *cpu2_dma_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "dma"               ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu2_dma_attrList = icmNewAttrList();

    handles.cpu2_dma_p = icmNewPSE(
        "cpu2_dma"          ,   // name
        cpu2_dma_path       ,   // model
        cpu2_dma_attrList   ,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu2_dma_p, handles.cpu2_dmabus_b, "MWRITE", 1, 0x0, 0xffffffff);

    icmConnectPSEBus( handles.cpu2_dma_p, handles.cpu2_dmabus_b, "MREAD", 1, 0x0, 0xffffffff);

    icmConnectPSEBus( handles.cpu2_dma_p, handles.cpum_dbus_b, "DMACSP", 0, 0x5120000, 0x512001f);
    icmSetPSEdiagnosticLevel(handles.cpu2_dma_p, 3);


////////////////////////////////////////////////////////////////////////////////
//                                PSE cpu3_dma
////////////////////////////////////////////////////////////////////////////////


    const char *cpu3_dma_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "dma"               ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu3_dma_attrList = icmNewAttrList();

    handles.cpu3_dma_p = icmNewPSE(
        "cpu3_dma"          ,   // name
        cpu3_dma_path       ,   // model
        cpu3_dma_attrList   ,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu3_dma_p, handles.cpu3_dmabus_b, "MWRITE", 1, 0x0, 0xffffffff);

    icmConnectPSEBus( handles.cpu3_dma_p, handles.cpu3_dmabus_b, "MREAD", 1, 0x0, 0xffffffff);

    icmConnectPSEBus( handles.cpu3_dma_p, handles.cpum_dbus_b, "DMACSP", 0, 0x5130000, 0x513001f);
    icmSetPSEdiagnosticLevel(handles.cpu3_dma_p, 3);


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

    icmConnectPSEBus( handles.comparator_p, handles.cpum_dbus_b, "COMP_CSR_SLAVE", 0, 0x5000000, 0x5000fff);
    icmSetPSEdiagnosticLevel(handles.comparator_p, 3);


    icmConnectProcessorBusses( handles.cpu0_c, handles.cpu0_ibus_b, handles.cpu0_dbus_b );

    icmConnectProcessorBusses( handles.cpu1_c, handles.cpu1_ibus_b, handles.cpu1_dbus_b );

    icmConnectProcessorBusses( handles.cpu2_c, handles.cpu2_ibus_b, handles.cpu2_dbus_b );

    icmConnectProcessorBusses( handles.cpu3_c, handles.cpu3_ibus_b, handles.cpu3_dbus_b );

    icmConnectProcessorBusses( handles.cpum_c, handles.cpum_ibus_b, handles.cpum_dbus_b );

////////////////////////////////////////////////////////////////////////////////
//                            Memory shared_memory
////////////////////////////////////////////////////////////////////////////////

    handles.shared_memory_m = icmNewMemory("shared_memory", 0x7, 0x3ff);


    icmConnectMemoryToBus( handles.smbus_b, "sp1", handles.shared_memory_m, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                             Memory main_memory
////////////////////////////////////////////////////////////////////////////////

    handles.main_memory_m = icmNewMemory("main_memory", 0x7, 0x128000);


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


    icmConnectMemoryToBus( handles.cpu0_ibus_b, "sp1", handles.cpu0_breakmem_m, 0x8008020);

////////////////////////////////////////////////////////////////////////////////
//                           Memory cpu0_scratchpad
////////////////////////////////////////////////////////////////////////////////

    handles.cpu0_scratchpad_m = icmNewMemory("cpu0_scratchpad", 0x7, 0x3fff);


    icmConnectMemoryToBus( handles.cpu0_iobus_b, "sp1", handles.cpu0_scratchpad_m, 0x200000);

////////////////////////////////////////////////////////////////////////////////
//                            Memory cpu1_breakmem
////////////////////////////////////////////////////////////////////////////////

    handles.cpu1_breakmem_m = icmNewMemory("cpu1_breakmem", 0x7, 0xe0);


    icmConnectMemoryToBus( handles.cpu1_ibus_b, "sp1", handles.cpu1_breakmem_m, 0x8008020);

////////////////////////////////////////////////////////////////////////////////
//                           Memory cpu1_scratchpad
////////////////////////////////////////////////////////////////////////////////

    handles.cpu1_scratchpad_m = icmNewMemory("cpu1_scratchpad", 0x7, 0x3fff);


    icmConnectMemoryToBus( handles.cpu1_iobus_b, "sp1", handles.cpu1_scratchpad_m, 0x200000);

////////////////////////////////////////////////////////////////////////////////
//                            Memory cpu2_breakmem
////////////////////////////////////////////////////////////////////////////////

    handles.cpu2_breakmem_m = icmNewMemory("cpu2_breakmem", 0x7, 0xe0);


    icmConnectMemoryToBus( handles.cpu2_ibus_b, "sp1", handles.cpu2_breakmem_m, 0x8008020);

////////////////////////////////////////////////////////////////////////////////
//                           Memory cpu2_scratchpad
////////////////////////////////////////////////////////////////////////////////

    handles.cpu2_scratchpad_m = icmNewMemory("cpu2_scratchpad", 0x7, 0x3fff);


    icmConnectMemoryToBus( handles.cpu2_iobus_b, "sp1", handles.cpu2_scratchpad_m, 0x200000);

////////////////////////////////////////////////////////////////////////////////
//                            Memory cpu3_breakmem
////////////////////////////////////////////////////////////////////////////////

    handles.cpu3_breakmem_m = icmNewMemory("cpu3_breakmem", 0x7, 0xe0);


    icmConnectMemoryToBus( handles.cpu3_ibus_b, "sp1", handles.cpu3_breakmem_m, 0x8008020);

////////////////////////////////////////////////////////////////////////////////
//                           Memory cpu3_scratchpad
////////////////////////////////////////////////////////////////////////////////

    handles.cpu3_scratchpad_m = icmNewMemory("cpu3_scratchpad", 0x7, 0x3fff);


    icmConnectMemoryToBus( handles.cpu3_iobus_b, "sp1", handles.cpu3_scratchpad_m, 0x200000);

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
//                          Bridge cpu0_out_bridge_d
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_dbus_b, handles.smbus_b, "cpu0_out_bridge_d", "sp1", "mp1", 0x0, 0x3ff, 0x3000000);

////////////////////////////////////////////////////////////////////////////////
//                         Bridge cpu0_mm_out_bridge_i
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_ibus_b, handles.mmbus_b, "cpu0_mm_out_bridge_i", "sp1", "mp1", 0x0, 0x127fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                         Bridge cpu0_mm_out_bridge_d
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_dbus_b, handles.mmbus_b, "cpu0_mm_out_bridge_d", "sp1", "mp1", 0x0, 0x127fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                              Bridge cpu0_mutex
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_dbus_b, handles.mutexbus_b, "cpu0_mutex", "sp1", "mp1", 0x0, 0x7, 0x4500000);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpu0_irq_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_dbus_b, handles.cpuirqbus_b, "cpu0_irq_bridge", "sp1", "mp1", 0x0, 0x23ff, 0x4000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu0_flash_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_dbus_b, handles.flashbus_b, "cpu0_flash_bridge", "sp1", "mp1", 0x0, 0x3fffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu0_local_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_dbus_b, handles.cpu0_iobus_b, "cpu0_local_bridge", "sp1", "mp1", 0x0, 0x7fffff, 0x8000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu1_out_bridge_d
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_dbus_b, handles.smbus_b, "cpu1_out_bridge_d", "sp1", "mp1", 0x0, 0x3ff, 0x3000000);

////////////////////////////////////////////////////////////////////////////////
//                         Bridge cpu1_mm_out_bridge_i
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_ibus_b, handles.mmbus_b, "cpu1_mm_out_bridge_i", "sp1", "mp1", 0x0, 0x127fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                         Bridge cpu1_mm_out_bridge_d
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_dbus_b, handles.mmbus_b, "cpu1_mm_out_bridge_d", "sp1", "mp1", 0x0, 0x127fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                              Bridge cpu1_mutex
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_dbus_b, handles.mutexbus_b, "cpu1_mutex", "sp1", "mp1", 0x0, 0x7, 0x4500000);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpu1_irq_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_dbus_b, handles.cpuirqbus_b, "cpu1_irq_bridge", "sp1", "mp1", 0x0, 0x23ff, 0x4000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu1_flash_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_dbus_b, handles.flashbus_b, "cpu1_flash_bridge", "sp1", "mp1", 0x0, 0x3fffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu1_local_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_dbus_b, handles.cpu1_iobus_b, "cpu1_local_bridge", "sp1", "mp1", 0x0, 0x7fffff, 0x8000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu2_out_bridge_d
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_dbus_b, handles.smbus_b, "cpu2_out_bridge_d", "sp1", "mp1", 0x0, 0x3ff, 0x3000000);

////////////////////////////////////////////////////////////////////////////////
//                         Bridge cpu2_mm_out_bridge_i
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_ibus_b, handles.mmbus_b, "cpu2_mm_out_bridge_i", "sp1", "mp1", 0x0, 0x127fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                         Bridge cpu2_mm_out_bridge_d
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_dbus_b, handles.mmbus_b, "cpu2_mm_out_bridge_d", "sp1", "mp1", 0x0, 0x127fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                              Bridge cpu2_mutex
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_dbus_b, handles.mutexbus_b, "cpu2_mutex", "sp1", "mp1", 0x0, 0x7, 0x4500000);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpu2_irq_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_dbus_b, handles.cpuirqbus_b, "cpu2_irq_bridge", "sp1", "mp1", 0x0, 0x23ff, 0x4000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu2_flash_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_dbus_b, handles.flashbus_b, "cpu2_flash_bridge", "sp1", "mp1", 0x0, 0x3fffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu2_local_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_dbus_b, handles.cpu2_iobus_b, "cpu2_local_bridge", "sp1", "mp1", 0x0, 0x7fffff, 0x8000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu3_out_bridge_d
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_dbus_b, handles.smbus_b, "cpu3_out_bridge_d", "sp1", "mp1", 0x0, 0x3ff, 0x3000000);

////////////////////////////////////////////////////////////////////////////////
//                         Bridge cpu3_mm_out_bridge_i
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_ibus_b, handles.mmbus_b, "cpu3_mm_out_bridge_i", "sp1", "mp1", 0x0, 0x127fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                         Bridge cpu3_mm_out_bridge_d
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_dbus_b, handles.mmbus_b, "cpu3_mm_out_bridge_d", "sp1", "mp1", 0x0, 0x127fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                              Bridge cpu3_mutex
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_dbus_b, handles.mutexbus_b, "cpu3_mutex", "sp1", "mp1", 0x0, 0x7, 0x4500000);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpu3_irq_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_dbus_b, handles.cpuirqbus_b, "cpu3_irq_bridge", "sp1", "mp1", 0x0, 0x23ff, 0x4000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu3_flash_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_dbus_b, handles.flashbus_b, "cpu3_flash_bridge", "sp1", "mp1", 0x0, 0x3fffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu3_local_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_dbus_b, handles.cpu3_iobus_b, "cpu3_local_bridge", "sp1", "mp1", 0x0, 0x7fffff, 0x8000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpum_out_bridge_d
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.smbus_b, "cpum_out_bridge_d", "sp1", "mp1", 0x0, 0x3ff, 0x3000000);

////////////////////////////////////////////////////////////////////////////////
//                         Bridge cpum_mm_out_bridge_i
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_ibus_b, handles.mmbus_b, "cpum_mm_out_bridge_i", "sp1", "mp1", 0x0, 0x127fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                         Bridge cpum_mm_out_bridge_d
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.mmbus_b, "cpum_mm_out_bridge_d", "sp1", "mp1", 0x0, 0x127fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                              Bridge cpum_mutex
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.mutexbus_b, "cpum_mutex", "sp1", "mp1", 0x0, 0x7, 0x4500000);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpum_irq_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.cpuirqbus_b, "cpum_irq_bridge", "sp1", "mp1", 0x0, 0x23ff, 0x4000000);

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


    icmNewBusBridge(handles.cpu0_dmabus_b, handles.cpu0_iobus_b, "cpu0_dmasp_bridge", "sp1", "mp1", 0x0, 0x7fffff, 0x8000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu0_dmamm_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_dmabus_b, handles.mmbus_b, "cpu0_dmamm_bridge", "sp1", "mp1", 0x0, 0xf9fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu1_dmasp_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_dmabus_b, handles.cpu1_iobus_b, "cpu1_dmasp_bridge", "sp1", "mp1", 0x0, 0x7fffff, 0x8000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu1_dmamm_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_dmabus_b, handles.mmbus_b, "cpu1_dmamm_bridge", "sp1", "mp1", 0x0, 0xf9fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu2_dmasp_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_dmabus_b, handles.cpu2_iobus_b, "cpu2_dmasp_bridge", "sp1", "mp1", 0x0, 0x7fffff, 0x8000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu2_dmamm_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_dmabus_b, handles.mmbus_b, "cpu2_dmamm_bridge", "sp1", "mp1", 0x0, 0xf9fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu3_dmasp_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_dmabus_b, handles.cpu3_iobus_b, "cpu3_dmasp_bridge", "sp1", "mp1", 0x0, 0x7fffff, 0x8000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu3_dmamm_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_dmabus_b, handles.mmbus_b, "cpu3_dmamm_bridge", "sp1", "mp1", 0x0, 0xf9fff, 0x400000);

    icmLoadProcessorMemory(handles.cpu0_c, "../../fprint_fpga/quartus/full_system_arria_4/software/ucos0/ucos0.elf", 0, 0, 1);

    icmLoadProcessorMemory(handles.cpu1_c, "../../fprint_fpga/quartus/full_system_arria_4/software/ucos1/ucos1.elf", 0, 0, 1);

    icmLoadProcessorMemory(handles.cpu2_c, "../../fprint_fpga/quartus/full_system_arria_4/software/ucos2/ucos2.elf", 0, 0, 1);

    icmLoadProcessorMemory(handles.cpu3_c, "../../fprint_fpga/quartus/full_system_arria_4/software/ucos3/ucos3.elf", 0, 0, 1);

    icmLoadProcessorMemory(handles.cpum_c, "../../fprint_fpga/quartus/full_system_arria_4/software/ucosMon/ucosMon.elf", 0, 0, 1);


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
    handles.cpu2_irq4_n = icmNewNet("handles.cpu2_irq4_n" );

    icmConnectProcessorNet( handles.cpu2_c, handles.cpu2_irq4_n, "d_irq4", 0);

    icmConnectPSENet( handles.cpu2_jtag_uart_p, handles.cpu2_irq4_n, "irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpu2_irq0_n = icmNewNet("handles.cpu2_irq0_n" );

    icmConnectProcessorNet( handles.cpu2_c, handles.cpu2_irq0_n, "d_irq0", 0);

    icmConnectPSENet( handles.cpu2_timer_10ms_p, handles.cpu2_irq0_n, "irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpu2_irq1_n = icmNewNet("handles.cpu2_irq1_n" );

    icmConnectProcessorNet( handles.cpu2_c, handles.cpu2_irq1_n, "d_irq1", 0);

    icmConnectPSENet( handles.cpu_irq_p, handles.cpu2_irq1_n, "cpu2_irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.fprint_write_2_n = icmNewNet("handles.fprint_write_2_n" );

    icmConnectProcessorNet( handles.cpu2_c, handles.fprint_write_2_n, "fprint_write_out", 0);

    icmConnectPSENet( handles.cpu2_fprint_p, handles.fprint_write_2_n, "FPRINT_WRITE", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.fprint_write_address_2_n = icmNewNet("handles.fprint_write_address_2_n" );

    icmConnectProcessorNet( handles.cpu2_c, handles.fprint_write_address_2_n, "fprint_write_out_address", 0);

    icmConnectPSENet( handles.cpu2_fprint_p, handles.fprint_write_address_2_n, "FPRINT_WRITE_ADDRESS", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.fprint_write_data_2_n = icmNewNet("handles.fprint_write_data_2_n" );

    icmConnectProcessorNet( handles.cpu2_c, handles.fprint_write_data_2_n, "fprint_write_out_data", 0);

    icmConnectPSENet( handles.cpu2_fprint_p, handles.fprint_write_data_2_n, "FPRINT_WRITE_DATA", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpu3_irq4_n = icmNewNet("handles.cpu3_irq4_n" );

    icmConnectProcessorNet( handles.cpu3_c, handles.cpu3_irq4_n, "d_irq4", 0);

    icmConnectPSENet( handles.cpu3_jtag_uart_p, handles.cpu3_irq4_n, "irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpu3_irq0_n = icmNewNet("handles.cpu3_irq0_n" );

    icmConnectProcessorNet( handles.cpu3_c, handles.cpu3_irq0_n, "d_irq0", 0);

    icmConnectPSENet( handles.cpu3_timer_10ms_p, handles.cpu3_irq0_n, "irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpu3_irq1_n = icmNewNet("handles.cpu3_irq1_n" );

    icmConnectProcessorNet( handles.cpu3_c, handles.cpu3_irq1_n, "d_irq1", 0);

    icmConnectPSENet( handles.cpu_irq_p, handles.cpu3_irq1_n, "cpu3_irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.fprint_write_3_n = icmNewNet("handles.fprint_write_3_n" );

    icmConnectProcessorNet( handles.cpu3_c, handles.fprint_write_3_n, "fprint_write_out", 0);

    icmConnectPSENet( handles.cpu3_fprint_p, handles.fprint_write_3_n, "FPRINT_WRITE", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.fprint_write_address_3_n = icmNewNet("handles.fprint_write_address_3_n" );

    icmConnectProcessorNet( handles.cpu3_c, handles.fprint_write_address_3_n, "fprint_write_out_address", 0);

    icmConnectPSENet( handles.cpu3_fprint_p, handles.fprint_write_address_3_n, "FPRINT_WRITE_ADDRESS", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.fprint_write_data_3_n = icmNewNet("handles.fprint_write_data_3_n" );

    icmConnectProcessorNet( handles.cpu3_c, handles.fprint_write_data_3_n, "fprint_write_out_data", 0);

    icmConnectPSENet( handles.cpu3_fprint_p, handles.fprint_write_data_3_n, "FPRINT_WRITE_DATA", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_irq4_n = icmNewNet("handles.cpum_irq4_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_irq4_n, "d_irq4", 0);

    icmConnectPSENet( handles.cpum_jtag_uart_p, handles.cpum_irq4_n, "irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_irq0_n = icmNewNet("handles.cpum_irq0_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_irq0_n, "d_irq0", 0);

    icmConnectPSENet( handles.cpum_timer_10ms_p, handles.cpum_irq0_n, "irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_irq1_n = icmNewNet("handles.cpum_irq1_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_irq1_n, "d_irq1", 0);

    icmConnectPSENet( handles.cpu_irq_p, handles.cpum_irq1_n, "cpum_irq", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_irq5_n = icmNewNet("handles.cpum_irq5_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_irq5_n, "d_irq5", 0);

    icmConnectPSENet( handles.cpu0_dma_p, handles.cpum_irq5_n, "DMA_IRQ", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_irq6_n = icmNewNet("handles.cpum_irq6_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_irq6_n, "d_irq6", 0);

    icmConnectPSENet( handles.cpu1_dma_p, handles.cpum_irq6_n, "DMA_IRQ", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_irq7_n = icmNewNet("handles.cpum_irq7_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_irq7_n, "d_irq7", 0);

    icmConnectPSENet( handles.cpu2_dma_p, handles.cpum_irq7_n, "DMA_IRQ", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_irq8_n = icmNewNet("handles.cpum_irq8_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_irq8_n, "d_irq8", 0);

    icmConnectPSENet( handles.cpu3_dma_p, handles.cpum_irq8_n, "DMA_IRQ", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_irq3_n = icmNewNet("handles.cpum_irq3_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_irq3_n, "d_irq3", 0);

    icmConnectPSENet( handles.comparator_p, handles.cpum_irq3_n, "COMP_IRQ", 0);
}
