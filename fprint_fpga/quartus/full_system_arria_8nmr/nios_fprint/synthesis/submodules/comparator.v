`include "crc_defines.v"
module comparator(

	input									clk,
	input									reset,	

	//global
	output reg [(`CRC_KEY_WIDTH-1):0]      	comparator_task_id,
	output reg[1:0]							comparator_logical_core_id,

	//csr_registers	
	output 									comparator_status_write,
	output reg	 							comparator_mismatch_detected,
	input 									csr_status_ack,
	
	input									comparator_nmr,
	
	//fprint_registers
	input [`CRC_WIDTH - 1: 0]				fprint_0,
	input [`CRC_WIDTH - 1: 0]				fprint_1,
	input [`CRC_WIDTH - 1: 0]				fprint_2,
	
	input [(`CRC_KEY_SIZE-1):0]				fprint_checkin,
	
	output									fprint_reset_task,
	input									fprint_reset_task_ack,
	
	//oflow_registers
	input [`CRC_KEY_SIZE-1 : 0]				oflow_fprints_ready,
	input [`CRC_KEY_SIZE-1 : 0]				oflow_fprints_remaining,
	input [1:0]								oflow_fprints_remaining_logical_core_id,
	
	output									comparator_count_dec,
	input									oflow_count_dec_ack,
	
	output									oflow_reset_task,
	input									oflow_reset_task_ack,
	
	//comp registers
	output 									comparator_inc_tail_pointer,
	input									comp_inc_tail_pointer_ack,
	
	output									comp_reset_task,
	input									comp_reset_task_ack
);

//First combinational check if a task is ready for comparison
wire[`CRC_KEY_WIDTH:0]				task_check;
wire 								fprints_match;
wire								fprints_match_0_1;
wire								fprints_match_1_2;
wire								fprints_match_2_0;

/*************************************************************
* Decode
**************************************************************/

assign task_check = oflow_fprints_ready[0]	| fprint_checkin[0]		?	0	: oflow_fprints_ready[1]	| fprint_checkin[1]		? 1		: 
					oflow_fprints_ready[2]	| fprint_checkin[2]		?	2	: oflow_fprints_ready[3]	| fprint_checkin[3]		? 3		:
					oflow_fprints_ready[4]	| fprint_checkin[4]		?	4	: oflow_fprints_ready[5]	| fprint_checkin[5] 	? 5		:
					oflow_fprints_ready[6]	| fprint_checkin[6]		?	6	: oflow_fprints_ready[7]	| fprint_checkin[7]		? 7		:
					oflow_fprints_ready[8]	| fprint_checkin[8]		?	8	: oflow_fprints_ready[9]	| fprint_checkin[9]		? 9		:
					oflow_fprints_ready[10]	| fprint_checkin[10]	?	10	: oflow_fprints_ready[11]	| fprint_checkin[11]	? 11	:
					oflow_fprints_ready[12]	| fprint_checkin[12]	?	12	: oflow_fprints_ready[13]	| fprint_checkin[13]	? 13	:
					oflow_fprints_ready[14]	| fprint_checkin[14]	?	14	: oflow_fprints_ready[15]	| fprint_checkin[15]	? 15	:
					16; //takes 16 when no task is checked out.

assign fprints_match_0_1 = (fprint_0 == fprint_1);
assign fprints_match_1_2 = (fprint_1 == fprint_2);
assign fprints_match_2_0 = (fprint_2 == fprint_0);

assign fprints_match = fprints_match_0_1 & ((fprints_match_1_2 & fprints_match_2_0) | ~comparator_nmr);

/*************************************************************
* FSM
**************************************************************/
reg[3:0] state;

parameter idle								= 0;
parameter st_set_task 						= 1;
parameter st_load_pointer  					= 2;
parameter st_load_fprint   					= 3;
parameter st_check_task_status				= 4;
parameter st_compare_fprints				= 6;
parameter st_comparator_inc_tail_pointer	= 7;
parameter st_comparator_count_dec			= 8;
parameter st_comparator_mismatch_detected	= 9;
parameter st_fprint_reset_task				= 10;
parameter st_comp_reset_task				= 11;
parameter st_oflow_reset_task				= 12;
parameter st_comparator_status_write		= 13;

always @ (posedge clk or posedge reset)
begin
	if (reset) begin
		state = idle;
	end else begin
		case (state)
			idle:
				if(~task_check[`CRC_KEY_WIDTH])
					state = st_set_task;
					
			st_set_task:
				state = st_load_pointer;
			
			st_load_pointer:
				state = st_load_fprint;
			
			st_load_fprint:
				state = st_check_task_status;
			
			st_check_task_status:
				if(oflow_fprints_ready[comparator_task_id])
					state = st_compare_fprints;
				else if(fprint_checkin[comparator_task_id]) begin
					if(oflow_fprints_remaining[comparator_task_id])
						state = st_comparator_mismatch_detected;
					else
						state = st_fprint_reset_task;
				end else
					state = idle;
				
			st_compare_fprints:
				if(fprints_match)
					state = st_comparator_inc_tail_pointer;
				else 
					state = st_comparator_mismatch_detected;
			
			st_comparator_inc_tail_pointer:
				if(comp_inc_tail_pointer_ack)
					state = st_comparator_count_dec;
			
			st_comparator_count_dec:
				if(oflow_count_dec_ack)
					state = st_check_task_status;
			
			st_comparator_mismatch_detected:
				state = st_fprint_reset_task;
			
			st_fprint_reset_task:
				if(fprint_reset_task_ack)
					state = st_comp_reset_task;
					
			st_comp_reset_task:
				if(comp_reset_task_ack) begin
					if(comparator_mismatch_detected)
						state = st_oflow_reset_task;
					else
						state = st_comparator_status_write;
				end
				
			st_oflow_reset_task:
				if(oflow_reset_task_ack)
					state = st_comparator_status_write;
			
			st_comparator_status_write:
				if(csr_status_ack)
					state = idle;
		endcase
	end
end

/*************************************************************
* FSM outputs
**************************************************************/
assign comparator_inc_tail_pointer	=	(state == st_comparator_inc_tail_pointer);
assign comparator_count_dec			=	(state == st_comparator_count_dec);
assign fprint_reset_task			=	(state == st_fprint_reset_task);
assign comp_reset_task				=	(state == st_comp_reset_task);
assign oflow_reset_task				=	(state == st_oflow_reset_task);
assign comparator_status_write		=	(state == st_comparator_status_write);

always @ (posedge clk or posedge reset)
begin
	if(reset)
		comparator_mismatch_detected = 0;
	else begin
		if(state == st_comparator_mismatch_detected)
			comparator_mismatch_detected = 1;
		else if(state == idle)
			comparator_mismatch_detected = 0;
	end
end


always @ (posedge clk) begin

	if(state == st_comparator_mismatch_detected) begin
		if(fprints_match) begin
			comparator_logical_core_id =	~comparator_nmr ? 3 :
											(~fprints_match_0_1 & ~fprints_match_2_0) ? 0 :
											(~fprints_match_1_2 & ~fprints_match_0_1) ? 1 :
											(~fprints_match_2_0 & ~fprints_match_1_2) ? 2 :
											3;

		end else
			comparator_logical_core_id = oflow_fprints_remaining_logical_core_id;
	end

end

/*************************************************************
* Registers
**************************************************************/

//When the comparator starts working, lock in the task because
//it can change in the middle of the state loop
always @ (posedge clk or posedge reset)
begin
	if(reset)
		comparator_task_id = `CRC_KEY_WIDTH'h0;
	else begin
		if (state == st_set_task)
			comparator_task_id = task_check[`CRC_KEY_WIDTH-1:0];
	end
		
end

endmodule
