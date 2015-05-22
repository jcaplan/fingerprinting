
module lifo
  #(
    parameter depth = 32,
    parameter width = 32,
    parameter log2_depth = log2(depth),
    parameter log2_depthp1 = log2(depth+1)
    )
  (
   input clk,
   input reset,
   output reg empty_n,
  // output reg full,
  // output reg [log2_depthp1-1:0] count,
   input push,
   input [width-1:0] push_data,
   input pop,
   output [width-1:0] tos
   );

function integer log2;
    input [31:0] value;
    begin
		value = value-1;
		for (log2=0; value>0; log2=log2+1)
			value = value>>1;
		end
endfunction
//reg [width-1:0] tos_reg;
reg [depth - 1:0] ptr;
reg [width - 1:0] stack [depth-1:0];
reg[depth - 1:0] next_pointer_pop;
reg[depth - 1 :0] read_address; 		
always @(posedge clk or posedge reset) begin
	if (reset)begin
		ptr <= 0;
		next_pointer_pop = 0;
		empty_n = 0;
		read_address = 0;
	end else if(clk) begin			
		if (push)begin
			stack[ptr] <= push_data;
			next_pointer_pop = ptr;		
			ptr <= ptr+1;
			read_address = ptr;
		end else if (pop)begin
			ptr <= next_pointer_pop;
			read_address = next_pointer_pop-1;
			next_pointer_pop = next_pointer_pop	- 1;			
		end
		
		if(ptr == 0)
			empty_n = 0;
		else
			empty_n = 1;
	end
end

assign tos = stack[read_address];
endmodule
