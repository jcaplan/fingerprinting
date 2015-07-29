`include "crc_defines.v"
module oflow_registers(

	input											clk,
	input											reset,
	
	//processors
	output											oflow_write,
	output [`NIOS_DATA_WIDTH-1 : 0]					oflow_writedata,
	output [`NIOS_ADDRESS_WIDTH-1 : 0]				oflow_address,
	input											oflow_waitrequest,
	
	//global
	output reg [(`CRC_KEY_WIDTH-1):0]				oflow_task_id,
	output reg [1:0]								oflow_logical_core_id,
	input [(`CRC_KEY_WIDTH-1):0]					oflow_physical_core_id,
	
	//csr_registers
	input [`CRC_RAM_ADDRESS_WIDTH-1 : 0]			csr_fprint_maxcount,
	input											oflow_nmr,
	
	//fprint_registers
	input [`CRC_KEY_WIDTH-1 : 0]					fprint_task_id,
	input [1:0]										fprint_logical_core_id,

	input											fprint_count_inc,
	output											oflow_count_inc_ack,
	
	//comparator
	input [`CRC_KEY_WIDTH-1 : 0]					comparator_task_id,
		
	input											comparator_count_dec,
	output											oflow_count_dec_ack,
	
	output reg [`CRC_KEY_SIZE-1 : 0]				oflow_fprints_ready,
	output reg [`CRC_KEY_SIZE-1 : 0]				oflow_fprints_remaining,
	output reg [1:0]								oflow_fprints_remaining_logical_core_id,
	
	input											oflow_reset_task,
	output											oflow_reset_task_ack
);

wire											fprint_oflow_status;
wire											comparator_oflow_status;

wire [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_count;
			
wire											overflow;
wire											underflow;


reg	[`CRC_KEY_SIZE-1 : 0]						overflow_status_reg_0;
reg	[`CRC_KEY_SIZE-1 : 0]						overflow_status_reg_1;
reg	[`CRC_KEY_SIZE-1 : 0]						overflow_status_reg_2;

wire											fprint_oflow_status_0;
wire											fprint_oflow_status_1;
wire											fprint_oflow_status_2;
wire											comparator_oflow_status_0;
wire											comparator_oflow_status_1;
wire											comparator_oflow_status_2;
	
wire											reset_task;

wire											set_task;

wire											count_inc;
wire											count_dec;

wire											fprint_regs_set;
wire											comparator_regs_set;

wire											count_logical_id;
									
wire											oflow_status_set; 
wire											oflow_status_reset;

reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_mem_count_0		[(`CRC_KEY_SIZE-1):0];
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_mem_count_1		[(`CRC_KEY_SIZE-1):0];
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_mem_count_2		[(`CRC_KEY_SIZE-1):0];

wire [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_count_0;
wire [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_count_1;
wire [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_count_2;
wire [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				comparator_count_0;
wire [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				comparator_count_1;
wire [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				comparator_count_2;

wire											fifo_uflow_write_done;

wire [`CRC_KEY_WIDTH+`CRC_KEY_WIDTH : 0]		fifo_data_in;
wire											fifo_rd_en;
wire											fifo_wr_en;
wire [`CRC_KEY_WIDTH+`CRC_KEY_WIDTH : 0]		fifo_data_out;
wire											fifo_empty;
wire											fifo_full;

/*************************************************************
* Decode
**************************************************************/

assign fprint_oflow_status		=	fprint_logical_core_id == 0 ? fprint_oflow_status_0 :
									fprint_logical_core_id == 1 ? fprint_oflow_status_1 :
									fprint_logical_core_id == 2 ? fprint_oflow_status_2 :
									0;								
assign comparator_oflow_status	=	comparator_oflow_status_0 | comparator_oflow_status_1 | comparator_oflow_status_2;

assign fprint_count				=	fprint_logical_core_id == 0 ? fprint_count_0 :
									fprint_logical_core_id == 1 ? fprint_count_1 :
									fprint_logical_core_id == 2 ? fprint_count_2 :
									0;

assign overflow					=	(~fprint_oflow_status) & (fprint_count >= csr_fprint_maxcount);
assign underflow				=	(comparator_oflow_status_0 & (comparator_count_0 == 0)) |
									(comparator_oflow_status_1 & (comparator_count_1 == 0)) |
									(comparator_oflow_status_2 & (comparator_count_2 == 0) & oflow_nmr); 


assign fifo_uflow_write_done	=	oflow_nmr ? (oflow_logical_core_id == 2) : (oflow_logical_core_id == 1);


/*************************************************************
fprint regs
**************************************************************/
initial begin

	oflow_fprints_ready							= 0;
	oflow_fprints_remaining						= 0;
	oflow_fprints_remaining_logical_core_id		= 0;

end

always @ (posedge clk) begin
	
	if(reset_task) begin
		
		oflow_fprints_ready[comparator_task_id]		= 0;
		oflow_fprints_remaining[comparator_task_id]	= 0;
		oflow_fprints_remaining_logical_core_id		= 0;
		
	end else if(fprint_regs_set) begin
		
		oflow_fprints_ready[fprint_task_id]			=	~((fprint_count_0 == 0) | (fprint_count_1 == 0) | ((fprint_count_2 == 0) & oflow_nmr));
		oflow_fprints_remaining[fprint_task_id]		=	~((fprint_count_0 == 0) & (fprint_count_1 == 0) & ((fprint_count_2 == 0) | ~oflow_nmr));
		oflow_fprints_remaining_logical_core_id		=	~((fprint_count_0 == fprint_count_1) | (fprint_count_2 == fprint_count_0)) ? 0 :
														~((fprint_count_1 == fprint_count_2) | (fprint_count_0 == fprint_count_1)) ? 1 :
														~((fprint_count_2 == fprint_count_0) | (fprint_count_1 == fprint_count_2)) ? 2 :
														3;
													
	end else if (comparator_regs_set) begin
		
		oflow_fprints_ready[comparator_task_id]		=	~((comparator_count_0 == 0) | (comparator_count_1 == 0) | ((comparator_count_2 == 0) & oflow_nmr));
		oflow_fprints_remaining[comparator_task_id]	=	~((comparator_count_0 == 0) & (comparator_count_1 == 0) & ((comparator_count_2 == 0) | ~oflow_nmr));
		oflow_fprints_remaining_logical_core_id		=	~((comparator_count_0 == comparator_count_1) | (comparator_count_2 == comparator_count_0)) ? 0 :
														~((comparator_count_1 == comparator_count_2) | (comparator_count_0 == comparator_count_1)) ? 1 :
														~((comparator_count_2 == comparator_count_0) | (comparator_count_1 == comparator_count_2)) ? 2 :
														3;
	end
	
end

/*************************************************************
* fprint mem count FSM
**************************************************************/
reg[3:0] state;

parameter idle						= 0;
parameter st_count_inc				= 1;
parameter st_count_dec				= 2;
parameter st_fprint_regs_set		= 3;
parameter st_comparator_regs_set	= 4;
parameter st_fifo_oflow_write		= 5;
parameter st_fifo_uflow_write		= 6;
parameter st_oflow_status_set		= 7;
parameter st_oflow_status_reset		= 8;
parameter st_oflow_count_inc_ack	= 9;
parameter st_oflow_count_dec_ack	= 10;
parameter st_reset_task				= 11;
parameter st_oflow_reset_task_ack	= 12;

always @(posedge clk or posedge reset)
begin
	if(reset)
		state = idle;
	else begin
		case(state)
			idle:
				if(oflow_reset_task)
					state = st_reset_task;
				else if(fprint_count_inc)
					state = st_count_inc;
				else if(comparator_count_dec)
					state = st_count_dec;
			
			st_count_inc:
				state = st_fprint_regs_set;
			
			st_count_dec:
				state = st_comparator_regs_set;
				
			st_fprint_regs_set:
				if(overflow)
					state = st_fifo_oflow_write;
				else
					state = st_oflow_count_inc_ack;
					
			st_comparator_regs_set:
				if(underflow)
					state = st_fifo_uflow_write;
				else
					state = st_oflow_count_dec_ack;
			
			st_fifo_oflow_write:
				state = st_oflow_status_set;
				
			st_fifo_uflow_write:
				if(fifo_uflow_write_done)
					state = st_oflow_status_reset;
			
			st_oflow_status_set:
				state = st_oflow_count_inc_ack;
			
			st_oflow_status_reset:
				state = st_oflow_count_dec_ack;
				
			st_oflow_count_inc_ack:
				state = idle;
				
			st_oflow_count_dec_ack:
				state = idle;
			
			st_reset_task:
				state = st_oflow_reset_task_ack;
			
			st_oflow_reset_task_ack:
				state = idle;

		endcase
	end
end

/*************************************************************
* fprint mem count FSM outputs
**************************************************************/
assign set_task					=	(state == idle);

assign count_inc				=	(state == st_count_inc);
assign count_dec				=	(state == st_count_dec);
	
assign fprint_regs_set			=	(state == st_fprint_regs_set);
assign comparator_regs_set		=	(state == st_comparator_regs_set);
	
assign fifo_wr_en				=	(state == st_fifo_oflow_write) ? 1 :
									(state == st_fifo_uflow_write) ? 
									oflow_logical_core_id == 2 ? comparator_oflow_status_2 & (comparator_count_2 == 0) :
									oflow_logical_core_id == 1 ? comparator_oflow_status_1 & (comparator_count_1 == 0) :
									oflow_logical_core_id == 0 ? comparator_oflow_status_0 & (comparator_count_0 == 0) :
									0 : 0;
									
assign fifo_data_in				=	(state == st_fifo_oflow_write) ? {1'b1, oflow_physical_core_id, fprint_task_id}		: 
									(state == st_fifo_uflow_write) ? {1'b0, oflow_physical_core_id, comparator_task_id}	:
									0;
									
assign count_logical_id			=	(state == st_fifo_uflow_write);

assign oflow_status_set			=	(state == st_oflow_status_set); 
assign oflow_status_reset		=	(state == st_oflow_status_reset);

assign oflow_count_inc_ack		=	(state == st_oflow_count_inc_ack);
assign oflow_count_dec_ack		=	(state == st_oflow_count_dec_ack);


assign reset_task				=	(state == st_reset_task);
assign oflow_reset_task_ack		=	(state == st_oflow_reset_task_ack);

always @ (posedge clk) begin
	if(set_task) begin
		if(fprint_count_inc)
			oflow_task_id = fprint_task_id;
		else if(comparator_count_dec)
			oflow_task_id = comparator_task_id;
	end
end

always @ (posedge clk) begin
	if(count_logical_id)
		oflow_logical_core_id = oflow_logical_core_id + 1;
	else if(count_inc)
		oflow_logical_core_id = fprint_logical_core_id;
	else if(count_dec)
		oflow_logical_core_id = 0;
end

/*************************************************************
fprint mem count registers
**************************************************************/
integer k;

initial begin
	for(k=0 ; k<`CRC_KEY_SIZE ; k=k+1) begin
		fprint_mem_count_0[k] = 0;
		fprint_mem_count_1[k] = 0;
		fprint_mem_count_2[k] = 0;
	end
end

// logical core 0
always @ (posedge clk) begin

	if (count_inc & (fprint_logical_core_id == 0))
		fprint_mem_count_0[fprint_task_id] = fprint_count_0 + 1;
	
	if(reset_task)
		fprint_mem_count_0[comparator_task_id] = 0;
	else if (count_dec)
		fprint_mem_count_0[comparator_task_id] = comparator_count_0 - 1;

end
assign fprint_count_0 = fprint_mem_count_0[fprint_task_id];
assign comparator_count_0 = fprint_mem_count_0[comparator_task_id];

// logical core 1
always @ (posedge clk) begin

	if (count_inc & (fprint_logical_core_id == 1))
		fprint_mem_count_1[fprint_task_id] = fprint_count_1 + 1;
	
	if(reset_task)
		fprint_mem_count_1[comparator_task_id] = 0;
	else if (count_dec)
		fprint_mem_count_1[comparator_task_id] = comparator_count_1 - 1;

end
assign fprint_count_1 = fprint_mem_count_1[fprint_task_id];
assign comparator_count_1 = fprint_mem_count_1[comparator_task_id];
		
// logical core 2
always @ (posedge clk) begin
	
	if (count_inc & (fprint_logical_core_id == 2))
		fprint_mem_count_2[fprint_task_id] = fprint_count_2 + 1;
	
	if(reset_task)
		fprint_mem_count_2[comparator_task_id] = 0;
	else if (count_dec & oflow_nmr)
		fprint_mem_count_2[comparator_task_id] = comparator_count_2 - 1;

end
assign fprint_count_2 = fprint_mem_count_2[fprint_task_id];
assign comparator_count_2 = fprint_mem_count_2[comparator_task_id];
	
/*************************************************************
oflow status regs
**************************************************************/
integer l;

	initial begin
		for(l=0 ; l<`CRC_KEY_SIZE ; l=l+1) begin
			overflow_status_reg_0 = 0;
			overflow_status_reg_1 = 0;
			overflow_status_reg_2 = 0;
		end
	end

always @ (posedge clk)
begin
	if(oflow_status_set & (fprint_logical_core_id == 0)) begin
		overflow_status_reg_0[fprint_task_id] = 1;
	end
	
	if((oflow_status_reset & (comparator_count_0 == 0)) | reset_task) begin
		overflow_status_reg_0[comparator_task_id] = 0;
	end
end
assign fprint_oflow_status_0		= overflow_status_reg_0[fprint_task_id];
assign comparator_oflow_status_0	= overflow_status_reg_0[comparator_task_id];


always @ (posedge clk)
begin
	if(oflow_status_set & (fprint_logical_core_id == 1)) begin
		overflow_status_reg_1[fprint_task_id] = 1;
	end
	
	if((oflow_status_reset & (comparator_count_1 == 0)) | reset_task) begin
		overflow_status_reg_1[comparator_task_id] = 0;
	end
end
assign fprint_oflow_status_1		= overflow_status_reg_1[fprint_task_id];
assign comparator_oflow_status_1	= overflow_status_reg_1[comparator_task_id];


always @ (posedge clk)
begin
	if(oflow_status_set & (fprint_logical_core_id == 2)) begin
		overflow_status_reg_2[fprint_task_id] = 1;
	end
	
	if((oflow_status_reset & (comparator_count_2 == 0) & oflow_nmr) | reset_task) begin
		overflow_status_reg_2[comparator_task_id] = 0;
	end
end
assign fprint_oflow_status_2		= overflow_status_reg_2[fprint_task_id];
assign comparator_oflow_status_2	= overflow_status_reg_2[comparator_task_id];

/*************************************************************
* output FIFO
**************************************************************/
fifo #(`CRC_KEY_WIDTH+`CRC_KEY_WIDTH+1, `CRC_KEY_SIZE)
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
				if(~oflow_waitrequest)
					fifo_state = idle;
		endcase
	end
end

/*************************************************************
* processor interface FSM outputs
**************************************************************/
assign fifo_rd_en			=	(fifo_state == st_fifo_rd_en);

assign oflow_write			=	(fifo_state == st_output_ready);
assign oflow_address		=	`OFLOW_OFFSET+{fifo_data_out[`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : `CRC_KEY_WIDTH],20'h0};
assign oflow_writedata		=	{10'h200}+fifo_data_out;

endmodule
