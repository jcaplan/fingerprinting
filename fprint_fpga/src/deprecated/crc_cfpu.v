`include "crc_defines.v"

module crc_cfpu(
	input															clk,
	input															reset,

	input 		[(`COMPARATOR_ADDRESS_WIDTH-1):0]					fprint_address,
	input															fprint_write,
	input 		[(`NIOS_DATA_WIDTH-1):0]							fprint_writedata,
	output															fprint_waitrequest,

	input 		[(`COMP_CSR_WIDTH-1):0]								csr_address,
	input															csr_read,
	output wire [(`NIOS_DATA_WIDTH-1):0]							csr_readdata,
	input															csr_write,
	input 		[(`NIOS_DATA_WIDTH-1):0]							csr_writedata,
	output wire														csr_waitrequest,
	
	output wire														irq,
	output wire														io_release
);


//******************************************
//Signal declarations
//******************************************

wire 											comparator_increment_ram_address;
wire [(`CRC_KEY_WIDTH-1):0]						comparator_current_task;
wire 											comparator_register_write;
wire [(`CRC_KEY_SIZE-1):0] 						ch_in_reg[`MAX_REDUNDANCY-1:0];
wire [(`CRC_KEY_SIZE-1):0] 						ch_out_reg[`MAX_REDUNDANCY-1:0];

wire 		 									comparator_collision_detected;
wire [(`CRC_RAM_ADDRESS_WIDTH-1):0]				head_pointer[`MAX_REDUNDANCY-1:0];
wire [(`CRC_RAM_ADDRESS_WIDTH-1):0]				tail_pointer[`MAX_REDUNDANCY-1:0];
wire [(`CRC_KEY_SIZE-1):0]						pause_reg[`MAX_REDUNDANCY-1:0];
wire [(`CRC_WIDTH-1):0] 						crc_out[`MAX_REDUNDANCY-1:0];
wire											pause_strobe[`MAX_REDUNDANCY-1:0];
reg [(`COMPARATOR_EXCEPTION_REG_WIDTH-1):0] 	exception_register;
reg [(`CRC_KEY_SIZE-1):0] 						success_reg;
reg [(`CRC_KEY_SIZE-1):0] 						fail_reg;

wire 											exception_reg_sel_a;
wire 											exception_reg_sel;
wire 											comp_reset;

reg [(`COMPARATOR_ADDRESS_WIDTH-1):0]  			address;
reg [(`NIOS_DATA_WIDTH-1):0] 					data;


wire											fprint_overflow[`MAX_REDUNDANCY-1:0];
reg												fprint_overflow_reg[`MAX_REDUNDANCY-1:0];
wire 											overflow_occurred;
wire [`CRC_KEY_WIDTH-1:0]						current_task_ex[`MAX_REDUNDANCY-1:0];

//wire 											slave_write[`MAX_REDUNDANCY-1:0];
wire 											csr_write_i[`MAX_REDUNDANCY-1:0];
wire [(`CRC_KEY_SIZE-1):0]                      fprints_ready[`MAX_REDUNDANCY-1:0];
wire 											fprints_checked;
wire											waitrequest[`MAX_REDUNDANCY-1:0];
wire 											dir_comp_w_ack[`MAX_REDUNDANCY-1:0];
wire 											comp_exception_reg_write;

wire success_reg_sel_a;
wire fail_reg_sel_a;
//assign fifo_in_waitrequest = 0;
assign csr_waitrequest = 0;


assign exception_reg_sel_a  = (csr_address == `COMPARATOR_EXCEPTION_OFFSET);
assign success_reg_sel_a    = (csr_address == `COMPARATOR_SUCCESS_REG_OFFSET);
assign fail_reg_sel_a       = (csr_address == `COMPARATOR_FAIL_REG_OFFSET);
assign exception_reg_sel  = csr_write & exception_reg_sel_a;
assign csr_readdata = (exception_reg_sel_a && exception_register) | (success_reg_sel_a && success_reg)
							| (fail_reg_sel_a && fail_reg);
assign irq = exception_register[`EXCEPTION_REG_INTERRUPT_BIT];
assign fprint_waitrequest = waitrequest[0] & waitrequest[1]	;

//Truth table for waitrequest
//0 0 -> x
//0 1 -> 0
//1 0 -> 0
//1 1 -> 1

//******************************************
//State logic
//******************************************

//Exception register is in common territory so the state logic
//should ignore the write when it is to the exception register
//or else it will mistake it for a write to the start or end directory.


//Need to examine the upper bits of the address to determine which core is sending data... 
//or do we split the data up and use the upper 4 MSBs to indicate the core...
//address is already 4x...

//1.Need to read from the fifo
//1a.Get the address
//1b.get the data


//When the fifo is no longer empty we get the address.
//On the next clock cycle we get the data
//The next clock cycle after that we activate the signal
//for the appropriate core to update its state

always@(posedge clk or posedge reset)begin
	if(reset)begin
		address = 0;
		data = 0;
	end else begin
		if(fprint_write)begin
			address = fprint_address;
			data = fprint_writedata;
		end
	end
end




//******************************************
//Exception Register
//******************************************

generate
	genvar i;
	for(i = 0; i < `MAX_REDUNDANCY; i = i + 1)begin : overflow
		always @ (posedge clk or posedge reset)begin
			if(reset)begin
				fprint_overflow_reg[i] = 0;
			end else begin
				if(fprint_overflow[i])
					fprint_overflow_reg[i] = 1;
				else if(exception_register[`EXCEPTION_OVERFLOW_BIT])
					fprint_overflow_reg[i] = 0;
			end
		end
	end	

	//How to generalize this line?
	assign overflow_occurred = fprint_overflow_reg[0] | fprint_overflow_reg[1];

endgenerate


//if many tasks finish in quick succession, problem for processor to read register
//When a task is finished the comparator should go into a wait stait until the interrupt is serviced
always @ (posedge clk or posedge reset)
begin
	if(reset)begin
		exception_register = 0;
		success_reg = 0;
		fail_reg = 0;
	end else begin
		if(comp_exception_reg_write)begin
			exception_register[`EXCEPTION_REG_INTERRUPT_BIT] = 1;
			exception_register[`EXCEPTION_REG_TASK_BITS] = comparator_current_task;
			exception_register[`EXCEPTION_REG_EX_BIT] = comparator_collision_detected;
			success_reg[comparator_current_task] = ~comparator_collision_detected;
			fail_reg[comparator_current_task]    = comparator_collision_detected;
		end else if(exception_reg_sel)begin
			exception_register = csr_writedata;
			success_reg = 0;
			fail_reg = 0;
		end else if(overflow_occurred)begin
			exception_register[`EXCEPTION_REG_INTERRUPT_BIT] = 1;
			exception_register[`EXCEPTION_REG_EX_BIT] = 0;
			exception_register[`EXCEPTION_OVERFLOW_BIT] = 1;			
			exception_register[`EXCEPTION_REG_TASK_BITS] = 0;
			exception_register[`EXCEPTION_CPUID_BIT] = 0;
			
		end
	end
end


//******************************************
//State logic
//******************************************


//Need to do core translation business out here somehow...
generate
	assign comp_reset = reset | pause_strobe[0] | pause_strobe[1];
	for(i = 0; i < `MAX_REDUNDANCY; i = i + 1)begin : state_regs
		//assign comp_reset = comp_reset | pause_strobe[i];
		//assign slave_write[i] = (fprint_write) & (fprint_address[7:4] == i);
		assign csr_write_i[i] = csr_write & (csr_address[9:6] == i);

		crc_state_regs state_regs(
			.clk(clk),
			.reset(reset),
			.data_queue_rdata(data),
			.address_queue_rdata(address),
			.slave_write(fprint_write),
			.csr_data(csr_writedata),
			.csr_address(csr_address),
			.csr_write(csr_write_i[i]),
			.crc_out(crc_out[i]),
			.checkout(ch_out_reg[i]),
			.checkin(ch_in_reg[i]),
			.pause(pause_reg[i]),
			.pause_strobe(pause_strobe[i]),
			.head_pointer(head_pointer[i]),
			.tail_pointer(tail_pointer[i]),
			.comparator_increment_ram_address(comparator_increment_ram_address),
			.comparator_current_task(comparator_current_task),
			.comparator_register_write(comparator_register_write),
			.comparator_collision_detected(comparator_collision_detected),
			.fprint_overflow(fprint_overflow[i]),
			.current_task_ex(current_task_ex[i]),
			.fprints_ready_reg(fprints_ready[i]),
			.fprints_checked(fprints_checked),
			.waitrequest(waitrequest[i]),
			.comp_w_ack(dir_comp_w_ack[i])
		);	
	end
endgenerate



//******************************************
//Comparator
//******************************************

crc_comparator cmp(
	.clk(clk),
	.rst(reset),
	.increment_dir_address(comparator_increment_ram_address), 							// output - increment ram address
	.crc_in1(crc_out[0]), 												//crc0 in
	.crc_in2(crc_out[1]),													//crc1 in
	.dir_adr_r(comparator_current_task),												//current task being compared
	.spr_we_i1(fprint_write),				//slave0 write (in case cpu and comparator want access to status registers)
	.spr_we_i2(fprint_write),				//slave1 write
	.spr_we_o(comparator_register_write),					//comparator wants address to a control register (which?)
	.ch_in_reg1(ch_in_reg[0]),				//check in register 0
	.ch_in_reg2(ch_in_reg[1]),				//check in register 1
	.ch_out_reg1(ch_out_reg[0]),				//check out register 0
	.ch_out_reg2(ch_out_reg[1]),				//check out register 1
	.comp_col_ex(comparator_collision_detected),				//high for 1 cycle on detection of error
	.io_release(io_release),				//comparison successful
	.head_pointer1(head_pointer[0]),			//head pointer 0
	.tail_pointer1(tail_pointer[0]),			//tail pointer 0
	.head_pointer2(head_pointer[1]),			//head pointer 1
	.tail_pointer2(tail_pointer[1]),			//tail pointer 1

	.pause_task1(pause_reg[0]),				//pause reg0
	.pause_task2(pause_reg[1]),				//pause reg1
	.irq_in(irq),
	.fprints_ready1(fprints_ready[0]),
	.fprints_ready2(fprints_ready[1]),
	.fprints_checked(fprints_checked),
	.dir_w_ack1(dir_comp_w_ack[0]),
	.dir_w_ack2(dir_comp_w_ack[1]),
	.exception_register_write(comp_exception_reg_write),
	.csr_write(csr_write),
	.csr_read(csr_read)
);

	endmodule
