`include "crc_defines.v"

`define BYTE 8
`define CLK_PERIOD 10

`define FPRINT_STARTER		'hdead0000
`define MAX_DIR_SIZE		`CRC_RAM_SIZE/16
`define DIR_SIZE			20
`define MAXCOUNT			10
`define FPRINT_SPEED		20
`define FPRINT_NUM			1000
`define FPRINT_SKIP_NUM		500
`define FPRINT_SKIP_OFF		-1
`define NUM_RUNS			3
`define FAIL_RUN			-1
`define NUM_TASKS			2

module tb_cfpu();

reg 										clk;
reg 										reset;

/******************************
cfpu interface signals
*******************************/
wire [9:0]									csr_address;
wire										csr_read;
wire [31:0]									csr_readdata;
wire										csr_write;
wire [31:0]									csr_writedata;
wire										csr_waitrequest;

wire [(`COMPARATOR_ADDRESS_WIDTH-1):0]		fprint_address;
wire										fprint_write;
wire [(`NIOS_DATA_WIDTH-1):0]				fprint_writedata;
wire										fprint_waitrequest;

wire										oflow_write;
wire [`NIOS_DATA_WIDTH-1 : 0]				oflow_writedata;
wire [`NIOS_ADDRESS_WIDTH-1 : 0]			oflow_address;
reg											oflow_waitrequest;

wire										irq;


/******************************
fprint and csr bus arbitration
*******************************/

reg											fsm_fprint_write [`CRC_KEY_SIZE-1 : 0];
reg [(`NIOS_DATA_WIDTH-1):0]				fsm_fprint_writedata [`CRC_KEY_SIZE-1 : 0];
reg [(`COMPARATOR_ADDRESS_WIDTH-1):0]		fsm_fprint_address [`CRC_KEY_SIZE-1 : 0];
wire [`CRC_KEY_SIZE-1 : 0]					fsm_fprint_waitrequest;


reg											fsm_csr_write [`CRC_KEY_SIZE-1 : 0];
reg [(`NIOS_DATA_WIDTH-1):0]				fsm_csr_writedata [`CRC_KEY_SIZE-1 : 0];
reg											fsm_csr_read [`CRC_KEY_SIZE-1 : 0];
reg [(`COMPARATOR_ADDRESS_WIDTH-1):0]		fsm_csr_address [`CRC_KEY_SIZE-1 : 0];
wire [`CRC_KEY_SIZE-1 : 0]					fsm_csr_waitrequest;

reg											irq_csr_write;
reg [(`NIOS_DATA_WIDTH-1):0]				irq_csr_writedata;
reg											irq_csr_read;
reg [(`COMPARATOR_ADDRESS_WIDTH-1):0]		irq_csr_address;
wire [`CRC_KEY_SIZE-1 : 0]					irq_csr_waitrequest;


cfpu cfpu_block(
	clk,
	reset,
	
	csr_address,
	csr_read,
	csr_readdata,
	csr_write,
	csr_writedata,
	csr_waitrequest,
	
	fprint_address,
	fprint_write,
	fprint_writedata,
	fprint_waitrequest,

	oflow_write,
	oflow_writedata,
	oflow_address,
	oflow_waitrequest,
	
	irq
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
							 
assign fsm_csr_waitrequest = (state_csr == waitforsignal) ? 
										  ((csr_waitrequest<<turn_csr) | (~(16'b1<<turn_csr))) : 
										  16'hffff;
										  
assign irq_csr_waitrequest = state_csr == st_isr ? csr_waitrequest : 1;


/***********************************************=***
task and core control signals (for 16 each)
***************************************************/
reg [`CRC_KEY_SIZE-1 : 0]				core_start;
reg [`CRC_KEY_SIZE-1 : 0]				core_done;
reg [`CRC_KEY_SIZE-1 : 0]				core_busy;
reg [`CRC_KEY_SIZE-1 : 0]				core_enable_fprints;
reg [1:0]								core_logical_id [`CRC_KEY_SIZE-1 : 0];
reg [`CRC_KEY_WIDTH-1 : 0]				core_task_id [`CRC_KEY_SIZE-1 : 0];
reg [31:0]								core_fprint_count [`CRC_KEY_SIZE-1 : 0];
reg [31:0]								core_fprint_total_count [`CRC_KEY_SIZE-1 : 0];
reg [31:0]								core_fprint_skip_num [`CRC_KEY_SIZE-1 : 0];
time [`CRC_KEY_SIZE-1 : 0]				core_fprint_spd;

reg [`CRC_KEY_SIZE-1 : 0]				task_start;
reg [`CRC_KEY_SIZE-1 : 0]				task_done;
reg [`CRC_KEY_SIZE-1 : 0]				task_busy;
reg [`CRC_KEY_SIZE-1 : 0]				task_nmr;
reg [31:0]								task_total_fprints [`CRC_KEY_SIZE-1 : 0];
reg [31:0]								task_fprint_mem_maxcount [`CRC_KEY_SIZE-1 : 0];
reg [31:0]								task_core0_fprint_skip_num [`CRC_KEY_SIZE-1 : 0];
reg [31:0]								task_core1_fprint_skip_num [`CRC_KEY_SIZE-1 : 0];
reg [31:0]								task_core2_fprint_skip_num [`CRC_KEY_SIZE-1 : 0];
reg [`CRC_KEY_WIDTH-1 : 0]				task_physical_core_id_0 [`CRC_KEY_SIZE-1 : 0];
reg [`CRC_KEY_WIDTH-1 : 0]				task_physical_core_id_1 [`CRC_KEY_SIZE-1 : 0];
reg [`CRC_KEY_WIDTH-1 : 0]				task_physical_core_id_2 [`CRC_KEY_SIZE-1 : 0];
reg [31:0]								task_success_count [`CRC_KEY_SIZE-1 : 0];
reg	[31:0]								task_fail_count [`CRC_KEY_SIZE-1 : 0];
time [`CRC_KEY_SIZE-1 : 0]				task_core0_fprint_spd;
time [`CRC_KEY_SIZE-1 : 0]				task_core1_fprint_spd;
time [`CRC_KEY_SIZE-1 : 0]				task_core2_fprint_spd;

reg										testing;
reg										run;
reg [31:0]								num_run;
reg [31:0]								irq_count;

/******************************
Set directory start pointer
*******************************/
task automatic set_directory_start;
input integer task_id;
input integer logical_id;
input integer pointer_start;

begin
    #10
    fsm_csr_write[task_id]		=	1;
	fsm_csr_address[task_id]	=	`CRC_DIRECTORY_START_BASE;
    fsm_csr_writedata[task_id]	=	logical_id << 3*`BYTE | 
									task_id << 2*`BYTE |
									pointer_start;
    @ (negedge fsm_csr_waitrequest[task_id]) #10;
    fsm_csr_write[task_id]		=	0;
    fsm_csr_address[task_id]	=	0;
	fsm_csr_writedata[task_id]	=	0;
end
endtask

/******************************
Set directory end pointer
*******************************/
task automatic set_directory_end;
input integer task_id;
input integer logical_id;
input integer pointer_end;
begin
#10
    fsm_csr_write[task_id]		=	1;
    fsm_csr_address[task_id]	=	`CRC_DIRECTORY_END_BASE;
    fsm_csr_writedata[task_id]	=	logical_id << 3*`BYTE |
									task_id << 2*`BYTE |
									pointer_end;
    @ (negedge fsm_csr_waitrequest[task_id]) #10;
    fsm_csr_write[task_id]		=	0;
    fsm_csr_address[task_id]	=	0;
    fsm_csr_writedata[task_id]	=	0;
end
endtask

/******************************
Set core assignment
*******************************/
task automatic set_core_assignment;
input integer task_id;
input integer logical_id;
input integer physical_id;
begin
#10
    fsm_csr_write[task_id]		=	1;
    fsm_csr_address[task_id]	=	`COMPARATOR_CORE_ASSIGNMENT_OFFSET;
    fsm_csr_writedata[task_id]	=	logical_id << 3*`BYTE |
									task_id << 2*`BYTE | 
									physical_id;

    @ (negedge fsm_csr_waitrequest[task_id]) #10;
    fsm_csr_write[task_id]		=	0;
    fsm_csr_address[task_id]	=	0;
    fsm_csr_writedata[task_id]	=	0;
end
endtask

/******************************
Set maxcount value
*******************************/
task automatic set_maxcount_value;
input integer task_id;
input integer logical_id;
input integer maxcount;
begin
#10
    fsm_csr_write[task_id]		=	1;
    fsm_csr_address[task_id]	=	`COMPARATOR_MAXCOUNT_REG_OFFSET;
    fsm_csr_writedata[task_id]	=	logical_id << 3*`BYTE |
									task_id << 2*`BYTE | 
									maxcount;
    @ (negedge fsm_csr_waitrequest[task_id]) #10;
    fsm_csr_write[task_id]		=	0;
    fsm_csr_address[task_id]	=	0;
    fsm_csr_writedata[task_id]	=	0;

end
endtask

/******************************
Set nmr
*******************************/
task automatic set_nmr;
input integer task_id;
input integer nmr;
begin

    fsm_csr_write[task_id]		=	1;
    fsm_csr_address[task_id] 	=	`COMPARATOR_NMR_OFFSET;
    fsm_csr_writedata[task_id]	=	task_id << 2*`BYTE |
									nmr;
    @ (negedge fsm_csr_waitrequest[task_id]) #10;
    fsm_csr_write[task_id]		=	0;
    fsm_csr_address[task_id]	=	0;
    fsm_csr_writedata[task_id]	=	0;


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
	//$display("time: %d, task enabled: %d",$time,task_id);
	
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
Disable a task
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
Reset a task
*******************************/
task automatic reset_task;
input integer task_id;
begin
	core_enable_fprints[task_physical_core_id_0[task_id]] = 1;
	core_enable_fprints[task_physical_core_id_1[task_id]] = 1;
end
endtask

/******************************
perform a task
*******************************/
task automatic perform_task;

input integer	task_id;
input integer	nmr;
input integer	total_fprints;
input integer	fprint_mem_maxcount;
input integer	physical_core_id_0;
input integer	physical_core_id_1;
input integer	physical_core_id_2;
input integer	core0_fprint_skip_num;
input integer	core1_fprint_skip_num;
input integer	core2_fprint_skip_num;
input time		core0_fprint_spd;
input time		core1_fprint_spd;
input time		core2_fprint_spd;


begin

	wait(task_done[task_id] & core_done[physical_core_id_0] & core_done[physical_core_id_1] & (core_done[physical_core_id_2] | ~ nmr))

	task_done[task_id] = 0;

	task_nmr[task_id] = nmr;	
	task_total_fprints[task_id] = total_fprints;
	task_fprint_mem_maxcount[task_id] = fprint_mem_maxcount;
	task_physical_core_id_0[task_id] = physical_core_id_0;
	task_physical_core_id_1[task_id] = physical_core_id_1;
	task_physical_core_id_2[task_id] = physical_core_id_2;
	task_core0_fprint_skip_num[task_id] = core0_fprint_skip_num;
	task_core1_fprint_skip_num[task_id] = core1_fprint_skip_num;
	task_core2_fprint_skip_num[task_id] = core2_fprint_skip_num;
	task_core0_fprint_spd[task_id] = core0_fprint_spd;
	task_core1_fprint_spd[task_id] = core1_fprint_spd;
	task_core2_fprint_spd[task_id] = core2_fprint_spd;
	
	task_start[task_id] = 1;
		
end
endtask

/*****************************************************************
Initial Conditions
******************************************************************/

integer i,j;

integer init_complete = 0;
initial begin
	clk = 0;
	
	reset = 1;
	
	oflow_waitrequest = 1;
	
	for(i=0 ; i<16 ; i = i+1) begin
		core_start[i] = 0;
		core_done[i] = 1;
		core_busy[i] = 0;
		core_enable_fprints[i] = 1;
		core_logical_id[i] = 0;
		core_task_id[i] = 0;
		core_fprint_count[i] = 0;
		core_fprint_total_count[i] = 0;
		core_fprint_skip_num[i] = 0;
		core_fprint_spd[i] = 0;
		
		task_start[i] = 0;		
		task_done[i] = 1;
		task_busy[i] = 0;
		task_nmr = 0;
		task_total_fprints[i] = 0;
		task_fprint_mem_maxcount[i] = 0;
		task_core0_fprint_skip_num[i] = 0;
		task_core1_fprint_skip_num[i] = 0;
		task_core2_fprint_skip_num[i] = 0;
		task_physical_core_id_0[i] = 0;
		task_physical_core_id_1[i] = 0;
		task_physical_core_id_2[i] = 0;
		task_success_count[i] = 0;
		task_fail_count[i] = 0;
		task_core0_fprint_spd[i] = 0;
		task_core1_fprint_spd[i] = 0;
		task_core2_fprint_spd[i] = 0;
	end
	
	#5 reset = 0;
	
	/********************************************************************
	* DIRECTORY INFO													
	* write start and end pointer for all 16 tasks, all logical cores						
	********************************************************************/
	if(`DIR_SIZE > `MAX_DIR_SIZE) begin
		for(i = 0; i < 16; i = i + 1)begin
			for(j = 0; j < 3; j = j + 1)begin
				set_directory_start(i, j, i*`MAX_DIR_SIZE);
				set_directory_end(i, j, (i+1)*`MAX_DIR_SIZE-1);
			end
		end
	end else begin
		for(i = 0; i < 16; i = i + 1)begin
			for(j = 0; j < 3; j = j + 1)begin
				set_directory_start(i, j, i*`DIR_SIZE);
				set_directory_end(i, j, (i+1)*`DIR_SIZE-1);
			end
		end
	end

	/********************************************************************
	* NMR SETUP												
	* default DMR
	********************************************************************/
	for(i = 0; i < 16; i = i + 1)begin
		set_nmr(i,0);
	end

	irq_count = 0;
	num_run = 0;
	run = 0;
	testing = 1;

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
			task_busy[task_num] = 1;
			//$display("starting up task %d at time %d",task_num,$time);
			
			set_nmr(task_num, task_nmr[task_num]);

			core_done[task_physical_core_id_0[task_num]] = 0;
			core_done[task_physical_core_id_1[task_num]] = 0;
			if(task_nmr[task_num])
				core_done[task_physical_core_id_2[task_num]] = 0;
			
			set_core_assignment(task_num, 0, task_physical_core_id_0[task_num]);
			set_core_assignment(task_num, 1, task_physical_core_id_1[task_num]);
			if(task_nmr[task_num])
				set_core_assignment(task_num, 2, task_physical_core_id_2[task_num]);

			set_maxcount_value(task_num, 0, task_fprint_mem_maxcount[task_num]);
			set_maxcount_value(task_num, 1, task_fprint_mem_maxcount[task_num]);
			if(task_nmr[task_num])
				set_maxcount_value(task_num, 2, task_fprint_mem_maxcount[task_num]);
			
			core_fprint_spd[task_physical_core_id_0[task_num]] = task_core0_fprint_spd[task_num];
			core_fprint_spd[task_physical_core_id_1[task_num]] = task_core1_fprint_spd[task_num];
			if(task_nmr[task_num])
				core_fprint_spd[task_physical_core_id_2[task_num]] = task_core2_fprint_spd[task_num];
			
			core_fprint_total_count[task_physical_core_id_0[task_num]] = task_total_fprints[task_num];
			core_fprint_total_count[task_physical_core_id_1[task_num]] = task_total_fprints[task_num];
			if(task_nmr[task_num])
				core_fprint_total_count[task_physical_core_id_2[task_num]] = task_total_fprints[task_num];
			
			core_fprint_skip_num[task_physical_core_id_0[task_num]] = task_core0_fprint_skip_num[task_num];
			core_fprint_skip_num[task_physical_core_id_1[task_num]] = task_core1_fprint_skip_num[task_num];
			if(task_nmr[task_num])
				core_fprint_skip_num[task_physical_core_id_2[task_num]] = task_core2_fprint_skip_num[task_num];
			
			core_task_id[task_physical_core_id_0[task_num]] = task_num;
			core_task_id[task_physical_core_id_1[task_num]] = task_num;
			if(task_nmr[task_num])
				core_task_id[task_physical_core_id_2[task_num]] = task_num;
			
			core_logical_id[task_physical_core_id_0[task_num]] = 0;
			core_logical_id[task_physical_core_id_1[task_num]] = 1;
			if(task_nmr[task_num])
				core_logical_id[task_physical_core_id_2[task_num]] = 2;
		
			enable_task(task_physical_core_id_0[task_num],task_num);
			enable_task(task_physical_core_id_1[task_num],task_num);
			if(task_nmr[task_num])
				enable_task(task_physical_core_id_2[task_num],task_num);
			
			core_start[task_physical_core_id_0[task_num]] = 1;
			core_start[task_physical_core_id_1[task_num]] = 1;
			if(task_nmr[task_num])
				core_start[task_physical_core_id_2[task_num]] = 1;
			
			//$display("task %d started sending at time %d",task_num,$time);
			
			wait(core_done[task_physical_core_id_0[task_num]] & core_done[task_physical_core_id_1[task_num]] & (core_done[task_physical_core_id_2[task_num]] | ~task_nmr[task_num]))
				
			disable_task(task_physical_core_id_0[task_num],task_num);
			disable_task(task_physical_core_id_1[task_num],task_num);
			if(task_nmr[task_num])
				disable_task(task_physical_core_id_2[task_num],task_num);
			
			//$display("task %d finished sending at time %d",task_num,$time);
				
			task_done[task_num] = 1;
			task_busy[task_num] = 0;			
		end	
	end
endgenerate

/* 16 individual processes to handle 16 physical cores */
generate
	genvar physical_core;
	for(physical_core=0 ; physical_core<16 ; physical_core = physical_core+1) begin
		always @(posedge core_start[physical_core]) begin
			core_start[physical_core] = 0;
			core_busy[physical_core] = 1;
			
			for(core_fprint_count[physical_core] = 0; core_fprint_count[physical_core] < core_fprint_total_count[physical_core]; core_fprint_count[physical_core] = core_fprint_count[physical_core] + 1) begin
				
				// skip a particular fingerprint to simulate failed tasks
				if(core_fprint_count[physical_core] == core_fprint_skip_num[physical_core])
					core_fprint_count[physical_core] = core_fprint_count[physical_core] + 1;
				
				// wait for set time to simulate cores with different speeds
				#core_fprint_spd[physical_core];
				
				// wait until the core has underflowed
				wait(core_enable_fprints[physical_core] == 1) 
				
				// write the fingerprint
				write_fingerprint(physical_core,core_task_id[physical_core],`FPRINT_STARTER + core_fprint_count[physical_core]);

			end
			
			core_done[physical_core] = 1;
			core_busy[physical_core] = 0;
		end
	end
endgenerate


/* overflow interface control process */
always @(posedge oflow_write) begin
	#10
	//$display("Oflow write, time %d", $time);
	if(oflow_address == (`OFLOW_OFFSET+{oflow_writedata[`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : `CRC_KEY_WIDTH],20'h0})) begin
		if(oflow_writedata[`CRC_KEY_WIDTH+`CRC_KEY_WIDTH]) begin
			core_enable_fprints[oflow_writedata[`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : `CRC_KEY_WIDTH]] = 0;
			//$display("Overflowing core %d, time %d", oflow_writedata[`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : `CRC_KEY_WIDTH], $time);
		end else begin
			core_enable_fprints[oflow_writedata[`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : `CRC_KEY_WIDTH]] = 1;
			//$display("Underflowing core %d, time %d", oflow_writedata[`CRC_KEY_WIDTH+`CRC_KEY_WIDTH-1 : `CRC_KEY_WIDTH], $time);
		end
	
		#10
		oflow_waitrequest = 0;
		#10
		oflow_waitrequest = 1;
	end
end

/******************************
ISR
*******************************/

always @ (posedge irq)
begin
	//#200
	irq_csr_read = 1;
	irq_csr_address = `COMPARATOR_SUCCESS_REG_OFFSET;
	@ (negedge irq_csr_waitrequest) 
		for(i=0 ; i<16 ; i++) begin
			if(csr_readdata & 1<<i) begin
				task_success_count[i] = task_success_count[i] + 1;
				$display("task %d successful %d times",i,task_success_count[i]);
				//assert(cfpu_block.oflow_registers.physical_id_table[i] == 0) 
					//$display("task %d successful %d times",i,task_success_count[i]);
				//else
					//$error("task %d successful but not underflowed",i);
			end
		end
	#10;
	
	irq_csr_address = `COMPARATOR_FAIL_REG_OFFSET;
	@ (negedge irq_csr_waitrequest) 
		for(i=0 ; i<16 ; i++) begin
			assert ((csr_readdata & 3<<(i*2)) == 3<<(i*2)) else begin
				$display("task %d failed, time %d",i,$time);
				reset_task(i);
			end
		end
	#10;
	
	irq_csr_read = 0;
	irq_csr_write = 1;
	irq_csr_writedata = 0;
	irq_csr_address = `COMPARATOR_EXCEPTION_OFFSET;
	@ (negedge irq_csr_waitrequest) #10;
	
	irq_csr_write = 0;
	irq_csr_writedata = 0;
	irq_csr_address = 0;

	irq_count = irq_count+1;
end


/******************************
main testing
*******************************/

always @(posedge run)begin : test1
	
	run = 0;
	
	/************************************************************************
	* perform_task() parameters:
	*
	* task id : 0-15
	* nmr: 0 = DMR, 1 = TMR
	* total fingerprints : integer
	* overflow count : integer < 9 (change directory for bigger values)
	* physical core 0 : 0-15
	* physical core 1 : 0-15
	* physical core 2 : 0-15
	* core 0 fprint skip num : integer < total fprints, = -1 to deactivate
	* core 1 fprint skip num : integer < total fprints, = -1 to deactivate
	* core 2 fprint skip num : integer < total fprints, = -1 to deactivate
	* core 0 fprint speed : multiple of 10
	* core 1 fprint speed : multiple of 10
	* core 2 fprint speed : multiple of 10
	*************************************************************************/
	
	// DMR TEST CONFIG (at the top)
	//`define NUM_RUNS			3
	//`define FAIL_RUN			2
	//`define NUM_TASKS			4
	
	/*
	if(num_run == `FAIL_RUN-1) begin
		perform_task( 2 , 0, `FPRINT_NUM , `MAXCOUNT , 0 , 1 , 0, `FPRINT_SKIP_NUM, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SPEED, `FPRINT_SPEED, `FPRINT_SPEED );
		perform_task( 3 , 0, `FPRINT_NUM , `MAXCOUNT , 2 , 3 , 0, `FPRINT_SKIP_OFF, `FPRINT_SKIP_NUM, `FPRINT_SKIP_OFF, `FPRINT_SPEED, `FPRINT_SPEED, `FPRINT_SPEED );
		perform_task( 4 , 0, `FPRINT_NUM , `MAXCOUNT , 4 , 5 , 0, `FPRINT_SKIP_NUM, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SPEED, `FPRINT_SPEED, `FPRINT_SPEED );
		perform_task( 5 , 0, `FPRINT_NUM , `MAXCOUNT , 6 , 7 , 0, `FPRINT_SKIP_OFF, `FPRINT_SKIP_NUM, `FPRINT_SKIP_OFF, `FPRINT_SPEED, `FPRINT_SPEED, `FPRINT_SPEED );
	end else begin
		perform_task( 2 , 0, `FPRINT_NUM , `MAXCOUNT , 0 , 1 , 0, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SPEED, `FPRINT_SPEED, `FPRINT_SPEED );
		perform_task( 3 , 0, `FPRINT_NUM , `MAXCOUNT , 2 , 3 , 0, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SPEED, `FPRINT_SPEED, `FPRINT_SPEED );
		perform_task( 4 , 0, `FPRINT_NUM , `MAXCOUNT , 4 , 5 , 0, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SPEED, `FPRINT_SPEED, `FPRINT_SPEED );
		perform_task( 5 , 0, `FPRINT_NUM , `MAXCOUNT , 6 , 7 , 0, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SPEED, `FPRINT_SPEED, `FPRINT_SPEED );
	end
	*/

	// TMR TEST CONFIG (at the top)
	//`define NUM_RUNS			3
	//`define FAIL_RUN			10
	//`define NUM_TASKS			2
	
	///*	
	if(num_run == `FAIL_RUN-1) begin
		perform_task( 2 , 1, `FPRINT_NUM , `MAXCOUNT , 0 , 1 , 2, `FPRINT_SKIP_NUM, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SPEED, `FPRINT_SPEED, `FPRINT_SPEED );
		perform_task( 3 , 0, `FPRINT_NUM , `MAXCOUNT , 3 , 4 , 5, `FPRINT_SKIP_OFF, `FPRINT_SKIP_NUM, `FPRINT_SKIP_OFF, `FPRINT_SPEED, `FPRINT_SPEED, `FPRINT_SPEED );
	end else begin
		perform_task( 2 , 1, `FPRINT_NUM , `MAXCOUNT , 0 , 1 , 2, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SPEED, `FPRINT_SPEED, `FPRINT_SPEED );
		perform_task( 3 , 0, `FPRINT_NUM , `MAXCOUNT , 3 , 4 , 5, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SKIP_OFF, `FPRINT_SPEED, `FPRINT_SPEED, `FPRINT_SPEED );

	end
	//*/

end


always@(posedge clk) begin 
	
	wait(init_complete == 1);

	if(num_run == `NUM_RUNS) begin
		$display("Finishing");
		$stop;
	end
	
	wait((task_busy == 0) & (core_busy == 0));
	
	run = 1;
	wait(irq_count == `NUM_TASKS);
	irq_count = 0;
	
	num_run = num_run + 1;
end

endmodule
