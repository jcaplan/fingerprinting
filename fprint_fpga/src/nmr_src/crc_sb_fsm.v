
module crc_sb_fsm(
			input 				clk, 
			input 				rst, 
			input 				task_begin,
			input 				task_complete,
			input 				count_max,
			input 				wrte,
			output wire[1:0] 	sb_mux_sel,
			output wire 		sb_write,
			input 				pause_reset,
			input 				unpause_setfsm

);

/******************************
FSM STATE Parameters
*******************************/
parameter init = 0;
parameter wait_1 = 1;
parameter store_cs_1 = 2;
parameter wait_2 = 3;
parameter store_crc = 4;
parameter store_last_crc = 5;
parameter store_cs_2 = 6;
parameter pause = 7;
parameter unpause = 8;
reg[3:0] state;
reg wrte_reg;





always @ (posedge clk or posedge rst)
begin
	if (rst == 1) begin
		state = init;
		wrte_reg = 0;
	end else begin
	
		case (state)
			init:
				if(task_begin)
					state = store_cs_1;
				else if(unpause_setfsm)
					state = wait_2;
				else
					state = init;
			store_cs_1:
				state = wait_2;
			wait_2:
				if(count_max & (wrte | wrte_reg))
					state = store_crc;
				else if(task_complete)
					state = store_last_crc;
				else if(pause_reset)
					state = init;
				else
					state = wait_2;
			store_crc:
				if(task_complete)
					state = store_last_crc;
				else
					state = wait_2;
			store_last_crc:
				state = store_cs_2;
			store_cs_2:
				state = init;
				
			pause:
				state = init;			
			unpause:
				state = wait_2;
			default:
				state = init;
		endcase
		wrte_reg = wrte;
	end
end

/*****************************
FSM output signal assignments
******************************/
assign sb_mux_sel = ((state == store_last_crc) | (state == store_crc)) ? 1 : (state == pause) ? 2 : (state == unpause) ?  3 : 0;
assign sb_write =  ~((state == init) | (state == wait_2));


endmodule
