`include "crc_defines.v"
module csr_decode(
	input										clk,
	input										reset,
	input 		[(`COMP_CSR_WIDTH-1):0]			csr_address,
	input 		[(`NIOS_DATA_WIDTH-1):0]		csr_writedata,

	
	output 							exception_reg_sel,
	output 							success_reg_sel,
	output 							fail_reg_sel,
	output 							start_p_sel,
	output 							end_p_sel,
	output							core_assignment_sel,
	output [(`CRC_KEY_WIDTH-1):0] 	dir_pointer_offset,
	output [3:0] 				  	core_id,
	output [3:0] 					core_assignment_offset,
	output [3:0] 					core_assignment_data
);

assign exception_reg_sel  		= (csr_address == `COMPARATOR_EXCEPTION_OFFSET);
assign success_reg_sel    		= (csr_address == `COMPARATOR_SUCCESS_REG_OFFSET);
assign fail_reg_sel       		= (csr_address == `COMPARATOR_FAIL_REG_OFFSET);
assign start_p_sel   			= (csr_address[`DIRECTORY_BITS] == 4'h1);   
assign end_p_sel     			= (csr_address[`DIRECTORY_BITS] == 4'h2);  
assign core_assignment_sel 		= (csr_address[5:0] == `COMPARATOR_CORE_ASSIGNMENT_OFFSET);
assign dir_pointer_offset 		= csr_address[(`CRC_KEY_WIDTH-1):0];
assign core_id 					= csr_address[9:6];
assign core_assignment_offset 	= csr_writedata[7:4];
assign core_assignment_data     = csr_writedata[3:0];

endmodule
