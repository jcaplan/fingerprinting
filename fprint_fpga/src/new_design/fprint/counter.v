`include "defines.v"
module counter(
			clk,rst,count_reset,en,mode,
			q,
			carry,
			pause_store,
			unpause_load,
			unpause_popstack,
			counter_max_set,
			counter_max_value
);


//mode = 1 : +2, mode = 0; +1
input clk;
input rst;
input count_reset;
input en;
input mode;
input	pause_store;
input	unpause_load;
input	unpause_popstack;
input	counter_max_set;
input[`COUNTER_WIDTH - 1 : 0] counter_max_value;

parameter w = `COUNTER_WIDTH;
//CHANGE BACK TO 10


output[w-1:0] q;
reg[w-1:0] q;
reg[w-1:0] old_count;
wire[w-1:0] stack_count;
wire[w-1:0] stack_count_limit;
wire max_min_one;
wire max;
output carry;
reg[`COUNTER_WIDTH - 1 : 0 ] count_limit;


assign max_min_one = (q == count_limit - 1);
assign max = (q == count_limit);
assign carry = 0;//mode ? max_min_one : max;
always @ (posedge clk or posedge count_reset)
begin
	if (count_reset)begin
		q = 0;
		old_count = 0;
	end else begin
		//Increment by 2 or 1
		if(en == 1) begin
			old_count = q;
			q = max | (max_min_one & mode) ? 0 : (q + (mode ? 2 : 1));
		end else if(unpause_load) begin
			old_count = stack_count;
			q = stack_count;
		end			
	end
end

always @ (posedge clk or posedge rst)
begin
	if(rst)begin
		count_limit = `COUNTER_MAX_DEFAULT;
	end else begin
		if(counter_max_set)
			count_limit = counter_max_value;
		else if(unpause_load) 
			count_limit = stack_count_limit;
	end
end

lifo 
  #(
    .depth(`PAUSE_DEPTH),
    .width(w)
    )paused_counter_stack
  (
   .clk(clk),
   .reset(rst),
   .push(pause_store),
   .push_data(old_count),
   .pop(unpause_popstack),
   .tos(stack_count) //top of stack
);


lifo 
  #(
    .depth(`PAUSE_DEPTH),
    .width(w)
    )paused_counter_limit_stack
  (
   .clk(clk),
   .reset(rst),
   .push(pause_store),
   .push_data(count_limit),
   .pop(unpause_popstack),
   .tos(stack_count_limit) //top of stack
);
endmodule
