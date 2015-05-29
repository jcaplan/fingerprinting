

module system_top_level 
(
	input				osc_clk,
	input				reset_n,

	output wire [3:0]  led_pio_external_connection_export,                 //                 led_pio_external_connection.export
	input  wire        processor1_0_button_pio_external_connection_export, // processor1_0_button_pio_external_connection.export
	input  wire        processor0_0_button_pio_external_connection_export  // processor0_0_button_pio_external_connection.export
	
);

// reset interface
assign flash_reset_n = reset_n;


nios_fprint nios_fprint_inst(
		.clk_clk(osc_clk),                                            //                                         clk.clk
		.led_pio_external_connection_export(led_pio_external_connection_export),                 //                 led_pio_external_connection.export
		.processor1_0_button_pio_external_connection_export(processor0_0_button_pio_external_connection_export), // processor1_0_button_pio_external_connection.export
		.processor0_0_button_pio_external_connection_export(processor0_0_button_pio_external_connection_export), // processor0_0_button_pio_external_connection.export
		.reset_reset_n(reset_n)                                       //                                       reset.reset_n
	);

//No buffer required. One write per task cycle. store current task,
// pio_external = pio when io_key = stored_active_task and io_release = 1;


	
endmodule
