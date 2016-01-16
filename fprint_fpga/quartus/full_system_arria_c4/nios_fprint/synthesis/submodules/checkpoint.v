`include "defines.v"
module checkpoint(

	input											clk,
	input											reset,
	
	//processors
	output											checkpoint_write,
	output [`NIOS_DATA_WIDTH-1 : 0]					checkpoint_writedata,
	output [`NIOS_ADDRESS_WIDTH-1 : 0]				checkpoint_address,
	input											checkpoint_waitrequest,

	//csr_registers
	output reg [1:0]								checkpoint_logical_core_id,
	input											checkpoint_nmr,
	
	input [(`CRC_KEY_WIDTH-1):0]					checkpoint_physical_core_id,	

	//comparator
	input[(`CRC_KEY_WIDTH-1):0]						comparator_task_id,
	input											comparator_checkpoint,
	output											checkpoint_ack
);

wire											checkpoint_done;
wire											checkpoint_logical_core_count;

wire [`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : 0]		fifo_data_in;
wire											fifo_rd_en;
wire											fifo_wr_en;
wire [`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : 0]		fifo_data_out;
wire											fifo_empty;
wire											fifo_full;

/*************************************************************
* checkpoint FSM
**************************************************************/
reg[3:0] state;

parameter idle					= 0;
parameter st_checkpoint			= 1;
parameter st_checkpoint_ack		= 2;

always @(posedge clk or posedge reset)
begin
	if(reset)
		state = idle;
	else begin
		case(state)

			idle:
				if(comparator_checkpoint)
					state = st_checkpoint;
			
			st_checkpoint:
				if(checkpoint_done)
					state = st_checkpoint_ack;
			
			st_checkpoint_ack:
				state = idle;

		endcase
	end
end

/*************************************************************
* checkpoint FSM outputs
**************************************************************/
assign checkpoint_logical_core_count	=	(state == st_checkpoint);
assign checkpoint_ack					=	(state == st_checkpoint_ack);

always @ (posedge clk) begin

	if (checkpoint_logical_core_count)
		checkpoint_logical_core_id = checkpoint_logical_core_id + 1;
	else
		checkpoint_logical_core_id = 0;
end

assign checkpoint_done = checkpoint_nmr ? checkpoint_logical_core_id == 2 : checkpoint_logical_core_id == 1;

assign fifo_wr_en				=	(state == st_checkpoint);								
assign fifo_data_in				=	{checkpoint_physical_core_id, comparator_task_id};

/*************************************************************
* output FIFO
**************************************************************/
fifo #(`CRC_KEY_WIDTH+`CRC_KEY_WIDTH, `CRC_KEY_SIZE)
overflow_fifo (
clk,
reset,
fifo_data_in,
fifo_rd_en,
fifo_wr_en,
fifo_data_out,
fifo_empty,
fifo_full
);

/*************************************************************
* processor interface FSM
**************************************************************/
reg[3:0] fifo_state;

parameter st_fifo_rd_en		= 1;
parameter st_output_ready	= 2;

always @(posedge clk or posedge reset)
begin
	if(reset)
		fifo_state = idle;
	else begin
		case(fifo_state)
			idle:
				if(~fifo_empty)
					fifo_state = st_fifo_rd_en;
			
			st_fifo_rd_en:
				fifo_state = st_output_ready;
			
			st_output_ready:
				if(~checkpoint_waitrequest)
					fifo_state = idle;
		endcase
	end
end

/*************************************************************
* processor interface FSM outputs
**************************************************************/
assign fifo_rd_en				=	(fifo_state == st_fifo_rd_en);

assign checkpoint_write			=	(fifo_state == st_output_ready);
assign checkpoint_address		=	`CHECKPOINT_OFFSET+{fifo_data_out[`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : `CRC_KEY_WIDTH],20'h0};
assign checkpoint_writedata		=	{9'h100}+fifo_data_out;

endmodule
