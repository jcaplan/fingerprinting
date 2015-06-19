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
	output[3:0]											physical_core_id,
	output[3:0]											fprint_task_id,
	input													logical_core_id,
	
	//trom CSR when weading max count info
	input													csr_maxcount_write,
	input [(`NIOS_DATA_WIDTH-1):0]				csr_maxcount_writedata,
	
	output reg [(`CRC_KEY_SIZE-1):0]				LC0_maxcount_status_reg,
	output reg [(`CRC_KEY_SIZE-1):0]				LC1_maxcount_status_reg,
	
	//to comp registers to get head pointer 
	//other signals covered above
	input [`CRC_RAM_ADDRESS_WIDTH-1:0]			fprint_head_pointer,
	output												increment_head_pointer,
	input													increment_tail_pointer,
	input													increment_hp_ack,

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

wire										store_reg;
wire										fprint_block;

reg [`CRC_WIDTH/2 - 1: 0]					fprint0_0;
reg [`CRC_WIDTH/2 - 1: 0]					fprint0_1;
reg [`CRC_WIDTH/2 - 1: 0]					fprint1_0;
reg [`CRC_WIDTH/2 - 1: 0]					fprint1_1;

reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]		fprint_mem_count_0 [(`CRC_KEY_SIZE-1):0];
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]		fprint_mem_count_1 [(`CRC_KEY_SIZE-1):0];

reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]		fprint_mem_maxcount [(`CRC_KEY_SIZE-1):0];

reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]		fprint_mem_count_head_0;	
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]		fprint_mem_count_head_1;
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]		fprint_mem_count_tail_0;
reg [`CRC_RAM_ADDRESS_WIDTH-1 : 0]		fprint_mem_count_tail_1;


/*************************************************************
* Decode
**************************************************************/
wire checkout_sel;
wire checkin_sel;
wire fprint_sel;
wire pause_sel;
wire unpause_sel;

wire [`CRC_KEY_WIDTH-1 : 0]					csr_maxcount_task_id;
wire [`CRC_RAM_ADDRESS_WIDTH-1 : 0]			csr_maxcount_value;

wire [`CRC_KEY_WIDTH-1 : 0]					maxcount_task_id;
wire [`CRC_RAM_ADDRESS_WIDTH-1 : 0]			maxcount_value;

wire [(`CRC_KEY_SIZE-1):0]						LC0_status_inc_update;
wire [(`CRC_KEY_SIZE-1):0]						LC0_status_dec_update;
wire [(`CRC_KEY_SIZE-1):0]						LC1_status_inc_update;
wire [(`CRC_KEY_SIZE-1):0]						LC1_status_dec_update;

wire fprint_mem_count_inc_0;
wire fprint_mem_count_inc_1;
wire fprint_mem_count_dec;

assign physical_core_id       = fprint_address[7:4];
assign fprint_task_id         = fprint_writedata[3:0];

assign checkout_sel = (fprint_address[3:0] == `COMPARATOR_CS_OFFSET) & fprint_writedata[`CRC_KEY_WIDTH]; 
assign checkin_sel  = (fprint_address[3:0] == `COMPARATOR_CS_OFFSET) & ~fprint_writedata[`CRC_KEY_WIDTH];
assign fprint_sel       = (fprint_address[3:0] == `COMPARATOR_CRC_OFFSET);

assign pause_sel     = (fprint_address[3:0] == `COMPARATOR_PAUSE_OFFSET);
assign unpause_sel   = (fprint_address[3:0] == `COMPARATOR_UNPAUSE_OFFSET);

assign fprint_block     = (fprint_writedata[5]);

assign csr_maxcount_task_id = csr_maxcount_writedata[(`CRC_RAM_ADDRESS_WIDTH+`CRC_KEY_WIDTH-1) : `CRC_RAM_ADDRESS_WIDTH];
assign csr_maxcount_value = csr_maxcount_writedata[`CRC_RAM_ADDRESS_WIDTH-1 : 0];

assign fprint_mem_count_inc_0 =	increment_head_pointer ? increment_hp_ack ? ~logical_core_id ?
											increment_tail_pointer ? fprint_task_id != comp_task ? 1 :0 :1 :0 :0 :0;
											
assign fprint_mem_count_inc_1 =	increment_head_pointer ? increment_hp_ack ? logical_core_id ?
											increment_tail_pointer ? fprint_task_id != comp_task ? 1 :0 :1 :0 :0 :0;
											
assign fprint_mem_count_dec	=	increment_tail_pointer ? increment_hp_ack ? fprint_task_id != comp_task ? 1 :0 :1 :0;

assign LC0_status_inc_update =	fprint_mem_count_inc_0 ? 
											fprint_mem_count_head_0 >= fprint_mem_maxcount[fprint_task_id] - 1? 
											1<<fprint_task_id :0 :0;
											
assign LC0_status_dec_update = 	fprint_mem_count_dec ? 
											fprint_mem_count_tail_0 <= fprint_mem_maxcount[comp_task] ?
											1<<comp_task :0 :0;

assign LC1_status_inc_update =	fprint_mem_count_inc_1 ? 
											fprint_mem_count_head_1 >= fprint_mem_maxcount[fprint_task_id] - 1? 
											1<<fprint_task_id :0 :0;
											
assign LC1_status_dec_update = 	fprint_mem_count_dec ? 
											fprint_mem_count_tail_1 <= fprint_mem_maxcount[comp_task] ?
											1<<comp_task :0 :0;
											
/*************************************************************
* FSM
**************************************************************/
reg[3:0] state;

parameter idle 						= 0;
parameter st_reg_store 				= 1;
parameter st_waitrequest    		= 2;
parameter st_crc_store_0    		= 3;
parameter st_crc_store_1    		= 4;
parameter st_increment_head_pointer = 5;
parameter st_task_verified			= 6;

always @(posedge clk or posedge reset)
begin
	if(reset)
		state = idle;
	else begin
		case(state)
			idle:
				if(fprint_write)begin
					if(~fprint_sel) 
						state = st_reg_store;
					else if(fprint_sel) //If not checked out then don't store crc
						state = st_crc_store_0;
				end else if(comp_task_verified)
					state = st_task_verified;
			st_reg_store:
				state = st_waitrequest;
			st_crc_store_0:
				if(checkout_reg[logical_core_id][fprint_task_id])
					//wait for the head pointer
					state = st_crc_store_1;
				else 
					state = st_waitrequest;
			st_crc_store_1:
				//store the crc
				//increment head pointer if both blocks have arrived:
				if(fprint_block)
					state = st_increment_head_pointer;
				else 
					state = st_waitrequest;
			st_increment_head_pointer:
				if(increment_hp_ack)
					state = st_waitrequest;
			st_waitrequest:
				state = idle;	
			st_task_verified:
				state = idle;
		endcase
	end
end

/*************************************************************
* FSM outputs
**************************************************************/
assign store_reg  			= (state == st_reg_store);
assign fprint_waitrequest 	= ~(state == st_waitrequest);
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
	end else begin			
		if(state == st_task_verified)begin
			checkout_reg[0][comp_task] = 0;
			checkout_reg[1][comp_task] = 0;
			checkin_reg[0][comp_task] = 0;
			checkin_reg[1][comp_task] = 0;
		end else if(store_reg)begin
			if(checkout_sel)
				checkout_reg[logical_core_id][fprint_writedata[`CRC_KEY_WIDTH-1:0]] = 1;
			//If checkin occurs after a mismatch is detected but the monitor could not reset
			//the affected core BEFORE the end of the faulty task then ignore checkin
			//i.e. how are we checking in if we aren't checked out first?
			//Comparator has already reset the checkout register.
			if (checkin_sel & checkout_reg[logical_core_id][fprint_writedata[`CRC_KEY_WIDTH-1:0]])
				checkin_reg[logical_core_id][fprint_writedata[`CRC_KEY_WIDTH-1:0]] = 1;

			if(pause_sel)
				pause_reg[logical_core_id][fprint_writedata[`CRC_KEY_WIDTH-1:0]] = 1;
			
			if(unpause_sel)
				pause_reg[logical_core_id][fprint_writedata[`CRC_KEY_WIDTH-1:0]] = 0;
		end
	end
end

/*************************************************************
max count sram

we need ONE write: at address 'csr_maxcount_task_id'
we need TWO reads: at addresses 'fprint_task_id' and 'comp_task'

how?  
**************************************************************/
integer i;
	
	initial begin
		for(i=0 ; i<16 ; i=i+1) begin
			fprint_mem_maxcount[i] = 0;
		end
	end

always @ (posedge clk)
begin
	if(csr_maxcount_write) begin
		fprint_mem_maxcount[csr_maxcount_task_id] = csr_maxcount_value;
	end
end


/*************************************************************
fprint mem counters
**************************************************************/
	
	initial begin
		for(i=0 ; i<16 ; i=i+1) begin
			fprint_mem_count_0[i] = 0;
			fprint_mem_count_1[i] = 0;
		end
	end

	// logical core 0
	always @ (posedge clk) begin
		if (fprint_mem_count_inc_0) begin
			fprint_mem_count_0[fprint_task_id] <= fprint_mem_count_head_0 + 1;
			fprint_mem_count_head_0 <= fprint_mem_count_head_0 + 1;
		end else
			fprint_mem_count_head_0 <= fprint_mem_count_0[fprint_task_id];
	end

	always @ (posedge clk) begin
		if (fprint_mem_count_dec) begin
			fprint_mem_count_0[fprint_task_id] <= fprint_mem_count_tail_0 - 1;
			fprint_mem_count_tail_0 <= fprint_mem_count_tail_0 - 1;
		end else
			fprint_mem_count_tail_0 <= fprint_mem_count_0[fprint_task_id];
	end
	
	
	// logical core 1
	always @ (posedge clk) begin
		if (fprint_mem_count_inc_1) begin
			fprint_mem_count_1[fprint_task_id] <= fprint_mem_count_head_1 + 1;
			fprint_mem_count_head_1 <= fprint_mem_count_head_1 + 1;
		end else
			fprint_mem_count_head_1 <= fprint_mem_count_1[fprint_task_id];
	end

	always @ (posedge clk) begin
		if (fprint_mem_count_dec) begin
			fprint_mem_count_1[fprint_task_id] <= fprint_mem_count_tail_1 - 1;
			fprint_mem_count_tail_1 <= fprint_mem_count_tail_1 - 1;
		end else
			fprint_mem_count_tail_1 <= fprint_mem_count_1[fprint_task_id];
	end

/*************************************************************
logical core count status registers 
**************************************************************/	
always @ (posedge clk or posedge reset)
begin
	if(reset)begin
		LC0_maxcount_status_reg = 0;
		LC1_maxcount_status_reg = 0;
	end else begin
			LC0_maxcount_status_reg = (LC0_maxcount_status_reg | LC0_status_inc_update) & (~ LC0_status_dec_update);
			LC1_maxcount_status_reg = (LC1_maxcount_status_reg | LC1_status_inc_update) & (~ LC1_status_dec_update);
	end
end


/*************************************************************
* CRC
**************************************************************/

always @ (posedge clk)
begin
	fprint0_0 = fprint_mem_0_0[comp_tail_pointer0];
	if(store_fprint & ~logical_core_id & ~fprint_block)begin
		fprint_mem_0_0[fprint_head_pointer] = fprint_writedata[31:16]; 
	end
end

always @ (posedge clk)
begin
	fprint0_1 = fprint_mem_0_1[comp_tail_pointer0];
	if(store_fprint & ~logical_core_id & fprint_block)begin
		fprint_mem_0_1[fprint_head_pointer] = fprint_writedata[31:16]; 
	end
end

always @ (posedge clk)
begin
	fprint1_0 = fprint_mem_1_0[comp_tail_pointer1];
	if(store_fprint & logical_core_id & ~fprint_block)begin
		fprint_mem_1_0[fprint_head_pointer] = fprint_writedata[31:16]; 
	end
end

always @ (posedge clk)
begin
	fprint1_1 = fprint_mem_1_1[comp_tail_pointer1];
	if(store_fprint & logical_core_id & fprint_block)begin
		fprint_mem_1_1[fprint_head_pointer] = fprint_writedata[31:16]; 
	end
end


endmodule
