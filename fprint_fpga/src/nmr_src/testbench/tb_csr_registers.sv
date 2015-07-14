`include "crc_defines.v"

`define BYTE 8
`define CLK_PERIOD 10

module tb_csr_registers();


reg									clk;
reg									reset;	

//processor
reg [(`COMP_CSR_WIDTH-1):0]			csr_address;
reg									csr_read;
wire [(`NIOS_DATA_WIDTH-1):0]		csr_readdata;
reg									csr_write;
reg [(`NIOS_DATA_WIDTH-1):0]		csr_writedata;
wire 								csr_waitrequest;

//global
wire [1:0]							csr_logical_core_id;
wire [(`CRC_KEY_WIDTH-1):0]			csr_task_id;
wire [(`CRC_KEY_WIDTH-1):0]			csr_physical_core_id;

//comparator	
reg 								comparator_status_write;
reg 		 						comparator_collision_detected;
reg [1:0]							comparator_logical_core_id;
reg [(`CRC_KEY_WIDTH-1):0]			comparator_task_id;
wire 								csr_status_ack;

//fprint_registers	
wire								csr_cat_write;
reg									fprint_cat_ack;

//counter_registers
wire								csr_maxcount_write;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0]	csr_maxcount_data;
reg									counter_maxcount_ack;

//comp_registers
wire								csr_pointer_start_write;
wire								csr_pointer_end_write;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0]	csr_pointer_data;
reg									comp_pointer_ack;

//irq
wire 								irq;



reg [1:0]							cat_mem[`CRC_KEY_SIZE-1:0][`CRC_KEY_SIZE-1:0];
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]	maxcount_mem[`CRC_KEY_SIZE-1:0][2:0];
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]	pointer_start_mem[`CRC_KEY_SIZE-1:0][2:0];
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]	pointer_end_mem[`CRC_KEY_SIZE-1:0][2:0];

reg									status_collision_detected;
reg [1:0]							status_collision_logical_core_id;
reg [(`CRC_KEY_WIDTH-1):0]			status_collison_task_id;

reg									init_complete;


integer i,j;

csr_registers csr_register_block(

	clk,
	reset,	
	
	//processor
	csr_address,
	csr_read,
	csr_readdata,
	csr_write,
	csr_writedata,
	csr_waitrequest,

	//global
	csr_logical_core_id,
	csr_task_id,
	csr_physical_core_id,
	
	//comparator	
	comparator_status_write,
	comparator_collision_detected,
	comparator_logical_core_id,
	comparator_task_id,
	csr_status_ack,

	//fprint_registers	
	csr_cat_write,
	fprint_cat_ack,

	//counter_registers
	csr_maxcount_write,
	csr_maxcount_data,
	counter_maxcount_ack,

	//comp_registers
	csr_pointer_start_write,
	csr_pointer_end_write,
	csr_pointer_data,
	comp_pointer_ack,
	
	//irq
	irq
);

/******************************
Set core assignment table
*******************************/
task automatic csr_set_cat;
input integer logical_id;
input integer task_id;
input integer physical_id;
begin
    #`CLK_PERIOD
    csr_write		=	1;
	csr_address		=	`COMPARATOR_CORE_ASSIGNMENT_OFFSET;
    csr_writedata	=	logical_id << 3*`BYTE |
						task_id << 2*`BYTE | 
						physical_id;
	@ (negedge csr_waitrequest) #`CLK_PERIOD;
    
	csr_write 		=	0;
    csr_writedata	=	0;
    csr_address		=	0;
end
endtask

/******************************
Set maxcount
*******************************/
task automatic csr_set_maxcount;
input integer logical_id;
input integer task_id;
input integer maxcount;
begin
    #`CLK_PERIOD
    csr_write		=	1;
	csr_address		=	`COMPARATOR_MAXCOUNT_REG_OFFSET;
    csr_writedata	=	logical_id << 3*`BYTE |
						task_id << 2*`BYTE | 
						maxcount;
	@ (negedge csr_waitrequest) #`CLK_PERIOD;
    
	csr_write 		=	0;
    csr_writedata	=	0;
    csr_address		=	0;
end
endtask

/******************************
Set directory start pointer
*******************************/
task automatic csr_set_pointer_start;
input integer logical_id;
input integer task_id;
input integer pointer_start;
begin
    #`CLK_PERIOD
    csr_write		=	1;
	csr_address		=	`CRC_DIRECTORY_START_BASE;
    csr_writedata	=	logical_id << 3*`BYTE | 
						task_id << 2*`BYTE |
						pointer_start;
	@ (negedge csr_waitrequest) #`CLK_PERIOD;
    
	csr_write		=	0;
    csr_writedata	=	0;
    csr_address		=	0;
end
endtask

/******************************
Set directory end pointer
*******************************/
task automatic csr_set_pointer_end;
input integer logical_id;
input integer task_id;
input integer pointer_end;
begin
    #`CLK_PERIOD
    csr_write		=	1;
	csr_address		=	`CRC_DIRECTORY_END_BASE;
    csr_writedata	=	logical_id << 3*`BYTE |
						task_id << 2*`BYTE |
						pointer_end;
	@ (negedge csr_waitrequest) #`CLK_PERIOD;
    
	csr_write 		=	0;
    csr_writedata	=	0;
    csr_address		=	0;
end
endtask

/******************************
 Comparator write status
*******************************/
task automatic comparator_set_status;
input integer status;
input integer logical_id;
input integer task_id;
begin

    #`CLK_PERIOD
    comparator_status_write					= 1;
	comparator_collision_detected			= status;
	comparator_logical_core_id	= logical_id;
	comparator_task_id						= task_id;
	@ (posedge csr_status_ack) #`CLK_PERIOD;
	
    comparator_status_write					= 0;
	comparator_task_id						= 0;
	comparator_collision_detected			= 0;
	comparator_logical_core_id	= 0;

end
endtask

/******************************
ISR
*******************************/
task automatic csr_read_status;
input integer status;
input integer logical_id;
input integer task_id;
begin
    #`CLK_PERIOD
    	
	csr_read = 1;
	csr_address = `COMPARATOR_EXCEPTION_OFFSET;
    @ (negedge csr_waitrequest) #`CLK_PERIOD;
	assert(csr_readdata[`EXCEPTION_REG_EX_BIT] == status) else $error("Status mismatch, exception reg");
	assert(csr_readdata[`EXCEPTION_REG_TASK_BITS] == task_id) else $error("Task mismatch, ,exception reg");
	
	if(status == 0) begin
		
		csr_address = `COMPARATOR_SUCCESS_REG_OFFSET;
		@ (negedge csr_waitrequest) #`CLK_PERIOD;
		for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
			if(i==task_id) begin
				assert(csr_readdata[i] == 1) else $error("success reg");
			end else begin
				assert(csr_readdata[i] == 0) else $error("success reg");
			end
		end
	
	end else begin
		
		csr_address = `COMPARATOR_FAIL_REG_OFFSET;
		@ (negedge csr_waitrequest) #`CLK_PERIOD;
		for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
			if(i==task_id) begin
				assert(csr_readdata[(2*i)+:2] == logical_id) else $error("fail reg");
			end else begin
				assert(csr_readdata[(2*i)+:2] == 2'b11) else $error("fail reg");
			end
		end
    
	end
    
	csr_read = 0;
    csr_write = 1;
    csr_writedata = 0;
    csr_address = `COMPARATOR_EXCEPTION_OFFSET;
 	@ (negedge csr_waitrequest) #`CLK_PERIOD;
    
	csr_write = 0;
    csr_writedata = 0;
    csr_address = 0;
end
endtask

task automatic set_status_internals;
input integer status;
input integer logical_id;
input integer task_id;

	status_collision_detected			= status;
	status_collision_logical_core_id	= logical_id;
	status_collison_task_id				= task_id;

endtask


task automatic comparator_write_test;
input integer status;
input integer logical_id;
input integer task_id;

	set_status_internals(status, logical_id, task_id);
	comparator_set_status(status, logical_id, task_id);
	@ (negedge irq) #`CLK_PERIOD;

endtask


/******************************
Initial
*******************************/
initial begin
	
	init_complete							= 0;
	
	clk										= 0;

	csr_address								= 0;
	csr_read								= 0;
	csr_write								= 0;
	csr_writedata							= 0;
	
	comparator_status_write					= 0;
	comparator_collision_detected			= 0;
	comparator_logical_core_id	= 0;
	comparator_task_id						= 0;
	
	fprint_cat_ack							= 0;
	counter_maxcount_ack						= 0;
	comp_pointer_ack						= 0;

	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<`CRC_KEY_SIZE ; j=j+1) begin
			cat_mem[i][j] = 0;
		end
	end
	
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			maxcount_mem[i][j] = 0;
			pointer_start_mem[i][j] = 0;
			pointer_end_mem[i][j] = 0;
		end
	end
	
	reset = 1;
	#`CLK_PERIOD
	reset = 0;
	
	init_complete = 1;

end


always begin
        clk = #((`CLK_PERIOD)/2) ~clk;
end

always @ (posedge csr_cat_write) begin
	cat_mem[csr_task_id][csr_physical_core_id] = csr_logical_core_id;
	#`CLK_PERIOD
	fprint_cat_ack = 1;
	#`CLK_PERIOD
	fprint_cat_ack = 0;	
end

always @ (posedge csr_maxcount_write) begin
	maxcount_mem[csr_task_id][csr_logical_core_id] = csr_maxcount_data;
	#`CLK_PERIOD
	counter_maxcount_ack = 1;
	#`CLK_PERIOD
	counter_maxcount_ack = 0;	
end

always @ (posedge csr_pointer_start_write) begin
	pointer_start_mem[csr_task_id][csr_logical_core_id] = csr_pointer_data;
	#`CLK_PERIOD
	comp_pointer_ack = 1;
	#`CLK_PERIOD
	comp_pointer_ack = 0;	
end

always @ (posedge csr_pointer_end_write) begin
	pointer_end_mem[csr_task_id][csr_logical_core_id] = csr_pointer_data;
	#`CLK_PERIOD
	comp_pointer_ack = 1;
	#`CLK_PERIOD
	comp_pointer_ack = 0;	
end

always @ (posedge irq) begin
	csr_read_status(status_collision_detected, status_collision_logical_core_id, status_collison_task_id);
end

/*****************************************************************
Things to test:
	1. Processor actions:
		a. Write to each core assignment table entry
		b. Write to each maxcount entry
		c. Write to each start pointer index
		d. Write to each end pointer index
		e. Read from each status register
		f. Write to the exception register
	2. Comparator actions:
		a. write to the status registers
******************************************************************/

always @(posedge init_complete)begin
	
	/**1. a**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<`CRC_KEY_SIZE ; j=j+1) begin
			csr_set_cat(2'b10, i, j);
		end
	end
	
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<`CRC_KEY_SIZE ; j=j+1) begin
			assert(cat_mem[i][j] == 2'b10) else $error("cat");
		end
	end
	
	$display("Finished test 1. a\t time: %d", $time);
	
	/**1. b**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			csr_set_maxcount(j, i, 21);
		end
	end
	
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			assert(maxcount_mem[i][j] == 21) else $error("maxcount");
		end
	end
	
	$display("Finished test 1. b\t time: %d", $time);
	
	
	/**1. c**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			csr_set_pointer_start(j, i, 21);
		end
	end
	
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			assert(pointer_start_mem[i][j] == 21) else $error("pointer start");
		end
	end
	
	$display("Finished test 1. c\t time: %d", $time);
	
	/**1. d**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			csr_set_pointer_end(j, i, 21);
		end
	end
	
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			assert(pointer_end_mem[i][j] == 21) else $error("pointer end");
		end
	end
	
	$display("Finished test 1. d\t time: %d", $time);
	
	/**2. a (and 1. e,f through ISR) **/
	
	
	// Not working in loops for some reason
	/*
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
				
			comparator_write_test(0,j,i);
			comparator_write_test(1,j,i);
			
		end
	end
	*/
	
	//Hard coded loop just because
	comparator_write_test(0,0,0);
	comparator_write_test(1,0,0);
	comparator_write_test(0,1,0);
	comparator_write_test(1,1,0);
	comparator_write_test(0,2,0);
	comparator_write_test(1,2,0);
	
	comparator_write_test(0,0,1);
	comparator_write_test(1,0,1);
	comparator_write_test(0,1,1);
	comparator_write_test(1,1,1);
	comparator_write_test(0,2,1);
	comparator_write_test(1,2,1);
	
	comparator_write_test(0,0,2);
	comparator_write_test(1,0,2);
	comparator_write_test(0,1,2);
	comparator_write_test(1,1,2);
	comparator_write_test(0,2,2);
	comparator_write_test(1,2,2);
	
	comparator_write_test(0,0,3);
	comparator_write_test(1,0,3);
	comparator_write_test(0,1,3);
	comparator_write_test(1,1,3);
	comparator_write_test(0,2,3);
	comparator_write_test(1,2,3);
	
	comparator_write_test(0,0,4);
	comparator_write_test(1,0,4);
	comparator_write_test(0,1,4);
	comparator_write_test(1,1,4);
	comparator_write_test(0,2,4);
	comparator_write_test(1,2,4);
	
	comparator_write_test(0,0,5);
	comparator_write_test(1,0,5);
	comparator_write_test(0,1,5);
	comparator_write_test(1,1,5);
	comparator_write_test(0,2,5);
	comparator_write_test(1,2,5);
	
	comparator_write_test(0,0,6);
	comparator_write_test(1,0,6);
	comparator_write_test(0,1,6);
	comparator_write_test(1,1,6);
	comparator_write_test(0,2,6);
	comparator_write_test(1,2,6);
	
	comparator_write_test(0,0,7);
	comparator_write_test(1,0,7);
	comparator_write_test(0,1,7);
	comparator_write_test(1,1,7);
	comparator_write_test(0,2,7);
	comparator_write_test(1,2,7);
	
	comparator_write_test(0,0,8);
	comparator_write_test(1,0,8);
	comparator_write_test(0,1,8);
	comparator_write_test(1,1,8);
	comparator_write_test(0,2,8);
	comparator_write_test(1,2,8);
	
	comparator_write_test(0,0,9);
	comparator_write_test(1,0,9);
	comparator_write_test(0,1,9);
	comparator_write_test(1,1,9);
	comparator_write_test(0,2,9);
	comparator_write_test(1,2,9);
	
	comparator_write_test(0,0,10);
	comparator_write_test(1,0,10);
	comparator_write_test(0,1,10);
	comparator_write_test(1,1,10);
	comparator_write_test(0,2,10);
	comparator_write_test(1,2,10);
	
	comparator_write_test(0,0,11);
	comparator_write_test(1,0,11);
	comparator_write_test(0,1,11);
	comparator_write_test(1,1,11);
	comparator_write_test(0,2,11);
	comparator_write_test(1,2,11);
	
	comparator_write_test(0,0,12);
	comparator_write_test(1,0,12);
	comparator_write_test(0,1,12);
	comparator_write_test(1,1,12);
	comparator_write_test(0,2,12);
	comparator_write_test(1,2,12);
	
	comparator_write_test(0,0,13);
	comparator_write_test(1,0,13);
	comparator_write_test(0,1,13);
	comparator_write_test(1,1,13);
	comparator_write_test(0,2,13);
	comparator_write_test(1,2,13);
	
	comparator_write_test(0,0,14);
	comparator_write_test(1,0,14);
	comparator_write_test(0,1,14);
	comparator_write_test(1,1,14);
	comparator_write_test(0,2,14);
	comparator_write_test(1,2,14);
	
	comparator_write_test(0,0,15);
	comparator_write_test(1,0,15);
	comparator_write_test(0,1,15);
	comparator_write_test(1,1,15);
	comparator_write_test(0,2,15);
	comparator_write_test(1,2,15);

	//finishing
	$display("Finished all tests, stopping\t time: %d", $time);
	$stop;
end

endmodule