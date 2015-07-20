`include "crc_defines.v"

module cfpu(
	input										clk,
	input										reset,

	input [(`COMP_CSR_WIDTH-1):0]				csr_address,
	input										csr_read,
	output [(`NIOS_DATA_WIDTH-1):0]				csr_readdata,
	input										csr_write,
	input [(`NIOS_DATA_WIDTH-1):0]				csr_writedata,
	output										csr_waitrequest,
	
	input [(`COMPARATOR_ADDRESS_WIDTH-1):0]		fprint_address,
	input										fprint_write,
	input [(`NIOS_DATA_WIDTH-1):0]				fprint_writedata,
	output										fprint_waitrequest,
    
   	output										oflow_write,
	output [`NIOS_DATA_WIDTH-1 : 0]				oflow_writedata,
	output [`NIOS_ADDRESS_WIDTH-1 : 0]			oflow_address,
	input										oflow_waitrequest,
	
	output										irq
);

	
wire [(`CRC_KEY_WIDTH-1):0]				comparator_task_id;
wire [1:0]								comparator_logical_core_id;
wire 									comparator_status_write;
wire 		 							comparator_mismatch_detected;
wire  									csr_status_ack;	
wire 									comparator_nmr;
wire [(`CRC_KEY_WIDTH-1):0]				fprint_task_id;
wire [(`CRC_KEY_WIDTH-1):0]				fprint_physical_core_id;
wire [1:0]								fprint_logical_core_id;
wire [(`CRC_KEY_SIZE-1):0]				fprint_nmr;
wire [(`CRC_KEY_WIDTH-1):0]				oflow_task_id;
wire [1:0]								oflow_logical_core_id;
wire [(`CRC_KEY_WIDTH-1):0]				oflow_physical_core_id;
wire [(`CRC_KEY_WIDTH-1):0]				csr_task_id;
wire [1:0]								csr_logical_core_id;
wire [`CRC_RAM_ADDRESS_WIDTH-1 : 0]		csr_fprint_maxcount;
wire 									csr_pointer_start_write;
wire 									csr_pointer_end_write;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0]		csr_pointer_data;
wire 									comp_pointer_ack;
wire 									fprint_count_inc;
wire 									oflow_count_inc_ack;
wire  									fprint_inc_head_pointer;
wire 									comp_inc_head_pointer_ack;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0]     	fprint_head_pointer;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0] 		comp_tail_pointer_0;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0] 		comp_tail_pointer_1;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0] 		comp_tail_pointer_2;
wire [`CRC_WIDTH - 1: 0] 				fprint_0;
wire [`CRC_WIDTH - 1: 0]             	fprint_1;
wire [`CRC_WIDTH - 1: 0]             	fprint_2;
wire 									fprint_reset_task;
wire  									fprint_reset_task_ack;
wire [(`CRC_KEY_SIZE-1):0]  			fprint_checkin;
wire 									comparator_inc_tail_pointer;
wire 									comp_inc_tail_pointer_ack;
wire									comp_reset_task;
wire									comp_reset_task_ack;
wire 									comparator_count_dec;
wire 									oflow_count_dec_ack;
wire [`CRC_KEY_SIZE-1 : 0]				oflow_fprints_ready;
wire [`CRC_KEY_SIZE-1 : 0]				oflow_fprints_remaining;
wire [1:0]								oflow_fprints_remaining_logical_core_id;
wire 									oflow_reset_task;
wire 									oflow_reset_task_ack;


csr_registers csr_registers(

	clk,
	reset,	
	
	//processor
	csr_address,
	csr_read,
	csr_readdata,
	csr_write,
	csr_writedata,
	csr_waitrequest,

	//comparator	
	comparator_task_id,
	comparator_logical_core_id,
	
	comparator_status_write,
	comparator_mismatch_detected,
	csr_status_ack,
	
	comparator_nmr,

	//fprint_registers	
	fprint_task_id,
	fprint_physical_core_id,
	fprint_logical_core_id,

	fprint_nmr,

	//oflow_registers
	oflow_task_id,
	oflow_logical_core_id,
	oflow_physical_core_id,
	
	csr_fprint_maxcount,
	oflow_nmr,


	//comp_registers

	csr_task_id,
	csr_logical_core_id,

	csr_pointer_start_write,
	csr_pointer_end_write,
	csr_pointer_data,
	comp_pointer_ack,
	
	//irq
	irq
);

fprint_registers fprint_registers(

	clk,
	reset,

	//processors	
	fprint_address,
	fprint_write,
	fprint_writedata,
	fprint_waitrequest,

	//csr_registers
	fprint_task_id,
	fprint_physical_core_id,
	fprint_logical_core_id,

	fprint_nmr,

	//oflow_registers
	fprint_count_inc,
	oflow_count_inc_ack,
	
	//comp_registers
	fprint_inc_head_pointer,
	comp_inc_head_pointer_ack,
	
	fprint_head_pointer,
	comp_tail_pointer_0,
	comp_tail_pointer_1,
	comp_tail_pointer_2,
	fprint_0,
	fprint_1,
	fprint_2,
	
	//comparator
	comparator_task_id,
	
	fprint_reset_task,
	fprint_reset_task_ack,
	
	fprint_checkin

);

comp_registers comp_registers(

	clk,
	reset,
	
	//csr_registers
	csr_logical_core_id,
	csr_task_id,
	
	csr_pointer_start_write,
	csr_pointer_end_write,
	csr_pointer_data,
	comp_pointer_ack,

	//fprint_registers
	fprint_logical_core_id,
	fprint_task_id,
	
	fprint_inc_head_pointer,
	comp_inc_head_pointer_ack,
	
	fprint_head_pointer,
	comp_tail_pointer_0,
	comp_tail_pointer_1,
	comp_tail_pointer_2,

	//comparator
	comparator_task_id,
	
	comparator_inc_tail_pointer,
	comp_inc_tail_pointer_ack,
	
	comp_reset_task,
	comp_reset_task_ack
);

oflow_registers oflow_registers(

	clk,
	reset,
	
	//processors
	oflow_write,
	oflow_writedata,
	oflow_address,
	oflow_waitrequest,
	
	//global
	oflow_task_id,
	oflow_logical_core_id,
	oflow_physical_core_id,
	
	//csr_registers
	csr_fprint_maxcount,
	oflow_nmr,
	
	//fprint_registers
	fprint_task_id,
	fprint_logical_core_id,
	
	fprint_count_inc,
	oflow_count_inc_ack,
	
	//comparator
	comparator_task_id,
		
	comparator_count_dec,
	oflow_count_dec_ack,
	
	oflow_fprints_ready,
	oflow_fprints_remaining,
	oflow_fprints_remaining_logical_core_id,
	
	oflow_reset_task,
	oflow_reset_task_ack
);

comparator comparator(

	clk,
	reset,	

	//global
	comparator_task_id,
	comparator_logical_core_id,

	//csr_registers	
	comparator_status_write,
	comparator_mismatch_detected,
	csr_status_ack,
	
	comparator_nmr,
	
	//fprint_registers
	fprint_0,
	fprint_1,
	fprint_2,
	
	fprint_checkin,
	
	fprint_reset_task,
	fprint_reset_task_ack,
	
	//oflow_registers
	oflow_fprints_ready,
	oflow_fprints_remaining,
	oflow_fprints_remaining_logical_core_id,
	
	comparator_count_dec,
	oflow_count_dec_ack,
	
	oflow_reset_task,
	oflow_reset_task_ack,
	
	//comp registers
	comparator_inc_tail_pointer,
	comp_inc_tail_pointer_ack,
	
	comp_reset_task,
	comp_reset_task_ack
);

endmodule
