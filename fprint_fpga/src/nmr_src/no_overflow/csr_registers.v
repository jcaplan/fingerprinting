`include "crc_defines.v"
module csr_registers(

	//From processor
	input									clk,
	input									reset,	
	input 	[(`COMP_CSR_WIDTH-1):0]			csr_address,
	input									csr_read,
	output [(`NIOS_DATA_WIDTH-1):0]			csr_readdata,
	input									csr_write,
	input 		[(`NIOS_DATA_WIDTH-1):0]	csr_writedata,
	output 									csr_waitrequest,
	
	//From comparator
	input 									comp_status_write,
	output 									comp_status_ack,
	input [(`CRC_KEY_WIDTH-1):0]			comp_task,
	input 		 							comp_collision_detected,

	//To fprint registers
	input[3:0] 								physical_core_id,
	input[3:0]								fprint_task_id,
	output 		 							logical_core_id,

	//To comparator registers

	output [`CRC_RAM_ADDRESS_WIDTH-1:0]     head_tail_data,
	output [(`CRC_KEY_WIDTH-1):0]           head_tail_offset,
	output 									set_head_tail,
	input 									head_tail_ack,
	output reg[`CRC_RAM_ADDRESS_WIDTH-1:0]	start_pointer_ex,
	output reg[`CRC_RAM_ADDRESS_WIDTH-1:0]  end_pointer_ex,
	output reg [`CRC_RAM_ADDRESS_WIDTH-1:0]	start_pointer_comp,
	output reg [`CRC_RAM_ADDRESS_WIDTH-1:0] end_pointer_comp,
	output 									irq


);

reg [(`COMPARATOR_EXCEPTION_REG_WIDTH-1):0] 	exception_reg;
reg [(`CRC_KEY_SIZE-1):0] 						success_reg;
reg [(`CRC_KEY_SIZE-1):0] 						fail_reg;
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 				start_pointer_mem  	[(`CRC_KEY_SIZE-1):0];				//16 registers storing 10 bit pointers (RAM address size = 512), 1 per task 
reg [`CRC_RAM_ADDRESS_WIDTH-1:0] 				end_pointer_mem		[(`CRC_KEY_SIZE-1):0];
wire 											csr_write_reg;

reg  [(`CRC_KEY_WIDTH-1):0]               		core_assignment_reg	[1:0][(`CRC_KEY_SIZE-1):0]; 

/*************************************************************
* Decode
**************************************************************/
wire 									exception_reg_sel;
wire 									success_reg_sel;
wire 									fail_reg_sel;
wire 									start_p_sel;
wire 									end_p_sel;
wire [(`CRC_KEY_WIDTH-1):0] 			dir_pointer_offset;
wire [1:0]                              core_id;
wire [3:0] 								core_assignment_offset;
wire [3:0] 								core_assignment_data;
assign exception_reg_sel  		= (csr_address == `COMPARATOR_EXCEPTION_OFFSET);
assign success_reg_sel    		= (csr_address == `COMPARATOR_SUCCESS_REG_OFFSET);
assign fail_reg_sel       		= (csr_address == `COMPARATOR_FAIL_REG_OFFSET);
assign start_p_sel   			= (csr_address[`DIRECTORY_BITS] == 4'h1);   
assign end_p_sel     			= (csr_address[`DIRECTORY_BITS] == 4'h2);  
assign core_assignment_sel 		= (csr_address[5:0] == `COMPARATOR_CORE_ASSIGNMENT_OFFSET);
assign dir_pointer_offset 		= csr_address[(`CRC_KEY_WIDTH-1):0];
assign core_id 					= csr_address[7:6];
assign core_assignment_offset 	= csr_writedata[7:4];
assign core_assignment_data     = csr_writedata[3:0];
assign irq 						= exception_reg[`EXCEPTION_REG_INTERRUPT_BIT];

/*************************************************************
* FSM
**************************************************************/
reg[3:0] state;

parameter idle 				= 0;
parameter st_csr_write 		= 1;
parameter st_csr_read  		= 2;
parameter st_waitrequest 	= 3;
parameter st_comp_write     = 4;
parameter st_comp_ack       = 5;
parameter st_set_head_tail  = 6;
always @(posedge clk or posedge reset)
begin
	if(reset)
		state = idle;
	else begin
		case(state)
			idle:
				if(csr_write)
					state = st_csr_write;
				else if(csr_read)
					state = st_csr_read;
				else if(comp_status_write & ~irq)
					state = st_comp_write;
			st_csr_write:
				//After writing the start or end pointer
				//Write the head and tail pointers:
				if(start_p_sel)
					state = st_set_head_tail;
				else
					state = st_waitrequest;
			st_csr_read:
				state = st_waitrequest;
			st_waitrequest:
				state = idle;
			st_comp_write:
				state = st_comp_ack;
			st_comp_ack:
				state = idle;
			st_set_head_tail:
				if(head_tail_ack)
					state = st_waitrequest;
		endcase
	end
end

/*************************************************************
* FSM outputs
**************************************************************/
assign csr_waitrequest 	= ~(state == st_waitrequest); //asserts low
assign csr_write_reg 	= (state == st_csr_write);
assign csr_readdata 	= exception_reg_sel ? exception_reg : success_reg_sel ? success_reg :
							fail_reg_sel ? fail_reg : 0; 
assign comp_reg_write 	= (state == st_comp_write);
assign comp_status_ack  = (state == st_comp_ack);
assign set_head_tail    = (state == st_set_head_tail);
assign head_tail_offset = dir_pointer_offset;
assign head_tail_data   = csr_writedata[`CRC_RAM_ADDRESS_WIDTH-1:0];
/*************************************************************
* Registers
**************************************************************/
always @(posedge clk or posedge reset)
begin
	if(reset)begin
		exception_reg = 0;
		success_reg = 0;
		fail_reg = 0;
	end else begin
		if(csr_write_reg & exception_reg_sel)begin
			success_reg = 0;
			fail_reg = 0;
			exception_reg = csr_writedata;
		end else if(comp_reg_write)begin
			exception_reg[`EXCEPTION_REG_INTERRUPT_BIT] = 1;
			exception_reg[`EXCEPTION_REG_TASK_BITS] = comp_task;
			exception_reg[`EXCEPTION_REG_EX_BIT] = comp_collision_detected;
			success_reg[comp_task] = ~comp_collision_detected;
			fail_reg[comp_task]    = comp_collision_detected;
		end
	end
end

/*************************************************************
* Start and End pointers
**************************************************************/
always @ (posedge clk)
begin
	if(csr_write_reg & start_p_sel)
		start_pointer_mem[dir_pointer_offset] = csr_writedata[`CRC_RAM_ADDRESS_WIDTH-1:0];
	//Dual port read...
	start_pointer_ex = start_pointer_mem[fprint_task_id];
	start_pointer_comp = start_pointer_mem[comp_task];
	
end						

always @ (posedge clk)
begin
	if(csr_write_reg & end_p_sel)
		end_pointer_mem[dir_pointer_offset] = csr_writedata[`CRC_RAM_ADDRESS_WIDTH-1:0];

	//Dual port read...
	end_pointer_ex = end_pointer_mem[fprint_task_id];
	end_pointer_comp = end_pointer_mem[comp_task];	
end	

/*************************************************************
* Core assignment table
**************************************************************/

always @ (posedge clk)
begin
	if(csr_write_reg & core_assignment_sel)begin
			core_assignment_reg[core_id][core_assignment_offset] = core_assignment_data[3:0];
	end
end

assign logical_core_id = (core_assignment_reg[1][fprint_task_id] == physical_core_id) ? 1 : 0;

endmodule
