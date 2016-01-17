
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Sat Jan 16 18:19:25 2016
//
////////////////////////////////////////////////////////////////////////////////


// This file constructs the platform according to TCL script.
// This file should NOT need to be edited.

////////////////////////////////////////////////////////////////////////////////

void platformConstructor(void) {

    icmInitAttrs FprintPlatform_attrs = ICM_STOP_ON_CTRLC|ICM_VERBOSE;

    icmInitPlatform(ICM_VERSION, FprintPlatform_attrs, 0, 0, "FprintPlatform");

    icmDocNodeP doc1_node = icmDocSectionAdd(0, "Licensing");
    icmDocTextAdd(doc1_node, "Open Source Apache 2.0");
    icmDocNodeP doc_11_node = icmDocSectionAdd(0, "Description");
    icmDocTextAdd(doc_11_node, "This platform models the Altera Cyclone III 3c120.\n\nThe processor is an Altera Nios_II procesor, Nios_II_F.");
    icmDocNodeP doc_21_node = icmDocSectionAdd(0, "Limitations");
    icmDocTextAdd(doc_21_node, "Peripherals are modeled only to the extent required to boot and run Operating Systems such as Linux.");

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
//                               Bus swresetbus
////////////////////////////////////////////////////////////////////////////////


    handles.swresetbus_b = icmNewBus( "swresetbus", 32);

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
//                               Bus cpu2_iobus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu2_iobus_b = icmNewBus( "cpu2_iobus", 32);

////////////////////////////////////////////////////////////////////////////////
//                               Bus cpu2_tlbbus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu2_tlbbus_b = icmNewBus( "cpu2_tlbbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                             Bus cpu2_tlbbus_in
////////////////////////////////////////////////////////////////////////////////


    handles.cpu2_tlbbus_in_b = icmNewBus( "cpu2_tlbbus_in", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpu2_dbus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu2_dbus_b = icmNewBus( "cpu2_dbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpu2_ibus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu2_ibus_b = icmNewBus( "cpu2_ibus", 32);

////////////////////////////////////////////////////////////////////////////////
//                           Bus cpu2_tlb_fprint_bus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu2_tlb_fprint_bus_b = icmNewBus( "cpu2_tlb_fprint_bus", 32);

////////////////////////////////////////////////////////////////////////////////
//                               Bus cpu3_iobus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu3_iobus_b = icmNewBus( "cpu3_iobus", 32);

////////////////////////////////////////////////////////////////////////////////
//                               Bus cpu3_tlbbus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu3_tlbbus_b = icmNewBus( "cpu3_tlbbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                             Bus cpu3_tlbbus_in
////////////////////////////////////////////////////////////////////////////////


    handles.cpu3_tlbbus_in_b = icmNewBus( "cpu3_tlbbus_in", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpu3_dbus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu3_dbus_b = icmNewBus( "cpu3_dbus", 32);

////////////////////////////////////////////////////////////////////////////////
//                                Bus cpu3_ibus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu3_ibus_b = icmNewBus( "cpu3_ibus", 32);

////////////////////////////////////////////////////////////////////////////////
//                           Bus cpu3_tlb_fprint_bus
////////////////////////////////////////////////////////////////////////////////


    handles.cpu3_tlb_fprint_bus_b = icmNewBus( "cpu3_tlb_fprint_bus", 32);

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
//                               Bus resetmonbus
////////////////////////////////////////////////////////////////////////////////


    handles.resetmonbus_b = icmNewBus( "resetmonbus", 32);

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
    icmAddUns64Attr(cpu0_attrList, "BREAK_VECTOR", 0x08000820LL);
    icmAddUns64Attr(cpu0_attrList, "EXCEPTION_VECTOR", 0x00464020LL);
    icmAddUns64Attr(cpu0_attrList, "RESET_VECTOR", 0x00464000LL);
    icmAddUns64Attr(cpu0_attrList, "HW_DIVIDE", 1LL);
    icmAddUns64Attr(cpu0_attrList, "HW_MULTIPLY", 1LL);
    icmAddUns64Attr(cpu0_attrList, "HW_MULX", 1LL);
    icmAddUns64Attr(cpu0_attrList, "INCLUDE_MMU", 0LL);
    icmAddUns64Attr(cpu0_attrList, "DATA_ADDR_WIDTH", 0x1cLL);
    icmAddUns64Attr(cpu0_attrList, "INST_ADDR_WIDTH", 0x1cLL);
    icmAddUns64Attr(cpu0_attrList, "TEST_HALT_EXIT", 1LL);
    icmAddUns64Attr(cpu0_attrList, "INCLUDE_MPU", 1LL);
    icmAddUns64Attr(cpu0_attrList, "MPU_USE_LIMIT_FOR_REGION_RANGE", 1LL);
    icmAddUns64Attr(cpu0_attrList, "MPU_NUMBER_DATA_REGIONS", 16LL);
    icmAddUns64Attr(cpu0_attrList, "MPU_MINIMUM_DATA_REGION_SIZE", 64LL);
    icmAddUns64Attr(cpu0_attrList, "MPU_NUMBER_INSTRUCTION_REGIONS", 8LL);
    icmAddUns64Attr(cpu0_attrList, "MPU_MINIMUM_INSTRUCTION_REGION_SIZE", 64LL);
    icmAddUns64Attr(cpu0_attrList, "EXCEPTION_EXTRA_INFORMATION", 1LL);
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
    icmAddUns64Attr(cpu1_attrList, "BREAK_VECTOR", 0x08008020LL);
    icmAddUns64Attr(cpu1_attrList, "EXCEPTION_VECTOR", 0x00432020LL);
    icmAddUns64Attr(cpu1_attrList, "RESET_VECTOR", 0x00432000LL);
    icmAddUns64Attr(cpu1_attrList, "HW_DIVIDE", 1LL);
    icmAddUns64Attr(cpu1_attrList, "HW_MULTIPLY", 1LL);
    icmAddUns64Attr(cpu1_attrList, "HW_MULX", 1LL);
    icmAddUns64Attr(cpu1_attrList, "INCLUDE_MMU", 0LL);
    icmAddUns64Attr(cpu1_attrList, "DATA_ADDR_WIDTH", 0x1cLL);
    icmAddUns64Attr(cpu1_attrList, "INST_ADDR_WIDTH", 0x1cLL);
    icmAddUns64Attr(cpu1_attrList, "TEST_HALT_EXIT", 1LL);
    icmAddUns64Attr(cpu1_attrList, "INCLUDE_MPU", 1LL);
    icmAddUns64Attr(cpu1_attrList, "MPU_USE_LIMIT_FOR_REGION_RANGE", 1LL);
    icmAddUns64Attr(cpu1_attrList, "MPU_NUMBER_DATA_REGIONS", 16LL);
    icmAddUns64Attr(cpu1_attrList, "MPU_MINIMUM_DATA_REGION_SIZE", 64LL);
    icmAddUns64Attr(cpu1_attrList, "MPU_NUMBER_INSTRUCTION_REGIONS", 8LL);
    icmAddUns64Attr(cpu1_attrList, "MPU_MINIMUM_INSTRUCTION_REGION_SIZE", 64LL);
    icmAddUns64Attr(cpu1_attrList, "EXCEPTION_EXTRA_INFORMATION", 1LL);
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
    icmAddUns64Attr(cpu2_attrList, "BREAK_VECTOR", 0x08000820LL);
    icmAddUns64Attr(cpu2_attrList, "EXCEPTION_VECTOR", 0x00496020LL);
    icmAddUns64Attr(cpu2_attrList, "RESET_VECTOR", 0x00496000LL);
    icmAddUns64Attr(cpu2_attrList, "HW_DIVIDE", 1LL);
    icmAddUns64Attr(cpu2_attrList, "HW_MULTIPLY", 1LL);
    icmAddUns64Attr(cpu2_attrList, "HW_MULX", 1LL);
    icmAddUns64Attr(cpu2_attrList, "INCLUDE_MMU", 0LL);
    icmAddUns64Attr(cpu2_attrList, "DATA_ADDR_WIDTH", 0x1cLL);
    icmAddUns64Attr(cpu2_attrList, "INST_ADDR_WIDTH", 0x1cLL);
    icmAddUns64Attr(cpu2_attrList, "TEST_HALT_EXIT", 1LL);
    icmAddUns64Attr(cpu2_attrList, "INCLUDE_MPU", 1LL);
    icmAddUns64Attr(cpu2_attrList, "MPU_USE_LIMIT_FOR_REGION_RANGE", 1LL);
    icmAddUns64Attr(cpu2_attrList, "MPU_NUMBER_DATA_REGIONS", 16LL);
    icmAddUns64Attr(cpu2_attrList, "MPU_MINIMUM_DATA_REGION_SIZE", 64LL);
    icmAddUns64Attr(cpu2_attrList, "MPU_NUMBER_INSTRUCTION_REGIONS", 8LL);
    icmAddUns64Attr(cpu2_attrList, "MPU_MINIMUM_INSTRUCTION_REGION_SIZE", 64LL);
    icmAddUns64Attr(cpu2_attrList, "EXCEPTION_EXTRA_INFORMATION", 1LL);
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
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "nios_ii"           ,    // name
        "1.0"               ,    // version
        "model"                  // model
    );

    icmAttrListP cpu3_attrList = icmNewAttrList();
    icmAddStringAttr(cpu3_attrList, "variant", "Nios_II_S");
    icmAddUns64Attr(cpu3_attrList, "BREAK_VECTOR", 0x08000820LL);
    icmAddUns64Attr(cpu3_attrList, "EXCEPTION_VECTOR", 0x004c8020LL);
    icmAddUns64Attr(cpu3_attrList, "RESET_VECTOR", 0x004c8000LL);
    icmAddUns64Attr(cpu3_attrList, "HW_DIVIDE", 1LL);
    icmAddUns64Attr(cpu3_attrList, "HW_MULTIPLY", 1LL);
    icmAddUns64Attr(cpu3_attrList, "HW_MULX", 1LL);
    icmAddUns64Attr(cpu3_attrList, "INCLUDE_MMU", 0LL);
    icmAddUns64Attr(cpu3_attrList, "DATA_ADDR_WIDTH", 0x1cLL);
    icmAddUns64Attr(cpu3_attrList, "INST_ADDR_WIDTH", 0x1cLL);
    icmAddUns64Attr(cpu3_attrList, "TEST_HALT_EXIT", 1LL);
    icmAddUns64Attr(cpu3_attrList, "INCLUDE_MPU", 1LL);
    icmAddUns64Attr(cpu3_attrList, "MPU_USE_LIMIT_FOR_REGION_RANGE", 1LL);
    icmAddUns64Attr(cpu3_attrList, "MPU_NUMBER_DATA_REGIONS", 16LL);
    icmAddUns64Attr(cpu3_attrList, "MPU_MINIMUM_DATA_REGION_SIZE", 64LL);
    icmAddUns64Attr(cpu3_attrList, "MPU_NUMBER_INSTRUCTION_REGIONS", 8LL);
    icmAddUns64Attr(cpu3_attrList, "MPU_MINIMUM_INSTRUCTION_REGION_SIZE", 64LL);
    icmAddUns64Attr(cpu3_attrList, "EXCEPTION_EXTRA_INFORMATION", 1LL);
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
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "nios_ii"           ,    // name
        "1.0"               ,    // version
        "model"                  // model
    );

    icmAttrListP cpum_attrList = icmNewAttrList();
    icmAddStringAttr(cpum_attrList, "variant", "Nios_II_S");
    icmAddUns64Attr(cpum_attrList, "BREAK_VECTOR", 0x08000820LL);
    icmAddUns64Attr(cpum_attrList, "EXCEPTION_VECTOR", 0x00400020LL);
    icmAddUns64Attr(cpum_attrList, "RESET_VECTOR", 0x00400000LL);
    icmAddUns64Attr(cpum_attrList, "HW_DIVIDE", 0LL);
    icmAddUns64Attr(cpum_attrList, "HW_MULTIPLY", 0LL);
    icmAddUns64Attr(cpum_attrList, "HW_MULX", 0LL);
    icmAddUns64Attr(cpum_attrList, "INCLUDE_MMU", 0LL);
    icmAddUns64Attr(cpum_attrList, "DATA_ADDR_WIDTH", 0x1cLL);
    icmAddUns64Attr(cpum_attrList, "INST_ADDR_WIDTH", 0x1cLL);
    icmAddUns64Attr(cpum_attrList, "TEST_HALT_EXIT", 1LL);
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
//                                PSE sw_reset
////////////////////////////////////////////////////////////////////////////////


    const char *sw_reset_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "sw_reset"          ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP sw_reset_attrList = icmNewAttrList();

    handles.sw_reset_p = icmNewPSE(
        "sw_reset"          ,   // name
        sw_reset_path       ,   // model
        sw_reset_attrList   ,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.sw_reset_p, handles.swresetbus_b, "SW_RESET_SLAVE", 0, 0x0, 0x23ff);

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

    icmConnectPSEBus( handles.cpu0_tlb_p, handles.cpu0_tlbbus_in_b, "TLB_SLAVE", 0, 0x0, 0x0);

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
    icmAddUns64Attr(cpu0_jtag_uart_attrList, "writeIRQThreshold", 8LL);
    icmAddUns64Attr(cpu0_jtag_uart_attrList, "readIRQThreshold", 8LL);
    icmAddUns64Attr(cpu0_jtag_uart_attrList, "writeBufferDepth", 64LL);
    icmAddUns64Attr(cpu0_jtag_uart_attrList, "readBufferDepth", 64LL);
    icmAddUns64Attr(cpu0_jtag_uart_attrList, "console", 1LL);
    icmAddUns64Attr(cpu0_jtag_uart_attrList, "finishOnDisconnect", 1LL);

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
    icmAddUns64Attr(cpu0_timer_10ms_attrList, "timerFrequency", 50000000LL);
    icmAddUns64Attr(cpu0_timer_10ms_attrList, "timeoutPeriod", 50000LL);

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
        "IntervalTimer64Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu0_timestamp_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu0_timestamp_attrList, "timerFrequency", 50000000LL);
    icmAddUns64Attr(cpu0_timestamp_attrList, "timeoutPeriod", 50000LL);

    handles.cpu0_timestamp_p = icmNewPSE(
        "cpu0_timestamp"    ,   // name
        cpu0_timestamp_path ,   // model
        cpu0_timestamp_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu0_timestamp_p, handles.cpu0_iobus_b, "sp1", 0, 0x0, 0x27);

////////////////////////////////////////////////////////////////////////////////
//                               PSE cpu0_fprint
////////////////////////////////////////////////////////////////////////////////


    const char *cpu0_fprint_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "fprint"            ,    // name
        "3.0"               ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu0_fprint_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu0_fprint_attrList, "coreID", 0LL);

    handles.cpu0_fprint_p = icmNewPSE(
        "cpu0_fprint"       ,   // name
        cpu0_fprint_path    ,   // model
        cpu0_fprint_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

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

    icmConnectPSEBus( handles.cpu1_tlb_p, handles.cpu1_tlbbus_in_b, "TLB_SLAVE", 0, 0x0, 0x0);

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
    icmAddUns64Attr(cpu1_jtag_uart_attrList, "writeIRQThreshold", 8LL);
    icmAddUns64Attr(cpu1_jtag_uart_attrList, "readIRQThreshold", 8LL);
    icmAddUns64Attr(cpu1_jtag_uart_attrList, "writeBufferDepth", 64LL);
    icmAddUns64Attr(cpu1_jtag_uart_attrList, "readBufferDepth", 64LL);
    icmAddUns64Attr(cpu1_jtag_uart_attrList, "console", 1LL);
    icmAddUns64Attr(cpu1_jtag_uart_attrList, "finishOnDisconnect", 1LL);

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
    icmAddUns64Attr(cpu1_timer_10ms_attrList, "timerFrequency", 50000000LL);
    icmAddUns64Attr(cpu1_timer_10ms_attrList, "timeoutPeriod", 50000LL);

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
        "IntervalTimer64Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu1_timestamp_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu1_timestamp_attrList, "timerFrequency", 50000000LL);
    icmAddUns64Attr(cpu1_timestamp_attrList, "timeoutPeriod", 50000LL);

    handles.cpu1_timestamp_p = icmNewPSE(
        "cpu1_timestamp"    ,   // name
        cpu1_timestamp_path ,   // model
        cpu1_timestamp_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu1_timestamp_p, handles.cpu1_iobus_b, "sp1", 0, 0x0, 0x27);

////////////////////////////////////////////////////////////////////////////////
//                               PSE cpu1_fprint
////////////////////////////////////////////////////////////////////////////////


    const char *cpu1_fprint_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "fprint"            ,    // name
        "3.0"               ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu1_fprint_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu1_fprint_attrList, "coreID", 1LL);

    handles.cpu1_fprint_p = icmNewPSE(
        "cpu1_fprint"       ,   // name
        cpu1_fprint_path    ,   // model
        cpu1_fprint_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu1_fprint_p, handles.cpu1_iobus_b, "FPRINT_CSR_SLAVE", 0, 0x100000, 0x10013f);

    icmConnectPSEBus( handles.cpu1_fprint_p, handles.cpu1_tlb_fprint_bus_b, "FPRINT_TLB_SLAVE", 0, 0x0, 0x3ff);

////////////////////////////////////////////////////////////////////////////////
//                                PSE cpu2_tlb
////////////////////////////////////////////////////////////////////////////////


    const char *cpu2_tlb_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "tlb"               ,    // name
        "1.0"               ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu2_tlb_attrList = icmNewAttrList();

    handles.cpu2_tlb_p = icmNewPSE(
        "cpu2_tlb"          ,   // name
        cpu2_tlb_path       ,   // model
        cpu2_tlb_attrList   ,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu2_tlb_p, handles.cpu2_tlbbus_b, "TLB_MASTER", 1, 0x0, 0xffffffff);

    icmConnectPSEBus( handles.cpu2_tlb_p, handles.cpu2_tlb_fprint_bus_b, "TLB_FPRINT_MASTER", 1, 0x0, 0x7ffffff);

    icmConnectPSEBus( handles.cpu2_tlb_p, handles.cpu2_tlbbus_in_b, "TLB_SLAVE", 0, 0x0, 0x0);

    icmConnectPSEBus( handles.cpu2_tlb_p, handles.cpu2_iobus_b, "TLB_CSR", 0, 0x300000, 0x3003ff);
    icmSetPSEdiagnosticLevel(handles.cpu2_tlb_p, 3);


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
    icmAddUns64Attr(cpu2_jtag_uart_attrList, "writeIRQThreshold", 8LL);
    icmAddUns64Attr(cpu2_jtag_uart_attrList, "readIRQThreshold", 8LL);
    icmAddUns64Attr(cpu2_jtag_uart_attrList, "writeBufferDepth", 64LL);
    icmAddUns64Attr(cpu2_jtag_uart_attrList, "readBufferDepth", 64LL);
    icmAddUns64Attr(cpu2_jtag_uart_attrList, "console", 1LL);
    icmAddUns64Attr(cpu2_jtag_uart_attrList, "finishOnDisconnect", 1LL);

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
    icmAddUns64Attr(cpu2_timer_10ms_attrList, "timerFrequency", 50000000LL);
    icmAddUns64Attr(cpu2_timer_10ms_attrList, "timeoutPeriod", 50000LL);

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
        "IntervalTimer64Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu2_timestamp_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu2_timestamp_attrList, "timerFrequency", 50000000LL);
    icmAddUns64Attr(cpu2_timestamp_attrList, "timeoutPeriod", 50000LL);

    handles.cpu2_timestamp_p = icmNewPSE(
        "cpu2_timestamp"    ,   // name
        cpu2_timestamp_path ,   // model
        cpu2_timestamp_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu2_timestamp_p, handles.cpu2_iobus_b, "sp1", 0, 0x0, 0x27);

////////////////////////////////////////////////////////////////////////////////
//                               PSE cpu2_fprint
////////////////////////////////////////////////////////////////////////////////


    const char *cpu2_fprint_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "fprint"            ,    // name
        "3.0"               ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu2_fprint_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu2_fprint_attrList, "coreID", 0LL);

    handles.cpu2_fprint_p = icmNewPSE(
        "cpu2_fprint"       ,   // name
        cpu2_fprint_path    ,   // model
        cpu2_fprint_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu2_fprint_p, handles.cpu2_iobus_b, "FPRINT_CSR_SLAVE", 0, 0x100000, 0x10013f);

    icmConnectPSEBus( handles.cpu2_fprint_p, handles.cpu2_tlb_fprint_bus_b, "FPRINT_TLB_SLAVE", 0, 0x0, 0x3ff);

////////////////////////////////////////////////////////////////////////////////
//                                PSE cpu3_tlb
////////////////////////////////////////////////////////////////////////////////


    const char *cpu3_tlb_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "tlb"               ,    // name
        "1.0"               ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu3_tlb_attrList = icmNewAttrList();

    handles.cpu3_tlb_p = icmNewPSE(
        "cpu3_tlb"          ,   // name
        cpu3_tlb_path       ,   // model
        cpu3_tlb_attrList   ,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu3_tlb_p, handles.cpu3_tlbbus_b, "TLB_MASTER", 1, 0x0, 0xffffffff);

    icmConnectPSEBus( handles.cpu3_tlb_p, handles.cpu3_tlb_fprint_bus_b, "TLB_FPRINT_MASTER", 1, 0x0, 0x7ffffff);

    icmConnectPSEBus( handles.cpu3_tlb_p, handles.cpu3_tlbbus_in_b, "TLB_SLAVE", 0, 0x0, 0x0);

    icmConnectPSEBus( handles.cpu3_tlb_p, handles.cpu3_iobus_b, "TLB_CSR", 0, 0x300000, 0x3003ff);
    icmSetPSEdiagnosticLevel(handles.cpu3_tlb_p, 3);


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
    icmAddUns64Attr(cpu3_jtag_uart_attrList, "writeIRQThreshold", 8LL);
    icmAddUns64Attr(cpu3_jtag_uart_attrList, "readIRQThreshold", 8LL);
    icmAddUns64Attr(cpu3_jtag_uart_attrList, "writeBufferDepth", 64LL);
    icmAddUns64Attr(cpu3_jtag_uart_attrList, "readBufferDepth", 64LL);
    icmAddUns64Attr(cpu3_jtag_uart_attrList, "console", 1LL);
    icmAddUns64Attr(cpu3_jtag_uart_attrList, "finishOnDisconnect", 1LL);

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
    icmAddUns64Attr(cpu3_timer_10ms_attrList, "timerFrequency", 50000000LL);
    icmAddUns64Attr(cpu3_timer_10ms_attrList, "timeoutPeriod", 50000LL);

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
        "IntervalTimer64Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu3_timestamp_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu3_timestamp_attrList, "timerFrequency", 50000000LL);
    icmAddUns64Attr(cpu3_timestamp_attrList, "timeoutPeriod", 50000LL);

    handles.cpu3_timestamp_p = icmNewPSE(
        "cpu3_timestamp"    ,   // name
        cpu3_timestamp_path ,   // model
        cpu3_timestamp_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu3_timestamp_p, handles.cpu3_iobus_b, "sp1", 0, 0x0, 0x27);

////////////////////////////////////////////////////////////////////////////////
//                               PSE cpu3_fprint
////////////////////////////////////////////////////////////////////////////////


    const char *cpu3_fprint_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "fprint"            ,    // name
        "3.0"               ,    // version
        "pse"                    // model
    );

    icmAttrListP cpu3_fprint_attrList = icmNewAttrList();
    icmAddUns64Attr(cpu3_fprint_attrList, "coreID", 0LL);

    handles.cpu3_fprint_p = icmNewPSE(
        "cpu3_fprint"       ,   // name
        cpu3_fprint_path    ,   // model
        cpu3_fprint_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpu3_fprint_p, handles.cpu3_iobus_b, "FPRINT_CSR_SLAVE", 0, 0x100000, 0x10013f);

    icmConnectPSEBus( handles.cpu3_fprint_p, handles.cpu3_tlb_fprint_bus_b, "FPRINT_TLB_SLAVE", 0, 0x0, 0x3ff);

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
    icmAddUns64Attr(cpum_jtag_uart_attrList, "writeIRQThreshold", 8LL);
    icmAddUns64Attr(cpum_jtag_uart_attrList, "readIRQThreshold", 8LL);
    icmAddUns64Attr(cpum_jtag_uart_attrList, "writeBufferDepth", 64LL);
    icmAddUns64Attr(cpum_jtag_uart_attrList, "readBufferDepth", 64LL);
    icmAddUns64Attr(cpum_jtag_uart_attrList, "console", 1LL);
    icmAddUns64Attr(cpum_jtag_uart_attrList, "finishOnDisconnect", 1LL);

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
    icmAddUns64Attr(cpum_timer_10ms_attrList, "timerFrequency", 50000000LL);
    icmAddUns64Attr(cpum_timer_10ms_attrList, "timeoutPeriod", 50000LL);

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
        "IntervalTimer64Core",    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP cpum_timestamp_attrList = icmNewAttrList();
    icmAddUns64Attr(cpum_timestamp_attrList, "timerFrequency", 50000000LL);
    icmAddUns64Attr(cpum_timestamp_attrList, "timeoutPeriod", 50000LL);

    handles.cpum_timestamp_p = icmNewPSE(
        "cpum_timestamp"    ,   // name
        cpum_timestamp_path ,   // model
        cpum_timestamp_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.cpum_timestamp_p, handles.cpum_iobus_b, "sp1", 0, 0x0, 0x27);

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

    icmConnectPSEBus( handles.cpu1_dma_p, handles.cpum_dbus_b, "DMACSP", 0, 0x2110000, 0x211001f);
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

    icmConnectPSEBus( handles.cpu2_dma_p, handles.cpum_dbus_b, "DMACSP", 0, 0x2120000, 0x212001f);
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

    icmConnectPSEBus( handles.cpu3_dma_p, handles.cpum_dbus_b, "DMACSP", 0, 0x2130000, 0x213001f);
    icmSetPSEdiagnosticLevel(handles.cpu3_dma_p, 3);


////////////////////////////////////////////////////////////////////////////////
//                               PSE comparator
////////////////////////////////////////////////////////////////////////////////


    const char *comparator_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "comparator"        ,    // name
        "2.0"               ,    // version
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


////////////////////////////////////////////////////////////////////////////////
//                              PSE reset_monitor
////////////////////////////////////////////////////////////////////////////////


    const char *reset_monitor_path = icmGetVlnvString(
        0                   ,    // path (0 if from the product directory)
        "mcgill.ca"         ,    // vendor
        0                   ,    // library
        "reset_monitor"     ,    // name
        0                   ,    // version
        "pse"                    // model
    );

    icmAttrListP reset_monitor_attrList = icmNewAttrList();

    handles.reset_monitor_p = icmNewPSE(
        "reset_monitor"     ,   // name
        reset_monitor_path  ,   // model
        reset_monitor_attrList,   // attrlist
        0,       // unused
        0        // unused
    );

    icmConnectPSEBus( handles.reset_monitor_p, handles.resetmonbus_b, "RESET_MONITOR_SLAVE", 0, 0x0, 0x3ff);
    icmSetPSEdiagnosticLevel(handles.reset_monitor_p, 3);


    icmConnectProcessorBusByName( handles.cpu0_c, "INSTRUCTION", handles.cpu0_ibus_b );

    icmConnectProcessorBusByName( handles.cpu0_c, "DATA", handles.cpu0_dbus_b );

    icmConnectProcessorBusByName( handles.cpu1_c, "INSTRUCTION", handles.cpu1_ibus_b );

    icmConnectProcessorBusByName( handles.cpu1_c, "DATA", handles.cpu1_dbus_b );

    icmConnectProcessorBusByName( handles.cpu2_c, "INSTRUCTION", handles.cpu2_ibus_b );

    icmConnectProcessorBusByName( handles.cpu2_c, "DATA", handles.cpu2_dbus_b );

    icmConnectProcessorBusByName( handles.cpu3_c, "INSTRUCTION", handles.cpu3_ibus_b );

    icmConnectProcessorBusByName( handles.cpu3_c, "DATA", handles.cpu3_dbus_b );

    icmConnectProcessorBusByName( handles.cpum_c, "INSTRUCTION", handles.cpum_ibus_b );

    icmConnectProcessorBusByName( handles.cpum_c, "DATA", handles.cpum_dbus_b );

////////////////////////////////////////////////////////////////////////////////
//                            Memory shared_memory
////////////////////////////////////////////////////////////////////////////////

    handles.shared_memory_m = icmNewMemory("shared_memory", 0x7, 0x3ff);


    icmConnectMemoryToBus( handles.smbus_b, "sp1", handles.shared_memory_m, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                             Memory main_memory
////////////////////////////////////////////////////////////////////////////////

    handles.main_memory_m = icmNewMemory("main_memory", 0x7, 0xf9fff);


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

    handles.cpu0_scratchpad_m = icmNewMemory("cpu0_scratchpad", 0x7, 0x7fff);


    icmConnectMemoryToBus( handles.cpu0_tlbbus_b, "sp1", handles.cpu0_scratchpad_m, 0x4200000);

////////////////////////////////////////////////////////////////////////////////
//                            Memory cpu1_breakmem
////////////////////////////////////////////////////////////////////////////////

    handles.cpu1_breakmem_m = icmNewMemory("cpu1_breakmem", 0x7, 0xe0);


    icmConnectMemoryToBus( handles.cpu1_iobus_b, "sp1", handles.cpu1_breakmem_m, 0x8020);

////////////////////////////////////////////////////////////////////////////////
//                           Memory cpu1_scratchpad
////////////////////////////////////////////////////////////////////////////////

    handles.cpu1_scratchpad_m = icmNewMemory("cpu1_scratchpad", 0x7, 0x7fff);


    icmConnectMemoryToBus( handles.cpu1_tlbbus_b, "sp1", handles.cpu1_scratchpad_m, 0x4200000);

////////////////////////////////////////////////////////////////////////////////
//                            Memory cpu2_breakmem
////////////////////////////////////////////////////////////////////////////////

    handles.cpu2_breakmem_m = icmNewMemory("cpu2_breakmem", 0x7, 0xe0);


    icmConnectMemoryToBus( handles.cpu2_iobus_b, "sp1", handles.cpu2_breakmem_m, 0x8020);

////////////////////////////////////////////////////////////////////////////////
//                           Memory cpu2_scratchpad
////////////////////////////////////////////////////////////////////////////////

    handles.cpu2_scratchpad_m = icmNewMemory("cpu2_scratchpad", 0x7, 0x7fff);


    icmConnectMemoryToBus( handles.cpu2_tlbbus_b, "sp1", handles.cpu2_scratchpad_m, 0x4200000);

////////////////////////////////////////////////////////////////////////////////
//                            Memory cpu3_breakmem
////////////////////////////////////////////////////////////////////////////////

    handles.cpu3_breakmem_m = icmNewMemory("cpu3_breakmem", 0x7, 0xe0);


    icmConnectMemoryToBus( handles.cpu3_iobus_b, "sp1", handles.cpu3_breakmem_m, 0x8020);

////////////////////////////////////////////////////////////////////////////////
//                           Memory cpu3_scratchpad
////////////////////////////////////////////////////////////////////////////////

    handles.cpu3_scratchpad_m = icmNewMemory("cpu3_scratchpad", 0x7, 0x7fff);


    icmConnectMemoryToBus( handles.cpu3_tlbbus_b, "sp1", handles.cpu3_scratchpad_m, 0x4200000);

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


    icmNewBusBridge(handles.cpu0_tlbbus_b, handles.mmbus_b, "cpu0_mm_out_bridge", "sp1", "mp1", 0x0, 0xf9fff, 0x400000);

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


    icmNewBusBridge(handles.cpu1_tlbbus_b, handles.mmbus_b, "cpu1_mm_out_bridge", "sp1", "mp1", 0x0, 0xf9fff, 0x400000);

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
//                            Bridge cpu2_d_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_dbus_b, handles.cpu2_tlbbus_in_b, "cpu2_d_bridge", "sp1", "mp1", 0x0, 0xffffffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                            Bridge cpu2_i_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_ibus_b, handles.cpu2_tlbbus_in_b, "cpu2_i_bridge", "sp1", "mp1", 0x0, 0xffffffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpu2_out_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_tlbbus_b, handles.smbus_b, "cpu2_out_bridge", "sp1", "mp1", 0x0, 0x3ff, 0x2500000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu2_mm_out_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_tlbbus_b, handles.mmbus_b, "cpu2_mm_out_bridge", "sp1", "mp1", 0x0, 0xf9fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                              Bridge cpu2_mutex
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_tlbbus_b, handles.mutexbus_b, "cpu2_mutex", "sp1", "mp1", 0x0, 0x7, 0x2300000);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpu2_irq_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_tlbbus_b, handles.cpuirqbus_b, "cpu2_irq_bridge", "sp1", "mp1", 0x0, 0x23ff, 0x2200000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu2_flash_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_tlbbus_b, handles.flashbus_b, "cpu2_flash_bridge", "sp1", "mp1", 0x0, 0x3fffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu2_local_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_tlbbus_b, handles.cpu2_iobus_b, "cpu2_local_bridge", "sp1", "mp1", 0x0, 0x7fffff, 0x8000000);

////////////////////////////////////////////////////////////////////////////////
//                            Bridge cpu3_d_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_dbus_b, handles.cpu3_tlbbus_in_b, "cpu3_d_bridge", "sp1", "mp1", 0x0, 0xffffffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                            Bridge cpu3_i_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_ibus_b, handles.cpu3_tlbbus_in_b, "cpu3_i_bridge", "sp1", "mp1", 0x0, 0xffffffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpu3_out_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_tlbbus_b, handles.smbus_b, "cpu3_out_bridge", "sp1", "mp1", 0x0, 0x3ff, 0x2500000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu3_mm_out_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_tlbbus_b, handles.mmbus_b, "cpu3_mm_out_bridge", "sp1", "mp1", 0x0, 0xf9fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                              Bridge cpu3_mutex
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_tlbbus_b, handles.mutexbus_b, "cpu3_mutex", "sp1", "mp1", 0x0, 0x7, 0x2300000);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpu3_irq_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_tlbbus_b, handles.cpuirqbus_b, "cpu3_irq_bridge", "sp1", "mp1", 0x0, 0x23ff, 0x2200000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu3_flash_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_tlbbus_b, handles.flashbus_b, "cpu3_flash_bridge", "sp1", "mp1", 0x0, 0x3fffff, 0x0);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu3_local_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_tlbbus_b, handles.cpu3_iobus_b, "cpu3_local_bridge", "sp1", "mp1", 0x0, 0x7fffff, 0x8000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpum_out_bridge_d
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.smbus_b, "cpum_out_bridge_d", "sp1", "mp1", 0x0, 0x3ff, 0x2500000);

////////////////////////////////////////////////////////////////////////////////
//                         Bridge cpum_mm_out_bridge_i
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_ibus_b, handles.mmbus_b, "cpum_mm_out_bridge_i", "sp1", "mp1", 0x0, 0xf9fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                         Bridge cpum_mm_out_bridge_d
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.mmbus_b, "cpum_mm_out_bridge_d", "sp1", "mp1", 0x0, 0xf9fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                              Bridge cpum_mutex
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.mutexbus_b, "cpum_mutex", "sp1", "mp1", 0x0, 0x7, 0x2300000);

////////////////////////////////////////////////////////////////////////////////
//                           Bridge cpum_irq_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.cpuirqbus_b, "cpum_irq_bridge", "sp1", "mp1", 0x0, 0x23ff, 0x2200000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpum_reset_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.swresetbus_b, "cpum_reset_bridge", "sp1", "mp1", 0x0, 0x23ff, 0x3000000);

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
//                          Bridge cpu2_dmasp_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_dmabus_b, handles.cpu2_tlbbus_b, "cpu2_dmasp_bridge", "sp1", "mp1", 0x4000000, 0x87fffff, 0x4000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu2_dmamm_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_dmabus_b, handles.mmbus_b, "cpu2_dmamm_bridge", "sp1", "mp1", 0x0, 0xf9fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu3_dmasp_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_dmabus_b, handles.cpu3_tlbbus_b, "cpu3_dmasp_bridge", "sp1", "mp1", 0x4000000, 0x87fffff, 0x4000000);

////////////////////////////////////////////////////////////////////////////////
//                          Bridge cpu3_dmamm_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_dmabus_b, handles.mmbus_b, "cpu3_dmamm_bridge", "sp1", "mp1", 0x0, 0xf9fff, 0x400000);

////////////////////////////////////////////////////////////////////////////////
//                            Bridge comp_bridge_0
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_tlbbus_b, handles.compbus_b, "comp_bridge_0", "sp1", "mp1", 0x1000000, 0x1000fff, 0x1000000);

////////////////////////////////////////////////////////////////////////////////
//                            Bridge comp_bridge_1
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_tlbbus_b, handles.compbus_b, "comp_bridge_1", "sp1", "mp1", 0x1000000, 0x1000fff, 0x1000000);

////////////////////////////////////////////////////////////////////////////////
//                            Bridge comp_bridge_2
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_tlbbus_b, handles.compbus_b, "comp_bridge_2", "sp1", "mp1", 0x1000000, 0x1000fff, 0x1000000);

////////////////////////////////////////////////////////////////////////////////
//                            Bridge comp_bridge_3
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_tlbbus_b, handles.compbus_b, "comp_bridge_3", "sp1", "mp1", 0x1000000, 0x1000fff, 0x1000000);

////////////////////////////////////////////////////////////////////////////////
//                        Bridge cpum_reset_mon_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpum_dbus_b, handles.resetmonbus_b, "cpum_reset_mon_bridge", "sp1", "mp1", 0x0, 0x3ff, 0x2800000);

////////////////////////////////////////////////////////////////////////////////
//                        Bridge cpu0_reset_mon_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu0_tlbbus_b, handles.resetmonbus_b, "cpu0_reset_mon_bridge", "sp1", "mp1", 0x0, 0x3ff, 0x2800000);

////////////////////////////////////////////////////////////////////////////////
//                        Bridge cpu1_reset_mon_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu1_tlbbus_b, handles.resetmonbus_b, "cpu1_reset_mon_bridge", "sp1", "mp1", 0x0, 0x3ff, 0x2800000);

////////////////////////////////////////////////////////////////////////////////
//                        Bridge cpu2_reset_mon_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu2_tlbbus_b, handles.resetmonbus_b, "cpu2_reset_mon_bridge", "sp1", "mp1", 0x0, 0x3ff, 0x2800000);

////////////////////////////////////////////////////////////////////////////////
//                        Bridge cpu3_reset_mon_bridge
////////////////////////////////////////////////////////////////////////////////


    icmNewBusBridge(handles.cpu3_tlbbus_b, handles.resetmonbus_b, "cpu3_reset_mon_bridge", "sp1", "mp1", 0x0, 0x3ff, 0x2800000);


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

    icmConnectProcessorNet( handles.cpu2_c, handles.fprint_write_0_n, "fprint_write_out", 0);

    icmConnectProcessorNet( handles.cpu3_c, handles.fprint_write_0_n, "fprint_write_out", 0);

    icmConnectPSENet( handles.cpu0_fprint_p, handles.fprint_write_0_n, "FPRINT_WRITE", 0);

    icmConnectPSENet( handles.cpu2_fprint_p, handles.fprint_write_0_n, "FPRINT_WRITE", 0);

    icmConnectPSENet( handles.cpu3_fprint_p, handles.fprint_write_0_n, "FPRINT_WRITE", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.fprint_write_address_0_n = icmNewNet("handles.fprint_write_address_0_n" );

    icmConnectProcessorNet( handles.cpu0_c, handles.fprint_write_address_0_n, "fprint_write_out_address", 0);

    icmConnectProcessorNet( handles.cpu2_c, handles.fprint_write_address_0_n, "fprint_write_out_address", 0);

    icmConnectProcessorNet( handles.cpu3_c, handles.fprint_write_address_0_n, "fprint_write_out_address", 0);

    icmConnectPSENet( handles.cpu0_fprint_p, handles.fprint_write_address_0_n, "FPRINT_WRITE_ADDRESS", 0);

    icmConnectPSENet( handles.cpu2_fprint_p, handles.fprint_write_address_0_n, "FPRINT_WRITE_ADDRESS", 0);

    icmConnectPSENet( handles.cpu3_fprint_p, handles.fprint_write_address_0_n, "FPRINT_WRITE_ADDRESS", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.fprint_write_data_0_n = icmNewNet("handles.fprint_write_data_0_n" );

    icmConnectProcessorNet( handles.cpu0_c, handles.fprint_write_data_0_n, "fprint_write_out_data", 0);

    icmConnectProcessorNet( handles.cpu2_c, handles.fprint_write_data_0_n, "fprint_write_out_data", 0);

    icmConnectProcessorNet( handles.cpu3_c, handles.fprint_write_data_0_n, "fprint_write_out_data", 0);

    icmConnectPSENet( handles.cpu0_fprint_p, handles.fprint_write_data_0_n, "FPRINT_WRITE_DATA", 0);

    icmConnectPSENet( handles.cpu2_fprint_p, handles.fprint_write_data_0_n, "FPRINT_WRITE_DATA", 0);

    icmConnectPSENet( handles.cpu3_fprint_p, handles.fprint_write_data_0_n, "FPRINT_WRITE_DATA", 0);

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

////////////////////////////////////////////////////////////////////////////////
    handles.cpum_notify1_n = icmNewNet("handles.cpum_notify1_n" );

    icmConnectProcessorNet( handles.cpum_c, handles.cpum_notify1_n, "d_irq13", 0);

    icmConnectPSENet( handles.reset_monitor_p, handles.cpum_notify1_n, "cpum_notify", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpu0_reset1_n = icmNewNet("handles.cpu0_reset1_n" );

    icmConnectPSENet( handles.sw_reset_p, handles.cpu0_reset1_n, "cpu0_reset", 0);

    icmConnectPSENet( handles.cpu0_fprint_p, handles.cpu0_reset1_n, "FPRINT_RESET", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpu1_reset1_n = icmNewNet("handles.cpu1_reset1_n" );

    icmConnectPSENet( handles.sw_reset_p, handles.cpu1_reset1_n, "cpu1_reset", 0);

    icmConnectPSENet( handles.cpu1_fprint_p, handles.cpu1_reset1_n, "FPRINT_RESET", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpu2_reset1_n = icmNewNet("handles.cpu2_reset1_n" );

    icmConnectPSENet( handles.sw_reset_p, handles.cpu2_reset1_n, "cpu2_reset", 0);

    icmConnectPSENet( handles.cpu2_fprint_p, handles.cpu2_reset1_n, "FPRINT_RESET", 0);

////////////////////////////////////////////////////////////////////////////////
    handles.cpu3_reset1_n = icmNewNet("handles.cpu3_reset1_n" );

    icmConnectPSENet( handles.sw_reset_p, handles.cpu3_reset1_n, "cpu3_reset", 0);

    icmConnectPSENet( handles.cpu3_fprint_p, handles.cpu3_reset1_n, "FPRINT_RESET", 0);
}
