`include "crc_defines.v"
module tb_comparator();

reg 									clk;
reg 									reset;

/******************************
cfpu
*******************************/
reg [(`COMPARATOR_ADDRESS_WIDTH-1):0]	fprint_address;
reg												fprint_write;
reg [(`NIOS_DATA_WIDTH-1):0]				fprint_writedata;
wire												fprint_waitrequest;

reg [9:0]										csr_address;
reg												csr_read;
wire [31:0]										csr_readdata;
reg												csr_write;
reg [31:0]										csr_writedata;
wire												csr_waitrequest;

wire												oflow_write;
wire [`NIOS_DATA_WIDTH : 0]				oflow_writedata;
wire [`CRC_KEY_WIDTH-1 : 0]				oflow_address;
wire												oflow_waitrequest;

wire												irq;

//wire												io_release;

//wire												comp_status_ack;
//wire [(`CRC_KEY_WIDTH-1):0]				comp_task;


/******************************
unused internals
*******************************/
//wire 							comp_status_write;
//wire 		 					comp_collision_detected;
//wire							exception_reg_sel;
//wire							success_reg_sel;
//wire							fail_reg_sel;
//wire							start_p_sel;
//wire							end_p_sel;
//wire							core_assignment_sel;
//wire [(`CRC_KEY_WIDTH-1):0]	dir_pointer_offset;
//wire [3:0]					core_id;
//wire [3:0]					core_assignment_offset;
//wire [3:0]					core_assignment_data;
//wire							core_a_ack;
//wire [(`CRC_KEY_WIDTH-1):0]	comp_head_read_adr; //0-15 tasks
//wire [(`CRC_KEY_WIDTH-1):0]	comp_tail_read_adr; //0-15 tasks


cfpu cfpu_block(
	clk,
	reset,

	fprint_address,
	fprint_write,
	fprint_writedata,
	fprint_waitrequest,
	
	csr_address,
	csr_read,
	csr_readdata,
	csr_write,
	csr_writedata,
	csr_waitrequest,
	
	oflow_write,
	oflow_writedata,
	oflow_address,
	oflow_waitrequest,
	
	irq
	//io_release
);

/******************************
Set directory start pointer
*******************************/
task automatic set_directory_start;
input integer core;
input[31:0]  key;
input[31:0] offset;
begin
    #10
    csr_write = 1;
    csr_writedata = offset;
    csr_address = `CRC_DIRECTORY_START_BASE + (core << 6) + key;
    @ (negedge csr_waitrequest) #10;
    csr_write = 0;
    csr_writedata = 0;
    csr_address = 0;
end
endtask

/******************************
Set directory end pointer
*******************************/
task automatic set_directory_end;
input integer core;
input[31:0]  key;
input[31:0] offset;
begin
#10
    csr_write = 1;
    csr_writedata = offset ;
    csr_address = `CRC_DIRECTORY_END_BASE + (core << 6) + key;
    @ (negedge csr_waitrequest) #10;
    csr_write = 0;
    csr_writedata = 0;
    csr_address = 0;
end
endtask

/******************************
Set core assignment
*******************************/
task automatic set_core_assignment;
input integer core;
input[31:0]   key;
input[31:0]   core_id;
begin
#10
    csr_write = 1;
    csr_writedata = core_id | (key << 4) ;
    csr_address = (core << 6) + `COMPARATOR_CORE_ASSIGNMENT_OFFSET;
    @ (negedge csr_waitrequest) #10;
    csr_write = 0;
    csr_writedata = 0;
    csr_address = 0;
end
endtask

/******************************
Set maxcount value
*******************************/
task automatic set_maxcount_value;
input[3:0] task_num;
input[`CRC_RAM_ADDRESS_WIDTH-1 : 0] maxcount;
begin
#10
    csr_write = 1;
    csr_writedata = (task_num << `CRC_RAM_ADDRESS_WIDTH) + maxcount;
    csr_address = `COMPARATOR_MAXCOUNT_REG_OFFSET;
    @ (negedge csr_waitrequest) #10;
    csr_write = 0;
    csr_writedata = 0;
    csr_address = 0;
end
endtask

/******************************
ISR
*******************************/
task automatic read_status;
begin
    #10
    //Read from the successreg
    csr_read = 1;
    csr_address = `COMPARATOR_SUCCESS_REG_OFFSET;
    @ (negedge csr_waitrequest) #10;
    csr_address = `COMPARATOR_FAIL_REG_OFFSET;
	@ (negedge csr_waitrequest) #10;
    csr_address = `COMPARATOR_EXCEPTION_OFFSET;
    @ (negedge csr_waitrequest) #10;
    csr_read = 0;
    csr_write = 1;
    csr_writedata = 0;
    csr_address = `COMPARATOR_EXCEPTION_OFFSET;
 	@ (negedge csr_waitrequest) #10;
    csr_write = 0;
    csr_writedata = 0;
    csr_address = 0;
end
endtask

/******************************
Simulate comparator input
*******************************/
// task comp_write;
// input integer current_task;
// input integer status;
// begin

//     #10
//     comp_status_write = 1;
// 	force comp_task = current_task;
// 	comp_collision_detected = status;
// 	@ (posedge comp_status_ack) #10;
// 	comp_status_write = 0;
// 	release comp_task;
// 	comp_collision_detected = 0;

// end
// endtask

/******************************
Write a Fingerprint
*******************************/
task automatic write_fingerprint;
input integer core_id;
input integer task_id;
input integer fingerprint;
begin
	//Takes two writes now...
    #10
	fprint_address = (core_id << 4) + `COMPARATOR_CRC_OFFSET;
	fprint_writedata = {fingerprint[15:0],12'b0,task_id[3:0]};
	fprint_write = 1;
	@ (negedge fprint_waitrequest) #10;
	fprint_writedata = {fingerprint[31:16],12'b10,task_id[3:0]};
	@ (negedge fprint_waitrequest) #10;
	fprint_write =0 ;
	fprint_writedata = 0;
	fprint_address = 0;
end
endtask

/******************************
Enable a task
*******************************/
task automatic enable_task;
input integer core_id;
input integer task_id;
begin
	//Takes two writes now...
    #10
	fprint_address = (core_id << 4) + 0;
	fprint_writedata = {1'b1,task_id[3:0]};
	fprint_write = 1;
	@ (negedge fprint_waitrequest) #10;
	fprint_write =0 ;
	fprint_writedata = 0;
	fprint_address = 0;
end
endtask

/******************************
Enable a task
*******************************/
task automatic disable_task;
input integer core_id;
input integer task_id;
begin
	//Takes two writes now...
    #10
	fprint_address = (core_id << 4) + 0;
	fprint_writedata = {1'b0,task_id[3:0]};
	fprint_write = 1;
	@ (negedge fprint_waitrequest) #10;
	fprint_write =0 ;
	fprint_writedata = 0;
	fprint_address = 0;
end
endtask

/*****************************************************************
Initial Conditions
******************************************************************/


integer init_complete = 0;
initial begin
    clk = 0;

    reset = 1;

    csr_write = 0;
    csr_writedata = 0;
    csr_address = 0;
    csr_read = 0;

    // comp_status_write = 0;
    // comp_collision_detected = 0;

    #5 reset = 0;
    init_complete = 1;
end


always begin
	clk = #5 ~clk;
end

/*****************************************************************
Test 1
Things to test:
	1. Processor actions:
		a. Write to each start pointer index
		b. Write to each end pointer index
		c. Write to each core assignment table entry for each core
		d. Read from each status register
		e. Write to the exception register
	2. Comparator actions:
		a. write to the status registers

Explicitly demonstrate when two requests occur at the same time
******************************************************************/

integer test1_complete = 0;
integer i,j;

/******************************
ISR
*******************************
always @ (posedge irq)
begin
    #200
    //Read from the successreg
    csr_read = 1;
    csr_address = `COMPARATOR_SUCCESS_REG_OFFSET;
    #20
    csr_address = `COMPARATOR_FAIL_REG_OFFSET;
    #20
    csr_read = 0;
    csr_write = 1;
    csr_writedata = 0;
    csr_address = `COMPARATOR_EXCEPTION_OFFSET;
#10
    csr_write = 0;
    csr_writedata = 0;
    csr_address = 0;
end


/******************************
main testing
*******************************/
always @(posedge init_complete)begin : test1 

	//CPU:
	for(i = 0; i < 5; i = i + 1)begin
		set_directory_start(0,i,i*10);
		//b. Write to each end pointer index
		set_directory_end(0,i,(i+1)*10-1);
    end
	//c. Write to each core assignment table entry for each core
    for(i = 0; i < 2; i = i + 1)
    	for(j = 0; j < 16; j = j + 1)begin
    		set_core_assignment(i,j,i);
		end
   
	set_maxcount_value(0,1);
	
	enable_task(0,0);
	enable_task(1,0);
	for(i = 0; i < 50; i = i + 1)begin
	#50
		write_fingerprint(0,0,'hdeadbeef + i);
		write_fingerprint(1,0,'hdeadbeef + i);
	end
	write_fingerprint(0,0,'hdeadbeef + 1);
	write_fingerprint(1,0,'hdeadbeef + 2);
	#100
	disable_task(0,0);
	disable_task(1,0);

	 $display("Test 1 completes at time %d",$time);
end 


endmodule
