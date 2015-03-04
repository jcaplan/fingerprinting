`include "crc_defines.v"
module tb_comparator();


wire 									comp_status_write;
wire 		 							comp_collision_detected;
reg 	[(`COMPARATOR_ADDRESS_WIDTH-1):0]	fprint_address;
reg											fprint_write;
reg 	[(`NIOS_DATA_WIDTH-1):0]			fprint_writedata;
wire fprint_waitrequest = 0;
reg 									clk;
reg 									reset;
reg[9:0]                              	csr_address;
reg                                    	csr_read;
wire [31:0]                            	csr_readdata;
reg                                    	csr_write;
reg[31:0]                              	csr_writedata;
wire                                   	csr_waitrequest;


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



wire [(`CRC_KEY_WIDTH-1):0]      		comp_head_read_adr; //0-15 tasks
wire [(`CRC_KEY_WIDTH-1):0]      		comp_tail_read_adr; //0-15 tasks
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

wire 			 						comp_mismatch_detected;

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
