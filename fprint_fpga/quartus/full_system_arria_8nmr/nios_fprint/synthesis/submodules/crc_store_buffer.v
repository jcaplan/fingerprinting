`include "crc_defines.v"
module crc_store_buffer#(
	parameter ADDR_WIDTH = 8
)(
	input								clk,
	input								reset,
	
	input								write,
	input  [`NIOS_DATA_WIDTH-1:0]		write_data,
	input  [`NIOS_ADDRESS_WIDTH-1:0]	write_address,
	
	output [`NIOS_ADDRESS_WIDTH-1:0]	master_address,
	output								master_read,
	input								master_waitrequest,
	input [`NIOS_DATA_WIDTH-1:0]		master_readdata,
	output								master_write,
	output[`NIOS_DATA_WIDTH-1:0]		master_writedata,
	input								pause_reset,
	input								fprint_enable,
	input								crc_ready,
	input								data_write,
	input [`CRC_KEY_WIDTH-1:0]          crc_task
	
);



/******************************************************
SIGNAL DECLARATIONS
*******************************************************/

wire [`CRC_DATA_WIDTH-1:0]			fifo_data_in;
wire 								fifo_rd_en;
wire								fifo_wr_en;
wire [`CRC_DATA_WIDTH-1:0]			fifo_data_out;
wire								fifo_empty;
wire								fifo_full;

wire 								crc_task_rd_en;
wire [`CRC_KEY_WIDTH-1:0]			crc_task_data_out;
reg	 [3:0]							state;
reg									crc_ready_reg;
reg									fprint_legal;
/******************************************************
STATE VALUES
*******************************************************/

parameter	init = 0;
parameter	load_data = 1;
parameter	address_check = 2;
parameter	m_write = 3;
parameter   crc_write_0 = 4;
parameter   crc_write_1 = 5;
parameter   crc_write_2 = 6;

/*******************************************************
COMBINATIONAL ASSIGNMENTS
********************************************************/

assign fifo_data_in 		= {write_data,write_address};
assign fifo_wr_en 			= write;
assign master_writedata 	= (state == crc_write_0) ? {fifo_data_out[42:27],12'b0,crc_task_data_out} : (state == crc_write_2) ? {fifo_data_out[58:43],12'b10,crc_task_data_out} :  fifo_data_out[58:27];
assign master_address 		= fifo_data_out[26:0];
assign fifo_rd_en 			= (state == load_data);
assign crc_task_rd_en       = (fifo_rd_en | (state == crc_write_1));
assign master_write			= (state == m_write) | (state == crc_write_0) | (state == crc_write_2);
assign master_read			= 0;

/*******************************************************
SEQUENTIAL ASSIGNMENTS
********************************************************/

//legal_fprint is used so that the exact timing of 
//the last write does not matter. If the last legal fingerprint
//is also the end of a block and therefore passed on to the comparator,
//it will only be released after another write comes in.
//Even if that last write is the junk pause write, we still want it
//to pass the correct fingerprint.
always @ (posedge clk or posedge reset)
begin
	if(reset)begin
		crc_ready_reg = 0;
	end else begin
		if(crc_ready)
			crc_ready_reg = 1;
		else if((master_write) || (pause_reset == 1))
			crc_ready_reg = 0;
	end
end

always @ (posedge clk or posedge reset)
begin
	if(reset)begin
		fprint_legal = 0;
	end else begin
		if(crc_ready_reg & data_write)
			fprint_legal = 1;
		else if((master_write) || (pause_reset == 1))
			fprint_legal = 0;
	
	end
end

/*******************************************************
FSM
********************************************************/

always @ (posedge clk or posedge reset)
begin
	if(reset)begin
		state = init;
	end else begin
		case(state)
			init:
				if(!fifo_empty)
					state = load_data;
				else
					state = init;
			load_data:				
				state = address_check;
			address_check:
			/* Here is where the address needs to be checked.
				 * If it is a fingerprint, then we need to wait to find out
				 * if it is a faulty fingerprint because the one extra
				 * write required to pause fingerprinting could create an 
				 * extra fingerprint in the unlucky event where the 
				 * extra write happens to be the last write in a block. In this case
				 * we wait to see what comes first: 
				 * 			1) a pause signal (which means skip the write)
				 * 			2) either new data or the end of the task (which means write the data).
				 *			(new data can be the junk data)
				 */
				 
				 if(master_address[5:0] == `COMPARATOR_CRC_OFFSET_4x)begin
					if(pause_reset)
						state = init;
					else if((fprint_enable == 0) || (fprint_legal == 1))
						state = crc_write_0;
					else
						state = address_check;
				 end else 
					state = m_write;
			m_write:
				if(!master_waitrequest)
					state = init;
			crc_write_0:
				if(!master_waitrequest)
					state = crc_write_1;
			crc_write_1:
				state = crc_write_2;
			crc_write_2:
				if(!master_waitrequest)
					state = init;
			default:
				state = init;
		endcase
	end
end


/*******************************************************
FIFO
********************************************************/
fifo #(
	.DATA_WIDTH(`CRC_DATA_WIDTH),		//32 CRC bits and 27 address bits
	.ADDR_WIDTH(ADDR_WIDTH)		//8 fingerprints, plenty of safety room
) data (
	.clk(clk), 					// Clock input
	.rst(reset), 					// Active high reset
	.data_in(fifo_data_in), 	// Data input
	.rd_en(fifo_rd_en), 		// Read enable
	.wr_en(fifo_wr_en), 		// Write Enable
	.data_out(fifo_data_out), 	// Data Output
	.empty(fifo_empty), 		// FIFO empty
	.full(fifo_full)    	   	// FIFO full
); 

//require the MSB of the register (enabled or disabled)
fifo #(
	.DATA_WIDTH(`CRC_KEY_WIDTH),		//32 CRC bits and 27 address bits
	.ADDR_WIDTH(ADDR_WIDTH)		//8 fingerprints, plenty of safety room
	)  crc_task_fifo (
	.clk(clk), 					// Clock input
	.rst(reset), 					// Active high reset
	.data_in(crc_task), 	// Data input
	.rd_en(crc_task_rd_en), 		// Read enable
	.wr_en(fifo_wr_en), 		// Write Enable
	.data_out(crc_task_data_out) 	// Data Output
	//.empty(fifo_empty), 		// FIFO empty
	//.full(fifo_full)    	   	// FIFO full
); 
 
endmodule
