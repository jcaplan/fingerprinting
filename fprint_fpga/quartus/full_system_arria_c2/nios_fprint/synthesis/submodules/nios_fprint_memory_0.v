// nios_fprint_memory_0.v

// Generated using ACDS version 13.1 162 at 2015.03.04.19:25:52

`timescale 1 ps / 1 ps
module nios_fprint_memory_0 (
		input  wire        clk_clk,                      //            clk.clk
		input  wire        reset_reset_n,                //          reset.reset_n
		output wire        mm_bridge_0_s0_waitrequest,   // mm_bridge_0_s0.waitrequest
		output wire [31:0] mm_bridge_0_s0_readdata,      //               .readdata
		output wire        mm_bridge_0_s0_readdatavalid, //               .readdatavalid
		input  wire [0:0]  mm_bridge_0_s0_burstcount,    //               .burstcount
		input  wire [31:0] mm_bridge_0_s0_writedata,     //               .writedata
		input  wire [22:0] mm_bridge_0_s0_address,       //               .address
		input  wire        mm_bridge_0_s0_write,         //               .write
		input  wire        mm_bridge_0_s0_read,          //               .read
		input  wire [3:0]  mm_bridge_0_s0_byteenable,    //               .byteenable
		input  wire        mm_bridge_0_s0_debugaccess    //               .debugaccess
	);

	wire  [31:0] mm_interconnect_0_onchip_memorymain_s1_writedata;  // mm_interconnect_0:onchip_memoryMain_s1_writedata -> onchip_memoryMain:writedata
	wire  [17:0] mm_interconnect_0_onchip_memorymain_s1_address;    // mm_interconnect_0:onchip_memoryMain_s1_address -> onchip_memoryMain:address
	wire         mm_interconnect_0_onchip_memorymain_s1_chipselect; // mm_interconnect_0:onchip_memoryMain_s1_chipselect -> onchip_memoryMain:chipselect
	wire         mm_interconnect_0_onchip_memorymain_s1_clken;      // mm_interconnect_0:onchip_memoryMain_s1_clken -> onchip_memoryMain:clken
	wire         mm_interconnect_0_onchip_memorymain_s1_write;      // mm_interconnect_0:onchip_memoryMain_s1_write -> onchip_memoryMain:write
	wire  [31:0] mm_interconnect_0_onchip_memorymain_s1_readdata;   // onchip_memoryMain:readdata -> mm_interconnect_0:onchip_memoryMain_s1_readdata
	wire   [3:0] mm_interconnect_0_onchip_memorymain_s1_byteenable; // mm_interconnect_0:onchip_memoryMain_s1_byteenable -> onchip_memoryMain:byteenable
	wire   [0:0] mm_bridge_0_m0_burstcount;                         // mm_bridge_0:m0_burstcount -> mm_interconnect_0:mm_bridge_0_m0_burstcount
	wire         mm_bridge_0_m0_waitrequest;                        // mm_interconnect_0:mm_bridge_0_m0_waitrequest -> mm_bridge_0:m0_waitrequest
	wire  [22:0] mm_bridge_0_m0_address;                            // mm_bridge_0:m0_address -> mm_interconnect_0:mm_bridge_0_m0_address
	wire  [31:0] mm_bridge_0_m0_writedata;                          // mm_bridge_0:m0_writedata -> mm_interconnect_0:mm_bridge_0_m0_writedata
	wire         mm_bridge_0_m0_write;                              // mm_bridge_0:m0_write -> mm_interconnect_0:mm_bridge_0_m0_write
	wire         mm_bridge_0_m0_read;                               // mm_bridge_0:m0_read -> mm_interconnect_0:mm_bridge_0_m0_read
	wire  [31:0] mm_bridge_0_m0_readdata;                           // mm_interconnect_0:mm_bridge_0_m0_readdata -> mm_bridge_0:m0_readdata
	wire         mm_bridge_0_m0_debugaccess;                        // mm_bridge_0:m0_debugaccess -> mm_interconnect_0:mm_bridge_0_m0_debugaccess
	wire   [3:0] mm_bridge_0_m0_byteenable;                         // mm_bridge_0:m0_byteenable -> mm_interconnect_0:mm_bridge_0_m0_byteenable
	wire         mm_bridge_0_m0_readdatavalid;                      // mm_interconnect_0:mm_bridge_0_m0_readdatavalid -> mm_bridge_0:m0_readdatavalid
	wire         rst_controller_reset_out_reset;                    // rst_controller:reset_out -> [mm_bridge_0:reset, mm_interconnect_0:mm_bridge_0_reset_reset_bridge_in_reset_reset, onchip_memoryMain:reset]
	wire         rst_controller_reset_out_reset_req;                // rst_controller:reset_req -> [onchip_memoryMain:reset_req, rst_translator:reset_req_in]

	nios_fprint_memory_0_onchip_memoryMain onchip_memorymain (
		.clk        (clk_clk),                                           //   clk1.clk
		.address    (mm_interconnect_0_onchip_memorymain_s1_address),    //     s1.address
		.clken      (mm_interconnect_0_onchip_memorymain_s1_clken),      //       .clken
		.chipselect (mm_interconnect_0_onchip_memorymain_s1_chipselect), //       .chipselect
		.write      (mm_interconnect_0_onchip_memorymain_s1_write),      //       .write
		.readdata   (mm_interconnect_0_onchip_memorymain_s1_readdata),   //       .readdata
		.writedata  (mm_interconnect_0_onchip_memorymain_s1_writedata),  //       .writedata
		.byteenable (mm_interconnect_0_onchip_memorymain_s1_byteenable), //       .byteenable
		.reset      (rst_controller_reset_out_reset),                    // reset1.reset
		.reset_req  (rst_controller_reset_out_reset_req)                 //       .reset_req
	);

	altera_avalon_mm_bridge #(
		.DATA_WIDTH        (32),
		.SYMBOL_WIDTH      (8),
		.ADDRESS_WIDTH     (23),
		.BURSTCOUNT_WIDTH  (1),
		.PIPELINE_COMMAND  (1),
		.PIPELINE_RESPONSE (1)
	) mm_bridge_0 (
		.clk              (clk_clk),                        //   clk.clk
		.reset            (rst_controller_reset_out_reset), // reset.reset
		.s0_waitrequest   (mm_bridge_0_s0_waitrequest),     //    s0.waitrequest
		.s0_readdata      (mm_bridge_0_s0_readdata),        //      .readdata
		.s0_readdatavalid (mm_bridge_0_s0_readdatavalid),   //      .readdatavalid
		.s0_burstcount    (mm_bridge_0_s0_burstcount),      //      .burstcount
		.s0_writedata     (mm_bridge_0_s0_writedata),       //      .writedata
		.s0_address       (mm_bridge_0_s0_address),         //      .address
		.s0_write         (mm_bridge_0_s0_write),           //      .write
		.s0_read          (mm_bridge_0_s0_read),            //      .read
		.s0_byteenable    (mm_bridge_0_s0_byteenable),      //      .byteenable
		.s0_debugaccess   (mm_bridge_0_s0_debugaccess),     //      .debugaccess
		.m0_waitrequest   (mm_bridge_0_m0_waitrequest),     //    m0.waitrequest
		.m0_readdata      (mm_bridge_0_m0_readdata),        //      .readdata
		.m0_readdatavalid (mm_bridge_0_m0_readdatavalid),   //      .readdatavalid
		.m0_burstcount    (mm_bridge_0_m0_burstcount),      //      .burstcount
		.m0_writedata     (mm_bridge_0_m0_writedata),       //      .writedata
		.m0_address       (mm_bridge_0_m0_address),         //      .address
		.m0_write         (mm_bridge_0_m0_write),           //      .write
		.m0_read          (mm_bridge_0_m0_read),            //      .read
		.m0_byteenable    (mm_bridge_0_m0_byteenable),      //      .byteenable
		.m0_debugaccess   (mm_bridge_0_m0_debugaccess)      //      .debugaccess
	);

	nios_fprint_memory_0_mm_interconnect_0 mm_interconnect_0 (
		.clk_0_clk_clk                                 (clk_clk),                                           //                               clk_0_clk.clk
		.mm_bridge_0_reset_reset_bridge_in_reset_reset (rst_controller_reset_out_reset),                    // mm_bridge_0_reset_reset_bridge_in_reset.reset
		.mm_bridge_0_m0_address                        (mm_bridge_0_m0_address),                            //                          mm_bridge_0_m0.address
		.mm_bridge_0_m0_waitrequest                    (mm_bridge_0_m0_waitrequest),                        //                                        .waitrequest
		.mm_bridge_0_m0_burstcount                     (mm_bridge_0_m0_burstcount),                         //                                        .burstcount
		.mm_bridge_0_m0_byteenable                     (mm_bridge_0_m0_byteenable),                         //                                        .byteenable
		.mm_bridge_0_m0_read                           (mm_bridge_0_m0_read),                               //                                        .read
		.mm_bridge_0_m0_readdata                       (mm_bridge_0_m0_readdata),                           //                                        .readdata
		.mm_bridge_0_m0_readdatavalid                  (mm_bridge_0_m0_readdatavalid),                      //                                        .readdatavalid
		.mm_bridge_0_m0_write                          (mm_bridge_0_m0_write),                              //                                        .write
		.mm_bridge_0_m0_writedata                      (mm_bridge_0_m0_writedata),                          //                                        .writedata
		.mm_bridge_0_m0_debugaccess                    (mm_bridge_0_m0_debugaccess),                        //                                        .debugaccess
		.onchip_memoryMain_s1_address                  (mm_interconnect_0_onchip_memorymain_s1_address),    //                    onchip_memoryMain_s1.address
		.onchip_memoryMain_s1_write                    (mm_interconnect_0_onchip_memorymain_s1_write),      //                                        .write
		.onchip_memoryMain_s1_readdata                 (mm_interconnect_0_onchip_memorymain_s1_readdata),   //                                        .readdata
		.onchip_memoryMain_s1_writedata                (mm_interconnect_0_onchip_memorymain_s1_writedata),  //                                        .writedata
		.onchip_memoryMain_s1_byteenable               (mm_interconnect_0_onchip_memorymain_s1_byteenable), //                                        .byteenable
		.onchip_memoryMain_s1_chipselect               (mm_interconnect_0_onchip_memorymain_s1_chipselect), //                                        .chipselect
		.onchip_memoryMain_s1_clken                    (mm_interconnect_0_onchip_memorymain_s1_clken)       //                                        .clken
	);

	altera_reset_controller #(
		.NUM_RESET_INPUTS          (1),
		.OUTPUT_RESET_SYNC_EDGES   ("deassert"),
		.SYNC_DEPTH                (2),
		.RESET_REQUEST_PRESENT     (1),
		.RESET_REQ_WAIT_TIME       (1),
		.MIN_RST_ASSERTION_TIME    (3),
		.RESET_REQ_EARLY_DSRT_TIME (1),
		.USE_RESET_REQUEST_IN0     (0),
		.USE_RESET_REQUEST_IN1     (0),
		.USE_RESET_REQUEST_IN2     (0),
		.USE_RESET_REQUEST_IN3     (0),
		.USE_RESET_REQUEST_IN4     (0),
		.USE_RESET_REQUEST_IN5     (0),
		.USE_RESET_REQUEST_IN6     (0),
		.USE_RESET_REQUEST_IN7     (0),
		.USE_RESET_REQUEST_IN8     (0),
		.USE_RESET_REQUEST_IN9     (0),
		.USE_RESET_REQUEST_IN10    (0),
		.USE_RESET_REQUEST_IN11    (0),
		.USE_RESET_REQUEST_IN12    (0),
		.USE_RESET_REQUEST_IN13    (0),
		.USE_RESET_REQUEST_IN14    (0),
		.USE_RESET_REQUEST_IN15    (0),
		.ADAPT_RESET_REQUEST       (0)
	) rst_controller (
		.reset_in0      (~reset_reset_n),                     // reset_in0.reset
		.clk            (clk_clk),                            //       clk.clk
		.reset_out      (rst_controller_reset_out_reset),     // reset_out.reset
		.reset_req      (rst_controller_reset_out_reset_req), //          .reset_req
		.reset_req_in0  (1'b0),                               // (terminated)
		.reset_in1      (1'b0),                               // (terminated)
		.reset_req_in1  (1'b0),                               // (terminated)
		.reset_in2      (1'b0),                               // (terminated)
		.reset_req_in2  (1'b0),                               // (terminated)
		.reset_in3      (1'b0),                               // (terminated)
		.reset_req_in3  (1'b0),                               // (terminated)
		.reset_in4      (1'b0),                               // (terminated)
		.reset_req_in4  (1'b0),                               // (terminated)
		.reset_in5      (1'b0),                               // (terminated)
		.reset_req_in5  (1'b0),                               // (terminated)
		.reset_in6      (1'b0),                               // (terminated)
		.reset_req_in6  (1'b0),                               // (terminated)
		.reset_in7      (1'b0),                               // (terminated)
		.reset_req_in7  (1'b0),                               // (terminated)
		.reset_in8      (1'b0),                               // (terminated)
		.reset_req_in8  (1'b0),                               // (terminated)
		.reset_in9      (1'b0),                               // (terminated)
		.reset_req_in9  (1'b0),                               // (terminated)
		.reset_in10     (1'b0),                               // (terminated)
		.reset_req_in10 (1'b0),                               // (terminated)
		.reset_in11     (1'b0),                               // (terminated)
		.reset_req_in11 (1'b0),                               // (terminated)
		.reset_in12     (1'b0),                               // (terminated)
		.reset_req_in12 (1'b0),                               // (terminated)
		.reset_in13     (1'b0),                               // (terminated)
		.reset_req_in13 (1'b0),                               // (terminated)
		.reset_in14     (1'b0),                               // (terminated)
		.reset_req_in14 (1'b0),                               // (terminated)
		.reset_in15     (1'b0),                               // (terminated)
		.reset_req_in15 (1'b0)                                // (terminated)
	);

endmodule