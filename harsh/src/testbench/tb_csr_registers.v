`include "crc_defines.v"
module tb_csr_registers();


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

reg 									comp_status_write;
wire  									comp_status_ack;
reg [(`CRC_KEY_WIDTH-1):0]			    comp_current_task;
reg 		 							comp_collision_detected;
reg 									core_a_ack;
csr_decode decoder(
	clk,
	reset,
	csr_address,
	csr_writedata,

	
	exception_reg_sel,
	success_reg_sel,
	fail_reg_sel,
	start_p_sel,
	end_p_sel,
	core_assignment_sel,
	dir_pointer_offset,
	core_id,
	core_assignment_offset
);

csr_registers csr_register_block(
	
	//From processor
	clk,
	reset,	
	csr_read,
	csr_readdata,
	csr_write,
	csr_writedata,
	csr_waitrequest,


	//From decoder
	exception_reg_sel,
	success_reg_sel,
	fail_reg_sel,
	start_p_sel,
	end_p_sel,
	core_assignment_sel,
	dir_pointer_offset,
	
	//From comparator
	comp_status_write,
	comp_status_ack,
	comp_current_task,
	comp_collision_detected,

	core_a_ack
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

task automatic comp_write;
input integer current_task;
input integer status;
begin

    #10
    comp_status_write = 1;
	comp_current_task = current_task;
	comp_collision_detected = status;
	@ (posedge comp_status_ack) #10;
	comp_status_write = 0;
	comp_current_task = 0;
	comp_collision_detected = 0;

end
endtask


initial begin
    clk = 0;

    reset = 1;

    csr_write = 0;
    csr_writedata = 0;
    csr_address = 0;
    csr_read = 0;

    comp_status_write = 0;
    comp_current_task = 0;
    comp_collision_detected = 0;

    #5 reset = 0;
end

always begin
        clk = #5 ~clk;
end

/*****************************************************************
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


//fake the fprint register for now
always @(posedge core_assignment_sel)begin
	#50 core_a_ack = 1;
	#10 core_a_ack = 0;
end
always @(negedge reset)begin : test1 
 
 	//CPU:
	for(j = 0; j < 16; j = j + 1)begin
		//a. Write to each start pointer index
		set_directory_start(0,j,j);
		//b. Write to each end pointer index
		set_directory_end(0,j,j);
    end
	//c. Write to each core assignment table entry for each core
    for(i = 0; i < 2; i = i + 1)
    	for(j = 0; j < 16; j = j + 1)begin
    		set_core_assignment(i,j,(i+1));
		end
   
	//d. Read from each status register
	//e. Write to the exception register
	read_status();

	//Comparator:
	//a. write to the status registers
	comp_write(2,0);
	#20 comp_write(3,0);
	#20 comp_write(4,1);
	#20 comp_write(5,1);

	read_status();
	test1_complete = 1;
	 $display("Test 1 completes at time %d",$time);
/*
reg 									comp_status_write;
wire  									comp_status_ack;
reg [(`CRC_KEY_WIDTH-1):0]			    comp_current_task;
reg 		 							comp_collision_detected;*/
end


/*****************************************************************
Here we check arbitration:
******************************************************************/

//First try and write to directory at the same time
always @ (posedge test1_complete)begin: test2_a
	set_directory_start(0,j,j);
	@ (posedge comp_status_ack) #10;
	read_status();
end

always @ (posedge test1_complete)begin: test2_b
	comp_write(3,0);
	comp_write(4,0);
end
endmodule
