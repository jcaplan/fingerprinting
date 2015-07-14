`include "crc_defines.v"
module comparator(

	input									clk,
	input									reset,	

	//global
	output reg [(`CRC_KEY_WIDTH-1):0]      	comparator_task_id, //0-15 tasks
	output [1:0]							comparator_logical_core_id,

	//csr_registers
	output 									comparator_status_write,
	output 		 							comparator_mismatch_detected,
	input 									csr_status_ack,
	
	
	//comp registers
	output 									comp_increment_tail_pointer,
	output 									comparator_task_id_verified,
	input 									fprint_reg_ack,
	
	output									comp_reset_task,
	input									reset_task_ack,

	//fprint reg
	input [`CRC_WIDTH - 1: 0]				fprint_0,
	input [`CRC_WIDTH - 1: 0]				fprint_1,
	input [`CRC_WIDTH - 1: 0]				fprint_2,


	
	input [(`CRC_KEY_SIZE-1):0]				checkin,
	input [(`CRC_KEY_SIZE-1):0]				fprints_ready

);

//First combinational check if a task is ready for comparison
wire[`CRC_KEY_WIDTH:0]				task_check;
wire 								fprints_match;
/*************************************************************
* Decode
**************************************************************/

assign task_check = fprints_ready[0] | checkin[0] ? 0 : fprints_ready[1] | checkin[1] ? 1 : 
					fprints_ready[2] | checkin[2] ? 2 : fprints_ready[3] | checkin[3] ? 3 :
					fprints_ready[4] | checkin[4]?  4 : fprints_ready[5] | checkin[5] ? 5 :
					fprints_ready[6] | checkin[6]? 6 : fprints_ready[7] | checkin[7] ? 7 :
					fprints_ready[8] | checkin[8]? 8 : fprints_ready[9] | checkin[9] ? 9 :
					fprints_ready[10] | checkin[10]? 10 : fprints_ready[11] | checkin[11] ? 11 :
					fprints_ready[12] | checkin[12]? 12 : fprints_ready[13] | checkin[13] ? 13 :
					fprints_ready[14] | checkin[14]? 14 : fprints_ready[15] | checkin[15] ? 15 :
					16; //takes 16 when no task is checked out.

assign fprints_match = (fprint_0 == fprint_1);

/*************************************************************
* FSM
**************************************************************/

reg[3:0] state;
parameter init						= 0;
parameter set_task 					= 1;
parameter load_pointer  			= 2;
parameter load_fprint   			= 3;
parameter check_task_status  		= 4;
parameter task_complete 			= 5;
parameter compare_fprints  			= 6;
parameter mismatch_detected 		= 7;
parameter task_verified   			= 8;
parameter increment_tail_pointer 	= 9;
parameter check_if_done				= 10;
parameter reset_fprint_ready 		= 11;
parameter st_reset_task				= 12;
parameter write_status_reg 			= 13;

always @ (posedge clk or posedge reset)
begin
	if (reset) begin
		state = init;
	end else begin
		case (state)
			init:
				if(~task_check[`CRC_KEY_WIDTH])
					state = set_task;
			set_task:
				state = load_pointer; //Give extra clock cycles because performance was strange on FPGA
			load_pointer:
				state = load_fprint;
			load_fprint:
				state = check_task_status;
			check_task_status:
				//If the task has completed
				if(fprints_ready[comparator_task_id])
					state = compare_fprints;
				else if(checkin[comparator_task_id])
					state = task_complete;	
			task_complete:
				//If the number of fingerprints doesn't match
				if(~head0_matches_head1)
					state = mismatch_detected;
				//Otherwise there's still work to do
				else 
					state = reset_fprint_ready;
			compare_fprints:
				if(fprints_match)
					state = increment_tail_pointer;
				else 
					state = mismatch_detected;
			increment_tail_pointer:
				state = check_if_done;
			check_if_done:
				if(tail0_matches_head0 | tail1_matches_head1)
						state = reset_fprint_ready;
				else	
					state = compare_fprints;
			reset_fprint_ready:
				if(reset_fprint_ack)
					if(checkin[comparator_task_id] | comp_mismatch_detected)
						state = task_verified;
					else
						state = init;
			mismatch_detected:
				state = task_verified;
			task_verified:
				if(fprint_reg_ack) begin
					if(comp_mismatch_detected)
						state = st_reset_task;
					else
						state = write_status_reg;
				end
			st_reset_task:
				if(reset_task_ack)
					state = write_status_reg;
			write_status_reg:
				if(comp_status_ack)
					state = init;
		endcase
	end
end

/*************************************************************
* FSM outputs
**************************************************************/
assign comp_reset_fprint_ready 		= (state == reset_fprint_ready);
assign comp_increment_tail_pointer 	= (state == increment_tail_pointer);
assign comparator_task_id_verified 			= (state == task_verified);
assign comp_status_write 			= (state == write_status_reg);
assign comp_reset_task				= (state == st_reset_task);


always @ (posedge clk or posedge reset)
begin
	if(reset)
		comp_mismatch_detected = 0;
	else begin
		if(state == mismatch_detected)
			comp_mismatch_detected = 1;
		else if(state == init)
			comp_mismatch_detected = 0;
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
		if (state == set_task)
			comparator_task_id = task_check[`CRC_KEY_WIDTH-1:0];
		else if (state == init)
			comparator_task_id = `CRC_KEY_WIDTH'h0;
	end
		
end




endmodule
