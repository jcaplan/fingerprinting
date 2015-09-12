`include "defines.v"
module comparator(

	input									clk,
	input									reset,	

	//global
	output reg [(`CRC_KEY_WIDTH-1):0]      	comparator_task_id,
	output reg[1:0]							comparator_logical_core_id,

	//csr_registers		
	input									comparator_nmr,
	input[`CRC_RAM_ADDRESS_WIDTH-1 : 0]		csr_task_maxcount,

	output 									comparator_status_write,
	output reg	 							comparator_mismatch_detected,
	input 									csr_status_ack,

	
	//fprint_registers
	input [`CRC_WIDTH - 1: 0]				fprint_0,
	input [`CRC_WIDTH - 1: 0]				fprint_1,
	input [`CRC_WIDTH - 1: 0]				fprint_2,
	
	input [(`CRC_KEY_SIZE-1):0]				fprint_checkin,
	
	output									fprint_reset_task,
	input									fprint_reset_task_ack,
	
	//checkpoint
	output									comparator_checkpoint,
	input									checkpoint_ack
);


wire[`CRC_KEY_WIDTH:0]				task_check;

wire 								fprints_match;
wire								fprints_match_0_1;
wire								fprints_match_1_2;
wire								fprints_match_2_0;

reg[`CRC_RAM_ADDRESS_WIDTH-1 : 0]	task_count[`CRC_KEY_SIZE-1 : 0];
wire								task_count_inc;
wire								task_maxcount_reached;
wire								task_count_reset;

integer	i;

/*************************************************************
* Decode
**************************************************************/

assign task_check = fprint_checkin[0]		?	0	: fprint_checkin[1]		? 1		: 
					fprint_checkin[2]		?	2	: fprint_checkin[3]		? 3		:
					fprint_checkin[4]		?	4	: fprint_checkin[5] 	? 5		:
					fprint_checkin[6]		?	6	: fprint_checkin[7]		? 7		:
					fprint_checkin[8]		?	8	: fprint_checkin[9]		? 9		:
					fprint_checkin[10]		?	10	: fprint_checkin[11]	? 11	:
					fprint_checkin[12]		?	12	: fprint_checkin[13]	? 13	:
					fprint_checkin[14]		?	14	: fprint_checkin[15]	? 15	:
					16; //takes 16 when no task is checked out.

assign fprints_match_0_1 = (fprint_0 == fprint_1);
assign fprints_match_1_2 = (fprint_1 == fprint_2);
assign fprints_match_2_0 = (fprint_2 == fprint_0);

assign fprints_match = fprints_match_0_1 & ((fprints_match_1_2 & fprints_match_2_0) | ~comparator_nmr);

assign task_maxcount_reached = (task_count[comparator_task_id] == csr_task_maxcount);

/*************************************************************
* FSM
**************************************************************/
reg[3:0] state;

parameter idle								= 0;
parameter st_set_task 						= 1;
parameter st_load_pointer  					= 2;
parameter st_load_fprint   					= 3;
parameter st_compare_fprints				= 4;
parameter st_comparator_checkpoint			= 5;
parameter st_task_count_inc					= 6;
parameter st_comparator_mismatch_detected	= 7;
parameter st_fprint_reset_task				= 8;
parameter st_task_count_reset				= 9;
parameter st_comparator_status_write		= 10;


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
				state = st_compare_fprints;
			
			st_compare_fprints:
				if(fprints_match)
					state = st_comparator_checkpoint;
				else 
					state = st_comparator_mismatch_detected;

			st_comparator_checkpoint:
				if(checkpoint_ack)
					state = st_task_count_inc;
 
			st_task_count_inc:
				state = st_fprint_reset_task;

			st_comparator_mismatch_detected:
				state = st_fprint_reset_task;
			
			st_fprint_reset_task:
				if(fprint_reset_task_ack)
					if(task_maxcount_reached)
						state = st_task_count_reset;
					else
						state = idle;

			st_task_count_reset:
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
assign comparator_checkpoint		=	(state == st_comparator_checkpoint);
assign task_count_inc				=	(state == st_task_count_inc);
assign fprint_reset_task			=	(state == st_fprint_reset_task);
assign task_count_reset				=	(state == st_task_count_reset);
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
		comparator_logical_core_id =	~comparator_nmr ? 3 :
										(~fprints_match_0_1 & ~fprints_match_2_0) ? 0 :
										(~fprints_match_1_2 & ~fprints_match_0_1) ? 1 :
										(~fprints_match_2_0 & ~fprints_match_1_2) ? 2 :
										3;
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

/*************************************************************
* Task Count
**************************************************************/
initial begin
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		task_count[i] = 0;
	end
end

always @ (posedge clk)
begin
	if(task_count_inc) begin
		task_count[comparator_task_id] = task_count[comparator_task_id] + 1;
	end
	
	if(task_count_reset) begin
		task_count[comparator_task_id] = 0;
	end
end

endmodule
