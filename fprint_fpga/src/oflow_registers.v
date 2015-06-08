`include "crc_defines.v"
module oflow_registers(

	input										clk,
	input										reset,
	
	output									oflow_write,
	output [`NIOS_DATA_WIDTH-1 : 0]		oflow_writedata,
	output [`NIOS_ADDRESS_WIDTH-1 : 0]		oflow_address,
	input										oflow_waitrequest,
	
	//from CSR when setting max count
	input										csr_maxcount_write,
	input [(`NIOS_DATA_WIDTH-1):0]	csr_maxcount_writedata,
	
	input										count_inc,
	input										count_dec,
	
	input	[`CRC_KEY_WIDTH-1 : 0]		count_inc_physical_core_id,
	
	input [`CRC_KEY_WIDTH-1 : 0]		count_inc_task_id,
	input [`CRC_KEY_WIDTH-1 : 0]		count_dec_task_id,
	
	input 									count_inc_logical_core_id
);


reg [`CRC_KEY_WIDTH : 0]							physical_id_table [`CRC_KEY_SIZE-1 : 0];
wire [`CRC_KEY_WIDTH-1 : 0]						overflow_physical_core_id;
wire [`CRC_KEY_WIDTH-1 : 0]						underflow_physical_core_id;
wire														overflow_present_on_inc;
wire														overflow_present_on_dec;
	
reg [`CRC_KEY_WIDTH-1 : 0]							count_inc_physical_core_id_reg;
	
reg [`CRC_KEY_WIDTH-1 : 0]							count_inc_task_id_reg;
reg [`CRC_KEY_WIDTH-1 : 0]							count_dec_task_id_reg;
	
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_mem_maxcount	[(`CRC_KEY_SIZE-1):0];
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_mem_count_0	[(`CRC_KEY_SIZE-1):0];
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_mem_count_1	[(`CRC_KEY_SIZE-1):0];
			
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_mem_count_on_inc_0;	
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_mem_count_on_inc_1;
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_mem_count_on_dec_0;
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_mem_count_on_dec_1;

wire														LC0_overflow;
wire														LC0_underflow;
wire														LC1_overflow;
wire														LC1_underflow;
			
wire														overflow;
wire														underflow;
	
reg														overflow_reg;
reg														underflow_reg;
			
wire														fprint_mem_count_inc_0;
wire														fprint_mem_count_inc_1;
wire														fprint_mem_count_dec_0;
wire														fprint_mem_count_dec_1;
	
wire [`CRC_KEY_WIDTH-1 : 0]						csr_maxcount_task_id;
wire [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				csr_maxcount_value;

wire [`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : 0]	overflow_fifo_data_in;
wire														overflow_fifo_rd_en;
wire														overflow_fifo_wr_en;
wire [`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : 0]	overflow_fifo_data_out;
wire														overflow_fifo_empty;
wire														overflow_fifo_full;

wire [`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : 0]	underflow_fifo_data_in;
wire														underflow_fifo_rd_en;
wire														underflow_fifo_wr_en;
wire [`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : 0]	underflow_fifo_data_out;
wire														underflow_fifo_empty;
wire														underflow_fifo_full;

reg [`CRC_KEY_WIDTH+`CRC_KEY_WIDTH : 0]		fifo_output;

/*************************************************************
* Decode
**************************************************************/
assign csr_maxcount_task_id = csr_maxcount_writedata[(`CRC_RAM_ADDRESS_WIDTH+`CRC_KEY_WIDTH-1) : `CRC_RAM_ADDRESS_WIDTH];
assign csr_maxcount_value = csr_maxcount_writedata[`CRC_RAM_ADDRESS_WIDTH-1 : 0];

assign fprint_mem_count_inc_0 =	count_inc ? ~count_inc_logical_core_id ? count_dec ? 
											count_inc_task_id == count_dec_task_id? 0 :1 :1 :0 :0;
									
assign fprint_mem_count_inc_1 =	count_inc ? count_inc_logical_core_id ? count_dec ? 
											count_inc_task_id == count_dec_task_id? 0 :1 :1 :0 :0;

assign fprint_mem_count_dec_0	=	count_dec ? count_inc ? ~count_inc_logical_core_id ? 
											count_inc_task_id == count_dec_task_id ? 0 :1 :1 :1 :0;
											
assign fprint_mem_count_dec_1	=	count_dec ? count_inc ? count_inc_logical_core_id ? 
											count_inc_task_id == count_dec_task_id ? 0 :1 :1 :1 :0;

assign LC0_overflow				=	fprint_mem_count_inc_0 ? 
											fprint_mem_count_on_inc_0 >= fprint_mem_maxcount[count_inc_task_id] - 1? 
											1 :0 :0;
											
assign LC0_underflow				= 	fprint_mem_count_dec_0 ? fprint_mem_count_on_dec_0 == 1 ? 1 :0 :0;

assign LC1_overflow				=	fprint_mem_count_inc_1 ? 
											fprint_mem_count_on_inc_1 >= fprint_mem_maxcount[count_inc_task_id] - 1? 
											1 :0 :0;
											
assign LC1_underflow				= 	fprint_mem_count_dec_1 ? fprint_mem_count_on_dec_1 == 1 ? 1 :0 :0;

assign overflow					=	LC0_overflow | LC1_overflow;
assign underflow					=	LC0_underflow & LC1_underflow;

assign overflow_fifo_wr_en		=	overflow & (~overflow_present_on_inc);
assign overflow_fifo_data_in	=	{count_inc_physical_core_id, count_inc_task_id};

assign underflow_fifo_wr_en	=	underflow & overflow_present_on_dec;
assign underflow_fifo_data_in	=	{underflow_physical_core_id, count_dec_task_id};


/*************************************************************
* overflow FIFO
**************************************************************/
fifo #(`CRC_KEY_WIDTH+`CRC_KEY_WIDTH, `CRC_KEY_SIZE/2)
overflow_fifo (
clk,
reset,
overflow_fifo_data_in,
overflow_fifo_rd_en,
overflow_fifo_wr_en,
overflow_fifo_data_out,
overflow_fifo_empty,
overflow_fifo_full
);

/*************************************************************
* underflow FIFO
**************************************************************/
fifo #(`CRC_KEY_WIDTH+`CRC_KEY_WIDTH, `CRC_KEY_SIZE/2)
underflow_fifo (
clk,
reset,
underflow_fifo_data_in,
underflow_fifo_rd_en,
underflow_fifo_wr_en,
underflow_fifo_data_out,
underflow_fifo_empty,
underflow_fifo_full
);

/*************************************************************
* processor interface FSM
**************************************************************/
reg[3:0] state;

parameter idle 							= 0;
parameter st_overflow_fifo_read		= 1;
parameter st_underflow_fifo_read		= 2;
parameter st_overflow_output			= 3;
parameter st_underflow_output			= 4;
parameter st_output_ready				= 5;

always @(posedge clk or posedge reset)
begin
	if(reset)
		state = idle;
	else begin
		case(state)
			idle:
				if(~overflow_fifo_empty)
					state = st_overflow_fifo_read;
				else if(~underflow_fifo_empty)
					state = st_underflow_fifo_read;
			
			st_overflow_fifo_read:
				state = st_overflow_output;
			
			st_underflow_fifo_read:
				state = st_underflow_output;
			
			st_overflow_output:
				state = st_output_ready;
				
			st_underflow_output:
				state = st_output_ready;
			
			st_output_ready:
				if(~oflow_waitrequest)
					state = idle;
		endcase
	end
end

/*************************************************************
* processor interface FSM outputs
**************************************************************/
assign overflow_fifo_rd_en		=	(state == st_overflow_fifo_read);
assign underflow_fifo_rd_en	=	(state == st_underflow_fifo_read);
wire a;
wire b;

assign a = state == st_overflow_output;
assign b = state == st_underflow_output;


always @ (posedge clk or posedge reset) begin
	if(reset)
		fifo_output = 0;
	else begin
		if(a)
			fifo_output = {1'b1,overflow_fifo_data_out};
		else if(b)
			fifo_output = {1'b0,underflow_fifo_data_out};
	end
end

assign oflow_write		= (state == st_output_ready);
assign oflow_address		=	`OFLOW_OFFSET+{fifo_output[`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : `CRC_KEY_WIDTH],20'h0};
assign oflow_writedata	=	{1'h1,9'h0}+fifo_output;


/*************************************************************
max count "SRAM"

we need ONE write: at address 'csr_maxcount_task_id'
we need TWO reads: at addresses 'fifo_fprint_task_id' and 'comp_task'

how? this is synthesized into registers right now
**************************************************************/
integer i;
	
	initial begin
		for(i=0 ; i<16 ; i=i+1) begin
			fprint_mem_maxcount[i] = 0;
		end
	end

always @ (posedge clk)
begin
	if(csr_maxcount_write) begin
		fprint_mem_maxcount[csr_maxcount_task_id] = csr_maxcount_value;
	end
end

/*************************************************************
fprint mem counters
**************************************************************/
integer j;

	initial begin
		for(j=0 ; j<16 ; j=j+1) begin
			fprint_mem_count_0[j] = 0;
			fprint_mem_count_1[j] = 0;
		end
	end

	// logical core 0
	always @ (posedge clk) begin
		if (fprint_mem_count_inc_0) begin
			fprint_mem_count_0[count_inc_task_id] = fprint_mem_count_on_inc_0 + 1;
		end
			fprint_mem_count_on_inc_0 = fprint_mem_count_0[count_inc_task_id];
	end

	always @ (posedge clk) begin
		if (fprint_mem_count_dec_0) begin
			fprint_mem_count_0[count_dec_task_id] = fprint_mem_count_on_dec_0 - 1;
		end
			fprint_mem_count_on_dec_0 = fprint_mem_count_0[count_dec_task_id];
	end
	
	
	// logical core 1
	always @ (posedge clk) begin
		if (fprint_mem_count_inc_1) begin
			fprint_mem_count_1[count_inc_task_id] = fprint_mem_count_on_inc_1 + 1;
		end
			fprint_mem_count_on_inc_1 = fprint_mem_count_1[count_inc_task_id];
	end

	always @ (posedge clk) begin
		if (fprint_mem_count_dec_1) begin
			fprint_mem_count_1[count_dec_task_id] = fprint_mem_count_on_dec_1 - 1;
		end
			fprint_mem_count_on_dec_1 = fprint_mem_count_1[count_dec_task_id];
	end
	
/*************************************************************
logical core count status registers 

We may want these in the future?
**************************************************************/
/*
always @ (posedge clk or posedge reset)
begin
	if(reset)begin
		LC0_maxcount_status_reg = 0;
		LC1_maxcount_status_reg = 0;
	end else begin
			LC0_maxcount_status_reg = (LC0_maxcount_status_reg | LC0_status_inc_update) & (~ LC0_status_dec_update);
			LC1_maxcount_status_reg = (LC1_maxcount_status_reg | LC1_status_inc_update) & (~ LC1_status_dec_update);
	end
end

*/

/*************************************************************
physical id table
**************************************************************/
integer k;

	initial begin
		for(k=0 ; k<16 ; k=k+1) begin
			physical_id_table[k] = 0;
		end
	end

always @ (posedge clk)
begin

	if(overflow & (~overflow_present_on_inc))
		physical_id_table[count_inc_task_id] = {1'b1,count_inc_physical_core_id};

	if(underflow)
		physical_id_table[count_dec_task_id] = 0;

end

assign overflow_present_on_inc = physical_id_table[count_inc_task_id][`CRC_KEY_WIDTH];
assign overflow_physical_core_id = physical_id_table[count_inc_task_id][`CRC_KEY_WIDTH-1 : 0];
assign overflow_present_on_dec = physical_id_table[count_dec_task_id][`CRC_KEY_WIDTH];		
assign underflow_physical_core_id = physical_id_table[count_dec_task_id][`CRC_KEY_WIDTH-1 : 0];
	
/*************************************************************
internal registers
**************************************************************/	
/*
always @ (posedge clk or posedge reset)
begin
	if(reset)begin
		count_inc_task_id_reg = 0;
		count_dec_task_id_reg = 0;
		count_inc_physical_core_id_reg = 0;
		overflow_reg = 0;
		underflow_reg = 0;
	end else begin
		count_inc_task_id_reg = count_inc_task_id;
		count_dec_task_id_reg = count_dec_task_id;
		count_inc_physical_core_id_reg = count_inc_physical_core_id;
		overflow_reg = overflow;
		underflow_reg = underflow;
	end
end
*/
	
endmodule
