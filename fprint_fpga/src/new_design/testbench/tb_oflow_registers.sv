`include "crc_defines.v"

`define BYTE 8
`define CLK_PERIOD 10

module tb_oflow_registers();

reg											clk;
reg											reset;

//processors
wire										oflow_write;
wire [`NIOS_DATA_WIDTH-1 : 0]				oflow_writedata;
wire [`NIOS_ADDRESS_WIDTH-1 : 0]			oflow_address;
reg											oflow_waitrequest;

//global
wire [(`CRC_KEY_WIDTH-1):0]					oflow_task_id;
wire [1:0]									oflow_logical_core_id;
wire [(`CRC_KEY_WIDTH-1):0]					oflow_physical_core_id;

//csr_registers
wire [`CRC_RAM_ADDRESS_WIDTH-1 : 0]			csr_fprint_maxcount;
wire										oflow_nmr;

//fprint_registers
reg [`CRC_KEY_WIDTH-1 : 0]					fprint_task_id;
reg [1:0]									fprint_logical_core_id;

reg											fprint_count_inc;
wire										oflow_count_inc_ack;

//comparator
reg [`CRC_KEY_WIDTH-1 : 0]					comparator_task_id;
	
reg											comparator_count_dec;
wire										oflow_count_dec_ack;

wire [`CRC_KEY_SIZE-1 : 0]					oflow_fprints_ready;
wire [`CRC_KEY_SIZE-1 : 0]					oflow_fprints_remaining;
wire [1:0]									oflow_fprints_remaining_logical_core_id;

reg											oflow_reset_task;
wire										oflow_reset_task_ack;
	
	
//testbench internals

reg											init_complete;

reg [`CRC_KEY_WIDTH-1:0]					bus_task_id;		
reg [`CRC_KEY_WIDTH-1:0]					bus_physical_core_id;	
reg [`CRC_KEY_WIDTH-1:0]					bus_status;	

reg [`CRC_KEY_WIDTH-1:0]					cat_mem_0[`CRC_KEY_SIZE-1:0];
reg [`CRC_KEY_WIDTH-1:0]					cat_mem_1[`CRC_KEY_SIZE-1:0];
reg [`CRC_KEY_WIDTH-1:0]					cat_mem_2[`CRC_KEY_SIZE-1:0];
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]			fprint_maxcount_mem	[(`CRC_KEY_SIZE-1):0][2:0];
reg [(`CRC_KEY_SIZE-1):0]					nmr_mem;

integer i,j,k;

oflow_registers oflow_registers(

	clk,
	reset,
	
	
	oflow_write,
	oflow_writedata,
	oflow_address,
	oflow_waitrequest,
	
	
	oflow_task_id,
	oflow_logical_core_id,
	oflow_physical_core_id,
	
	
	csr_fprint_maxcount,
	oflow_nmr,
	
	
	fprint_task_id,
	fprint_logical_core_id,
	
	fprint_count_inc,
	oflow_count_inc_ack,
	
	
	comparator_task_id,
	
	comparator_count_dec,
	oflow_count_dec_ack,
	
	oflow_fprints_ready,
	oflow_fprints_remaining,
	oflow_fprints_remaining_logical_core_id,
	
	oflow_reset_task,
	oflow_reset_task_ack
	
);

/******************************
fprint: count inc
*******************************/
task automatic count_inc;
input integer task_id;
input integer logical_id;

begin
	
	@ (posedge clk);
	fprint_task_id			=	task_id;
	fprint_logical_core_id	=	logical_id;
    fprint_count_inc		=	1;
	@ (posedge oflow_count_inc_ack);
	fprint_task_id			=	0;
	fprint_logical_core_id	=	0;
    fprint_count_inc		=	0;
end
endtask

/******************************
comparator: count dec
*******************************/
task automatic count_dec;
input integer task_id;

begin
	
	@ (posedge clk);
	comparator_task_id		=	task_id;
    comparator_count_dec	=	1;
	@ (posedge oflow_count_dec_ack);
	comparator_task_id		=	0;
	comparator_count_dec	=	0;
end
endtask

/******************************
comparator: reset task
*******************************/
task automatic reset_task;
input integer task_id;

begin
	
	@ (posedge clk);
	comparator_task_id	=	task_id;
    oflow_reset_task	=	1;
	@ (posedge oflow_reset_task_ack);
	comparator_task_id	=	0;
	oflow_reset_task	=	0;
end
endtask

/******************************
verify count
*******************************/
task automatic verify_count;
input integer task_id;
input integer logical_id;
input integer count_val;

begin

	if(logical_id == 0)
		assert(oflow_registers.fprint_mem_count_0[task_id] == count_val) else $error ("count, task %d, logical core %d", task_id, logical_id);
	else if(logical_id == 1)
		assert(oflow_registers.fprint_mem_count_1[task_id] == count_val) else $error ("count, task %d, logical core %d", task_id, logical_id);
	else if(logical_id == 2)
		assert(oflow_registers.fprint_mem_count_2[task_id] == count_val) else $error ("count, task %d, logical core %d", task_id, logical_id);

end
endtask

/******************************
verify fprints ready
*******************************/
task automatic verify_fprints_ready;
input integer task_id;
input integer fprints_ready;

begin

	assert(oflow_fprints_ready[task_id] == fprints_ready) else $error ("fprints ready, task %d", task_id);

end
endtask

/******************************
verify fprints remaining
*******************************/
task automatic verify_fprints_remaining;
input integer task_id;
input integer logical_id;
input integer fprints_remaining;

begin

	assert(oflow_fprints_remaining[task_id] == fprints_remaining) else $error ("fprints remaining, task %d", task_id);
	if(fprints_remaining)
		assert(oflow_fprints_remaining_logical_core_id == logical_id) else $error ("fprints remaining lcid %d, task %d, ", task_id, logical_id);

end
endtask

/******************************
verify reset task
*******************************/
task automatic verify_reset_task;
input integer task_id;

begin
	
	assert(oflow_registers.fprint_mem_count_0[task_id] == 0) else $error ("reset count, task %d, logical core %d", task_id, 0);
	assert(oflow_registers.fprint_mem_count_1[task_id] == 0) else $error ("reset count, task %d, logical core %d", task_id, 1);
	assert(oflow_registers.fprint_mem_count_2[task_id] == 0) else $error ("reset count, task %d, logical core %d", task_id, 2);
	
	assert(oflow_registers.overflow_status_reg_0[task_id] == 0) else $error ("reset status, task %d, logical core %d", task_id, 0);
	assert(oflow_registers.overflow_status_reg_1[task_id] == 0) else $error ("reset status, task %d, logical core %d", task_id, 1);
	assert(oflow_registers.overflow_status_reg_2[task_id] == 0) else $error ("reset status, task %d, logical core %d", task_id, 2);
	
	assert(oflow_fprints_ready[task_id] == 0) else $error ("reset fprints ready, task %d", task_id);
	assert(oflow_fprints_remaining[task_id] == 0) else $error ("reset fprints remaining, task %d", task_id);
	assert(oflow_fprints_remaining_logical_core_id == 0) else $error ("reset fprints remaining lcid, task %d", task_id);

end
endtask

/******************************
set bus variables
*******************************/
task automatic set_bus_variables;
input integer task_id;
input integer physical_id;
input integer status;

begin

	bus_task_id				=	task_id;
	bus_physical_core_id	=	physical_id;
	bus_status				=	status;
	
end
endtask

/******************************
check oflow bus
*******************************/
task automatic check_oflow_bus;
input integer task_id;
input integer physical_id;
input integer status;

begin


	assert(oflow_address[26:24] == 6) else $error ("oflow address offset, task %d", task_id);
	assert(oflow_address[23:20] == physical_id) else $error ("oflow address pcid, task %d", task_id);
	assert(oflow_writedata[8] == status) else $error ("oflow status, task %d", task_id);
	assert(oflow_writedata[7:4] == physical_id) else $error ("oflow physical_id, task %d", task_id);
	assert(oflow_writedata[3:0] == task_id) else $error ("oflow task id, task %d", task_id);
	
end
endtask

assign oflow_physical_core_id	=	(oflow_logical_core_id == 0) ? cat_mem_0[oflow_task_id] :
									(oflow_logical_core_id == 1) ? cat_mem_1[oflow_task_id] :
									(oflow_logical_core_id == 2) ? cat_mem_2[oflow_task_id] :
									0;
assign csr_fprint_maxcount		=	fprint_maxcount_mem[oflow_task_id][oflow_logical_core_id];
assign oflow_nmr				=	nmr_mem[oflow_task_id];


/******************************
Initial
*******************************/
initial begin
	
	init_complete			= 0;
	
	clk						= 0;

	oflow_waitrequest		= 1;
	fprint_task_id			= 0;
	fprint_logical_core_id	= 0;
	fprint_count_inc		= 0;
	comparator_task_id		= 0;
	comparator_count_dec	= 0;
	oflow_reset_task		= 0;
	
	nmr_mem					= 32'h00000000;
	for(i=0 ; i<`CRC_KEY_SIZE ; i++) begin
		cat_mem_0[i] = 7;
		cat_mem_1[i] = 8;
		cat_mem_2[i] = 9;
		for(j=0 ; j<3 ; j++) begin
			fprint_maxcount_mem[i][j] = 5;
		end
	end
	
	bus_task_id				= 0;
	bus_physical_core_id	= 0;
	bus_status				= 0;
	
	reset					= 1;
	@ (posedge clk);
	#`CLK_PERIOD
	reset					= 0;
	
	init_complete			= 1;

end

/******************************
clk
*******************************/
always begin
        clk = #((`CLK_PERIOD)/2) ~clk;
end

always @ (posedge oflow_write) begin

	@ (posedge clk);
	check_oflow_bus(bus_task_id, bus_physical_core_id, bus_status);
	oflow_waitrequest = 0;
	@ (posedge clk);
	oflow_waitrequest = 1;
end


/*****************************************************************
Things to test:
	1. fprint actions:
		a. count inc
	2. comparator actions:
		a. count dec, for both DMR and TMR
	3. comp actions
		a. reset task, for both DMR and TMR
	4. oflow actions
		a. fprints ready, for both DMR and TMR
		b. fprints remaining and logical core id, for both DMR and TMR
		c. oflow write, for multiple as well
		d. uflow write, multiple as well
		e. more sophisticated uflow test - 2/3 uflow
******************************************************************/
always @(posedge init_complete)begin
	
	/**1. a**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			count_inc(i,j);
			verify_count(i,j,1);
			if(~(j==2)) begin
				count_inc(i,j);
				verify_count(i,j,2);
			end
		end
	end
	
	$display("Finished test 1. a\t time: %d", $time);
	
	
	/**2. a**/
	//for DMR
	nmr_mem = 32'h00000000;
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		count_dec(i);
		verify_count(i,0,1);
		verify_count(i,1,1);
		verify_count(i,2,1);
	end
	
	//for TMR
	nmr_mem = 32'h0000ffff;
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		count_dec(i);
		verify_count(i,0,0);
		verify_count(i,1,0);
		verify_count(i,2,0);
	end
	
	$display("Finished test 2. a\t time: %d", $time);
	
	
	/**3. a**/
	// this one for fun
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		reset_task(i);
		verify_reset_task(i);
	end
	
	// overflow all tasks and cores (kinda performing future tests early)
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			set_bus_variables(i,j+7,1);
			for(k=1 ; k<=5 ; k=k+1) begin
				count_inc(i,j);
				verify_count(i,j,k);
			end
			@ (posedge oflow_waitrequest); 
		end
	end
	
	// now a reset that matters
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		reset_task(i);
		verify_reset_task(i);
	end
	
	$display("Finished test 3. a\t time: %d", $time);	
	
	
	/**4. a**/
	// for DMR
	nmr_mem = 32'h00000000;
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<2 ; j=j+1) begin
			verify_fprints_ready(i,0);
			count_inc(i,j);
			verify_count(i,j,1);
		end
		
		verify_fprints_ready(i,1);
		
		count_dec(i);
		verify_count(i,0,0);
		verify_count(i,1,0);
		
		verify_fprints_ready(i,0);
		
	end
	
	//for TMR
	nmr_mem = 32'h0000ffff;
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			verify_fprints_ready(i,0);
			count_inc(i,j);
			verify_count(i,j,1);
		end
		verify_fprints_ready(i,1);
		
		count_dec(i);
		verify_count(i,0,0);
		verify_count(i,1,0);
		verify_count(i,2,0);
		
		verify_fprints_ready(i,0);
	end
	
	$display("Finished test 4. a\t time: %d", $time);
	
	
	/**4. b**/
	// for DMR
	nmr_mem = 32'h00000000;
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		// logical core 0
		verify_fprints_remaining(i,0,0);
			
		count_inc(i,0);
		verify_count(i,0,1);
			
		verify_fprints_remaining(i,0,1);
			
		count_inc(i,1);
		verify_count(i,1,1);
		count_dec(i);
		verify_count(i,0,0);
		verify_count(i,1,0);
			
		verify_fprints_remaining(i,0,0);
		
		//logical core 1
		verify_fprints_remaining(i,1,0);
			
		count_inc(i,1);
		verify_count(i,1,1);
			
		verify_fprints_remaining(i,1,1);
			
		count_inc(i,0);
		verify_count(i,0,1);
		count_dec(i);
		verify_count(i,0,0);
		verify_count(i,1,0);
			
		verify_fprints_remaining(i,1,0);
	end
	
	//for TMR
	nmr_mem = 32'h0000ffff;
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		// logical core 0
		verify_fprints_remaining(i,0,0);
			
		count_inc(i,0);
		verify_count(i,0,1);
			
		verify_fprints_remaining(i,0,1);
			
		count_inc(i,1);
		count_inc(i,2);
		verify_count(i,1,1);
		verify_count(i,2,1);
		count_dec(i);
		verify_count(i,0,0);
		verify_count(i,1,0);
		verify_count(i,2,0);
			
		verify_fprints_remaining(i,0,0);
		
		//logical core 1
		verify_fprints_remaining(i,1,0);
			
		count_inc(i,1);
		verify_count(i,1,1);
			
		verify_fprints_remaining(i,1,1);
			
		count_inc(i,0);
		count_inc(i,2);
		verify_count(i,0,1);
		verify_count(i,2,1);
		count_dec(i);
		verify_count(i,0,0);
		verify_count(i,1,0);
		verify_count(i,2,0);
			
		verify_fprints_remaining(i,1,0);
		
		//logical core 2
		verify_fprints_remaining(i,2,0);
			
		count_inc(i,2);
		verify_count(i,2,1);
			
		verify_fprints_remaining(i,2,1);
			
		count_inc(i,0);
		count_inc(i,1);
		verify_count(i,0,1);
		verify_count(i,1,1);
		count_dec(i);
		verify_count(i,0,0);
		verify_count(i,1,0);
		verify_count(i,2,0);
			
		verify_fprints_remaining(i,2,0);
	end
	
	$display("Finished test 4. b\t time: %d", $time);
	
	
	/**4. c**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			set_bus_variables(i,j+7,1);
			for(k=1 ; k<=5 ; k=k+1) begin
				count_inc(i,j);
				verify_count(i,j,k);
			end
			@ (posedge oflow_waitrequest); 
		end
	end
	
	$display("Finished test 4. c\t time: %d", $time);


	/**4. d**/
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		set_bus_variables(i,7,0);
		
		for(k=4 ; k>=0 ; k=k-1) begin
			count_dec(i);
			verify_count(i,0,k);
			verify_count(i,1,k);
			verify_count(i,2,k);
		end
		
		@ (posedge oflow_waitrequest);
		set_bus_variables(i,8,0);
		@ (posedge oflow_waitrequest);
		set_bus_variables(i,9,0);
		@ (posedge oflow_waitrequest); 
	
	end
	
	
	$display("Finished test 4. d\t time: %d", $time);

	
	/**4. e**/
	
	// overflow cores 0 and 2
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		for(j=0 ; j<3 ; j=j+1) begin
			if(~(j==1)) begin
				set_bus_variables(i,j+7,1);
				for(k=1 ; k<=5 ; k=k+1) begin
					count_inc(i,j);
					verify_count(i,j,k);
				end
				@ (posedge oflow_waitrequest);
			end
		end
	end
	
	// get all the counts at 4 without overflowing logical core 1
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		count_inc(i,1);
		count_dec(i);
		count_inc(i,1);
		count_inc(i,1);
		count_inc(i,1);
		count_inc(i,1);
		verify_count(i,0,4);
		verify_count(i,1,4);
		verify_count(i,2,4);
	end	
	
	for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
		set_bus_variables(i,7,0);
		
		for(k=3 ; k>=0 ; k=k-1) begin
			count_dec(i);
			verify_count(i,0,k);
			verify_count(i,1,k);
			verify_count(i,2,k);
		end
		
		@ (posedge oflow_waitrequest);
		set_bus_variables(i,9,0);
		@ (posedge oflow_waitrequest); 
	
	end
	
	
	$display("Finished test 4. e\t time: %d", $time);
	
	
	//finishing
	$display("Finished all tests, stopping\t time: %d", $time);
	$stop;
end

endmodule