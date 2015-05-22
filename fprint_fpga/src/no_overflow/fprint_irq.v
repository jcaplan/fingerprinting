`timescale 1 ps / 1 ps

module fprint_irq (
		input clk,
		input reset,
		output wire irq_out,
		input wire irq
	);
	
	reg irq_reg;
	
	always @ (posedge clk or posedge reset)begin
		if(reset)
			irq_reg = 0;
		else
			irq_reg = irq;
	end
	
	assign irq_out = irq_reg;
	
endmodule
