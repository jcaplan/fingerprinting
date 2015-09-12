`include "defines.v"

module fprint_registers(

	//From processor
	input								    	clk,
	input								    	reset,
	input 	[(`COMPARATOR_ADDRESS_WIDTH-1):0]	fprint_address,
	input										fprint_write,
	input 	[(`NIOS_DATA_WIDTH-1):0]			fprint_writedata,
	output										fprint_waitrequest,

	//To CSR reg to get logical core id from physical
	output[3:0] 								physical_core_id,
	output[3:0] 								fprint_task_id,
	input  										logical_core_id,

	//to comp registers to get head pointer 
	//other signals covered above
	input [`CRC_RAM_ADDRESS_WIDTH-1:0]     		fprint_head_pointer,
	output 										increment_head_pointer,
	input 										increment_hp_ack,

	output [(`CRC_KEY_SIZE-1):0]  				checkin_reg_out,

	input [`CRC_RAM_ADDRESS_WIDTH-1:0] 			comp_tail_pointer0,
	input [`CRC_RAM_ADDRESS_WIDTH-1:0] 			comp_tail_pointer1,
	output [`CRC_WIDTH - 1: 0] 					fprint0,
	output [`CRC_WIDTH - 1: 0]             		fprint1,
	input 										comp_task_verified,
	output 										fprint_reg_ack,
	input [(`CRC_KEY_WIDTH-1):0] 				comp_task

);


reg  [(`CRC_KEY_SIZE-1):0]                  checkout_reg[1:0];
reg  [(`CRC_KEY_SIZE-1):0]                  checkin_reg[1:0];
reg  [(`CRC_KEY_SIZE-1):0]                  pause_reg[1:0]; //deprecated
//Need to do each half separately in Verilog...
reg  [`CRC_WIDTH/2 - 1: 0] 					fprint_mem_0_0[`CRC_RAM_SIZE -1 :0];
reg  [`CRC_WIDTH/2 - 1: 0] 					fprint_mem_0_1[`CRC_RAM_SIZE -1 :0];
reg  [`CRC_WIDTH/2 - 1: 0] 					fprint_mem_1_0[`CRC_RAM_SIZE -1 :0];
reg  [`CRC_WIDTH/2 - 1: 0] 					fprint_mem_1_1[`CRC_RAM_SIZE -1 :0];

wire 										store_reg;
wire 										fprint_block;

reg [`CRC_WIDTH/2 - 1: 0] 					fprint0_0;
reg [`CRC_WIDTH/2 - 1: 0] 					fprint0_1;
reg [`CRC_WIDTH/2 - 1: 0] 					fprint1_0;
reg [`CRC_WIDTH/2 - 1: 0] 					fprint1_1;

wire [`COMPARATOR_ADDRESS_WIDTH + `NIOS_DATA_WIDTH-1 : 0] fifo_data_in;
wire fifo_rd_en;
wire fifo_wr_en;
wire [`COMPARATOR_ADDRESS_WIDTH + `NIOS_DATA_WIDTH-1 : 0] fifo_data_out;
wire fifo_empty;
wire fifo_full;

reg 	[(`COMPARATOR_ADDRESS_WIDTH-1):0]	fprint_address_reg;
reg 	[(`NIOS_DATA_WIDTH-1):0]			fprint_writedata_reg;

wire 	[(`COMPARATOR_ADDRESS_WIDTH-1):0]	fifo_fprint_address;
wire 	[(`NIOS_DATA_WIDTH-1):0]			fifo_fprint_writedata;

/*************************************************************
* Decode
**************************************************************/
wire checkout_sel;
wire checkin_sel;
wire fprint_sel;
wire pause_sel;
wire unpause_sel;
assign physical_core_id			=	fifo_fprint_address[7:4];
assign fprint_task_id			=	fifo_fprint_writedata[3:0];
	
assign checkout_sel				=	(fifo_fprint_address[3:0] == `COMPARATOR_CS_OFFSET) & fifo_fprint_writedata[`CRC_KEY_WIDTH]; 
assign checkin_sel				=	(fifo_fprint_address[3:0] == `COMPARATOR_CS_OFFSET) & ~fifo_fprint_writedata[`CRC_KEY_WIDTH];
assign fprint_sel				=	(fifo_fprint_address[3:0] == `COMPARATOR_CRC_OFFSET);
	
assign pause_sel				=	(fifo_fprint_address[3:0] == `COMPARATOR_PAUSE_OFFSET);
assign unpause_sel				=	(fifo_fprint_address[3:0] == `COMPARATOR_UNPAUSE_OFFSET);
	
assign fprint_block				=	(fifo_fprint_writedata[5]);
	
assign fifo_data_in				=	{fprint_address_reg,fprint_writedata_reg};
assign fifo_fprint_address		=	fifo_data_out[`COMPARATOR_ADDRESS_WIDTH+`NIOS_DATA_WIDTH-1 : `NIOS_DATA_WIDTH];
assign fifo_fprint_writedata	=	fifo_data_out[`NIOS_DATA_WIDTH-1 : 0];

/*************************************************************
* FIFO FSM
**************************************************************/
reg[3:0] FIFO_state;

parameter idle 				= 0;
parameter st_waitrequest    = 1;

always @(posedge clk or posedge reset)
begin
	if(reset)
		FIFO_state = idle;
	else begin
		case(FIFO_state)
			idle:
				if(fprint_write & ~fifo_full)
					FIFO_state = st_waitrequest;
			st_waitrequest:
				FIFO_state = idle;	

		endcase
	end
end

/*************************************************************
* FIFO FSM outputs
**************************************************************/
assign fifo_wr_en           = (FIFO_state == st_waitrequest);
assign fprint_waitrequest   = ~(FIFO_state == st_waitrequest);

/*************************************************************
* FIFO
**************************************************************/
fifo #(`COMPARATOR_ADDRESS_WIDTH + `NIOS_DATA_WIDTH, 4) 
fifo_block (
clk,
reset,
fifo_data_in,
fifo_rd_en,
fifo_wr_en,
fifo_data_out,
fifo_empty,
fifo_full
);

/*************************************************************
* FSM
**************************************************************/
reg[3:0] state;

parameter st_read_fifo 				= 1;
parameter st_decode 				= 2;
parameter st_reg_store 				= 3;
parameter st_crc_store_0    		= 4;
parameter st_crc_store_1    		= 5;
parameter st_increment_head_pointer = 6;
parameter st_task_verified			= 7;
always @(posedge clk or posedge reset)
begin
	if(reset)
		state = idle;
	else begin
		case(state)
			idle:
				if(comp_task_verified)
					state = st_task_verified;
				else if(~fifo_empty)
                    state = st_read_fifo;
            st_read_fifo:
                state = st_decode;
            st_decode:
                if(~fprint_sel) 
                    state = st_reg_store;
				else if(fprint_sel) //If not checked out then don't store crc
					state = st_crc_store_0;
			st_reg_store:
				state = idle;
			st_crc_store_0:
				if(checkout_reg[logical_core_id][fprint_task_id])
					//wait for the head pointer
					state = st_crc_store_1;
				else 
					state = idle;
			st_crc_store_1:
				//store the crc
				//increment head pointer if both blocks have arrived:
				if(fprint_block)
					state = st_increment_head_pointer;
				else 
					state = idle;
			st_increment_head_pointer:
				if(increment_hp_ack)
					state = idle;
			st_task_verified:
				state = idle;
		endcase
	end
end

/*************************************************************
* FSM outputs
**************************************************************/
assign fifo_rd_en           = (state == st_read_fifo);
assign store_reg  			= (state == st_reg_store);
assign increment_head_pointer = (state == st_increment_head_pointer);
assign checkin_reg_out 		= checkin_reg[0] & checkin_reg[1];
assign fprint_reg_ack       = (state == st_task_verified);
assign fprint0 				= {fprint0_1,fprint0_0};
assign fprint1 				= {fprint1_1,fprint1_0};
assign store_fprint         = (state == st_crc_store_1);
/*************************************************************
* Registers
**************************************************************/
always @ (posedge clk or posedge reset)
begin
	if(reset)begin
		checkout_reg[0] = 0;
		checkin_reg[0] = 0;
		pause_reg[0] = 0;
		checkout_reg[1] = 0;
		checkin_reg[1] = 0;
		pause_reg[1] = 0;
        fprint_address_reg = 0;
        fprint_writedata_reg = 0;
	end else begin			
		if(state == st_task_verified)begin
			checkout_reg[0][comp_task] = 0;
			checkout_reg[1][comp_task] = 0;
			checkin_reg[0][comp_task] = 0;
			checkin_reg[1][comp_task] = 0;
		end else if(store_reg)begin
			if(checkout_sel)
				checkout_reg[logical_core_id][fifo_fprint_writedata[`CRC_KEY_WIDTH-1:0]] = 1;
			//If checkin occurs after a mismatch is detected but the monitor could not reset
			//the affected core BEFORE the end of the faulty task then ignore checkin
			//i.e. how are we checking in if we aren't checked out first?
			//Comparator has already reset the checkout register.
			if (checkin_sel & checkout_reg[logical_core_id][fifo_fprint_writedata[`CRC_KEY_WIDTH-1:0]])
				checkin_reg[logical_core_id][fifo_fprint_writedata[`CRC_KEY_WIDTH-1:0]] = 1;

			if(pause_sel)
				pause_reg[logical_core_id][fifo_fprint_writedata[`CRC_KEY_WIDTH-1:0]] = 1;
			
			if(unpause_sel)
				pause_reg[logical_core_id][fifo_fprint_writedata[`CRC_KEY_WIDTH-1:0]] = 0;
		end
        
        fprint_address_reg = fprint_address;
        fprint_writedata_reg = fprint_writedata;
	end
end


/*************************************************************
* CRC
**************************************************************/

always @ (posedge clk)
begin
	fprint0_0 = fprint_mem_0_0[comp_tail_pointer0];
	if(store_fprint & ~logical_core_id & ~fprint_block)begin
		fprint_mem_0_0[fprint_head_pointer] = fifo_fprint_writedata[31:16]; 
	end
end

always @ (posedge clk)
begin
	fprint0_1 = fprint_mem_0_1[comp_tail_pointer0];
	if(store_fprint & ~logical_core_id & fprint_block)begin
		fprint_mem_0_1[fprint_head_pointer] = fifo_fprint_writedata[31:16]; 
	end
end

always @ (posedge clk)
begin
	fprint1_0 = fprint_mem_1_0[comp_tail_pointer1];
	if(store_fprint & logical_core_id & ~fprint_block)begin
		fprint_mem_1_0[fprint_head_pointer] = fifo_fprint_writedata[31:16]; 
	end
end

always @ (posedge clk)
begin
	fprint1_1 = fprint_mem_1_1[comp_tail_pointer1];
	if(store_fprint & logical_core_id & fprint_block)begin
		fprint_mem_1_1[fprint_head_pointer] = fifo_fprint_writedata[31:16]; 
	end
end


endmodule
