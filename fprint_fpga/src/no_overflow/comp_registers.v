`include "crc_defines.v"
module comp_registers(

	//From processor
	input									clk,
	input									reset,

	input [`CRC_RAM_ADDRESS_WIDTH-1:0]     	head_tail_data,
	input [(`CRC_KEY_WIDTH-1):0]           	head_tail_offset,
	input 									set_head_tail,
	output 									head_tail_ack,

	//to send the head pointer to fprint 
	input 									logical_core_id,
	input [3:0] 							fprint_task_id,
	output [`CRC_RAM_ADDRESS_WIDTH-1:0]     fprint_head_pointer,

	input 									increment_head_pointer,
	output 									increment_hp_ack,
	input [`CRC_RAM_ADDRESS_WIDTH-1:0]		start_pointer_ex,
	input [`CRC_RAM_ADDRESS_WIDTH-1:0]      end_pointer_ex,
	input [`CRC_RAM_ADDRESS_WIDTH-1:0]		start_pointer_comp,
	input [`CRC_RAM_ADDRESS_WIDTH-1:0]      end_pointer_comp,


	input [(`CRC_KEY_WIDTH-1):0] 			comp_task,
	output [(`CRC_KEY_SIZE-1):0]  			fprints_ready_out,
	output  								head0_matches_head1,
	output 									tail0_matches_head0,
	output 									tail1_matches_head1,
	output reg[`CRC_RAM_ADDRESS_WIDTH-1:0]	comp_tail_pointer0,
	output reg[`CRC_RAM_ADDRESS_WIDTH-1:0] 	comp_tail_pointer1,
	input 									comp_increment_tail_pointer,
	input 									comp_reset_fprint_ready,
	output 									reset_fprint_ack,
	input 									comp_mismatch_detected

);

reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		head_pointer_mem_0  	[(`CRC_KEY_SIZE-1):0];				//16 registers storing 10 bit pointers (RAM address size = 512), 1 per task 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		head_pointer_mem_1  	[(`CRC_KEY_SIZE-1):0];				//16 registers storing 10 bit pointers (RAM address size = 512), 1 per task 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		tail_pointer_mem_0  	[(`CRC_KEY_SIZE-1):0];				//16 registers storing 10 bit pointers (RAM address size = 512), 1 per task 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		tail_pointer_mem_1  	[(`CRC_KEY_SIZE-1):0];				//16 registers storing 10 bit pointers (RAM address size = 512), 1 per task 

reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		fprint_head_pointer0; 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		fprint_head_pointer1; 
reg  [(`CRC_KEY_SIZE-1):0]       		fprints_ready		[1:0];

reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 		comp_head_pointer0;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]  		comp_head_pointer1;

wire 									reset_task;
/*************************************************************
* FSM
**************************************************************/
reg[3:0] state;

parameter idle 					= 0;
parameter st_set_pointers 		= 1;
parameter st_ack_pointers   	= 2;
parameter st_increment_hp_ack 	= 3;
parameter st_reset_fprint_ready = 4;
always @(posedge clk or posedge reset)
begin
	if(reset)
		state = idle;
	else begin
		case(state)
			idle:
				if(set_head_tail)
					state = st_set_pointers;
				else if(increment_head_pointer)
					state = st_increment_hp_ack;
				else if(comp_reset_fprint_ready)
					state = st_reset_fprint_ready;
			st_set_pointers:
				state = st_ack_pointers;
			st_ack_pointers:
				state = idle;
			st_increment_hp_ack:
				state = idle;
			st_reset_fprint_ready:
				state = idle;
		endcase
	end
end

/*************************************************************
* FSM outputs
**************************************************************/
assign head_tail_ack  		= (state == st_ack_pointers);
assign set_pointers         = (state == st_set_pointers);
assign increment_hp_ack 	= (state == st_increment_hp_ack);
assign fprints_ready_out    = fprints_ready[0] & fprints_ready[1];

assign head0_matches_head1  = (comp_head_pointer0 == comp_head_pointer1);
assign tail0_matches_head0  = (comp_tail_pointer0 == comp_head_pointer0);
assign tail1_matches_head1  = (comp_tail_pointer1 == comp_head_pointer1);
assign reset_fprint_ack     = (state == st_reset_fprint_ready);
assign reset_task 			= reset_fprint_ack & comp_mismatch_detected;

/*************************************************************
* Head and Tail pointers
**************************************************************/
always @ (posedge clk)
begin
	if(set_pointers)begin
		head_pointer_mem_0[head_tail_offset] = head_tail_data;
	end else if(state == st_increment_hp_ack)begin
		if(~logical_core_id)
			head_pointer_mem_0[fprint_task_id] = (fprint_head_pointer0 == end_pointer_ex) ? 
				start_pointer_ex : fprint_head_pointer0 + 1;
	end else if(reset_task)
		head_pointer_mem_0[comp_task] = start_pointer_comp;
	//Dual port read...
	fprint_head_pointer0 = head_pointer_mem_0[fprint_task_id];
	comp_head_pointer0   = head_pointer_mem_0[comp_task];
end			

always @ (posedge clk)
begin
	if(set_pointers)begin
		head_pointer_mem_1[head_tail_offset] = head_tail_data;
	end else if(state == st_increment_hp_ack)begin
		if(logical_core_id)
			head_pointer_mem_1[fprint_task_id] = 
			(fprint_head_pointer1 == end_pointer_ex) ? 
				start_pointer_ex : fprint_head_pointer1 + 1; 
	end else if(reset_task)
		head_pointer_mem_1[comp_task] = start_pointer_comp;

	//Dual port read...
	fprint_head_pointer1 = head_pointer_mem_1[fprint_task_id];
	comp_head_pointer1   = head_pointer_mem_1[comp_task];
end		


assign fprint_head_pointer = logical_core_id ? fprint_head_pointer1 : fprint_head_pointer0;


always @ (posedge clk)
begin
	if(set_pointers)begin
		tail_pointer_mem_0[head_tail_offset] = head_tail_data;
	end else if(comp_increment_tail_pointer)begin
		tail_pointer_mem_0[comp_task] = 
			(comp_tail_pointer0 == end_pointer_comp) ? 
				start_pointer_comp : comp_tail_pointer0 + 1; 
	end else if(reset_task)
		tail_pointer_mem_0[comp_task] = start_pointer_comp;
	comp_tail_pointer0   = tail_pointer_mem_0[comp_task];
end


always @ (posedge clk)
begin
	if(set_pointers)begin
		tail_pointer_mem_1[head_tail_offset] = head_tail_data;
	end else if(comp_increment_tail_pointer)begin
		tail_pointer_mem_1[comp_task] = 
			(comp_tail_pointer1 == end_pointer_comp) ? 
				start_pointer_comp : comp_tail_pointer1 + 1; 
	end else if(reset_task)
		tail_pointer_mem_1[comp_task] = start_pointer_comp;
	comp_tail_pointer1   = tail_pointer_mem_1[comp_task];
end			

/*************************************************************
* Fprint Ready Register
**************************************************************/
always @ (posedge clk or posedge reset)
begin
	if(reset)begin
		fprints_ready[0] = 0;
		fprints_ready[1] = 0;
	end else begin
		if(state == st_increment_hp_ack)
			fprints_ready[logical_core_id][fprint_task_id] = 1'b1;
		else if(state == st_reset_fprint_ready | reset_task)begin
			fprints_ready[0][comp_task] = 1'b0;	
			fprints_ready[1][comp_task] = 1'b0;
		end
	end
end			
endmodule
