// fingerprint_unit.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`include "crc_defines.v" 
`timescale 1 ps / 1 ps
module fingerprint_unit #(
		parameter AUTO_CLOCK_CLOCK_RATE = "-1"
	) (
		input  wire [7:0]  avs_s0_address,     //    s0.address
		input  wire        avs_s0_read,        //      .read
		output wire [31:0] avs_s0_readdata,    //      .readdata
		input  wire        avs_s0_write,       //      .write
		input  wire [31:0] avs_s0_writedata,   //      .writedata
		output wire        avs_s0_waitrequest, //      .waitrequest
		input  wire        clk,                // clock.clk
		input  wire        reset,               // reset.reset
		
		input  wire [7:0]  avs_s1_address,     //    s1.address
		input  wire        avs_s1_read,        //      .read
		output wire [31:0] avs_s1_readdata,    //      .readdata
		input  wire        avs_s1_write,       //      .write
		input  wire [31:0] avs_s1_writedata,   //      .writedata
		output wire        avs_s1_waitrequest, //      .waitrequest
		output wire		   irq,				   //		collision signal
		
	
		output 										io_release,
		output [`CRC_KEY_WIDTH-1:0]					io_key,
		output [`CRC_KEY_WIDTH:0]					active_task0

	);

	// TODO: Auto-generated HDL template
	reg[7:0] 	s0_address_reg;
	reg			s0_read;
	reg			s0_write;
	reg[7:0]	s0_writedata;
	
	always @ (posedge clk or negedge reset)
begin
	if (reset == 0)begin
		s0_address_reg = 0;
		s0_read = 0;
		s0_write = 0;
		s0_writedata = 0;
		
		
	end else begin
		s0_address_reg = avs_s0_address;
		s0_read = avs_s0_read;
		s0_write = avs_s0_write;
		s0_writedata = avs_s0_writedata;

	end
end

	assign avs_s0_readdata = 0;
	assign avs_s0_waitrequest = 1'b0;

	
endmodule
