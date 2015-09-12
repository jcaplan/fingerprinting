`timescale 1 ps / 1 ps
module cpu_checkpoint_irq (
		input  wire          clk,
		input  wire          reset,
		input  wire   [7:0]  avs_address,
		input  wire          avs_write,
		input  wire   [31:0] avs_writedata,
		input  wire          avs_read,
		output wire  [31:0] avs_readdata,		
		output wire         checkpoint_irq
	);

	reg [31:0] checkpoint_reg;
	
	always @ (posedge clk or posedge reset)
begin
	if (reset == 1)begin
		checkpoint_reg = 0;
	end else begin
		if(avs_write == 1) begin
			checkpoint_reg = avs_writedata;
		end 
	end
end

	assign checkpoint_irq = checkpoint_reg[8];
	assign avs_readdata = checkpoint_reg;
	
endmodule
