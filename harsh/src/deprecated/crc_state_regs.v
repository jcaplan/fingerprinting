`include "crc_defines.v"
module crc_state_regs(
	input                                           clk,
	input                                           reset,
	input wire [(`NIOS_DATA_WIDTH-1):0]             data_queue_rdata,
	input wire [(`COMPARATOR_ADDRESS_WIDTH-1):0]    address_queue_rdata,
	input                                           slave_write,
	input wire [(`NIOS_DATA_WIDTH-1):0]             csr_data,
	input wire [(`COMPARATOR_ADDRESS_WIDTH-1):0]    csr_address,
	input                                           csr_write,  
	output wire [(`CRC_WIDTH-1):0]                  crc_out,
	output wire [(`CRC_RAM_ADDRESS_WIDTH-1):0]      head_pointer,
	output wire [(`CRC_RAM_ADDRESS_WIDTH-1):0]      tail_pointer,
	output wire [(`CRC_KEY_SIZE-1):0]               checkout,
	output wire [(`CRC_KEY_SIZE-1):0]               checkin,
	output wire [(`CRC_KEY_SIZE-1):0]               pause,
	output wire                                     pause_strobe,
	input                                           comparator_increment_ram_address,
	input       [(`CRC_KEY_WIDTH-1):0]              comparator_current_task,
	input                                           comparator_register_write,
	input                                           comparator_collision_detected,
	output wire                                     fprint_overflow,
	output wire [(`CRC_KEY_WIDTH-1):0]              current_task_ex,
	output reg  [(`CRC_KEY_SIZE-1):0]               fprints_ready_reg,
	input  wire										fprints_checked,
	output wire										waitrequest,
	output wire 									comp_w_ack
);

/********************************************************************
State registers are maintained
*********************************************************************/



reg  [(`CRC_KEY_SIZE-1):0]                  checkout_reg;
reg  [(`CRC_KEY_SIZE-1):0]                  checkin_reg;
reg  [(`CRC_KEY_SIZE-1):0]                  pause_reg;
reg  [`CRC_KEY_WIDTH:0]                     currentstate;
reg                                         data_ready;
reg  [(`CRC_KEY_WIDTH-1):0]                 core_assignment_reg[(`CRC_KEY_SIZE-1):0];                  
reg   										fprint_buffer_write;
reg  [(`NIOS_DATA_WIDTH-1):0]    			fprint_buffer_data;



wire                                        check_out_sel;
wire                                        check_in_sel;
wire                                        pause_sel;
wire                                        unpause_sel;
wire                                        start_p_sel;
wire                                        end_p_sel;
wire                                        crc_sel;
wire                                        core_assignment_sel;
wire [3:0]                                  core_id;
wire [`CRC_KEY_WIDTH:0]                 	index;
wire 										dir_waitrequest;
wire 										do_crc_store;
wire 										do_comp_write;

reg[2:0] state;
parameter idle = 0;
parameter store_reg = 1;
parameter do_waitreq1 = 2;
parameter store_crc = 3;
parameter comp_write = 4;
parameter do_waitreq2 = 5;
parameter error_crc   = 6;


assign checkout      = checkout_reg;
assign checkin       = checkin_reg;
assign pause         = pause_reg;
assign pause_strobe  = pause_sel;
assign current_task_ex = currentstate[`CRC_KEY_WIDTH-1:0];

//********************************************************
//Second clock cycle for inputs - data classification
//********************************************************
always @ (posedge clk or posedge reset)
begin
	if(reset)begin
		data_ready = 0;
	end else begin 
		if(slave_write)
			data_ready = 1;
		else
			data_ready = 0;
	end
end

assign core_id       = address_queue_rdata[7:4];
assign core_match    = (core_id==core_assignment_reg[data_queue_rdata[3:0]]);

assign check_out_sel = data_ready & (address_queue_rdata[3:0] == `COMPARATOR_CS_OFFSET) & data_queue_rdata[`CRC_KEY_WIDTH]
						& core_match;    //////////
assign check_in_sel  = data_ready & (address_queue_rdata[3:0] == `COMPARATOR_CS_OFFSET) & ~data_queue_rdata[`CRC_KEY_WIDTH]
						& core_match;   /////////
assign crc_sel       = data_ready & (address_queue_rdata[3:0] == `COMPARATOR_CRC_OFFSET);

assign pause_sel     = data_ready & (address_queue_rdata[3:0] == `COMPARATOR_PAUSE_OFFSET)
						& core_match;
assign unpause_sel   = data_ready & (address_queue_rdata[3:0] == `COMPARATOR_UNPAUSE_OFFSET)
						& core_match;
assign start_p_sel   = csr_write & (csr_address[`DIRECTORY_BITS] == 4'h1 );   ///////////
assign end_p_sel     = csr_write & (csr_address[`DIRECTORY_BITS] == 4'h2);   ////////////
assign core_assignment_sel = csr_write & (csr_address[5:0] == `COMPARATOR_CORE_ASSIGNMENT_OFFSET);


//********************************************************
//Third clock cycle for inputs - data storage 
//********************************************************
always @ (posedge clk or posedge reset)
begin
	if(reset)begin
		checkout_reg = 0;
		checkin_reg = 0;
		pause_reg = 0;
		currentstate = 0;
	end else begin

		if(check_out_sel)begin
			checkout_reg[data_queue_rdata[`CRC_KEY_WIDTH-1:0]] = 1;
			currentstate = data_queue_rdata[`CRC_KEY_WIDTH:0];
		end else if(do_comp_write)
			checkout_reg[comparator_current_task] = 0;
		
		if(check_in_sel)begin
			checkin_reg[data_queue_rdata[`CRC_KEY_WIDTH-1:0]] = 1;
			currentstate = data_queue_rdata[`CRC_KEY_WIDTH:0];
		end else if(do_comp_write)
			checkin_reg[comparator_current_task] = 0;
		if(pause_sel)begin
			pause_reg[data_queue_rdata[`CRC_KEY_WIDTH-1:0]] = 1;
			currentstate = data_queue_rdata[`CRC_KEY_WIDTH:0];
		end
		if(unpause_sel)begin
			pause_reg[data_queue_rdata[`CRC_KEY_WIDTH-1:0]] = 0;
			currentstate = data_queue_rdata[`CRC_KEY_WIDTH:0];
		end 
		if(core_assignment_sel)begin
			core_assignment_reg[csr_data[7:4]] = csr_data[3:0];
		end
	end
end



always @(posedge clk or posedge reset)
begin
	if(reset)
		state = 0;
	else begin
		case(state)
			idle:
				if(check_out_sel | check_in_sel | pause_sel | unpause_sel | core_assignment_sel)
					state = store_reg;
				else if(crc_sel & ~index[`CRC_KEY_WIDTH])
					state = store_crc;
				else if(comparator_register_write)
					state = comp_write;
				else if(crc_sel & index[`CRC_KEY_WIDTH])
					state = error_crc;
				else begin
					state = idle;
				end 
			store_reg:
				state = do_waitreq1;
			store_crc:
				if(~dir_waitrequest)
					state = do_waitreq1;
				else begin
					state = store_crc;
				end
			error_crc:
				state = do_waitreq1;
			do_waitreq1:
				state = do_waitreq2;
			do_waitreq2:
				state = idle;
			comp_write:
				state = idle;
		endcase
	end
end

assign do_comp_write = (state == comp_write);
assign comp_w_ack = do_comp_write;
assign waitrequest = ~(((state == store_crc) & ~dir_waitrequest) | (state == store_reg) | (state == error_crc));
assign do_crc_store = (state == store_crc);
assign index = (core_assignment_reg[0] == core_id) & checkout[0] & ~pause_reg[0] & ~checkin[0] ? 0  :
				(core_assignment_reg[1] == core_id) & checkout[1] & ~pause_reg[1] & ~checkin[1] ? 1 :
				(core_assignment_reg[2] == core_id) & checkout[2] & ~pause_reg[2] & ~checkin[2] ? 2 :
				(core_assignment_reg[3] == core_id) & checkout[3] & ~pause_reg[3] & ~checkin[3] ? 3 :
				(core_assignment_reg[4] == core_id) & checkout[4] & ~pause_reg[4] & ~checkin[4] ? 4 :
				(core_assignment_reg[5] == core_id) & checkout[5] & ~pause_reg[5] & ~checkin[5] ? 5 :
				(core_assignment_reg[6] == core_id) & checkout[6] & ~pause_reg[6] & ~checkin[6] ? 6 :
				(core_assignment_reg[7] == core_id) & checkout[7] & ~pause_reg[7] & ~checkin[7] ? 7 :
				(core_assignment_reg[8] == core_id) & checkout[8] & ~pause_reg[8] & ~checkin[8] ? 8 :
				(core_assignment_reg[9] == core_id) & checkout[9] & ~pause_reg[9] & ~checkin[9] ? 9 :
				(core_assignment_reg[10] == core_id) & checkout[10] & ~pause_reg[10] & ~checkin[10] ? 10 :
				(core_assignment_reg[11] == core_id) & checkout[11] & ~pause_reg[11] & ~checkin[11] ? 11 :
				(core_assignment_reg[12] == core_id) & checkout[12] & ~pause_reg[12] & ~checkin[12] ? 12 :
				(core_assignment_reg[13] == core_id) & checkout[13] & ~pause_reg[13] & ~checkin[13] ? 13 :
				(core_assignment_reg[14] == core_id) & checkout[14] & ~pause_reg[14] & ~checkin[14] ? 14 :
				(core_assignment_reg[14] == core_id) & checkout[14] & ~pause_reg[14] & ~checkin[15] ? 15 :
				16;


always @ (posedge clk or posedge reset)
begin
	if(reset)begin
		fprints_ready_reg = 0;
	end else begin
		if(state == store_crc)begin
			fprints_ready_reg[index[`CRC_KEY_WIDTH-1:0]] = 1;
		end else if(comparator_register_write | fprints_checked)begin
			fprints_ready_reg[comparator_current_task] = 0;
			//If a collision has occured or the comparator has completed
			//its checks
		end 
	end
end



/********************************************************************
Directory and buffer to store CRCs according to stack
*********************************************************************/


	


//TEMPORARY ASSIGNMNETS FOR DEBUGGING



crc_directory d0 (
				.clk(clk),
				.reset(reset),
				.spr_addr(csr_address),                                 //address for control registers
				.spr_dat_i(csr_data),                                   //data write for control registers  
				.current_task_ex(index[`CRC_KEY_WIDTH-1:0]),                                 //current task being executed
				.current_task_comp(comparator_current_task),                            //current task being compared
				.increment_r_adr(comparator_increment_ram_address),
	
				.store_crc(do_crc_store),
				.start_pointer_sel(start_p_sel),
				.end_pointer_sel(end_p_sel),
				.head_pointer_out(head_pointer),
				.tail_pointer_out(tail_pointer),
				.collision(comparator_collision_detected),
				.overflow(fprint_overflow),
				.crc_out(crc_out),
				.crc_data_in(data_queue_rdata),
				.waitrequest(dir_waitrequest)
				
);  



endmodule
