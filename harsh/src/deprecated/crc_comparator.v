`include "crc_defines.v"
module crc_comparator(
	clk,
	rst,
	increment_dir_address,
	crc_in1,
	crc_in2,
	ram_adr_load,
	dir_adr_r,
	spr_we_i1,
	spr_we_i2,
	spr_we_o,
	ch_in_reg1,
	ch_in_reg2,
	ch_out_reg1,
	ch_out_reg2,
	comp_col_ex,
	io_release,
	head_pointer1,
	tail_pointer1,
	head_pointer2,
	tail_pointer2,
	pause_task1,
	pause_task2,
	irq_in,
	fprints_ready1,
	fprints_ready2,
	fprints_checked,
	dir_w_ack1,
	dir_w_ack2,
	exception_register_write,
	csr_write,
	csr_read

);

parameter cw = `CRC_WIDTH;
parameter kw = `CRC_KEY_WIDTH;
parameter daw = `CRC_RAM_ADDRESS_WIDTH;
input 						clk;
input 						rst;
output 						increment_dir_address;
input[cw-1 : 0]				crc_in1;
input[cw-1 : 0]				crc_in2;
output						ram_adr_load;
output[`CRC_KEY_WIDTH-1:0]	dir_adr_r;
reg[kw:0]					current_task;
input						spr_we_i1;
input						spr_we_i2;
output						spr_we_o;
input[`CRC_KEY_SIZE-1:0]	ch_in_reg1;
input[`CRC_KEY_SIZE-1:0]	ch_in_reg2;
input[`CRC_KEY_SIZE-1:0]	ch_out_reg1;
input[`CRC_KEY_SIZE-1:0]	ch_out_reg2;
output						comp_col_ex;
output						io_release;							//Signal to I/O Buffer to release data associated with task

input[`CRC_DIRECTORY_WIDTH-1:0]		head_pointer1;
input[`CRC_DIRECTORY_WIDTH-1:0]		head_pointer2;
input[`CRC_DIRECTORY_WIDTH-1:0]		tail_pointer1;
input[`CRC_DIRECTORY_WIDTH-1:0]		tail_pointer2;

input[`CRC_KEY_SIZE-1:0]			pause_task1;
input[`CRC_KEY_SIZE-1:0]			pause_task2;
wire[kw:0]				task_check_i;			
reg[3:0]				state;
wire					fprints_match;
input					irq_in;
input[(`CRC_KEY_SIZE-1):0] fprints_ready1;	
input[(`CRC_KEY_SIZE-1):0] fprints_ready2;
output reg				   fprints_checked;
input 						dir_w_ack1;
input 						dir_w_ack2;
reg 						dir_w_ack1_trigger;
reg 						dir_w_ack2_trigger;
output 						exception_register_write;
input 						csr_read;
input 						csr_write;
wire[`CRC_KEY_SIZE-1:0]	pause_task_n;
assign pause_task_n = ~(pause_task1 | pause_task2);
/*----------------------------------------------------------------------------
0: Highest priority
15: Lowest priority
16: No task 
When a task is checked out from both processors, it begins being compared.
More advanced prioritization can be achieved by giving priority to any task
that has been checked in on both processors, for instance.
-------------------------------------------------------------------------------*/
wire[`CRC_KEY_SIZE-1:0] ch_out;
assign ch_out = ch_out_reg1 & ch_out_reg2;
wire[`CRC_KEY_SIZE-1:0] ch_in;
assign ch_in = ch_in_reg1 & ch_in_reg2;
wire[`CRC_KEY_SIZE-1:0] fprints_ready;
assign fprints_ready = fprints_ready1 & fprints_ready2;


assign task_check_i = fprints_ready[0] | ch_in[0] ? 0 : fprints_ready[1] | ch_in[1] ? 1 : 
					fprints_ready[2] | ch_in[2] ? 2 : fprints_ready[3] | ch_in[3] ? 3 :
					fprints_ready[4] | ch_in[4]?  4 : fprints_ready[5] | ch_in[5] ? 5 :
					fprints_ready[6] | ch_in[6]? 6 : fprints_ready[7] | ch_in[7] ? 7 :
					fprints_ready[8] | ch_in[8]? 8 : fprints_ready[9] | ch_in[9] ? 9 :
					fprints_ready[10] | ch_in[10]? 10 : fprints_ready[11] | ch_in[11] ? 11 :
					fprints_ready[12] | ch_in[12]? 12 : fprints_ready[13] | ch_in[13] ? 13 :
					fprints_ready[14] | ch_in[14]? 14 : fprints_ready[15] | ch_in[15] ? 15 :
					16; //takes 16 when no task is checked out.
					
//current task is registered to avoid chance of race conditions.
					

assign fprints_match = (crc_in1[cw-1:0] == crc_in2[cw-1:0]);

					
parameter 			init 					= 0; 
parameter 			set_dir_adr 			= 1;
parameter			load_ram_adr			= 2;
parameter			test_keys 				= 3;
parameter			increment_ram_adr 		= 4;
parameter			collision_occurred 		= 5;
parameter			reset_key	 			= 6;
parameter			task_verified 			= 7;
parameter			pause 					= 8;
parameter 			irq_wait 				= 9;
parameter			increment_ram_adr_0		= 10;
parameter			load_wait1				= 11;
parameter			set_task				= 12;
parameter			complete 				= 13;
parameter 			load_wait2				= 14;



always @(posedge clk or posedge rst)begin
	if(rst)begin
		dir_w_ack1_trigger = 0;
		dir_w_ack2_trigger = 0;
	end else begin
		if(dir_w_ack1)
			dir_w_ack1_trigger <= 1;
		if(dir_w_ack2)
			dir_w_ack2_trigger <= 1;

		if(state == complete)begin
			dir_w_ack1_trigger <= 0;		
			dir_w_ack2_trigger <= 0;
		end
	end
end
//Deal with pause only frome init or test. All other states
//are steps of atomic processes that should not be interrupted

always @ (posedge clk or posedge rst)
begin
	if (rst) begin
		state = init;
	end else begin
		fprints_checked = 0;
		case (state)
			init:
				//If MSB of task_check_i = 0 -> proceed to next step
				//If MSB of task_check_i = 1 -> No task ready for comparison
				//If ch_in then a task has completed
				if(~task_check_i[kw])begin
					state = set_task;
				end else
					state = init;
			set_task:
				state = set_dir_adr;
			set_dir_adr:
				//Read pointers are retrieved from directory in this cycle
				state = load_ram_adr;
			load_ram_adr:
				//Data is retrieved from CRC buffer at pointer address retrieved from directory in this cycle
				state = load_wait1;
			load_wait1:
				state = load_wait2;
			load_wait2:
				state = test_keys;
			test_keys:
				//If both tasks have completed
				if(ch_in[current_task])begin
					//If the head pointers do not match then there are different numbers of checksums - collision
					if(head_pointer1 != head_pointer2)begin
						state = collision_occurred;
					end else begin
						//If the buffer has been cleared then the task is verified
						if(tail_pointer1 == head_pointer1)begin
							state = task_verified;
						end else begin //else the buffer has not been cleared
							//If the crcs match, then the next comparison can take place
							if(fprints_match)begin
								state = increment_ram_adr;
							//else crcs do not match so collision occurred. 
							end else begin
								state = collision_occurred;
							end
						end
					end
				end else begin //If at least one task has not completed
					//If either buffer is empty then wait
					if((tail_pointer1 == head_pointer1) || (tail_pointer2 == head_pointer2))begin
					//Clear the fprint_ready_reg
					fprints_checked = 1;
						state = increment_ram_adr_0;
					//If the crcs match, then the next comparison can take place
					end else begin
						if(fprints_match)begin
							state = increment_ram_adr;
						//else crcs do not match so collision occurred. 
						end else begin
							state = collision_occurred;
						end
					end
				end
				
				//Blindspot: Both processors have the same corrupted bit in the task key
				//This is a multievent scenario. Disregarded for time being.
	
			increment_ram_adr:
			//Needed to add an extra clock cycle because timing changed after making switches
			//to directory so that it would synthesize the pointers in SRAM instead of registers
				state = increment_ram_adr_0;
			increment_ram_adr_0:
				state = init;
			collision_occurred:
				//Comparator attempts to write to exception register. This register should be moved to this file?
				//It waits until both registers have successfully written to leave this state.
				if(dir_w_ack1_trigger & dir_w_ack2_trigger & ~csr_write & ~csr_read)
					state = irq_wait;
				else
					state = collision_occurred;
			irq_wait:
				if(irq_in)
					state = irq_wait;
				else
					state = init;
			task_verified:
				//Forward key to I/O buffer and assert release signal
				state = reset_key;
			reset_key:
				//Check in and check out registers are updated. Wait in this state until both operations
				//are successful. If registers are being written by processor, wait.
				if (dir_w_ack1_trigger & dir_w_ack2_trigger & ~csr_write & ~csr_read)
					state = complete;
				else
					state = reset_key;
			complete:
				if(~irq_in)
					state = init;
			default:
				state = init;
		endcase
	end
end


assign exception_register_write = ((state == reset_key) || (state == collision_occurred))
								 & dir_w_ack1_trigger & dir_w_ack2_trigger;
//Signal to increment RAM address by 1
assign increment_dir_address = (state == increment_ram_adr);
//Signal to set ram address to pointer stored in directory
//Data line already has the desired key number loaded
assign ram_adr_load = (state == load_ram_adr);

always @ (posedge clk or posedge rst)
begin
	if(rst)
		current_task = 'h10;
	else begin
		if (state == set_task)
			current_task = task_check_i	;
		else if (state == init)
			current_task = 'h10;
	end
		
end



//Stores the key for the current task being compared
assign dir_adr_r = current_task[kw-1:0];
//Write to control registers in reset key or collision states. Can only proceed when neither processor is writing to a control register
assign spr_we_o = ((state==reset_key) || (state == collision_occurred));
//Signal that a collision has occurred. Not used currently
assign comp_col_ex = (state == collision_occurred);
//Signal to I/O buffer that data is safe to release
assign io_release = (state == task_verified);


endmodule
