`include "crc_defines.v"

`define BYTE 8
`define CLK_PERIOD 10

module tb_csr_registers();

reg										clk;
reg										reset;	

//processor
reg [(`COMP_CSR_WIDTH-1):0]				csr_address;
reg										csr_read;
wire [(`NIOS_DATA_WIDTH-1):0]			csr_readdata;
reg										csr_write;
reg [(`NIOS_DATA_WIDTH-1):0]			csr_writedata;
wire 									csr_waitrequest;

//comparator	
reg [(`CRC_KEY_WIDTH-1):0]				comparator_task_id;
reg [1:0]								comparator_logical_core_id;

reg 									comparator_status_write;
reg 		 							comparator_mismatch_detected;
wire 									csr_status_ack;

wire									comparator_nmr;

//fprint_registers	
reg [(`CRC_KEY_WIDTH-1):0]				fprint_task_id;
reg [(`CRC_KEY_WIDTH-1):0]				fprint_physical_core_id;
wire [1:0]								fprint_logical_core_id;

wire [(`CRC_KEY_SIZE-1):0]				fprint_nmr;

//oflow_registers
reg [(`CRC_KEY_WIDTH-1):0]				oflow_task_id;
reg [1:0]								oflow_logical_core_id;
wire [(`CRC_KEY_WIDTH-1):0]				oflow_physical_core_id;

wire [`CRC_RAM_ADDRESS_WIDTH-1 : 0]		csr_fprint_maxcount;
wire									oflow_nmr;

//comp_registers
wire [(`CRC_KEY_WIDTH-1):0]				csr_task_id;
wire [1:0]								csr_logical_core_id;

wire									csr_pointer_start_write;
wire									csr_pointer_end_write;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0]     	csr_pointer_data;
reg										comp_pointer_ack;

//irq
wire 									irq;



//testbench internals
reg										status_mismatch_detected;
reg [`CRC_KEY_WIDTH-1:0]				status_mismatch_task_id;
reg [1:0]								status_mismatch_logical_core_id;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]     	pointer_data;

reg										init_complete;

integer i,j;

csr_registers csr_registers(
	clk,
	reset,	
	
	
	csr_address,
	csr_read,
	csr_readdata,
	csr_write,
	csr_writedata,
	csr_waitrequest,
	
	
	comparator_task_id,
	comparator_logical_core_id,
	
	comparator_status_write,
	comparator_mismatch_detected,
	csr_status_ack,
	
	comparator_nmr,
	
	
	fprint_task_id,
	fprint_physical_core_id,
	fprint_logical_core_id,
	
	fprint_nmr,
	
	oflow_task_id,
	oflow_logical_core_id,
	oflow_physical_core_id,
	
	csr_fprint_maxcount,
	oflow_nmr,
	

	csr_task_id,	
	csr_logical_core_id,
	
	csr_pointer_start_write,
	csr_pointer_end_write,
	csr_pointer_data,
	comp_pointer_ack,
	
	
	irq
);

/******************************
Set core assignment table
*******************************/
task automatic csr_set_cat;
input integer task_id;
input integer logical_id;
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
input integer task_id;
input integer logical_id;
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
input integer task_id;
input integer logical_id;
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
input integer task_id;
input integer logical_id;
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
Set nmr
*******************************/
task automatic csr_set_nmr;
input integer task_id;
input integer nmr;
begin
    #`CLK_PERIOD
    csr_write		=	1;
	csr_address		=	`COMPARATOR_NMR_OFFSET;
    csr_writedata	=	task_id << 2*`BYTE |
						nmr;
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
input integer task_id;
input integer logical_id;

begin

    #`CLK_PERIOD
    comparator_status_write					= 1;
	comparator_mismatch_detected			= status;
	comparator_task_id						= task_id;
	comparator_logical_core_id				= logical_id;

	@ (posedge csr_status_ack) #`CLK_PERIOD;
	
    comparator_status_write					= 0;
	comparator_mismatch_detected			= 0;
	comparator_task_id						= 0;
	comparator_logical_core_id				= 0;

end
endtask

/******************************
 fprint verify cat
*******************************/
task automatic fprint_verify_cat;
input integer task_id;
input integer logical_id;
input integer physical_id;

begin

    @ (posedge clk);
    fprint_task_id			= task_id;
	fprint_physical_core_id	= physical_id;
	@ (posedge clk);
	assert(fprint_logical_core_id == logical_id) else $error("fprint_verify_cat");
    fprint_task_id			= 0;
	fprint_physical_core_id	= 0;


end
endtask

/******************************
 oflow verify cat
*******************************/
task automatic oflow_verify_cat;
input integer task_id;
input integer logical_id;
input integer physical_id;

begin

    @ (posedge clk);
    oflow_task_id			= task_id;
	oflow_logical_core_id	= logical_id;
	@ (posedge clk);
	assert(oflow_physical_core_id == physical_id) else $error("oflow_verify_cat");
    oflow_task_id			= 0;
	oflow_logical_core_id	= 0;


end
endtask

/******************************
 oflow verify maxcount
*******************************/
task automatic oflow_verify_maxcount;
input integer task_id;
input integer logical_id;
input integer maxcount;

begin

    #`CLK_PERIOD
    oflow_task_id			= task_id;
	oflow_logical_core_id	= logical_id;
	@ (posedge clk);
	assert(csr_fprint_maxcount	== maxcount) else $error("oflow_verify_maxcount");
    oflow_task_id			= 0;
	oflow_logical_core_id	= 0;


end
endtask

/******************************
 Comparator verify nmr
*******************************/
task automatic comparator_verify_nmr;
input integer task_id;
input integer nmr;

begin

	comparator_task_id = task_id;
	@ (posedge clk);
	assert(comparator_nmr == nmr) else $error("comparator_verify_nmr");
	comparator_task_id = 0;

end
endtask

/******************************
 oflow verify nmr
*******************************/
task automatic oflow_verify_nmr;
input integer task_id;
input integer nmr;

begin

	oflow_task_id = task_id;
	@ (posedge clk);
	assert(oflow_nmr == nmr) else $error("oflow_verify_nmr");
	oflow_task_id = 0;

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
input integer task_id;
input integer logical_id;

	status_mismatch_detected			= status;
	status_mismatch_task_id				= task_id;
	status_mismatch_logical_core_id		= logical_id;
	
endtask


task automatic comparator_write_test;
input integer status;
input integer task_id;
input integer logical_id;


	set_status_internals(status, task_id, logical_id);
	comparator_set_status(status, task_id, logical_id);
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
	comparator_mismatch_detected			= 0;
	comparator_logical_core_id	= 0;
	comparator_task_id						= 0;
	
	reset = 1;
	@ (posedge clk);
	#`CLK_PERIOD
	reset = 0;
	
	init_complete = 1;

end


/******************************
clk
*******************************/
always begin
        clk = #((`CLK_PERIOD)/2) ~clk;
end

always @ (posedge irq) begin
	csr_read_status(status_mismatch_detected, status_mismatch_logical_core_id, status_mismatch_task_id);
end

always @ (posedge csr_pointer_start_write) begin
	assert(csr_pointer_data == pointer_data) else $error("start pointer write");
	comp_pointer_ack = 1;
	@ (posedge clk);
	comp_pointer_ack = 0;
end

always @ (posedge csr_pointer_end_write) begin
	assert(csr_pointer_data == pointer_data) else $error("end pointer write");
	comp_pointer_ack = 1;
	@ (posedge clk);
	comp_pointer_ack = 0;
end

/*****************************************************************
Things to test:
	1. Processor actions:
		a. Write to each core assignment table entry
		b. Write to each maxcount entry
		c. Write to each start pointer index
		d. Write to each end pointer index
		e. Write to each nmr entry
		f. Read from each status register
		g. Write to the exception register
	2. Comparator actions:
		a. write to the status registers
******************************************************************/

always @(posedge init_complete)begin
	
	/**1. a**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			csr_set_cat(i, j, j);
		end
	end
	
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			fprint_verify_cat(i, j, j);
			oflow_verify_cat(i, j, j);
		end
	end
	
	$display("Finished test 1. a\t time: %d", $time);
	
	/**1. b**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			csr_set_maxcount(i, j, i);
		end
	end
	
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			oflow_verify_maxcount(i, j, i);
		end
	end
	
	$display("Finished test 1. b\t time: %d", $time);
	
	
	/**1. c**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			pointer_data = i;
			csr_set_pointer_start(i, j, i);
		end
	end

	$display("Finished test 1. c\t time: %d", $time);
	
	/**1. d**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			pointer_data = i;
			csr_set_pointer_end(i, j, i);
		end
	end

	$display("Finished test 1. d\t time: %d", $time);
	
	/**1. e**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		csr_set_nmr(i, i%2);
	end
	
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		comparator_verify_nmr(i, i%2);
		oflow_verify_nmr(i, i%2);
	end
	$display("Finished test 1. e\t time: %d", $time);
	
	
	/**2. a (and 1. f,g through ISR) **/
	
	
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
	comparator_write_test(0, 0 , 0);
	comparator_write_test(1, 0 , 0);
	comparator_write_test(0, 0 , 1);
	comparator_write_test(1, 0 , 1);
	comparator_write_test(0, 0 , 2);
	comparator_write_test(1, 0 , 2);
	comparator_write_test(0, 1 , 0);
	comparator_write_test(1, 1 , 0);
	comparator_write_test(0, 1 , 1);
	comparator_write_test(1, 1 , 1);
	comparator_write_test(0, 1 , 2);
	comparator_write_test(1, 1 , 2);
	comparator_write_test(0, 2 , 0);
	comparator_write_test(1, 2 , 0);
	comparator_write_test(0, 2 , 1);
	comparator_write_test(1, 2 , 1);
	comparator_write_test(0, 2 , 2);
	comparator_write_test(1, 2 , 2);
	comparator_write_test(0, 3 , 0);
	comparator_write_test(1, 3 , 0);
	comparator_write_test(0, 3 , 1);
	comparator_write_test(1, 3 , 1);
	comparator_write_test(0, 3 , 2);
	comparator_write_test(1, 3 , 2);
	comparator_write_test(0, 4 , 0);
	comparator_write_test(1, 4 , 0);
	comparator_write_test(0, 4 , 1);
	comparator_write_test(1, 4 , 1);
	comparator_write_test(0, 4 , 2);
	comparator_write_test(1, 4 , 2);
	comparator_write_test(0, 5 , 0);
	comparator_write_test(1, 5 , 0);
	comparator_write_test(0, 5 , 1);
	comparator_write_test(1, 5 , 1);
	comparator_write_test(0, 5 , 2);
	comparator_write_test(1, 5 , 2);
	comparator_write_test(0, 6 , 0);
	comparator_write_test(1, 6 , 0);
	comparator_write_test(0, 6 , 1);
	comparator_write_test(1, 6 , 1);
	comparator_write_test(0, 6 , 2);
	comparator_write_test(1, 6 , 2);
	comparator_write_test(0, 7 , 0);
	comparator_write_test(1, 7 , 0);
	comparator_write_test(0, 7 , 1);
	comparator_write_test(1, 7 , 1);
	comparator_write_test(0, 7 , 2);
	comparator_write_test(1, 7 , 2);
	comparator_write_test(0, 8 , 0);
	comparator_write_test(1, 8 , 0);
	comparator_write_test(0, 8 , 1);
	comparator_write_test(1, 8 , 1);
	comparator_write_test(0, 8 , 2);
	comparator_write_test(1, 8 , 2);
	comparator_write_test(0, 9 , 0);
	comparator_write_test(1, 9 , 0);
	comparator_write_test(0, 9 , 1);
	comparator_write_test(1, 9 , 1);
	comparator_write_test(0, 9 , 2);
	comparator_write_test(1, 9 , 2);
	comparator_write_test(0, 10, 0);
	comparator_write_test(1, 10, 0);
	comparator_write_test(0, 10, 1);
	comparator_write_test(1, 10, 1);
	comparator_write_test(0, 10, 2);
	comparator_write_test(1, 10, 2);
	comparator_write_test(0, 11, 0);
	comparator_write_test(1, 11, 0);
	comparator_write_test(0, 11, 1);
	comparator_write_test(1, 11, 1);
	comparator_write_test(0, 11, 2);
	comparator_write_test(1, 11, 2);
	comparator_write_test(0, 12, 0);
	comparator_write_test(1, 12, 0);
	comparator_write_test(0, 12, 1);
	comparator_write_test(1, 12, 1);
	comparator_write_test(0, 12, 2);
	comparator_write_test(1, 12, 2);
	comparator_write_test(0, 13, 0);
	comparator_write_test(1, 13, 0);
	comparator_write_test(0, 13, 1);
	comparator_write_test(1, 13, 1);
	comparator_write_test(0, 13, 2);
	comparator_write_test(1, 13, 2);
	comparator_write_test(0, 14, 0);
	comparator_write_test(1, 14, 0);
	comparator_write_test(0, 14, 1);
	comparator_write_test(1, 14, 1);
	comparator_write_test(0, 14, 2);
	comparator_write_test(1, 14, 2);
	comparator_write_test(0, 15, 0);
	comparator_write_test(1, 15, 0);
	comparator_write_test(0, 15, 1);
	comparator_write_test(1, 15, 1);
	comparator_write_test(0, 15, 2);
	comparator_write_test(1, 15, 2);

	//finishing
	$display("Finished all tests, stopping\t time: %d", $time);
	$stop;
end

endmodule
