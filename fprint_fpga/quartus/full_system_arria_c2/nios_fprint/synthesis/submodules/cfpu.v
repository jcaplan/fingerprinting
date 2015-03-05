`include "crc_defines.v"

module cfpu(
	input															clk,
	input															reset,

	input 		[(`COMPARATOR_ADDRESS_WIDTH-1):0]					fprint_address,
	input															fprint_write,
	input 		[(`NIOS_DATA_WIDTH-1):0]							fprint_writedata,
	output															fprint_waitrequest,

	input 		[(`COMP_CSR_WIDTH-1):0]								csr_address,
	input															csr_read,
	output wire [(`NIOS_DATA_WIDTH-1):0]							csr_readdata,
	input															csr_write,
	input 		[(`NIOS_DATA_WIDTH-1):0]							csr_writedata,
	output wire														csr_waitrequest,
	
	output wire														irq,
	output wire														io_release
);


wire	exception_reg_sel;
wire	success_reg_sel;
wire	fail_reg_sel;
wire	start_p_sel;
wire	end_p_sel;
wire	core_assignment_sel;
wire	[(`CRC_KEY_WIDTH-1):0] dir_pointer_offset;
wire	[3:0] core_id;
wire	[3:0] core_assignment_offset;
wire    [3:0] core_assignment_data;
wire 	core_a_ack;


wire  									comp_status_ack;
wire [(`CRC_KEY_WIDTH-1):0]			    comp_task;



wire[3:0] 									physical_core_id;
wire[3:0]									fprint_task_id;
wire 		 								logical_core_id;

wire [`CRC_RAM_ADDRESS_WIDTH-1:0]     		head_tail_data;
wire [(`CRC_KEY_WIDTH-1):0]           		head_tail_offset;
wire 										set_head_tail;
wire 										head_tail_ack;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0]     		fprint_head_pointer;


wire 										increment_head_pointer;
wire 										increment_hp_ack;


wire [`CRC_RAM_ADDRESS_WIDTH-1:0]			start_pointer_ex;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0]  			end_pointer_ex;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0]			start_pointer_comp;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0]       	end_pointer_comp;



wire  									head0_matches_head1;
wire 									tail0_matches_head0;
wire 									tail1_matches_head1;
wire  [`CRC_RAM_ADDRESS_WIDTH-1:0]		comp_tail_pointer0;
wire  [`CRC_RAM_ADDRESS_WIDTH-1:0] 		comp_tail_pointer1;
wire  [`CRC_WIDTH - 1: 0] 				fprint0;
wire  [`CRC_WIDTH - 1: 0]              	fprint1;
wire  [(`CRC_KEY_SIZE-1):0]				fprints_ready;
wire  [(`CRC_KEY_SIZE-1):0] 			checkin;
wire 									comp_increment_tail_pointer;
wire 									comp_reset_fprint_ready;
wire 									reset_fprint_ack;
wire 									comp_task_verified;
wire 									fprint_reg_ack;
wire 									comp_mismatch_detected;
wire 									comp_status_write;
csr_registers csr_register_block(
	
	//From processor
	clk,
	reset,	
	csr_address,
	csr_read,
	csr_readdata,
	csr_write,
	csr_writedata,
	csr_waitrequest,
	
	//From comparator
	comp_status_write,
	comp_status_ack,
	comp_task,
	comp_mismatch_detected,



	physical_core_id,
	fprint_task_id,
	logical_core_id,


	head_tail_data,
	head_tail_offset,
	set_head_tail,
	head_tail_ack,
	start_pointer_ex,
	end_pointer_ex,
	start_pointer_comp,
	end_pointer_comp,
	irq
);

fprint_registers fprint_reg(

	//From processor
	clk,
	reset,
	fprint_address,
	fprint_write,
	fprint_writedata,
	fprint_waitrequest,


	//CORE ASSIGNMENT REGISTER USED HERE, STORED HERE
	physical_core_id,
	fprint_task_id,
	logical_core_id,

	fprint_head_pointer,
	increment_head_pointer,
	increment_hp_ack,
	checkin,

	comp_tail_pointer0,
	comp_tail_pointer1,
	fprint0,
	fprint1,
	comp_task_verified,
	fprint_reg_ack,
	comp_task


);

comp_registers comp_reg(

	//From processor
	//From processor
	clk,
	reset,

	head_tail_data,
	head_tail_offset,
	set_head_tail,
	head_tail_ack,

	logical_core_id,
	fprint_task_id,
	fprint_head_pointer,
	increment_head_pointer,
	increment_hp_ack,
	start_pointer_ex,
	end_pointer_ex,
	start_pointer_comp,
	end_pointer_comp,


	comp_task,
	fprints_ready,
	head0_matches_head1,
	tail0_matches_head0,
	tail1_matches_head1,
	comp_tail_pointer0,
	comp_tail_pointer1,
	comp_increment_tail_pointer,
	comp_reset_fprint_ready,
	reset_fprint_ack,
	comp_mismatch_detected
);



comparator comparator(

	clk,
	reset,

	//to comp registers
	comp_task, //0-15 tasks

	//from comp registers
	head0_matches_head1,
	tail0_matches_head0,
	tail1_matches_head1,
	fprints_ready,


	//from fprint reg
	checkin,
	fprint0,
	fprint1,
	comp_increment_tail_pointer,
	comp_reset_fprint_ready,
	reset_fprint_ack,
	comp_task_verified,
	fprint_reg_ack,
	comp_status_write,
	comp_status_ack,
	comp_mismatch_detected

);

	endmodule
