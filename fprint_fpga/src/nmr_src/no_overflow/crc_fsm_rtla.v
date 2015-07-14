`include "crc_defines.v"
module crc_fsm_rtla(
			input clk, 
			input rst, 
			input wrte, 					//Write from processor data bus
			input count_max,				//CRC block is full
			input en_fprint,				//Fingerprinting is enabled
			output en_fcs, 					//
			output sel, 
			output crc_ready,
			input  pause_reset,
			input  unpause_setfsm,
			input[`COUNTER_WIDTH-1:0] count

);

/******************************
FSM STATE Parameters
*******************************/
parameter init=2'd0,write=2'd1,block_done=2'd2,idle=2'd3,first_write=4,task_done=5;
reg[2:0] state;
reg[2:0] last_state;
reg		 count_max_reg;




always @ (posedge clk or posedge rst or posedge pause_reset)
begin
	if (rst | pause_reset) begin
		last_state = 0;
		count_max_reg = 0;
	end else begin
	last_state = state;
	count_max_reg = count_max;
	end
end
	

always @ (posedge clk or posedge rst or posedge pause_reset)
begin
	if (rst | pause_reset) begin
		state = init;
	end else begin
	
			
		case (state)
			init:
				//Go to write state when write is asserted
				if(unpause_setfsm)begin
					
					//If this is not the first go to idle
					//else stay in init
					if(count == 0)
						state = init;
					else
						state = idle;
					
				
				end else if(en_fprint)begin
					if(wrte)
						state = first_write;
					else
						state = init;
				end		
			first_write:	
				if(wrte)
					state = write;
				else
					state = idle;
			write:
				//if count_max and write, then block_done
				//if not count_max and write, then write
				//else idle
				if(count_max_reg | (count_max & (last_state == write)))begin
					if(wrte)begin
							state = first_write;
					end else
						state = init;
				end else begin
					if(wrte)
						state = write;
					else
						state = idle;
				end			
			block_done:
				//If the first block of the next fingerprint occurs immediately after the last block
				//Then return to write state, bypass init state
				if(wrte)
					state = write;
				else
					state = idle;
			task_done:
				state = init;
			idle:
				//if count max go to block_done state on write
				//if not count max, go to write state on write
				//otherwise remain in idle state
				if(!en_fprint)
					state = task_done;
				else if(count_max)begin
					if(wrte)
						state = write;
					else
						state = idle;
				end else begin
					if(wrte)
						state = write;
					else
						state = idle;
				end
			default:
				state = init;
		endcase	
	end
end

/*****************************
FSM output signal assignments
******************************/

assign en_fcs = (state == write) | (state == first_write) | (state == block_done);

assign sel = (state == write) | ((state == idle) && (~count_max | en_fprint));
assign crc_ready = (state == block_done) | (state == task_done) | ((state == write) && (count_max_reg | (count_max & (last_state == write))));

endmodule
