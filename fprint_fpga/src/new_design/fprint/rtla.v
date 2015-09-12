`include "defines.v"
module rtla(

	input								clk,
	input								rst,
	input [`CRC_DATA_WIDTH-1:0]			d_in,
	input								d_latch,
	input								sel,
	input								en_fcs,
	input								count_mode,
	output[`CRC_WIDTH-1:0]						crc_out,
	input								pause_store,
	input								pause_reset,
	input								unpause_load,
	input								unpause_popstack
);	
	
parameter dw = `CRC_DATA_WIDTH;
parameter cw = `CRC_WIDTH;
parameter gp = `CRC_GP;


/******************************************************
SIGNAL DECLARATIONS
*******************************************************/

reg [cw-1:0]				crc_out_i;
reg [cw-1:0] 				rtla[dw-1:0];
wire[cw-1:0]				g[dw-1:0];
wire[cw-1:0]				h[dw-1:0];
reg [dw-1:0]				d_in_i;
reg							count_mode_i;
wire [dw-1:dw-cw]			xor_out;
wire [dw-1:0]				stage_2;
reg[`CRC_WIDTH-1:0]			old_crc;
wire[`CRC_WIDTH-1:0]		load_data;


/******************************************************
RTLA DECLARATIONS
*******************************************************/
initial 
begin
/*Initialize RTLA inputs*/
rtla[0] =32'h0001da97;
rtla[1] =32'h0003b52e;
rtla[2] =32'h00076a5c;
rtla[3] =32'h000ed4b8;
rtla[4] =32'h001da970;
rtla[5] =32'h003b52e0;
rtla[6] =32'h0076a5c0;
rtla[7] =32'h00ed4b80;
rtla[8] =32'h01da9700;
rtla[9] =32'h03b52e00;
rtla[10] =32'h076a5c00;
rtla[11] =32'h0ed4b800;
rtla[12] =32'h1da97000;
rtla[13] =32'h3b52e000;
rtla[14] =32'h76a5c000;
rtla[15] =32'hed4b8000;
rtla[16] =32'hda96da97;
rtla[17] =32'hb52c6fb9;
rtla[18] =32'h6a5905e5;
rtla[19] =32'hd4b20bca;
rtla[20] =32'ha965cd03;
rtla[21] =32'h52ca4091;
rtla[22] =32'ha5948122;
rtla[23] =32'h4b28d8d3;
rtla[24] =32'h9651b1a6;
rtla[25] =32'h2ca2b9db;
rtla[26] =32'h594573b6;
rtla[27] =32'hb28ae76c;
rtla[28] =32'h6514144f;
rtla[29] =32'hca28289e;
rtla[30] =32'h94518bab;
rtla[31] =32'h28a2cdc1;
rtla[32] =32'h51459b82;
rtla[33] =32'ha28b3704;
rtla[34] =32'h4517b49f;
rtla[35] =32'h8a2f693e;
rtla[36] =32'h145f08eb;
rtla[37] =32'h28be11d6;
rtla[38] =32'h517c23ac;
rtla[39] =32'ha2f84758;
rtla[40] =32'h45f15427;
rtla[41] =32'h8be2a84e;
rtla[42] =32'h17c48a0b;
rtla[43] =32'h2f891416;
rtla[44] =32'h5f12282c;
rtla[45] =32'hbe245058;
rtla[46] =32'h7c497a27;
rtla[47] =32'hf892f44e;
rtla[48] =32'hf124320b;
rtla[49] =32'he249be81;
rtla[50] =32'hc492a795;
rtla[51] =32'h892495bd;
rtla[52] =32'h1248f1ed;
rtla[53] =32'h2491e3da;
rtla[54] =32'h4923c7b4;
rtla[55] =32'h92478f68;
rtla[56] =32'h248ec447;
rtla[57] =32'h491d888e;
rtla[58] =32'h923b111c;
end

/******************************************************
COMBINATIONAL LOGIC
*******************************************************/

/*When data is ready to be fingerprinted, if sel is high,
		then take the MSB XORed with the previous CRC.
		Otherwise, take the data alone.
		*/
assign xor_out = d_in_i[dw-1:dw-cw] ^ (sel ? (crc_out_i) : 0); 


genvar i;
assign stage_2 = {xor_out,d_in_i[dw-cw-1:0]};

generate
	for (i = 0; i < dw-cw; i = i +1)begin:gen1
		assign g[i] = count_mode_i ? (stage_2[i]?rtla[i]:0) : (0);
	end
	for (i = dw-cw; i<dw; i = i + 1)begin:gen2
		assign g[i] = count_mode_i ? (stage_2[i]?rtla[i]:0) : (stage_2[i]?rtla[i-(dw-cw)]:0);
	end
endgenerate

genvar j;
generate
	assign h[0] = g[0];
	for (j = 1; j < dw; j = j +1)begin:gen3
		assign h[j] = h[j-1]^g[j];
	end
endgenerate	

assign crc_out = crc_out_i;

/******************************************************
SEQUENTIAL LOGIC
*******************************************************/

always @ (posedge clk or posedge rst or posedge pause_reset)
begin	
	if (rst | pause_reset) begin
		d_in_i = 0;
		crc_out_i = 0;
		old_crc = 0;
	end else begin
		if (d_latch) begin
			d_in_i=  d_in[dw-1:0];
		end		
		if (en_fcs)begin
			old_crc = crc_out_i;
			crc_out_i = h[dw-1];
		end else if(unpause_load)begin
			crc_out_i = load_data;
			old_crc = load_data;
		end
	count_mode_i = count_mode;
	end

end

lifo 
  #(
    .depth(`PAUSE_DEPTH),
    .width(`CRC_WIDTH)
    )paused_crc_stack
  (
   .clk(clk),
   .reset(rst),
   .push(pause_store),
   .push_data(old_crc),
   .pop(unpause_popstack),
   .tos(load_data) //top of stack
);
   
   
   

//If pause_store, then put second to last CRC in buffer, reset crc_out_i reg to 0, 






endmodule
