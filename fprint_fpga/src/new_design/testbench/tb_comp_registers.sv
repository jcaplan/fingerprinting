`include "crc_defines.v"

`define BYTE 8
`define CLK_PERIOD 10

module tb_comp_registers();

reg									clk;
reg									reset;

//csr_registers
reg [1:0]							csr_logical_core_id;
reg [(`CRC_KEY_WIDTH-1):0]			csr_task_id;

reg									csr_pointer_start_write;
reg									csr_pointer_end_write;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0]	csr_pointer_data;
wire								comp_pointer_ack;

//fprint_registers
reg [1:0]							fprint_logical_core_id;
reg [(`CRC_KEY_WIDTH-1):0]			fprint_task_id;

reg 								fprint_inc_head_pointer;
wire 								comp_inc_head_pointer_ack;

wire [`CRC_RAM_ADDRESS_WIDTH-1:0]	fprint_head_pointer;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0]	comp_tail_pointer_0;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0]	comp_tail_pointer_1;
wire [`CRC_RAM_ADDRESS_WIDTH-1:0]	comp_tail_pointer_2;

//comparator
reg [(`CRC_KEY_WIDTH-1):0]			comparator_task_id;

reg 								comparator_inc_tail_pointer;
wire								comp_inc_tail_pointer_ack;

reg									comp_reset_task;
wire								comp_reset_task_ack;


//testbench internals
reg									init_complete;

integer i,j,k;

comp_registers comp_registers(

	clk,
	reset,
	
	
	csr_logical_core_id,
	csr_task_id,
	
	csr_pointer_start_write,
	csr_pointer_end_write,
	csr_pointer_data,
	comp_pointer_ack,
	
	
	fprint_logical_core_id,
	fprint_task_id,
	
	fprint_inc_head_pointer,
	comp_inc_head_pointer_ack,
	
	fprint_head_pointer,
	comp_tail_pointer_0,
	comp_tail_pointer_1,
	comp_tail_pointer_2,
	
	
	comparator_task_id,
	
	comparator_inc_tail_pointer,
	comp_inc_tail_pointer_ack,
	
	comp_reset_task,
	comp_reset_task_ack
);

/******************************
csr: set start pointer
*******************************/
task automatic set_start_pointer;
input integer task_id;
input integer logical_id;
input integer pointer_data;

begin
	
	@ (posedge clk);
	csr_task_id					=	task_id;
	csr_logical_core_id			=	logical_id;
    csr_pointer_start_write		=	1;
	csr_pointer_data			=	pointer_data;
	@ (posedge comp_pointer_ack) #`CLK_PERIOD;
	csr_task_id					=	0;
	csr_logical_core_id			=	0;
    csr_pointer_start_write		=	0;
	csr_pointer_data			=	0;
end
endtask

/******************************
csr: set end pointer
*******************************/
task automatic set_end_pointer;
input integer task_id;
input integer logical_id;
input integer pointer_data;

begin
   @ (posedge clk);
	csr_task_id					=	task_id;
	csr_logical_core_id			=	logical_id;
    csr_pointer_end_write		=	1;
	csr_pointer_data			=	pointer_data;
	@ (posedge comp_pointer_ack) #`CLK_PERIOD;
	csr_task_id					=	0;
	csr_logical_core_id			=	0;
    csr_pointer_end_write		=	0;
	csr_pointer_data			=	0;
end
endtask

/******************************
fprint: inc head pointer
*******************************/
task automatic inc_head_pointer;
input integer task_id;
input integer logical_id;

begin
    @ (posedge clk);
	fprint_task_id				=	task_id;
	fprint_logical_core_id		=	logical_id;
    fprint_inc_head_pointer		=	1;
	@ (posedge comp_inc_head_pointer_ack) #`CLK_PERIOD;
	fprint_task_id				=	0;
	fprint_logical_core_id		=	0;
    fprint_inc_head_pointer		=	0;
end
endtask

/******************************
comparator: inc tail pointer
*******************************/
task automatic inc_tail_pointer;
input integer task_id;

begin
    @ (posedge clk);
	comparator_task_id			=	task_id;
    comparator_inc_tail_pointer	=	1;
	@ (posedge comp_inc_tail_pointer_ack) #`CLK_PERIOD;
	comparator_task_id			=	0;
    comparator_inc_tail_pointer	=	0;
end
endtask

/******************************
comparator: reset task
*******************************/
task automatic reset_task;
input integer task_id;

begin
	@ (posedge clk);
	comparator_task_id		=	task_id;
    comp_reset_task	=	1;
	@ (posedge comp_reset_task_ack) #`CLK_PERIOD;
	comparator_task_id		=	0;
    comp_reset_task	=	0;
end
endtask

/******************************
verify start pointer
*******************************/
task automatic verify_start_pointer;
input integer task_id;
input integer logical_id;
input integer pointer_data;

begin
    if(logical_id == 0) 
		assert(comp_registers.start_pointer_mem_0[task_id] == pointer_data) else $error ("start pointer, task %d, logical core %d", task_id, logical_id);
	else if(logical_id == 1)
		assert(comp_registers.start_pointer_mem_1[task_id] == pointer_data) else $error ("start pointer, task %d, logical core %d", task_id, logical_id);
	else if(logical_id == 2)
		assert(comp_registers.start_pointer_mem_2[task_id] == pointer_data) else $error ("start pointer, task %d, logical core %d", task_id, logical_id);
end
endtask

/******************************
verify end pointer
*******************************/
task automatic verify_end_pointer;
input integer task_id;
input integer logical_id;
input integer pointer_data;

begin
    if(logical_id == 0) 
		assert(comp_registers.end_pointer_mem_0[task_id] == pointer_data) else $error ("end pointer, task %d, logical core %d", task_id, logical_id);
	else if(logical_id == 1)
		assert(comp_registers.end_pointer_mem_1[task_id] == pointer_data) else $error ("end pointer, task %d, logical core %d", task_id, logical_id);
	else if(logical_id == 2)
		assert(comp_registers.end_pointer_mem_2[task_id] == pointer_data) else $error ("end pointer, task %d, logical core %d", task_id, logical_id);
end
endtask

/******************************
verify head pointer
*******************************/
task automatic verify_head_pointer;
input integer task_id;
input integer logical_id;
input integer pointer_data;

begin
	fprint_task_id				=	task_id;
	fprint_logical_core_id		=	logical_id;
	@ (posedge clk) #1;
	assert(fprint_head_pointer == pointer_data) else $error ("head pointer, task %d, logical core %d, %d, %d", task_id, logical_id, fprint_head_pointer, pointer_data);
	@ (posedge clk);
	fprint_task_id				=	0;
	fprint_logical_core_id		=	0;
end
endtask

/******************************
verify tail pointer
*******************************/
task automatic verify_tail_pointer;
input integer task_id;
input integer logical_id;
input integer pointer_data;

begin
	comparator_task_id			=	task_id;
	@ (posedge clk) #1;
	
	if(logical_id == 0)
		assert(comp_tail_pointer_0 == pointer_data) else $error ("tail pointer, task %d, logical core %d", task_id, logical_id);
	else if(logical_id == 1)
		assert(comp_tail_pointer_1 == pointer_data) else $error ("tail pointer, task %d, logical core %d", task_id, logical_id);
	else if(logical_id == 2)
		assert(comp_tail_pointer_2 == pointer_data) else $error ("tail pointer, task %d, logical core %d", task_id, logical_id);
	
	@ (posedge clk);
	comparator_task_id			=	0;
end
endtask


/******************************
Initial
*******************************/
initial begin
	
	init_complete					= 0;
	clk								= 0;

	csr_logical_core_id				= 0;
	csr_task_id						= 0;
	
	csr_pointer_start_write			= 0;
	csr_pointer_end_write			= 0;
	csr_pointer_data				= 0;
	
	fprint_logical_core_id			= 0;
	fprint_task_id					= 0;
	
	fprint_inc_head_pointer			= 0;
	
	comparator_task_id				= 0;
	
	comparator_inc_tail_pointer		= 0;
	
	comp_reset_task			= 0;
	
	
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


/*****************************************************************
Things to test:
	1. csr actions:
		a. Write to each start pointer index
		b. Write to each end pointer index
	2. fprint actions:
		a. increment head pointer, check wrap around
	3. comparator actions:
		a. increment tail pointer, check wrap around
		b. reset task
******************************************************************/

always @(posedge init_complete)begin
	
	/**1. a**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			set_start_pointer(i, j, (i*30)+(j*10));
			verify_start_pointer(i, j, (i*30)+(j*10));
		end
	end
	
	@ (posedge clk) $display("Finished test 1. a\t time: %d", $time);
	
	
	/**1. b**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			set_end_pointer(i, j, (i*30)+(j*10)+9);
			verify_end_pointer(i, j, (i*30)+(j*10)+9);
		end
	end
	
	@ (posedge clk) $display("Finished test 1. b\t time: %d", $time);
	
	
	//Check that the head and tail pointers for all tasks and logical cores are default
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			verify_head_pointer(i, j, (i*30)+(j*10));
			verify_tail_pointer(i, j, (i*30)+(j*10));
		end
	end
	
	//@ (posedge clk) $display("head tail default verified");
	
	
	/**2. a**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			for(k=1 ; k<=10 ; k=k+1) begin
				inc_head_pointer(i,j);
			
				if(k==10) // wrap around 
					verify_head_pointer(i, j, (i*30)+(j*10));
				else
					verify_head_pointer(i, j, (i*30)+(j*10)+k);
				
				verify_tail_pointer(i, 0, (i*30));
				verify_tail_pointer(i, 1, (i*30)+10);
				verify_tail_pointer(i, 2, (i*30)+20);
			end
		end
	end
	
	$display("Finished test 2. a\t time: %d", $time);

	//Check that the head and tail pointers for all tasks and logical cores are default
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			verify_head_pointer(i, j, (i*30)+(j*10));
			verify_tail_pointer(i, j, (i*30)+(j*10));
		end
	end
	
	//@ (posedge clk) $display("head tail default verified");
	
	/**3. a**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(k=1 ; k<=10 ; k=k+1) begin
			inc_tail_pointer(i);
		
			if(k==10) begin // wrap around
				verify_tail_pointer(i, 0, (i*30));
				verify_tail_pointer(i, 1, (i*30)+10);
				verify_tail_pointer(i, 2, (i*30)+20);
			end else begin
				verify_tail_pointer(i, 0, (i*30)+k);
				verify_tail_pointer(i, 1, (i*30)+10+k);
				verify_tail_pointer(i, 2, (i*30)+20+k);
			end
			
			verify_head_pointer(i, 0, (i*30));
			verify_head_pointer(i, 1, (i*30)+10);
			verify_head_pointer(i, 2, (i*30)+20);
			

		end
	end
	
	$display("Finished test 3. a\t time: %d", $time);
	
	//Check that the head and tail pointers for all tasks and logical cores are default
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			verify_head_pointer(i, j, (i*30)+(j*10));
			verify_tail_pointer(i, j, (i*30)+(j*10));
		end
	end
	
	//@ (posedge clk) $display("head tail default verified");
	/**3. a**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
			
		inc_head_pointer(i,0);
		inc_head_pointer(i,1);
		inc_head_pointer(i,2);
		inc_tail_pointer(i);
		
		verify_tail_pointer(i, 0, (i*30)+1);
		verify_tail_pointer(i, 1, (i*30)+11);
		verify_tail_pointer(i, 2, (i*30)+21);		
		verify_head_pointer(i, 0, (i*30)+1);
		verify_head_pointer(i, 1, (i*30)+11);
		verify_head_pointer(i, 2, (i*30)+21);
		
		reset_task(i);
		
		verify_tail_pointer(i, 0, (i*30));
		verify_tail_pointer(i, 1, (i*30)+10);
		verify_tail_pointer(i, 2, (i*30)+20);		
		verify_head_pointer(i, 0, (i*30));
		verify_head_pointer(i, 1, (i*30)+10);
		verify_head_pointer(i, 2, (i*30)+20);

	end
	
	$display("Finished test 3. b\t time: %d", $time);
	
	//finishing
	$display("Finished all tests, stopping\t time: %d", $time);
	$stop;
end

endmodule
