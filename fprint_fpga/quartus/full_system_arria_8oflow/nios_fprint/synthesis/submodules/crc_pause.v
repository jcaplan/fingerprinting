`include "crc_defines.v"
module crc_pause(
	
	input 									clk,
	input 									rst,
	input									pause_reg,
	input									unpause_reg,
	input									fprint_enable,
	output									pause_store,
	output									pause_reset,
	output									waitrequest,
	output									unpause_load,
	output									unpause_setfsm_popstack,
	input									empty_n,
	input [`CRC_KEY_SIZE-1:0]				pause_task_reg,
	input [`CRC_KEY_WIDTH-1:0]				paused_cs_tos
);
	
	
	
	
/******************************
FSM STATE Parameters
*******************************/
parameter init = 0;
parameter store = 1;
parameter reset = 2;
parameter pause_release = 3;
reg[1:0] pause_state;

/******************************************************
Pause Loop
*******************************************************/


always @ (posedge clk or posedge rst)
begin
	if (rst == 1) begin
		pause_state = init;
	end else begin
		case (pause_state)
			init:
				if(pause_reg & fprint_enable)
					pause_state = store;
				else
					pause_state = init;
			store:
				pause_state = reset;
			reset:
				pause_state = pause_release;
			pause_release:
				pause_state = init;
			default:
				pause_state = init;
		endcase
	end
end

/******************************************************
Unpause Loop
*******************************************************/

parameter load = 1;
parameter setfsm = 2;
parameter unpause_release = 3;
reg[1:0] unpause_state;

always @ (posedge clk or posedge rst)
begin
	if (rst == 1) begin
		unpause_state = init;
	end else begin
		case (unpause_state)
			init:
				if(unpause_reg & empty_n & ~pause_task_reg[paused_cs_tos])   //add condition that if top of stack points to a bit that is high in the pause_task_reg, then ignore the strobe
					unpause_state = load;
				else
					unpause_state = init;
			load:
				unpause_state = setfsm;
			setfsm:
				unpause_state = unpause_release;
			unpause_release:
				unpause_state = init;
			default:
				unpause_state = init;
		endcase
	end
end


/******************************************************
COMBINATIONAL ASSIGNMENTS
*******************************************************/
assign pause_store				= (pause_state == store);
assign pause_reset				= (pause_state == reset);
assign unpause_load				= (unpause_state == load);
assign unpause_setfsm_popstack	= (unpause_state == setfsm);
assign waitrequest				= (pause_state == pause_release) | (unpause_state == unpause_release);
endmodule

