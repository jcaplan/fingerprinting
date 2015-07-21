`include "crc_defines.v"
module csr_registers(

	input										clk,
	input										reset,	
	
	//processor
	input [(`COMP_CSR_WIDTH-1):0]				csr_address,
	input										csr_read,
	output reg [(`NIOS_DATA_WIDTH-1):0]			csr_readdata,
	input										csr_write,
	input [(`NIOS_DATA_WIDTH-1):0]				csr_writedata,
	output 										csr_waitrequest,

	//comparator	
	input [(`CRC_KEY_WIDTH-1):0]				comparator_task_id,
	input [1:0]									comparator_logical_core_id,
	
	input 										comparator_status_write,
	input 		 								comparator_mismatch_detected,
	output 										csr_status_ack,
	
	output										comparator_nmr,

	//fprint_registers	
	input [(`CRC_KEY_WIDTH-1):0]				fprint_task_id,
	input [(`CRC_KEY_WIDTH-1):0]				fprint_physical_core_id,
	output [1:0]								fprint_logical_core_id,
	
	output [(`CRC_KEY_SIZE-1):0]				fprint_nmr,

	//oflow_registers
    input [(`CRC_KEY_WIDTH-1):0]				oflow_task_id,
	input [1:0]									oflow_logical_core_id,
	output [(`CRC_KEY_WIDTH-1):0]				oflow_physical_core_id,
	
	output [`CRC_RAM_ADDRESS_WIDTH-1 : 0]		csr_fprint_maxcount,
	output										oflow_nmr,

	//comp_registers
	output wire [(`CRC_KEY_WIDTH-1):0]			csr_task_id,	
	output wire [1:0]							csr_logical_core_id,

	output										csr_pointer_start_write,
	output										csr_pointer_end_write,
	output [`CRC_RAM_ADDRESS_WIDTH-1:0]     	csr_pointer_data,
	input										comp_pointer_ack,
	
	//irq
	output 										irq
);

/*************************************************************
* Internal registers
**************************************************************/
reg [(`COMPARATOR_EXCEPTION_REG_WIDTH-1):0] 	exception_reg;
reg [(`CRC_KEY_SIZE-1):0] 						success_reg;
reg [1:0]				 						fail_reg[(`CRC_KEY_SIZE-1):0];

reg [`CRC_KEY_WIDTH-1:0]						cat_mem_0[`CRC_KEY_SIZE-1:0];
reg [`CRC_KEY_WIDTH-1:0]						cat_mem_1[`CRC_KEY_SIZE-1:0];
reg [`CRC_KEY_WIDTH-1:0]						cat_mem_2[`CRC_KEY_SIZE-1:0];
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]				fprint_maxcount_mem	[(`CRC_KEY_SIZE-1):0][2:0];
reg [(`CRC_KEY_SIZE-1):0]						nmr_mem;


wire											csr_cat_write;
wire											csr_maxcount_write;
wire											csr_nmr_write;

wire [`CRC_RAM_ADDRESS_WIDTH-1:0]				csr_maxcount_data;
wire											csr_nmr_data;


wire [(`CRC_KEY_WIDTH-1):0]						csr_physical_core_id;
	
wire comparator_regs_write;


integer i,j;
/*************************************************************
* Decode
**************************************************************/
wire 								exception_reg_sel;
wire 								success_reg_sel;
wire 								fail_reg_sel;
wire								cat_sel;
wire 								maxcount_reg_sel;
wire 								pointer_start_sel;
wire 								pointer_end_sel;
wire								nmr_sel;
	
wire [`NIOS_DATA_WIDTH-1:0]			fail_reg_out;

assign exception_reg_sel  			= (csr_address == `COMPARATOR_EXCEPTION_OFFSET);
assign success_reg_sel    			= (csr_address == `COMPARATOR_SUCCESS_REG_OFFSET);
assign fail_reg_sel       			= (csr_address == `COMPARATOR_FAIL_REG_OFFSET);
assign pointer_start_sel   			= (csr_address[`DIRECTORY_BITS] == 4'h1);   
assign pointer_end_sel     			= (csr_address[`DIRECTORY_BITS] == 4'h2); 
assign maxcount_reg_sel				= (csr_address == `COMPARATOR_MAXCOUNT_REG_OFFSET);
assign cat_sel			 			= (csr_address[5:0] == `COMPARATOR_CORE_ASSIGNMENT_OFFSET);
assign nmr_sel						= (csr_address == `COMPARATOR_NMR_OFFSET);


assign csr_task_id					= csr_writedata[19:16];
assign csr_logical_core_id			= csr_writedata[25:24];
assign csr_physical_core_id			= csr_writedata[3:0];

assign csr_maxcount_data			= csr_writedata[`CRC_RAM_ADDRESS_WIDTH-1:0];

assign csr_pointer_data				= csr_writedata[`CRC_RAM_ADDRESS_WIDTH-1:0];

assign csr_nmr_data					= csr_writedata[0];

assign irq 							= exception_reg[`EXCEPTION_REG_INTERRUPT_BIT];
assign fail_reg_out					=	{	fail_reg[15],
											fail_reg[14],
											fail_reg[13],
											fail_reg[12],
											fail_reg[11],
											fail_reg[10],
											fail_reg[9],
											fail_reg[8],
											fail_reg[7],
											fail_reg[6],
											fail_reg[5],
											fail_reg[4],
											fail_reg[3],
											fail_reg[2],
											fail_reg[1],
											fail_reg[0]
										};

/*************************************************************
* FSM
**************************************************************/
reg[3:0] state;

parameter idle							= 0;
parameter st_csr_regs_write				= 1;
parameter st_csr_cat_write				= 2;
parameter st_csr_maxcount_write			= 3;
parameter st_csr_pointer_start_write	= 4;
parameter st_csr_pointer_end_write		= 5;
parameter st_csr_nmr_write				= 6;
parameter st_csr_read					= 7;
parameter st_waitrequest				= 8;
parameter st_comparator_regs_write		= 9;
parameter st_comparator_status_ack		= 10;

always @(posedge clk or posedge reset)
begin
	if(reset)
		state = idle;
	else begin
		case(state)
			idle:
				if(csr_write) begin
					if(exception_reg_sel) begin
						state = st_csr_regs_write;
					end else if (cat_sel) begin
						state = st_csr_cat_write;
					end else if (maxcount_reg_sel) begin
						state = st_csr_maxcount_write;
					end else if (pointer_start_sel) begin
						state = st_csr_pointer_start_write;
					end else if (pointer_end_sel) begin
						state = st_csr_pointer_end_write;
					end else if (nmr_sel) begin
						state = st_csr_nmr_write;
					end
				end else if(csr_read) begin
					state = st_csr_read;
				end else if(comparator_status_write & ~irq) begin
					state = st_comparator_regs_write;
				end
			
			st_csr_regs_write:
				state = st_waitrequest;
					
			st_csr_cat_write:
					state = st_waitrequest;
			
			st_csr_maxcount_write:
					state = st_waitrequest;
					
			st_csr_pointer_start_write:
				if(comp_pointer_ack)
					state = st_waitrequest;
					
			st_csr_pointer_end_write:
				if(comp_pointer_ack)
					state = st_waitrequest;
			
			st_csr_nmr_write:
				state = st_waitrequest;
				
			st_csr_read:
				state = st_waitrequest;
			
			st_waitrequest:
				state = idle;
			
			st_comparator_regs_write:
				state = st_comparator_status_ack;
			
			st_comparator_status_ack:
				state = idle;
		endcase
	end
end

/*************************************************************
* FSM outputs
**************************************************************/
// output signals
assign csr_regs_write			=	(state == st_csr_regs_write);
assign csr_cat_write			=	(state == st_csr_cat_write);
assign csr_maxcount_write		=	(state == st_csr_maxcount_write);
assign csr_pointer_start_write	=	(state == st_csr_pointer_start_write);
assign csr_pointer_end_write	=	(state == st_csr_pointer_end_write);
assign csr_nmr_write			=	(state == st_csr_nmr_write);
assign csr_waitrequest			=	~(state == st_waitrequest);

assign csr_status_ack			=	(state == st_comparator_status_ack);

// internal signals
assign comparator_regs_write	=	(state == st_comparator_regs_write);
								
/*************************************************************
* csr readdata register
**************************************************************/
always @(posedge clk or posedge reset)
begin
	if(reset)begin
		csr_readdata = 0;
	end else begin
		if(state == st_csr_read)begin
			csr_readdata = exception_reg_sel ? exception_reg : success_reg_sel ? success_reg :
									fail_reg_sel ? fail_reg_out : 0; 
		end 
	end
end

/*************************************************************
* Registers
**************************************************************/
always @(posedge clk or posedge reset)
begin
	if(reset)begin
		exception_reg = 0;
		success_reg = 0;
		for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
			fail_reg[i] = 3;
		end
	end else begin
		if(csr_regs_write)begin
			success_reg = 0;
			for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
				fail_reg[i] = 3;
			end
			exception_reg = csr_writedata;
		end else if(comparator_regs_write)begin
			exception_reg[`EXCEPTION_REG_INTERRUPT_BIT] = 1;
			exception_reg[`EXCEPTION_REG_TASK_BITS] = comparator_task_id;
			exception_reg[`EXCEPTION_REG_EX_BIT] = comparator_mismatch_detected;
			success_reg[comparator_task_id] = ~comparator_mismatch_detected;
			if(comparator_mismatch_detected) begin
				fail_reg[comparator_task_id] = comparator_logical_core_id;
			end
		end
	end
end

/*************************************************************
* Core assignment table
**************************************************************/
initial begin
	for(i=0 ; i<`CRC_KEY_WIDTH ; i=i+1) begin
		cat_mem_0[i] = 0;
		cat_mem_1[i] = 0;
		cat_mem_2[i] = 0;
	end
end


always @ (posedge clk) begin
	
	if(csr_cat_write & (csr_logical_core_id == 0)) begin
		cat_mem_0[csr_task_id] = csr_physical_core_id;
	end
	
	if(csr_cat_write & (csr_logical_core_id == 1)) begin
		cat_mem_1[csr_task_id] = csr_physical_core_id;
	end
	
	if(csr_cat_write & (csr_logical_core_id == 2)) begin
		cat_mem_2[csr_task_id] = csr_physical_core_id;
	end
	
end

// asynchronous for now
assign fprint_logical_core_id	=	cat_mem_0[fprint_task_id] == fprint_physical_core_id ? 0 :
									cat_mem_1[fprint_task_id] == fprint_physical_core_id ? 1 :
									cat_mem_2[fprint_task_id] == fprint_physical_core_id ? 2 :
									3;
assign oflow_physical_core_id	=	(oflow_logical_core_id == 0) ? cat_mem_0[oflow_task_id] :
									(oflow_logical_core_id == 1) ? cat_mem_1[oflow_task_id] :
									(oflow_logical_core_id == 2) ? cat_mem_2[oflow_task_id] :
									0;

/*************************************************************
max count register
**************************************************************/
initial begin
		for(i=0 ; i<`CRC_KEY_SIZE ; i=i+1) begin
			for(j=0 ; j<3 ; j=j+1) begin
				fprint_maxcount_mem[i][j] = 0;
			end
		end
	end

always @ (posedge clk)
begin
	if(csr_maxcount_write) begin
		fprint_maxcount_mem[csr_task_id][csr_logical_core_id] = csr_maxcount_data;
	end
end

assign csr_fprint_maxcount = fprint_maxcount_mem[oflow_task_id][oflow_logical_core_id];

/*************************************************************
* NMR
**************************************************************/
always @ (posedge clk or posedge reset) begin
	if(reset)
		nmr_mem = 0;
	else if(csr_nmr_write)
		nmr_mem[csr_task_id] = csr_nmr_data;
end

assign comparator_nmr	=	nmr_mem[comparator_task_id];
assign oflow_nmr		=	nmr_mem[oflow_task_id];
assign fprint_nmr		=	nmr_mem;

endmodule