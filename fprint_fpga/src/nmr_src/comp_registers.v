`include "crc_defines.v"
module comp_registers(

	input									clk,
	input									reset,
	
	//csr_registers
	input [1:0]								csr_logical_core_id,
	input [(`CRC_KEY_WIDTH-1):0]			csr_task_id,
	
	input									csr_pointer_start_write,
	input									csr_pointer_end_write,
	input [`CRC_RAM_ADDRESS_WIDTH-1:0]		csr_pointer_data,
	output									comp_pointer_ack,

	//fprint_registers
	input [1:0]								fprint_logical_core_id,
	input [(`CRC_KEY_WIDTH-1):0]			fprint_task_id,
	
	input 									fprint_inc_head_pointer,
	output 									comp_inc_head_pointer_ack,

	output [`CRC_RAM_ADDRESS_WIDTH-1:0]     fprint_head_pointer,
	output [`CRC_RAM_ADDRESS_WIDTH-1:0] 	comp_tail_pointer_0,
	output [`CRC_RAM_ADDRESS_WIDTH-1:0] 	comp_tail_pointer_1,
	output [`CRC_RAM_ADDRESS_WIDTH-1:0] 	comp_tail_pointer_2,

	//comparator
	input [(`CRC_KEY_WIDTH-1):0] 			comparator_task_id,
	input 									comparator_inc_tail_pointer,
	input									comparator_reset_task
);

reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		start_pointer_mem_0	[`CRC_KEY_SIZE-1:0]; 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		start_pointer_mem_1	[`CRC_KEY_SIZE-1:0]; 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		start_pointer_mem_2	[`CRC_KEY_SIZE-1:0]; 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		end_pointer_mem_0	[`CRC_KEY_SIZE-1:0];
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		end_pointer_mem_1	[`CRC_KEY_SIZE-1:0];
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		end_pointer_mem_2	[`CRC_KEY_SIZE-1:0];

reg [`CRC_RAM_ADDRESS_WIDTH-1:0]		start_pointer_ex_0;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]		start_pointer_ex_1;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]		start_pointer_ex_2;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]		end_pointer_ex_0;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]		end_pointer_ex_1;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]		end_pointer_ex_2;

reg [`CRC_RAM_ADDRESS_WIDTH-1:0]		start_pointer_comp_0;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]		start_pointer_comp_1;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]		start_pointer_comp_2;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]		end_pointer_comp_0;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]		end_pointer_comp_1;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]		end_pointer_comp_2;

reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		head_pointer_mem_0	[(`CRC_KEY_SIZE-1):0];	//16 registers storing 10 bit pointers (RAM address size = 512), 1 per task 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		head_pointer_mem_1	[(`CRC_KEY_SIZE-1):0];	//16 registers storing 10 bit pointers (RAM address size = 512), 1 per task 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		head_pointer_mem_2	[(`CRC_KEY_SIZE-1):0];	//16 registers storing 10 bit pointers (RAM address size = 512), 1 per task 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		tail_pointer_mem_0	[(`CRC_KEY_SIZE-1):0];	//16 registers storing 10 bit pointers (RAM address size = 512), 1 per task 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		tail_pointer_mem_1	[(`CRC_KEY_SIZE-1):0];	//16 registers storing 10 bit pointers (RAM address size = 512), 1 per task 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		tail_pointer_mem_2	[(`CRC_KEY_SIZE-1):0];	//16 registers storing 10 bit pointers (RAM address size = 512), 1 per task 
	
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		fprint_head_pointer_0; 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		fprint_head_pointer_1; 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		fprint_head_pointer_2; 

/*************************************************************
* FSM
**************************************************************/
reg[3:0] state;

parameter idle								= 0;
parameter st_pointer_start_write			= 1;
parameter st_pointer_end_write				= 2;
parameter st_comp_pointer_ack				= 3;
parameter st_inc_head_pointer				= 4;
parameter st_comp_inc_head_pointer_ack		= 5;
parameter st_inc_tail_pointer				= 6;
parameter st_comp_inc_tail_pointer_ack		= 7;
parameter st_reset_task						= 8;

always @(posedge clk or posedge reset)
begin
	if(reset)
		state = idle;
	else begin
		case(state)
			idle:
				if(csr_pointer_start_write)
					state = st_pointer_start_write;
				else if(csr_pointer_end_write)
					state = st_pointer_end_write;
				else if(fprint_inc_head_pointer)
					state = st_inc_head_pointer;
				else if(comparator_inc_tail_pointer)
					state = st_inc_tail_pointer;
				else if(comparator_reset_task)
					state = st_reset_task;
			
			st_pointer_start_write:
				state = st_comp_pointer_ack;
			
			st_pointer_end_write:
				state = st_comp_pointer_ack;
				
			st_comp_pointer_ack:
				state == idle;
			
			st_inc_head_pointer:
				state = st_comp_inc_head_pointer_ack;
				
			st_comp_inc_head_pointer_ack
				state = idle;
				
			st_inc_tail_pointer:
				state = st_comp_inc_tail_pointer_ack;
			
			st_comp_inc_tail_pointer_ack:
				state == idle;
				
			st_reset_task:
				state = idle;
		endcase
	end
end

/*************************************************************
* FSM outputs
**************************************************************/
assign pointer_start_write			= (state == st_pointer_start_write);
assign pointer_end_write			= (state == st_pointer_end_write);
assign comp_pointer_ack				= (state == st_comp_pointer_ack);

assign inc_head_pointer				= (state == st_inc_head_pointer);
assign comp_inc_head_pointer_ack	= (state == st_comp_inc_head_pointer_ack);

assign inc_tail_pointer				= (state == st_inc_tail_pointer);
assign comp_inc_tail_pointer_ack	= (state == st_comp_inc_tail_pointer_ack);

assign reset_task					= (state == st_reset_task);
/*************************************************************
* Start and End pointers
**************************************************************/
always @ (posedge clk)
begin
	if(pointer_start_write & (csr_logical_core_id == 0))
		start_pointer_mem_0[csr_task_id] = csr_pointer_data;

	//Dual port read...
	start_pointer_ex_0 = start_pointer_mem_0[fprint_task_id];
	start_pointer_comp_0 = start_pointer_mem_0[comparator_task_id];
end

always @ (posedge clk)
begin
	if(pointer_start_write & (csr_logical_core_id == 1))
		start_pointer_mem_1[csr_task_id] = csr_pointer_data;
		
	//Dual port read...
	start_pointer_ex_1 = start_pointer_mem_1[fprint_task_id];
	start_pointer_comp_1 = start_pointer_mem_1[comparator_task_id];
end		

always @ (posedge clk)
begin
	if(pointer_start_write & (csr_logical_core_id == 2))
		start_pointer_mem_2[csr_task_id] = csr_pointer_data;
		
	//Dual port read...
	start_pointer_ex_2 = start_pointer_mem_2[fprint_task_id];
	start_pointer_comp_2 = start_pointer_mem_2[comparator_task_id];
end		


always @ (posedge clk)
begin
	if(pointer_end_write & (csr_logical_core_id == 0))
		end_pointer_mem_0[csr_task_id] = csr_pointer_data;

	//Dual port read...
	end_pointer_ex_0 = end_pointer_mem_0[fprint_task_id];
	end_pointer_comp_0 = end_pointer_mem_0[comparator_task_id];	
end	

always @ (posedge clk)
begin
	if(pointer_end_write & (csr_logical_core_id == 1))
		end_pointer_mem_1[csr_task_id] = csr_pointer_data;

	//Dual port read...
	end_pointer_ex_1 = end_pointer_mem_1[fprint_task_id];
	end_pointer_comp_1 = end_pointer_mem_1[comparator_task_id];	
end	

always @ (posedge clk)
begin
	if(pointer_end_write & (csr_logical_core_id == 2))
		end_pointer_mem_2[csr_task_id] = csr_pointer_data;

	//Dual port read...
	end_pointer_ex_2 = end_pointer_mem_2[fprint_task_id];
	end_pointer_comp_2 = end_pointer_mem_2[comparator_task_id];	
end	

/*************************************************************
* Head and Tail pointers
**************************************************************/
always @ (posedge clk)
begin
	if(pointer_start_write & (csr_logical_core_id == 0)) begin
		head_pointer_mem_0[csr_task_id] = csr_pointer_data;
		
	end else if(inc_head_pointer & (fprint_logical_core_id == 0)) begin
		head_pointer_mem_0[fprint_task_id] = (fprint_head_pointer_0 == end_pointer_ex_0) ? start_pointer_ex_0 : fprint_head_pointer_0 + 1;

	//HOW?
	end else if(reset_task)
		head_pointer_mem_0[comparator_task_id] = start_pointer_comp_0;

	//Dual port read...
	fprint_head_pointer_0 = head_pointer_mem_0[fprint_task_id];
end			

always @ (posedge clk)
begin
	if(pointer_start_write & (csr_logical_core_id == 1)) begin
		head_pointer_mem_1[csr_task_id] = csr_pointer_data;
		
	end else if(inc_head_pointer & (fprint_logical_core_id == 1)) begin
		head_pointer_mem_1[fprint_task_id] = (fprint_head_pointer_1 == end_pointer_ex_1) ? start_pointer_ex_1 : fprint_head_pointer_1 + 1;

	//HOW?
	end else if(reset_task)
		head_pointer_mem_1[comparator_task_id] = start_pointer_comp_1;

	//Dual port read...
	fprint_head_pointer_1 = head_pointer_mem_1[fprint_task_id];
end		

always @ (posedge clk)
begin
	if(pointer_start_write & (csr_logical_core_id == 2)) begin
		head_pointer_mem_2[csr_task_id] = csr_pointer_data;
		
	end else if(inc_head_pointer & (fprint_logical_core_id == 2)) begin
		head_pointer_mem_2[fprint_task_id] = (fprint_head_pointer_2 == end_pointer_ex_2) ? start_pointer_ex_2 : fprint_head_pointer_2 + 1;

	//HOW?
	end else if(reset_task)
		head_pointer_mem_2[comparator_task_id] = start_pointer_comp_2;

	//Dual port read...
	fprint_head_pointer_1 = head_pointer_mem_2[fprint_task_id];
end	


assign fprint_head_pointer =	(fprint_logical_core_id == 2) ? fprint_head_pointer_2 : 
								(fprint_logical_core_id == 1) ? fprint_head_pointer_1 :
								(fprint_logical_core_id == 0) ? fprint_head_pointer_0 :
								0;


always @ (posedge clk)
begin
	if(pointer_end_write & (csr_logical_core_id == 0)) begin
		tail_pointer_mem_0[csr_task_id] = csr_pointer_data;
		
	end else if(inc_tail_pointer) begin
		tail_pointer_mem_0[comparator_task_id] = (comp_tail_pointer_0 == end_pointer_comp_0) ? start_pointer_comp_0 : comp_tail_pointer_0 + 1; 
	
	//HOW?
	end else if(reset_task)
		tail_pointer_mem_0[comparator_task_id] = start_pointer_comp_0;
	
	comp_tail_pointer_0   = tail_pointer_mem_0[comparator_task_id];
end


always @ (posedge clk)
begin
	if(pointer_end_write & (csr_logical_core_id == 1)) begin
		tail_pointer_mem_1[csr_task_id] = csr_pointer_data;
		
	end else if(comparator_inc_tail_pointer) begin
		tail_pointer_mem_1[comparator_task_id] = (comp_tail_pointer_1 == end_pointer_comp_1) ? start_pointer_comp_1 : comp_tail_pointer_1 + 1; 
	
	//HOW?
	end else if(reset_task)
		tail_pointer_mem_1[comparator_task_id] = start_pointer_comp_1;
	
	comp_tail_pointer_1   = tail_pointer_mem_1[comparator_task_id];
end			

always @ (posedge clk)
begin
	if(pointer_end_write & (csr_logical_core_id == 2)) begin
		tail_pointer_mem_2[csr_task_id] = csr_pointer_data;
		
	end else if(comparator_inc_tail_pointer) begin
		tail_pointer_mem_2[comparator_task_id] = (comp_tail_pointer_2 == end_pointer_comp_2) ? start_pointer_comp_2 : comp_tail_pointer_2 + 1; 
	
	//HOW?
	end else if(reset_task)
		tail_pointer_mem_2[comparator_task_id] = start_pointer_comp_2;
	
	comp_tail_pointer_2 = tail_pointer_mem_2[comparator_task_id];
end			
		
endmodule