`timescale 1 ps / 1 ps
module cpu_oflow_irq (
		input  wire          clk,
		input  wire          reset,
		input  wire   [7:0]  avs_address,
		input  wire          avs_write,
		input  wire   [31:0] avs_writedata,
		input  wire          avs_read,
		output wire  [31:0] avs_readdata,		
		output wire         oflow_irq
	);

	reg [31:0] oflow_reg;
	
	always @ (posedge clk or posedge reset)
begin
	if (reset == 1)begin
		oflow_reg = 0;
	end else begin
		if(avs_write == 1) begin
			oflow_reg = avs_writedata;
		end 
	end
end

	assign oflow_irq = oflow_reg[9];
	assign avs_readdata = oflow_reg;
	
endmodule
