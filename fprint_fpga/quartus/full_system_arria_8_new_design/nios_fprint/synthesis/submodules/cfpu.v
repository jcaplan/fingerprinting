`include "defines.v"

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
    
   	output										checkpoint_write,
	output [`NIOS_DATA_WIDTH-1 : 0]				checkpoint_writedata,
	output [`NIOS_ADDRESS_WIDTH-1 : 0]			checkpoint_address,
	input										checkpoint_waitrequest,
	
	output										irq
);

	wire [(`CRC_KEY_WIDTH-1):0]			comparator_task_id;
	wire [1:0]							comparator_logical_core_id;
	wire								comparator_nmr;
	wire[`CRC_RAM_ADDRESS_WIDTH-1 : 0]	csr_task_maxcount;
	wire 								comparator_status_write;
	wire 		 						comparator_mismatch_detected;
	wire 								csr_status_ack;
	wire [(`CRC_KEY_WIDTH-1):0]			fprint_task_id;
	wire [(`CRC_KEY_WIDTH-1):0]			fprint_physical_core_id;
	wire [1:0]							fprint_logical_core_id;
	wire [(`CRC_KEY_SIZE-1):0]			fprint_nmr;
	wire [1:0]							checkpoint_logical_core_id;
	wire [(`CRC_KEY_WIDTH-1):0]			checkpoint_physical_core_id;
	wire [`CRC_WIDTH - 1: 0]			fprint_0;
	wire [`CRC_WIDTH - 1: 0]			fprint_1;
	wire [`CRC_WIDTH - 1: 0]			fprint_2;
	wire [(`CRC_KEY_SIZE-1):0]			fprint_checkin;
	wire								fprint_reset_task;
	wire								fprint_reset_task_ack;
	wire								comparator_checkpoint;
	wire								checkpoint_ack;

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
	
	comparator_nmr,
	csr_task_maxcount,

	comparator_status_write,
	comparator_mismatch_detected,
	csr_status_ack,

	//fprint_registers	
	fprint_task_id,
	fprint_physical_core_id,
	fprint_logical_core_id,
	
	fprint_nmr,

	//checkpoint
	checkpoint_logical_core_id,
	checkpoint_physical_core_id,

	//irq
	irq
);

comparator comparator(

	clk,
	reset,	

	//global
	comparator_task_id,
	comparator_logical_core_id,

	//csr_registers		
	comparator_nmr,
	csr_task_maxcount,

	comparator_status_write,
	comparator_mismatch_detected,
	csr_status_ack,

	
	//fprint_registers
	fprint_0,
	fprint_1,
	fprint_2,
	
	fprint_checkin,
	
	fprint_reset_task,
	fprint_reset_task_ack,
	
	//checkpoint
	comparator_checkpoint,
	checkpoint_ack
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

	//comparator
	comparator_task_id,
	
	fprint_0,
	fprint_1,
	fprint_2,

	fprint_checkin,
	
	fprint_reset_task,
	fprint_reset_task_ack
);

checkpoint checkpoint(

	clk,
	reset,
	
	//processors
	checkpoint_write,
	checkpoint_writedata,
	checkpoint_address,
	checkpoint_waitrequest,

	//csr_registers
	checkpoint_logical_core_id,
	comparator_nmr,
	
	checkpoint_physical_core_id,	

	//comparator
	comparator_task_id,
	comparator_checkpoint,
	checkpoint_ack
);


endmodule
