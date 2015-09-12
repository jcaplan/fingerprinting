`include "crc_defines.v"

`define BYTE 8
`define CLK_PERIOD 10

module tb_fprint_registers();

reg								    		clk;
reg								    		reset;

//processors
reg 	[(`COMPARATOR_ADDRESS_WIDTH-1):0]	fprint_address;
reg											fprint_write;
reg 	[(`NIOS_DATA_WIDTH-1):0]			fprint_writedata;
wire										fprint_waitrequest;

//csr_registers
wire [(`CRC_KEY_WIDTH-1):0]					fprint_task_id;
wire [(`CRC_KEY_WIDTH-1):0]					fprint_physical_core_id;
wire [1:0]									fprint_logical_core_id;

wire [(`CRC_KEY_SIZE-1):0]					fprint_nmr;

//oflow_registers
wire										fprint_count_inc;
reg											oflow_count_inc_ack;

//comp_registers
wire 										fprint_inc_head_pointer;
reg 										comp_inc_head_pointer_ack;

reg [`CRC_RAM_ADDRESS_WIDTH-1:0]     		fprint_head_pointer;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 			comp_tail_pointer_0;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 			comp_tail_pointer_1;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 			comp_tail_pointer_2;
wire [`CRC_WIDTH - 1: 0] 					fprint_0;
wire [`CRC_WIDTH - 1: 0]             		fprint_1;
wire [`CRC_WIDTH - 1: 0]             		fprint_2;

//comparator
reg [(`CRC_KEY_WIDTH-1):0] 					comparator_task_id;

reg 										fprint_reset_task;
wire 										fprint_reset_task_ack;

wire [(`CRC_KEY_SIZE-1):0]  				fprint_checkin;


//testbench internals
reg											init_complete;

reg [`CRC_KEY_WIDTH-1:0]					cat_mem_0[`CRC_KEY_SIZE-1:0];
reg [`CRC_KEY_WIDTH-1:0]					cat_mem_1[`CRC_KEY_SIZE-1:0];
reg [`CRC_KEY_WIDTH-1:0]					cat_mem_2[`CRC_KEY_SIZE-1:0];
reg [(`CRC_KEY_SIZE-1):0]					nmr_mem;


integer i,j,k;

fprint_registers fprint_registers(

	clk,
	reset,
	
	
	fprint_address,
	fprint_write,
	fprint_writedata,
	fprint_waitrequest,
	
	
	fprint_task_id,
	fprint_physical_core_id,
	fprint_logical_core_id,
	
	fprint_nmr,
	
	
	fprint_count_inc,
	oflow_count_inc_ack,
	
	
	fprint_inc_head_pointer,
	comp_inc_head_pointer_ack,
	
	fprint_head_pointer,
	comp_tail_pointer_0,
	comp_tail_pointer_1,
	comp_tail_pointer_2,
	fprint_0,
	fprint_1,
	fprint_2,
	
	
	comparator_task_id,
	
	fprint_reset_task,
	fprint_reset_task_ack,
	
	fprint_checkin

);

/******************************
processor: set_checkout
*******************************/
task automatic set_checkout;
input integer task_id;
input integer physical_id;

begin

    @ (posedge clk)
	fprint_address		= (physical_id << 4) + 0;
	fprint_writedata	= {1'b1,task_id[3:0]};
	fprint_write		= 1;
	@ (negedge fprint_waitrequest);
	fprint_address		= 0;
	fprint_writedata	= 0;
	fprint_write		= 0;

end
endtask

/******************************
processor: set checkin
*******************************/
task automatic set_checkin;
input integer task_id;
input integer physical_id;

begin

    @ (posedge clk)
	fprint_address		= (physical_id << 4) + 0;
	fprint_writedata	= {1'b0,task_id[3:0]};
	fprint_write		= 1;
	@ (negedge fprint_waitrequest);
	fprint_address		= 0;
	fprint_writedata	= 0;
	fprint_write		= 0;

end
endtask

/******************************
comparator: reset task
*******************************/
task automatic reset_task;
input integer task_id;

begin

    @ (posedge clk)
	comparator_task_id	= task_id;
	fprint_reset_task	= 1;
	@ (posedge fprint_reset_task_ack);
	comparator_task_id	= 0;
	fprint_reset_task	= 0;

end
endtask

/******************************
verify checkout
*******************************/
task automatic verify_checkout;
input integer task_id;
input integer logical_id;
input integer checkout_val;

begin

	assert(fprint_registers.checkout_reg[logical_id][task_id] == checkout_val) else $error ("checkout, task %d, lcid %d", task_id, logical_id);

end
endtask

/******************************
verify checkin
*******************************/
task automatic verify_checkin;
input integer task_id;
input integer logical_id;
input integer checkin_val;

begin

	assert(fprint_registers.checkin_reg[logical_id][task_id] == checkin_val) else $error ("checkin, task %d, lcid %d", task_id, logical_id);

end
endtask

/******************************
verify reset task
*******************************/
task automatic verify_reset_task;
input integer task_id;

begin

    verify_checkout(task_id, 0, 0);
    verify_checkout(task_id, 1, 0);
    verify_checkout(task_id, 2, 0);
    verify_checkin(task_id, 0, 0);
    verify_checkin(task_id, 1, 0);
    verify_checkin(task_id, 2, 0);

end
endtask



assign fprint_logical_core_id	=	cat_mem_0[fprint_task_id] == fprint_physical_core_id ? 0 :
									cat_mem_1[fprint_task_id] == fprint_physical_core_id ? 1 :
									cat_mem_2[fprint_task_id] == fprint_physical_core_id ? 2 :
									3;

assign fprint_nmr				=	nmr_mem;

/******************************
Initial
*******************************/
initial begin
	
	init_complete				= 0;
	
	clk							= 0;


	fprint_address				= 0;
	fprint_write				= 0;
	fprint_writedata			= 0;
	oflow_count_inc_ack			= 0;
	comp_inc_head_pointer_ack	= 0;
	fprint_head_pointer			= 0;
	comp_tail_pointer_0			= 0;
	comp_tail_pointer_1			= 0;
	comp_tail_pointer_2			= 0;	
	comparator_task_id			= 0;
	fprint_reset_task			= 0;
	
	
	nmr_mem = 16'hffff;
	for(i=0 ; i<`CRC_KEY_SIZE ; i++) begin
		cat_mem_0[i] = 7;
		cat_mem_1[i] = 8;
		cat_mem_2[i] = 9;
	end
	
	reset = 1;
	@ (posedge clk);
	#`CLK_PERIOD
	reset = 0;
	init_complete				= 1;

end

/******************************
clk
*******************************/
always begin
        clk = #((`CLK_PERIOD)/2) ~clk;
end

/*****************************************************************
Things to test:
	1. Processor actions:
		a. checkout
		b. write a fingerprint
		c. checkin

	2. Comparator actions:
		a. reset task
******************************************************************/
always @(posedge init_complete)begin
	
	/**1. a**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<=2 ; j=j+1) begin
			verify_checkout(i, j, 0);
			verify_checkin(i, j, 0);
		end
	end	
	
	
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=7 ; j<=9 ; j=j+1) begin
			set_checkout(i,j);
		end
	end
	
	for(k=0 ; k<52 ; k++) begin
		#`CLK_PERIOD;
	end
	
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<=2 ; j=j+1) begin
			verify_checkout(i, j, 1);
			verify_checkin(i, j, 0);
		end
	end	

	$display("Finished test 1. a\t time: %d", $time);
	
	/**1. b**/
	
	$display("Finished test 1. b\t time: %d", $time);
	
	
	/**1. c**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<=2 ; j=j+1) begin
			verify_checkout(i, j, 1);
			verify_checkin(i, j, 0);
		end
	end	
	
	
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=7 ; j<=9 ; j=j+1) begin
			set_checkin(i,j);
		end
	end
	
	for(k=0 ; k<52 ; k++) begin
		#`CLK_PERIOD;
	end
	
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<=2 ; j=j+1) begin
			verify_checkout(i, j, 1);
			verify_checkin(i, j, 1);
		end
	end	
	
	$display("Finished test 1. c\t time: %d", $time);
	
	/**2. a**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		reset_task(i);
		verify_reset_task(i);
	end
	
	$display("Finished test 2. a\t time: %d", $time);
	


	//finishing
	$display("Finished all tests, stopping\t time: %d", $time);
	$stop;
end

endmodule
