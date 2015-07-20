`include "crc_defines.v"

module fprint_registers(

	input								    	clk,
	input								    	reset,

	//processors
	input 	[(`COMPARATOR_ADDRESS_WIDTH-1):0]	fprint_address,
	input										fprint_write,
	input 	[(`NIOS_DATA_WIDTH-1):0]			fprint_writedata,
	output										fprint_waitrequest,

	//csr_registers
	output [(`CRC_KEY_WIDTH-1):0]				fprint_task_id,
	output [(`CRC_KEY_WIDTH-1):0]				fprint_physical_core_id,
	input [1:0]									fprint_logical_core_id,
	
	input [(`CRC_KEY_SIZE-1):0]					fprint_nmr,

	//oflow_registers
	output										fprint_count_inc,
	input										oflow_count_inc_ack,
	
	//comp_registers
	output 										fprint_inc_head_pointer,
	input 										comp_inc_head_pointer_ack,

	input [`CRC_RAM_ADDRESS_WIDTH-1:0]     		fprint_head_pointer,
	input [`CRC_RAM_ADDRESS_WIDTH-1:0] 			comp_tail_pointer_0,
	input [`CRC_RAM_ADDRESS_WIDTH-1:0] 			comp_tail_pointer_1,
	input [`CRC_RAM_ADDRESS_WIDTH-1:0] 			comp_tail_pointer_2,
	output [`CRC_WIDTH - 1: 0] 					fprint_0,
	output [`CRC_WIDTH - 1: 0]             		fprint_1,
	output [`CRC_WIDTH - 1: 0]             		fprint_2,
	
	//comparator
	input [(`CRC_KEY_WIDTH-1):0] 				comparator_task_id,
	
	input 										fprint_reset_task,
	output 										fprint_reset_task_ack,
	
	output [(`CRC_KEY_SIZE-1):0]  				fprint_checkin

);

reg  [(`CRC_KEY_SIZE-1):0]                  checkout_reg[2:0];
reg  [(`CRC_KEY_SIZE-1):0]                  checkin_reg[2:0];

//Need to do each half separately in Verilog...
reg  [`CRC_WIDTH/2 - 1: 0] 					fprint_mem_0_0[`CRC_RAM_SIZE -1 :0];
reg  [`CRC_WIDTH/2 - 1: 0] 					fprint_mem_0_1[`CRC_RAM_SIZE -1 :0];
reg  [`CRC_WIDTH/2 - 1: 0] 					fprint_mem_1_0[`CRC_RAM_SIZE -1 :0];
reg  [`CRC_WIDTH/2 - 1: 0] 					fprint_mem_1_1[`CRC_RAM_SIZE -1 :0];
reg  [`CRC_WIDTH/2 - 1: 0] 					fprint_mem_2_0[`CRC_RAM_SIZE -1 :0];
reg  [`CRC_WIDTH/2 - 1: 0] 					fprint_mem_2_1[`CRC_RAM_SIZE -1 :0];

reg [`CRC_WIDTH/2 - 1: 0] 					fprint_0_0;
reg [`CRC_WIDTH/2 - 1: 0] 					fprint_0_1;
reg [`CRC_WIDTH/2 - 1: 0] 					fprint_1_0;
reg [`CRC_WIDTH/2 - 1: 0] 					fprint_1_1;
reg [`CRC_WIDTH/2 - 1: 0] 					fprint_2_0;
reg [`CRC_WIDTH/2 - 1: 0] 					fprint_2_1;

wire [`CRC_WIDTH/2 - 1: 0] 					fprint_mem_data_in;

wire 										fprint_regs_write;
wire										fprint_mem_write;
wire 										fprint_block;
wire										comparator_regs_write;

wire		fifo_rd_en;
wire		fifo_wr_en;
wire 		fifo_full;
wire		fifo_empty;
wire [31:0]	fifo_data_in;
wire [31:0]	fifo_data_out;

integer i,j;
/*************************************************************
* Decode
**************************************************************/
wire checkout_sel;
wire checkin_sel;
wire fprint_sel;

assign fifo_data_in				=	{fprint_writedata[31:16],2'b00,fprint_address[7:0],fprint_writedata[5:0]};

assign fprint_mem_data_in		=	fifo_data_out[31:16];
assign fprint_physical_core_id	=	fifo_data_out[13:10];
assign fprint_task_id			=	fifo_data_out[3:0];
	
assign checkout_sel				=	(fifo_data_out[9:6] == `COMPARATOR_CS_OFFSET) & fifo_data_out[4];
assign checkin_sel				=	(fifo_data_out[9:6] == `COMPARATOR_CS_OFFSET) & ~fifo_data_out[4];
assign fprint_sel				=	(fifo_data_out[9:6] == `COMPARATOR_CRC_OFFSET);
	
assign fprint_block				=	(fifo_data_out[5]);

assign fprint_0					=	{fprint_0_1,fprint_0_0};
assign fprint_1					=	{fprint_1_1,fprint_1_0};
assign fprint_2					=	{fprint_2_1,fprint_2_0};
assign checkin_reg_out			=	checkin_reg[0] & checkin_reg[1] & checkin_reg[2];
/*************************************************************
* FIFO FSM
**************************************************************/
reg[3:0] FIFO_state;

parameter idle 				= 0;
parameter st_fifo_wr_en		= 1;
parameter st_waitrequest    = 2;

always @(posedge clk or posedge reset)
begin
	if(reset)
		FIFO_state = idle;
	else begin
		case(FIFO_state)
			idle:
				if(fprint_write & ~fifo_full)
					FIFO_state = st_fifo_wr_en;
					
			st_fifo_wr_en:
				FIFO_state = st_waitrequest;
			
			st_waitrequest:
				FIFO_state = idle;	

		endcase
	end
end

/*************************************************************
* FIFO FSM outputs
**************************************************************/
assign fifo_wr_en           = (FIFO_state == st_fifo_wr_en);
assign fprint_waitrequest   = ~(FIFO_state == st_waitrequest);

/*************************************************************
* FIFO

**************************************************************/
fifo #(32, 4)
fifo_block (
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
* FSM
**************************************************************/
reg[3:0] state;

parameter st_comparator_regs_write		= 1;
parameter st_fprint_reset_task_ack	= 2;
parameter st_fifo_rd_en					= 3;
parameter st_decode						= 4;
parameter st_fprint_regs_write			= 5;
parameter st_fprint_mem_write			= 6;
parameter st_fprint_inc_head_pointer	= 7;
parameter st_fprint_count_inc			= 8;


always @(posedge clk or posedge reset)
begin
	if(reset)
		state = idle;
	else begin
		case(state)

			idle:
				if(fprint_reset_task)
					state = st_comparator_regs_write;
				else if(~fifo_empty)
                    state = st_fifo_rd_en;

			st_comparator_regs_write:
				state = st_fprint_reset_task_ack;
								
			st_fprint_reset_task_ack:
				state = idle;
			
			st_fifo_rd_en:
                state = st_decode;

			st_decode:
                if(fprint_sel) begin
					if(checkout_reg[fprint_logical_core_id][fprint_task_id])
						state = st_fprint_mem_write;
					else 
						state = idle;
				end else if(~fprint_sel)
                    state = st_fprint_regs_write;

			st_fprint_regs_write:
				state = idle;

			st_fprint_mem_write:
				if(fprint_block)
					state = st_fprint_inc_head_pointer;
				else 
					state = idle;

			st_fprint_inc_head_pointer:
				if(comp_inc_head_pointer_ack)
					state = st_fprint_count_inc;
					
			st_fprint_count_inc:
				if(oflow_count_inc_ack)
					state = idle;				
		endcase
	end
end

/*************************************************************
* FSM outputs
**************************************************************/
assign comparator_regs_write	= (state == st_comparator_regs_write);
assign fprint_reset_task_ack	= (state == st_fprint_reset_task_ack);

assign fifo_rd_en           	= (state == st_fifo_rd_en);
assign fprint_regs_write  		= (state == st_fprint_regs_write);
assign fprint_mem_write			= (state == st_fprint_mem_write);
assign fprint_inc_head_pointer	= (state == st_fprint_inc_head_pointer);
assign fprint_count_inc			= (state == st_fprint_count_inc);
/*************************************************************
* Registers
**************************************************************/
always @ (posedge clk or posedge reset)
begin
	if(reset)begin
	
		for(i=0 ; i<3 ; i=i+1) begin
			checkout_reg[i] = 0;
			checkin_reg[i] = 0;
		end

	end else begin			
		
		if(comparator_regs_write)begin
		
			for(i=0 ; i<3 ; i=i+1) begin
				checkout_reg[i][comparator_task_id] = 0;
				checkin_reg[i][comparator_task_id] = 0;
			end
		
		end else if(fprint_regs_write)begin
			
			if(checkout_sel & ~(fprint_logical_core_id == 3))
				checkout_reg[fprint_logical_core_id][fprint_task_id] = 1;

			if (checkin_sel & checkout_reg[fprint_logical_core_id][fprint_task_id])
				checkin_reg[fprint_logical_core_id][fprint_task_id] = 1;

		end
	end
end

assign fprint_checkin = checkin_reg[0] & checkin_reg[1] & (checkin_reg[2] | ~fprint_nmr);

/*************************************************************
* CRC
**************************************************************/

always @ (posedge clk)
begin
	fprint_0_0 = fprint_mem_0_0[comp_tail_pointer_0];
	if(fprint_mem_write & (fprint_logical_core_id == 0) & ~fprint_block)begin
		fprint_mem_0_0[fprint_head_pointer] = fprint_mem_data_in; 
	end
end

always @ (posedge clk)
begin
	fprint_0_1 = fprint_mem_0_1[comp_tail_pointer_0];
	if(fprint_mem_write & (fprint_logical_core_id == 0) & fprint_block)begin
		fprint_mem_0_1[fprint_head_pointer] = fprint_mem_data_in; 
	end
end

always @ (posedge clk)
begin
	fprint_1_0 = fprint_mem_1_0[comp_tail_pointer_1];
	if(fprint_mem_write & (fprint_logical_core_id == 1) & ~fprint_block)begin
		fprint_mem_1_0[fprint_head_pointer] = fprint_mem_data_in; 
	end
end

always @ (posedge clk)
begin
	fprint_1_1 = fprint_mem_1_1[comp_tail_pointer_1];
	if(fprint_mem_write & (fprint_logical_core_id == 1) & fprint_block)begin
		fprint_mem_1_1[fprint_head_pointer] = fprint_mem_data_in; 
	end
end

always @ (posedge clk)
begin
	fprint_2_0 = fprint_mem_2_0[comp_tail_pointer_2];
	if(fprint_mem_write & (fprint_logical_core_id == 2) & ~fprint_block)begin
		fprint_mem_2_0[fprint_head_pointer] = fprint_mem_data_in; 
	end
end

always @ (posedge clk)
begin
	fprint_2_1 = fprint_mem_2_1[comp_tail_pointer_2];
	if(fprint_mem_write & (fprint_logical_core_id == 2) & fprint_block)begin
		fprint_mem_2_1[fprint_head_pointer] = fprint_mem_data_in; 
	end
end


endmodule
