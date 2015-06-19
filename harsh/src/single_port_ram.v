//Modified version of Altera's example file

module single_port_ram(
	data,
	addr,
	we,
	clk,
	q
);
	parameter tagwidth = 20;
	parameter numentries = 32;
	parameter addresswidth = 26;

	input [tagwidth-1:0] data;
	input [addresswidth-1:0] addr;
	input we, clk;
	output [tagwidth-1:0] q;

	// Declare the RAM variable
	reg [tagwidth-1:0] ram[numentries-1:0];
	
	// Variable to hold the registered read address
//	reg [4:0] addr_reg;
	
	always @ (posedge clk)
	begin
		if (we) begin	// Write
			ram[addr] <= data;
		end
//		addr_reg <= addr;
	end
	assign q = ram[addr];
	
	// Continuous assignment implies read returns NEW data.
	// This is the natural behavior of the TriMatrix memory
	// blocks in Single Port mode.
//	assign q = ram[addr_reg];

endmodule
