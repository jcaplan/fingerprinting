`include "crc_defines.v"
module tb_comparator_maxcount();

reg 									clk;
reg 									reset;

/******************************
cfpu interface signals
*******************************/
wire [(`COMPARATOR_ADDRESS_WIDTH-1):0]	fprint_address;
wire												fprint_write;
wire [(`NIOS_DATA_WIDTH-1):0]				fprint_writedata;
wire												fprint_waitrequest;

wire [9:0]										csr_address;
wire												csr_read;
wire [31:0]										csr_readdata;
wire												csr_write;
wire [31:0]										csr_writedata;
wire												csr_waitrequest;

wire												oflow_write;
wire [`NIOS_DATA_WIDTH : 0]				oflow_writedata;
wire [`CRC_KEY_WIDTH-1 : 0]				oflow_address;
reg												oflow_waitrequest;

wire												irq;


/******************************
fprint and csr bus arbitration
*******************************/

reg													fsm_fprint_write [`CRC_KEY_SIZE-1 : 0];
reg [(`NIOS_DATA_WIDTH-1):0]					fsm_fprint_writedata [`CRC_KEY_SIZE-1 : 0];
reg [(`COMPARATOR_ADDRESS_WIDTH-1):0]		fsm_fprint_address [`CRC_KEY_SIZE-1 : 0];
wire [`CRC_KEY_SIZE-1 : 0]						fsm_fprint_waitrequest;


reg													fsm_csr_write [`CRC_KEY_SIZE-1 : 0];
reg [(`NIOS_DATA_WIDTH-1):0]					fsm_csr_writedata [`CRC_KEY_SIZE-1 : 0];
reg													fsm_csr_read [`CRC_KEY_SIZE-1 : 0];
wire [(`NIOS_DATA_WIDTH-1):0]					fsm_csr_readdata [`CRC_KEY_SIZE-1 : 0];
reg [(`COMPARATOR_ADDRESS_WIDTH-1):0]		fsm_csr_address [`CRC_KEY_SIZE-1 : 0];
wire [`CRC_KEY_SIZE-1 : 0]						fsm_csr_waitrequest;

reg													irq_csr_write;
reg [(`NIOS_DATA_WIDTH-1):0]					irq_csr_writedata;
reg													irq_csr_read;
wire [(`NIOS_DATA_WIDTH-1):0]					irq_csr_readdata;
reg [(`COMPARATOR_ADDRESS_WIDTH-1):0]		irq_csr_address;
wire [`CRC_KEY_SIZE-1 : 0]						irq_csr_waitrequest;


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
Arbitration simulation 
*******************************/
parameter idle = 0;
parameter waitforsignal = 1;
parameter st_isr = 2;

//Just make a simple priority scheme
integer state_fprint;
integer turn_fprint;

always @(posedge clk or posedge reset)begin
    if(reset)begin
        turn_fprint = 16;
		  state_fprint = idle;
    end else begin
        //First if bus is idle assign to highest priority
        case(state_fprint)
        idle: begin
            state_fprint = waitforsignal;
            if(fsm_fprint_write[0])
                turn_fprint = 0;
            else if(fsm_fprint_write[1])
					turn_fprint = 1;
				else if(fsm_fprint_write[2])
					turn_fprint = 2;
				else if(fsm_fprint_write[3])
					turn_fprint = 3;
				else if(fsm_fprint_write[4])
					turn_fprint = 4;
				else if(fsm_fprint_write[5])
					turn_fprint = 5;
				else if(fsm_fprint_write[6])
					turn_fprint = 6;
				else if(fsm_fprint_write[7])
					turn_fprint = 7;
				else if(fsm_fprint_write[8])
					turn_fprint = 8;
				else if(fsm_fprint_write[9])
					turn_fprint = 9;
				else if(fsm_fprint_write[10])
					turn_fprint = 10;
				else if(fsm_fprint_write[11])
					turn_fprint = 11;
				else if(fsm_fprint_write[12])
					turn_fprint = 12;
				else if(fsm_fprint_write[13])
					turn_fprint = 13;
				else if(fsm_fprint_write[14])
					turn_fprint = 14;
				else if(fsm_fprint_write[15])
					turn_fprint = 15;
            else begin
					turn_fprint = 16;
					state_fprint = idle;
            end
        end
        waitforsignal:
            if(fsm_fprint_waitrequest[turn_fprint] == 0)
                state_fprint = idle;
            else begin
                state_fprint = waitforsignal;
            end
        default:
            state_fprint = idle; 
        endcase  
    end
end

assign fprint_address = state_fprint == waitforsignal ? fsm_fprint_address[turn_fprint] : 0;
assign fprint_write = state_fprint == waitforsignal ? fsm_fprint_write[turn_fprint] : 0;
assign fprint_writedata = state_fprint == waitforsignal ? fsm_fprint_writedata[turn_fprint] : 0;
assign fsm_fprint_waitrequest = (state_fprint == waitforsignal) ? 
										  ((fprint_waitrequest<<turn_fprint) | (~(16'b1<<turn_fprint))) : 
										  16'hffff;
										  
integer state_csr;
integer turn_csr;

always @(posedge clk or posedge reset)begin
    if(reset)begin
        turn_csr = 16;
		  state_csr = idle;
    end else begin
        //First if bus is idle assign to highest priority
        case(state_csr)
        idle: begin
            state_csr = waitforsignal;
				if(irq)
					state_csr = st_isr;
            else if(fsm_csr_write[0] | fsm_csr_read[0])
                turn_csr = 0;
            else if(fsm_csr_write[1] | fsm_csr_read[1])
					turn_csr = 1;
				else if(fsm_csr_write[2] | fsm_csr_read[2])
					turn_csr = 2;
				else if(fsm_csr_write[3] | fsm_csr_read[3])
					turn_csr = 3;
				else if(fsm_csr_write[4] | fsm_csr_read[4])
					turn_csr = 4;
				else if(fsm_csr_write[5] | fsm_csr_read[5])
					turn_csr = 5;
				else if(fsm_csr_write[6] | fsm_csr_read[6])
					turn_csr = 6;
				else if(fsm_csr_write[7] | fsm_csr_read[7])
					turn_csr = 7;
				else if(fsm_csr_write[8] | fsm_csr_read[8])
					turn_csr = 8;
				else if(fsm_csr_write[9] | fsm_csr_read[9])
					turn_csr = 9;
				else if(fsm_csr_write[10] | fsm_csr_read[10])
					turn_csr = 10;
				else if(fsm_csr_write[11] | fsm_csr_read[11])
					turn_csr = 11;
				else if(fsm_csr_write[12] | fsm_csr_read[12])
					turn_csr = 12;
				else if(fsm_csr_write[13] | fsm_csr_read[13])
					turn_csr = 13;
				else if(fsm_csr_write[14] | fsm_csr_read[14])
					turn_csr = 14;
				else if(fsm_csr_write[15] | fsm_csr_read[15])
					turn_csr = 15;
            else begin
					turn_csr = 16;
					state_csr = idle;
            end
        end
        waitforsignal:
            if(fsm_csr_waitrequest[turn_csr] == 0)
                state_csr = idle;
            else begin
                state_csr = waitforsignal;
            end
		  st_isr:
				if(~irq)
					state_csr = idle;
        default:
            state_csr = idle; 
        endcase  
    end
end

assign csr_address = state_csr == st_isr ? irq_csr_address :
							state_csr == waitforsignal ? fsm_csr_address[turn_csr] : 0;
							
assign csr_write = state_csr == st_isr ? irq_csr_write :
						 state_csr == waitforsignal ? fsm_csr_write[turn_csr] : 0;
						 
assign csr_writedata = state_csr == st_isr ? irq_csr_writedata :
							  state_csr == waitforsignal ? fsm_csr_writedata[turn_csr] : 0;
							  
assign csr_read = state_csr == st_isr ? irq_csr_read :
						state_csr == waitforsignal ? fsm_csr_read[turn_csr] : 0;

assign irq_csr_readdata = state_csr == st_isr ? irq_csr_readdata : 0;

/*this read data assignment is garbage. got to multiplex the output*/

/*assign csr_readdata = state_csr == st_isr ? irq_csr_readdata :
							 state_csr == waitforsignal ? fsm_csr_readdata[turn_csr] : 0;
*/
							 
assign fsm_csr_waitrequest = (state_csr == waitforsignal) ? 
										  ((csr_waitrequest<<turn_csr) | (~(16'b1<<turn_csr))) : 
										  16'hffff;
										  
assign irq_csr_waitrequest = state_csr == st_isr ? csr_waitrequest : 1;


/***********************************************=***
task and core control signals (for 16 each)
***************************************************/
reg											core_start [`CRC_KEY_SIZE-1 : 0];
reg											core_done [`CRC_KEY_SIZE-1 : 0];
reg											core_enable_fprints [`CRC_KEY_SIZE-1 : 0];
reg											core_logical_id [`CRC_KEY_SIZE-1 : 0];
reg [`CRC_KEY_WIDTH-1 : 0]				core_task_id [`CRC_KEY_SIZE-1 : 0];
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]	core_fprint_count [`CRC_KEY_SIZE-1 : 0];
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]	core_fprint_total_count [`CRC_KEY_SIZE-1 : 0];
time											core_fprint_spd [`CRC_KEY_SIZE-1 : 0];

reg											task_start [`CRC_KEY_SIZE-1 : 0];
reg											task_done [`CRC_KEY_SIZE-1 : 0];
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]	task_total_fprints [`CRC_KEY_SIZE-1 : 0];
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]	task_fprint_mem_maxcount [`CRC_KEY_SIZE-1 : 0];
reg [`CRC_KEY_WIDTH-1 : 0]				task_physical_core_id_0 [`CRC_KEY_SIZE-1 : 0];
reg [`CRC_KEY_WIDTH-1 : 0]				task_physical_core_id_1 [`CRC_KEY_SIZE-1 : 0];
time											task_core0_fprint_spd [`CRC_KEY_SIZE-1 : 0];
time											task_core1_fprint_spd [`CRC_KEY_SIZE-1 : 0];

/******************************
Set directory start pointer
*******************************/
task automatic set_directory_start;
input integer core;
input[31:0]  key;
input[31:0] offset;
begin
    #10
    fsm_csr_write[key] = 1;
    fsm_csr_writedata[key] = offset;
    fsm_csr_address[key] = `CRC_DIRECTORY_START_BASE + (core << 6) + key;
    @ (negedge fsm_csr_waitrequest[key]) #10;
    fsm_csr_write[key] = 0;
    fsm_csr_writedata[key] = 0;
    fsm_csr_address[key] = 0;
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
    fsm_csr_write[key] = 1;
    fsm_csr_writedata[key] = offset ;
    fsm_csr_address[key] = `CRC_DIRECTORY_END_BASE + (core << 6) + key;
    @ (negedge fsm_csr_waitrequest[key]) #10;
    fsm_csr_write[key] = 0;
    fsm_csr_writedata[key] = 0;
    fsm_csr_address[key] = 0;
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
    fsm_csr_write[key] = 1;
    fsm_csr_writedata[key] = core_id | (key << 4) ;
    fsm_csr_address[key] = (core << 6) + `COMPARATOR_CORE_ASSIGNMENT_OFFSET;
    @ (negedge fsm_csr_waitrequest[key]) #10;
    fsm_csr_write[key] = 0;
    fsm_csr_writedata[key] = 0;
    fsm_csr_address[key] = 0;
end
endtask

/******************************
Set maxcount value
*******************************/
task automatic set_maxcount_value;
input[3:0] task_id;
input[`CRC_RAM_ADDRESS_WIDTH-1 : 0] maxcount;
begin
#10
    fsm_csr_write[task_id] = 1;
    fsm_csr_writedata[task_id] = (task_id << `CRC_RAM_ADDRESS_WIDTH) + maxcount;
    fsm_csr_address[task_id] = `COMPARATOR_MAXCOUNT_REG_OFFSET;
    @ (negedge fsm_csr_waitrequest[task_id]) #10;
    fsm_csr_write[task_id] = 0;
    fsm_csr_writedata[task_id] = 0;
    fsm_csr_address[task_id] = 0;
end
endtask

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
	fsm_fprint_address[core_id] = (core_id << 4) + `COMPARATOR_CRC_OFFSET;
	fsm_fprint_writedata[core_id] = {fingerprint[15:0],12'b0,task_id[3:0]};
	fsm_fprint_write[core_id] = 1;
	@ (negedge fsm_fprint_waitrequest[core_id]) #10;
	fsm_fprint_writedata[core_id] = {fingerprint[31:16],12'b10,task_id[3:0]};
	@ (negedge fsm_fprint_waitrequest[core_id]) #10;
	fsm_fprint_write[core_id] =0 ;
	fsm_fprint_writedata[core_id] = 0;
	fsm_fprint_address[core_id] = 0;

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
	fsm_fprint_address[core_id] = (core_id << 4) + 0;
	fsm_fprint_writedata[core_id] = {1'b1,task_id[3:0]};
	fsm_fprint_write[core_id] = 1;
	@ (negedge fsm_fprint_waitrequest[core_id]) #10;
	fsm_fprint_write[core_id] =0 ;
	fsm_fprint_writedata[core_id] = 0;
	fsm_fprint_address[core_id] = 0;

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
	fsm_fprint_address[core_id] = (core_id << 4) + 0;
	fsm_fprint_writedata[core_id] = {1'b0,task_id[3:0]};
	fsm_fprint_write[core_id] = 1;
	@ (negedge fsm_fprint_waitrequest[core_id]) #10;
	fsm_fprint_write[core_id] =0 ;
	fsm_fprint_writedata[core_id] = 0;
	fsm_fprint_address[core_id] = 0;
	
end
endtask

/******************************
perform a task
*******************************/
task automatic perform_task;

input integer	task_id;
input integer	total_fprints;
input integer	fprint_mem_maxcount;
input integer	physical_core_id_0;
input integer	physical_core_id_1;
input time		core0_fprint_spd;
input time		core1_fprint_spd;

begin

	wait(task_done[task_id] == 1)
	
	task_done[task_id] = 0;
	
	task_total_fprints[task_id] = total_fprints;
	task_fprint_mem_maxcount[task_id] = fprint_mem_maxcount;
	task_physical_core_id_0[task_id] = physical_core_id_0;
	task_physical_core_id_1[task_id] = physical_core_id_1;
   task_core0_fprint_spd[task_id] = core0_fprint_spd;
   task_core1_fprint_spd[task_id] = core1_fprint_spd;

	task_start[task_id] = 1;
		
end
endtask

/*****************************************************************
Initial Conditions
******************************************************************/

integer i;

integer init_complete = 0;
initial begin
	clk = 0;
	
	reset = 1;
	
	oflow_waitrequest = 1;
	
	for(i=0 ; i<16 ; i = i+1) begin
		core_start[i] = 0;
		core_done[i] = 1;
		core_enable_fprints[i] = 1;
		core_logical_id[i] = 0;
		core_task_id[i] = 0;
		core_fprint_count[i] = 0;
		core_fprint_total_count[i] = 0;
		core_fprint_spd[i] = 0;
		
		task_start[i] = 0;		
		task_done[i] = 1;
		task_total_fprints[i] = 0;
		task_fprint_mem_maxcount[i] = 0;
		task_physical_core_id_0[i] = 0;
		task_physical_core_id_1[i] = 0;
		task_core0_fprint_spd[i] = 0;
		task_core1_fprint_spd[i] = 0;
	end
	
	#5 reset = 0;
	
	init_complete = 1;
end

/* clock */
always begin
	clk = #5 ~clk;
end

/* 16 individual process to handle 16 tasks*/
generate
	genvar task_num;
	for(task_num=0 ; task_num<16 ; task_num = task_num+1) begin
		always @(posedge task_start[task_num]) begin
			task_start[task_num] = 0;
			
			wait(core_done[task_physical_core_id_0[task_num]] & core_done[task_physical_core_id_1[task_num]])
			
			core_done[task_physical_core_id_0[task_num]] = 0;
			core_done[task_physical_core_id_1[task_num]] = 0;
			
			set_core_assignment(0,task_num,task_physical_core_id_0[task_num]);
			set_core_assignment(1,task_num,task_physical_core_id_1[task_num]);
		
			set_maxcount_value(task_num,task_fprint_mem_maxcount[task_num]);
			
			core_fprint_spd[task_physical_core_id_0[task_num]] = task_core0_fprint_spd[task_num];
			core_fprint_spd[task_physical_core_id_1[task_num]] = task_core1_fprint_spd[task_num];
			
			core_fprint_total_count[task_physical_core_id_0[task_num]] = task_total_fprints[task_num];
			core_fprint_total_count[task_physical_core_id_1[task_num]] = task_total_fprints[task_num];
			
			core_task_id[task_physical_core_id_0[task_num]] = task_num;
			core_task_id[task_physical_core_id_1[task_num]] = task_num;
			
			core_logical_id[task_physical_core_id_0[task_num]] = 0;
			core_logical_id[task_physical_core_id_1[task_num]] = 1;
		
			enable_task(task_physical_core_id_0[task_num],task_num);
			enable_task(task_physical_core_id_1[task_num],task_num);
			
			core_start[task_physical_core_id_0[task_num]] = 1;
			core_start[task_physical_core_id_1[task_num]] = 1;
			
			wait(core_done[task_physical_core_id_0[task_num]] & core_done[task_physical_core_id_1[task_num]])
			
			core_enable_fprints[task_physical_core_id_0[task_num]] = 1;
			core_enable_fprints[task_physical_core_id_1[task_num]] = 1;
			
			disable_task(task_physical_core_id_0[task_num],task_num);
			disable_task(task_physical_core_id_1[task_num],task_num);
				
			task_done[task_num] = 1;

			$display("task %d completes at time %d",task_num,$time);
		end
	end
endgenerate

/* 16 individual processes to handle 16 physical cores */
generate
	genvar physical_core;
	for(physical_core=0 ; physical_core<16 ; physical_core = physical_core+1) begin
		always @(posedge core_start[physical_core]) begin
			core_start[physical_core] = 0;
			for(core_fprint_count[physical_core] = 0; core_fprint_count[physical_core] < core_fprint_total_count[physical_core]; core_fprint_count[physical_core] = core_fprint_count[physical_core] + 1)begin
				#core_fprint_spd[physical_core];
				wait(core_enable_fprints[physical_core] == 1) 
					write_fingerprint(physical_core,core_task_id[physical_core],'hdeadbeef + core_fprint_count[physical_core]);
			end
			core_done[physical_core] = 1;
		end
	end
endgenerate


/* overflow interface control process */
always @(posedge oflow_write) begin
	if(oflow_writedata[`CRC_KEY_WIDTH+`CRC_KEY_WIDTH]) 
		core_enable_fprints[oflow_writedata[`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : `CRC_KEY_WIDTH]] = 0;
	else
		core_enable_fprints[oflow_writedata[`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : `CRC_KEY_WIDTH]] = 1;
	
	#10
	oflow_waitrequest = 0;
	#10
	oflow_waitrequest = 1;
end

/******************************
ISR
*******************************/

always @ (posedge irq)
begin
	//#200
	irq_csr_read = 1;
	irq_csr_address = `COMPARATOR_SUCCESS_REG_OFFSET;
	@ (negedge irq_csr_waitrequest) #10;
	
	irq_csr_address = `COMPARATOR_FAIL_REG_OFFSET;
	@ (negedge irq_csr_waitrequest) #10;
	
	irq_csr_read = 0;
	irq_csr_write = 1;
	irq_csr_writedata = 0;
	irq_csr_address = `COMPARATOR_EXCEPTION_OFFSET;
	@ (negedge irq_csr_waitrequest) #10;
	
	irq_csr_write = 0;
	irq_csr_writedata = 0;
	irq_csr_address = 0;
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

/******************************
main testing
*******************************/

always @(posedge init_complete)begin : test1

	/******************************************************************
	* DIRECTORY INFO																	*
	* write start and end pointer for all 16 tasks for both logical	*
	* cores. each task has directory address space of 10.					*
	* note: code ignores logical id. no bug, just why.						*
	******************************************************************/
	for(i = 0; i < 16; i = i + 1)begin
		set_directory_start(0,i,i*10);
		set_directory_end(0,i,(i+1)*10-1);
	end
	
	/*****************************************************************
	* perform_task() parameters:
	*
	* task id : 0-15
	* total fingerprints : integer
	* overflow count : integer < 9 (change directory for bigger values)
	* physical core 0 : 0-15
	* physical core 1 : 0-15, /= (physical core 0)
	* core 0 fprint speed : multiple of 10
	* core 1 fprint speed : multiple of 10
	******************************************************************/
	
	
	//perform_task(2 , 99 , 8 , 4  ,5  , 50   , 500 );
	
	perform_task(0 , 99 , 8 , 2  ,5  , 500   , 5000 );
	perform_task(1 , 13 , 3 , 0  ,3  , 6000  , 400  );
	perform_task(2 , 56 , 5 , 1  ,7  , 200   , 1500 );
	perform_task(3 , 20 , 4 , 8  ,9  , 600   , 7000 );
	perform_task(4 , 44 , 7 , 10 ,13 , 3000  , 300  );
	perform_task(5 , 21 , 3 , 11 ,15 , 400   , 4000 );
	perform_task(6 , 03 , 1 , 4  ,6  , 10000 , 500  );
	perform_task(7 , 91 , 7 , 12 ,14 , 200   , 2500 );
		 
end

endmodule
