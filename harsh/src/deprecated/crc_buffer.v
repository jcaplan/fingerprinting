`include "crc_defines.v"

module crc_buffer(
				clk,
				data_in,
				write_en,
				addr_r,
				addr_w,
				data_out
);

parameter cw = `CRC_WIDTH;
parameter rw = `CRC_RAM_ADDRESS_WIDTH;
parameter rs = `CRC_RAM_SIZE;


input[cw-1:0]			data_in;
input					write_en;
input					clk;
input[rw-1:0]			addr_r;
input[rw-1:0]			addr_w;
output[cw-1:0] 			data_out;
reg [cw-1:0] 			mem [rs-1:0];
reg[rw-1:0]				addr_r_reg;




always @ (posedge clk)
begin
		if(write_en)begin
			mem[addr_w] = {data_in} ;
			 
		end
		
		addr_r_reg <= addr_r;
end

assign data_out = mem[addr_r_reg];

endmodule
