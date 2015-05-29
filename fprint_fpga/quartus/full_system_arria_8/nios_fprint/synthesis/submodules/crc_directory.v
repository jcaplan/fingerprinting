`include "crc_defines.v"

module crc_directory(
				clk,
				reset,
				spr_addr,				//address for control registers
				spr_dat_i,				//data write for control registers
				current_task_ex,		//current task being executed
				current_task_comp,		//current task being compared
				increment_r_adr,
				store_crc,
				start_pointer_sel,
				end_pointer_sel,
				head_pointer_out,
				tail_pointer_out,
				collision,
				overflow,
				crc_out,
				crc_data_in,
				waitrequest
);

parameter dw = `CRC_RAM_ADDRESS_WIDTH;
parameter ds = `CRC_DIRECTORY_SIZE;

input											clk;
input											reset;
input[`FPRINT_SPR_ADDRESS_WIDTH-1:0]			spr_addr;
input[(`NIOS_DATA_WIDTH-1):0]					spr_dat_i;
input[`CRC_KEY_WIDTH-1:0]						current_task_ex;
input[`CRC_KEY_WIDTH-1:0]						current_task_comp;
input											increment_r_adr;
input											store_crc;	

//output[dw-1:0] 									start_pointer_out;
//output[dw-1:0] 									end_pointer_out;
output[dw-1:0] 									head_pointer_out;
output[dw-1:0] 									tail_pointer_out;
input											collision;
output reg[`CRC_WIDTH-1:0]						crc_out;
input [`CRC_WIDTH-1:0]							crc_data_in;
output wire										waitrequest;

reg[dw-1:0] start_pointer_mem  	[(`CRC_KEY_SIZE-1):0];				//16 registers storing 10 bit pointers (RAM address size = 512), 1 per task 
reg[dw-1:0] end_pointer_mem		[(`CRC_KEY_SIZE-1):0];
reg[dw-1:0] head_pointer_mem	[(`CRC_KEY_SIZE-1):0];
reg[dw-1:0] tail_pointer_mem 	[(`CRC_KEY_SIZE-1):0];

					


wire [(`CRC_KEY_WIDTH-1):0] start_pointer_r_address_0;
wire [(`CRC_KEY_WIDTH-1):0] start_pointer_r_address_1;
wire [(`CRC_KEY_WIDTH-1):0] start_pointer_w_address;	
reg  [dw-1:0] start_pointer_comp;
reg  [dw-1:0] start_pointer_ex;

wire [(`CRC_KEY_WIDTH-1):0] end_pointer_r_address_0;
wire [(`CRC_KEY_WIDTH-1):0] end_pointer_r_address_1;
wire [(`CRC_KEY_WIDTH-1):0] end_pointer_w_address;
reg  [dw-1:0] end_pointer_comp;
reg  [dw-1:0] end_pointer_ex;

wire [(`CRC_KEY_WIDTH-1):0] head_pointer_r_address_0;
wire [(`CRC_KEY_WIDTH-1):0] head_pointer_r_address_1;
wire [(`CRC_KEY_WIDTH-1):0] head_pointer_w_address;
reg  [dw-1:0] head_pointer_comp;			
reg  [dw-1:0] head_pointer_ex;			


wire [(`CRC_KEY_WIDTH-1):0] tail_pointer_r_address_0;
wire [(`CRC_KEY_WIDTH-1):0] tail_pointer_r_address_1;
wire [(`CRC_KEY_WIDTH-1):0] tail_pointer_w_address;
reg  [dw-1:0] tail_pointer_comp;			
reg  [dw-1:0] tail_pointer_ex;			



reg [`CRC_KEY_WIDTH:0] task_id;
reg [`CRC_WIDTH - 1: 0] crc_reg;
reg [`CRC_WIDTH - 1: 0] crc_mem [50:0];//[`CRC_RAM_SIZE -1 :0];


input start_pointer_sel;
input end_pointer_sel;
output overflow;

//assign start_pointer_out 	= start_pointer[spr_addr[(`CRC_KEY_WIDTH-1):0]];
//assign end_pointer_out 		= end_pointer[spr_addr[(`CRC_KEY_WIDTH-1):0]];
assign head_pointer_out 	= head_pointer_comp;
assign tail_pointer_out 	= tail_pointer_comp;



assign overflow = (head_pointer_ex == tail_pointer_ex - 1) | 
					((head_pointer_ex == end_pointer_ex) & (tail_pointer_ex == start_pointer_ex) & task_id[`CRC_KEY_WIDTH]);
					//check that the task is actually running, in cases where the task is inactive and directory is not set
					//then need to ignore head_pointer_ex = end_pointer = tail_pointer_ex = start_pointer = 0
					


assign start_pointer_w_address =  spr_addr[(`CRC_KEY_WIDTH-1):0];
assign start_pointer_r_address_0 =  task_id	[`CRC_KEY_WIDTH-1:0];
assign start_pointer_r_address_1 =  current_task_comp;

assign end_pointer_w_address = spr_addr[(`CRC_KEY_WIDTH-1):0];
assign end_pointer_r_address_0 = 	task_id[`CRC_KEY_WIDTH-1:0];
assign end_pointer_r_address_1 = 	current_task_comp;


assign head_pointer_w_address = start_pointer_sel ? spr_addr[(`CRC_KEY_WIDTH-1):0] : task_id[`CRC_KEY_WIDTH-1:0] ;
assign tail_pointer_w_address = start_pointer_sel ? spr_addr[(`CRC_KEY_WIDTH-1):0] : current_task_comp;

assign head_pointer_r_address_0 = task_id[`CRC_KEY_WIDTH-1:0] ; 
assign head_pointer_r_address_1 = current_task_comp;

assign tail_pointer_r_address_0 = task_id[`CRC_KEY_WIDTH-1:0];
assign tail_pointer_r_address_1 = current_task_comp;



//FSM STYLE flow
reg [2:0] state;
parameter init = 0;
parameter load_pointer = 1;
parameter store = 2;
parameter increment_w_adr = 3;
parameter do_wr = 4;

assign waitrequest = ~(state == do_wr);

//FSM//
//What happens if two arrive too close together??
//Different cores send fprints at exact same time?
//Need to start using waitrequest
always @ (posedge clk or posedge reset)
begin
	if(reset)begin
		state = init;
		task_id = 0;
		crc_reg = 0;
	end else begin
		case(state)
			init: begin
				if(store_crc)begin
					task_id = current_task_ex;
					crc_reg = crc_data_in;
					state = load_pointer;
				end else begin
					state = init;
				end
			end
			load_pointer:
				state = store;
			//Wait a clock cycle for the appropriate pointer to load
			store:
				state = do_wr;
			do_wr:
				state = increment_w_adr;
			increment_w_adr:begin
				state = init;
			end
			//Store crc in buffer
		endcase
	end
end


always @ (posedge clk)
begin
	if(start_pointer_sel)
		start_pointer_mem[start_pointer_w_address] = spr_dat_i;
	start_pointer_ex = start_pointer_mem[start_pointer_r_address_0];
	start_pointer_comp = start_pointer_mem[start_pointer_r_address_1];
	
end						

always @ (posedge clk)
begin
	if(end_pointer_sel)
		end_pointer_mem[end_pointer_w_address] = spr_dat_i;
	end_pointer_ex = end_pointer_mem[end_pointer_r_address_0];
	end_pointer_comp = end_pointer_mem[end_pointer_r_address_1];	
end	

always @ (posedge clk)
begin
	head_pointer_comp = head_pointer_mem[head_pointer_r_address_1];
	head_pointer_ex = head_pointer_mem[head_pointer_r_address_0];
	if(collision)
		head_pointer_mem[head_pointer_w_address] = start_pointer_comp;
	else if(state == increment_w_adr)
		head_pointer_mem[head_pointer_w_address] = (head_pointer_ex == end_pointer_ex) ? start_pointer_ex : (head_pointer_ex + 1);
	else if(start_pointer_sel)
		head_pointer_mem[head_pointer_w_address] = spr_dat_i;
	
	
end		
	
always @ (posedge clk)
begin
	tail_pointer_comp = tail_pointer_mem[tail_pointer_r_address_1];
	tail_pointer_ex   = tail_pointer_mem[tail_pointer_r_address_0];
	if(collision)
		tail_pointer_mem[tail_pointer_w_address] = start_pointer_comp;
	else if(increment_r_adr)
		tail_pointer_mem[tail_pointer_w_address] = (tail_pointer_comp == end_pointer_comp) ? start_pointer_comp : (tail_pointer_comp + 1);
	else if(start_pointer_sel)
		tail_pointer_mem[tail_pointer_w_address] = spr_dat_i;
end			


always @ (posedge clk)
begin
	crc_out = crc_mem[tail_pointer_comp];
	if(state == store)begin
		crc_mem[head_pointer_ex] = crc_reg;
	end
end		

endmodule
