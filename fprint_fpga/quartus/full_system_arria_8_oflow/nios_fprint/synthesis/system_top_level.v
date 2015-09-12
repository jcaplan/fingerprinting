

module system_top_level 
(
	input				osc_clk,
	input				reset_n

//	output wire [3:0]  led_pio_external_connection_export,                 //                 led_pio_external_connection.export
//	input  wire        processor1_0_button_pio_external_connection_export, // processor1_0_button_pio_external_connection.export
//	input  wire        processor0_0_button_pio_external_connection_export  // processor0_0_button_pio_external_connection.export
//	output 		[26:0]	fsm_add,					//2.5V, Address
//	inout		[15:0]	fsm_data,					//2.5V, Data
//	output				flash_oen,				//2.5V, Flash Output Enable
//	output				flash_wen,				//2.5V, Flash Write Enable
//	output				flash_csn,				//2.5V, Flash Chip Enable
//	output				flash_clk,				//2.5V, Flash Clock 
//	output				flash_resetn,			//2.5V, Flash Reset 
//	output				flash_advn				//2.5V, Flash Address Valid 
);

// reset interface
//assign flash_resetn = 1;
//assign fsm_add[26:22] = 0;
//assign flash_advn = 0;
//assign flash_clk = 0;

	

	
nios_fprint nios_fprint_inst(
		.clk_clk(osc_clk),                                            //                                         clk.clk
		.reset_reset_n(reset_n)                                      //                                       reset.reset_n
//		.tristate_conduit_bridge_tcm_address_out(fsm_add[21:0]),            //                     tristate_conduit_bridge.tcm_address_out
//		.tristate_conduit_bridge_tcm_read_n_out(flash_oen),             //                                            .tcm_read_n_out
//		.tristate_conduit_bridge_tcm_write_n_out(flash_wen),            //                                            .tcm_write_n_out
//		.tristate_conduit_bridge_tcm_data_out(fsm_data),               //                                            .tcm_data_out
//		.tristate_conduit_bridge_tcm_chipselect_n_out(flash_csn)        //                                            .tcm_chipselect_n_out
	);

//No buffer required. One write per task cycle. store current task,
// pio_external = pio when io_key = stored_active_task and io_release = 1;


	
endmodule
