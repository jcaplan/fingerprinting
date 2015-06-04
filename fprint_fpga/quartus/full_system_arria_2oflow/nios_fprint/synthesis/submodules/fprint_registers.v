`include "crc_defines.v"
module fprint_registers(

	//From processor
	input													clk,
	input													reset,
	input [(`COMPARATOR_ADDRESS_WIDTH-1):0]	fprint_address,
	input													fprint_write,
	input [(`NIOS_DATA_WIDTH-1):0]				fprint_writedata,
	output												fprint_waitrequest,

	//To CSR reg to get logical core id from physical
	output[3:0]											csr_physical_core_id,
	output[3:0]											csr_fprint_task_id,
	input													csr_logical_core_id,
	
	output[3:0]											comp_fprint_task_id,
	output												comp_logical_core_id,
	
	//to comp registers to get head pointer 
	//other signals covered above
	input [`CRC_RAM_ADDRESS_WIDTH-1:0]			fprint_head_pointer,
	output												increment_head_pointer,
	input													increment_hp_ack,
	
	output[3:0]											count_inc_physical_core_id,

	output [(`CRC_KEY_SIZE-1):0]					checkin_reg_out,

	input [`CRC_RAM_ADDRESS_WIDTH-1:0]			comp_tail_pointer0,
	input [`CRC_RAM_ADDRESS_WIDTH-1:0]			comp_tail_pointer1,
	output [`CRC_WIDTH - 1: 0]						fprint0,
	output [`CRC_WIDTH - 1: 0]						fprint1,
	input													comp_task_verified,
	output												fprint_reg_ack,
	input [(`CRC_KEY_WIDTH-1):0]					comp_task
);


reg  [(`CRC_KEY_SIZE-1):0]					checkout_reg[1:0];
reg  [(`CRC_KEY_SIZE-1):0]					checkin_reg[1:0];
reg  [(`CRC_KEY_SIZE-1):0]					pause_reg[1:0]; //deprecated
//Need to do each half separately in Verilog...
reg  [`CRC_WIDTH/2 - 1: 0]					fprint_mem_0_0[`CRC_RAM_SIZE -1 :0];
reg  [`CRC_WIDTH/2 - 1: 0]					fprint_mem_0_1[`CRC_RAM_SIZE -1 :0];
reg  [`CRC_WIDTH/2 - 1: 0]					fprint_mem_1_0[`CRC_RAM_SIZE -1 :0];
reg  [`CRC_WIDTH/2 - 1: 0]					fprint_mem_1_1[`CRC_RAM_SIZE -1 :0];

wire												store_reg;
wire												fprint_block;
wire [`CRC_KEY_WIDTH-1 : 0]				physical_core_id;
wire [`CRC_KEY_WIDTH-1 : 0]				fprint_task_id;


reg [`CRC_WIDTH/2 - 1: 0]					fprint0_0;
reg [`CRC_WIDTH/2 - 1: 0]					fprint0_1;
reg [`CRC_WIDTH/2 - 1: 0]					fprint1_0;
reg [`CRC_WIDTH/2 - 1: 0]					fprint1_1;


wire [`CRC_KEY_WIDTH+`CRC_KEY_WIDTH+17 : 0] fprint_fifo_data_in;
wire fprint_fifo_rd_en;
wire fprint_fifo_wr_en;
wire [`CRC_KEY_WIDTH+`CRC_KEY_WIDTH+17 : 0] fprint_fifo_data_out;
wire fprint_fifo_empty;
wire fprint_fifo_full;

/*************************************************************
* Decode
**************************************************************/
wire checkout_sel;
wire checkin_sel;
wire fprint_sel;
wire pause_sel;
wire unpause_sel;

wire													fifo_fprint_block;
wire													fifo_logical_core_id;
wire [`CRC_KEY_WIDTH-1 : 0]					fifo_physical_core_id;
wire [`CRC_KEY_WIDTH-1 : 0]					fifo_fprint_task_id;
wire [15:0]											fifo_fprint;


assign physical_core_id	= fprint_address[7:4];
assign fprint_task_id	= fprint_writedata[3:0];
assign fprint_block		= fprint_writedata[5];

assign csr_physical_core_id	=	physical_core_id;
assign csr_fprint_task_id		=	fprint_task_id;

assign checkout_sel		= (fprint_address[3:0] == `COMPARATOR_CS_OFFSET) & fprint_writedata[`CRC_KEY_WIDTH]; 
assign checkin_sel		= (fprint_address[3:0] == `COMPARATOR_CS_OFFSET) & ~fprint_writedata[`CRC_KEY_WIDTH];
assign fprint_sel			= (fprint_address[3:0] == `COMPARATOR_CRC_OFFSET);

assign pause_sel			= (fprint_address[3:0] == `COMPARATOR_PAUSE_OFFSET);
assign unpause_sel		= (fprint_address[3:0] == `COMPARATOR_UNPAUSE_OFFSET);

assign fprint_fifo_data_in = {fprint_block,csr_logical_core_id,physical_core_id,fprint_task_id,fprint_writedata[31:16]};

assign fifo_fprint_block		= fprint_fifo_data_out[17 + `CRC_KEY_WIDTH + `CRC_KEY_WIDTH];
assign fifo_logical_core_id	= fprint_fifo_data_out[16 + `CRC_KEY_WIDTH + `CRC_KEY_WIDTH];
assign fifo_physical_core_id	= fprint_fifo_data_out[15 + `CRC_KEY_WIDTH + `CRC_KEY_WIDTH : 16 + `CRC_KEY_WIDTH];
assign fifo_fprint_task_id		= fprint_fifo_data_out[15 + `CRC_KEY_WIDTH : 16];
assign fifo_fprint				= fprint_fifo_data_out[15:0];
										
assign count_inc_physical_core_id = fifo_physical_core_id;
/*************************************************************
* outputs
**************************************************************/
assign checkin_reg_out			= checkin_reg[0] & checkin_reg[1];
assign fprint0						= {fprint0_1,fprint0_0};
assign fprint1						= {fprint1_1,fprint1_0};
assign comp_logical_core_id	= fifo_logical_core_id;
assign comp_fprint_task_id		= fifo_fprint_task_id;

/*************************************************************
* IP FSM
**************************************************************/
reg[3:0] state_IP;

parameter idle 						= 0;
parameter st_fifo_write				= 1;
parameter st_reg_store 				= 2; 
parameter st_waitrequest    		= 3;
parameter st_task_verified			= 4;

always @(posedge clk or posedge reset)
begin
	if(reset)
		state_IP = idle;
	else begin
		case(state_IP)
			idle:
				if(fprint_write)begin
					if(fprint_sel) begin
						if(fprint_fifo_full)
							state_IP = idle;
						else 
							state_IP = st_fifo_write;
					end else
						state_IP = st_reg_store;
				end else if(comp_task_verified)
					state_IP = st_task_verified;
			st_fifo_write:
				state_IP = st_waitrequest;
			st_reg_store:
				state_IP = st_waitrequest;
			st_waitrequest:
				state_IP = idle;	
			st_task_verified:
				state_IP = idle;
		endcase
	end
end

/*************************************************************
* IP FSM outputs
**************************************************************/
assign fprint_fifo_wr_en	= (state_IP == st_fifo_write);
assign store_reg  			= (state_IP == st_reg_store);
assign fprint_waitrequest 	= ~(state_IP == st_waitrequest);
assign fprint_reg_ack       = (state_IP == st_task_verified);


/*************************************************************
* fprint FIFO FSM
**************************************************************/
reg[3:0] state_FIFO;

parameter st_fifo_read					= 1;
parameter st_crc_store_0    			= 2;
parameter st_crc_store_1    			= 3;
parameter st_increment_head_pointer = 4;

always @(posedge clk or posedge reset)
begin
	if(reset)
		state_FIFO = idle;
	else begin
		case(state_FIFO)
			idle:
				if(fprint_fifo_empty)
					state_FIFO = idle;
				else
					state_FIFO = st_fifo_read;
			st_fifo_read:
				state_FIFO = st_crc_store_0;
			st_crc_store_0:
				if(checkout_reg[fifo_logical_core_id][fifo_fprint_task_id])
					//wait for the head pointer
					state_FIFO = st_crc_store_1;
				else 
					state_FIFO = idle;
			st_crc_store_1:
				//store the crc
				//increment head pointer if both blocks have arrived:
				if(fifo_fprint_block)
					state_FIFO = st_increment_head_pointer;
				else 
					state_FIFO = idle;
			st_increment_head_pointer:
				if(increment_hp_ack)
					state_FIFO = idle;
		endcase
	end
end

/*************************************************************
* fprint FIFO FSM outputs
**************************************************************/
assign fprint_fifo_rd_en		= (state_FIFO == st_fifo_read);
assign increment_head_pointer	= (state_FIFO == st_increment_head_pointer);
assign store_fprint				= (state_FIFO == st_crc_store_1);

/*************************************************************
* fprint FIFO
**************************************************************/
fifo #(18 + `CRC_KEY_WIDTH + `CRC_KEY_WIDTH, 8) // 18 => 1b,1b,16b (block,log_core,fprint)
fifo_block (
clk,
reset,
fprint_fifo_data_in,
fprint_fifo_rd_en,
fprint_fifo_wr_en,
fprint_fifo_data_out,
fprint_fifo_empty,
fprint_fifo_full
);

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
	end else begin			
		if(state_IP == st_task_verified)begin
			checkout_reg[0][comp_task] = 0;
			checkout_reg[1][comp_task] = 0;
			checkin_reg[0][comp_task] = 0;
			checkin_reg[1][comp_task] = 0;
		end else if(store_reg)begin
			if(checkout_sel)
				checkout_reg[csr_logical_core_id][fprint_writedata[`CRC_KEY_WIDTH-1:0]] = 1;
			//If checkin occurs after a mismatch is detected but the monitor could not reset
			//the affected core BEFORE the end of the faulty task then ignore checkin
			//i.e. how are we checking in if we aren't checked out first?
			//Comparator has already reset the checkout register.
			if (checkin_sel & checkout_reg[csr_logical_core_id][fprint_writedata[`CRC_KEY_WIDTH-1:0]])
				checkin_reg[csr_logical_core_id][fprint_writedata[`CRC_KEY_WIDTH-1:0]] = 1;

			if(pause_sel)
				pause_reg[csr_logical_core_id][fprint_writedata[`CRC_KEY_WIDTH-1:0]] = 1;
			
			if(unpause_sel)
				pause_reg[csr_logical_core_id][fprint_writedata[`CRC_KEY_WIDTH-1:0]] = 0;
		end
	end
end

/*************************************************************
* CRC
**************************************************************/

always @ (posedge clk)
begin
	fprint0_0 = fprint_mem_0_0[comp_tail_pointer0];
	if(store_fprint & ~fifo_logical_core_id & ~fifo_fprint_block)begin
		fprint_mem_0_0[fprint_head_pointer] = fifo_fprint;  // TAKE FROM FIFO
	end
end

always @ (posedge clk)
begin
	fprint0_1 = fprint_mem_0_1[comp_tail_pointer0];
	if(store_fprint & ~fifo_logical_core_id & fifo_fprint_block)begin
		fprint_mem_0_1[fprint_head_pointer] = fifo_fprint;  // TAKE FROM FIFO 
	end
end

always @ (posedge clk)
begin
	fprint1_0 = fprint_mem_1_0[comp_tail_pointer1];
	if(store_fprint & fifo_logical_core_id & ~fifo_fprint_block)begin
		fprint_mem_1_0[fprint_head_pointer] = fifo_fprint;  // TAKE FROM FIFO
	end
end

always @ (posedge clk)
begin
	fprint1_1 = fprint_mem_1_1[comp_tail_pointer1];
	if(store_fprint & fifo_logical_core_id & fifo_fprint_block)begin
		fprint_mem_1_1[fprint_head_pointer] = fifo_fprint;  // TAKE FROM FIFO
	end
end


endmodule
