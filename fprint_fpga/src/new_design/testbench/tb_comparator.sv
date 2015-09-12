`include "crc_defines.v"

`define BYTE 8
`define CLK_PERIOD 10

module tb_comparator();


reg											clk;
reg											reset;



//testbench internals

reg											init_complete;

/******************************
Initial
*******************************/
initial begin
	
	init_complete							= 0;
	
	clk										= 0;

	
	reset = 1;
	#`CLK_PERIOD
	reset = 0;
	
	init_complete = 1;

end


always begin
        clk = #((`CLK_PERIOD)/2) ~clk;
end



/*****************************************************************
Things to test:
	1. 
		a.

	2. 
		a.
******************************************************************/

always @(posedge init_complete)begin
	
	/**1. a**/

	
	$display("Finished test 1. a\t time: %d", $time);
	
	/**1. b**/

	$display("Finished test 1. b\t time: %d", $time);
	
	
	/**1. c**/

	$display("Finished test 1. c\t time: %d", $time);
	
	/**1. d**/

	$display("Finished test 1. d\t time: %d", $time);
	

	//finishing
	$display("Finished all tests, stopping\t time: %d", $time);
	$stop;
end

endmodule
