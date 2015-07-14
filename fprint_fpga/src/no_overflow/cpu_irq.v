// cpu_irq.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module cpu_irq (
		input  wire [7:0]  avs_s0_address,       //    s0.address
		input  wire        avs_s0_write,         //      .write
		input  wire [31:0] avs_s0_writedata,     //      .writedata
		input  wire        clk,                  // clock.clk
		input  wire        reset,                // reset.reset
		output wire        ins_irq0_irq          //  irq0.irq
	);

	reg irq;
	
	always @ (posedge clk or posedge reset)
begin
	if (reset == 1)begin
		irq = 0;
	end else if(avs_s0_write == 1) begin
		irq = avs_s0_writedata[0];
	end
end

	assign ins_irq0_irq = irq;
	

	
endmodule
