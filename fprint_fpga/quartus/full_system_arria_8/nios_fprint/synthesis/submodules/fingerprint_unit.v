// fingerprint_unit.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`include "crc_defines.v"
`timescale 1 ps / 1 ps
module fingerprint_unit #(
		parameter AUTO_CLOCK_CLOCK_RATE = "-1"
	) (
					
			clk,
			reset,
			//Control register avalon memory mapped 
			avs_s0_write,		//avs_s0_write
			avs_s0_read,		//avs_s0_read	
			avs_s0_address,		//avs_s0_address
			avs_s0_writedata,		//avs_s0_writedata
			avs_s0_readdata,		//avs_s0_readdata
		
			avs_s1_write,		//avs_s1_write
			avs_s1_read,		//avs_s1_read
			avs_s1_address,		//avs_s1_address
			avs_s1_writedata,		//avs_s1_writedata
			avs_s1_readdata,		//avs_s1_readdata
			
			//Signals from processor master, bypassing translation
			wrte0,
			wrte1,
			write_data0,
			write_data1,
			address_0,
			address_1,
			irq_out,
			//Output to I/O Buffer
			io_release,
			io_key,
			active_task0
			//active_task1
			);


input 									clk;	
input 									reset;
input[31:0]								write_data0,write_data1;
input[26:0]								address_0,address_1;
input 									wrte0,wrte1; 						//Write signal directly from processor master (Untranslated)

input 									avs_s0_write,avs_s1_write;			//Write signal for control registers 
input[31:0]								avs_s0_address,avs_s1_address;		//Address signal for control registers
input[31:0] 							avs_s0_writedata,avs_s1_writedata;	//Write data for control registers
output[31:0] 							avs_s0_readdata,avs_s1_readdata;	//Read data for control registers
input									avs_s0_read, avs_s1_read;			//Read signal for control registers

output									irq_out;

wire 									d_ack;								//Signal from comparator to buffer to increase read address by 1
wire[36:0] 	crc_out0,crc_out1;												//CRC output from fingerprint buffer to comparator
//wire 									ram_addr_load;						//Signal from comparator to change buffer read address to signal forwarded from directory
wire[`CRC_KEY_WIDTH-1:0] 				comp_dat_o;							//Signal from comparator, carries key of task being compared. 
wire		 							comp_spr_we;						//Signal fro comparator, to write to control registers
wire[`CRC_KEY_SIZE-1:0] 				ch_in_reg_0,ch_in_reg_1;			//Check in registers, read by comparator
wire[`CRC_KEY_SIZE-1:0]					ch_out_reg_0,ch_out_reg_1;			//Check out registers, read  by comparator
wire[`CRC_KEY_SIZE:0]					currentstate_reg_0,currentstate_reg_1;	//Current state registers, used in pause logic
wire									comp_col_ex;						//Signal from comparator that exception has occurred
wire[`CRC_DATA_WIDTH-1:0] 				d_in1,d_in2;						//Write data directly from processor master (Untranslated)


wire[`CRC_DIRECTORY_WIDTH-1:0]		head_pointer1;
wire[`CRC_DIRECTORY_WIDTH-1:0]		head_pointer2;
wire[`CRC_DIRECTORY_WIDTH-1:0]		tail_pointer1;
wire[`CRC_DIRECTORY_WIDTH-1:0]		tail_pointer2; 
 
output									io_release;							//Signal to I/O Buffer to release data associated with task
output [`CRC_KEY_WIDTH-1:0]				io_key;								//Key associated with data to be released.
output[`CRC_KEY_WIDTH:0]				active_task0;//,active_task1;


wire 								pause_sel_a1;
wire 								pause_sel1;
wire 								pause_sel_a2;
wire 								pause_sel2;
wire 								unpause_sel_a1;
wire 								unpause_sel1;
wire 								unpause_sel_a2;
wire 								unpause_sel2;

wire								pause_en1;
wire								pause_en2;
wire								pause_end1;
wire								pause_end2;
wire[`CRC_KEY_SIZE-1:0]				pause_task1;
wire[`CRC_KEY_SIZE-1:0]				pause_task2;

wire								unpause_cs_write1;				//restore current state
wire								unpause_crc_write1;
wire								unpause_counter_write1;
wire[31:0]							unpause_data1;
	
wire								unpause_cs_write2;				//restore current state
wire								unpause_crc_write2;
wire								unpause_counter_write2;
wire[31:0]							unpause_data2;


wire[`CRC_WIDTH-1:0]					crc_out_i1,crc_out_i2;
wire[`CRC_COUNTER_WIDTH-1:0]		crc_count1,crc_count2;

assign d_in1 = {address_0,write_data0};
assign d_in2 = {address_1,write_data1}; 
assign io_key = comp_dat_o;


assign pause_sel_a1 = (avs_s0_address == `FPRINT_SPR_PAUSE);
assign pause_sel1 = (avs_s0_read && pause_sel_a1);
assign pause_sel_a2 = (avs_s1_address == `FPRINT_SPR_PAUSE);
assign pause_sel2 = (avs_s1_read && pause_sel_a2);

assign unpause_sel_a1 = (avs_s0_address == `FPRINT_SPR_UNPAUSE);
assign unpause_sel1 = (avs_s0_read && unpause_sel_a1);
assign unpause_sel_a2 = (avs_s1_address == `FPRINT_SPR_UNPAUSE);
assign unpause_sel2 = (avs_s1_read && unpause_sel_a2);

wire count_mode;
assign count_mode = 1;
crc_fingerprint fprint1(
	.clk(clk),
	.rst(reset),
	.wrte(wrte0),
	.count_mode(count_mode),
	.d_ack(d_ack),
	.d_in(d_in1),
	.crc_out(crc_out0),
	.spr_write(avs_s0_write),
	.spr_read(avs_s0_read),
	.spr_addr(avs_s0_address),
	.spr_dat_i(avs_s0_writedata),
	.spr_dat_o(avs_s0_readdata),	
	.comp_dat_i(comp_dat_o),
	.comp_spr_we(comp_spr_we),
	.ch_in_reg_o(ch_in_reg_0),
	.ch_out_reg_o(ch_out_reg_0),
	.currentstate_reg_o(currentstate_reg_0),
	
	.unpause_cs_write(unpause_cs_write1),				//restore current state
	.unpause_crc_write(unpause_crc_write1),
	.unpause_counter_write(unpause_counter_write1),
	.unpause_data_in(unpause_data1),
	
	.pause_task_i(pause_task1),
	.comp_col_ex(comp_col_ex),
	.active_task(active_task0),
	.irq_out(irq_out),
	.head_pointer_out(head_pointer1),
	.tail_pointer_out(tail_pointer1),
	.crc_old(crc_out_i1),
	.crc_count(crc_count1)
);

crc_fingerprint fprint2(
	.clk(clk),
	.rst(reset),
	.wrte(wrte1),
	.count_mode(count_mode),
	.d_ack(d_ack),
	.d_in(d_in2),
	.crc_out(crc_out1),
	.spr_write(avs_s1_write),
	.spr_read(avs_s1_read),
	.spr_addr(avs_s1_address),
	.spr_dat_i(avs_s1_writedata),
	.spr_dat_o(avs_s1_readdata),	
	//.ram_addr_load(ram_addr_load),
	.comp_dat_i(comp_dat_o),
	.comp_spr_we(comp_spr_we),
	.ch_in_reg_o(ch_in_reg_1),
	.ch_out_reg_o(ch_out_reg_1),
	.currentstate_reg_o(currentstate_reg_1),
	
	.unpause_cs_write(unpause_cs_write2),				//restore current state
	.unpause_crc_write(unpause_crc_write2),
	.unpause_counter_write(unpause_counter_write2),
	.unpause_data_in(unpause_data2),
	
	.pause_task_i(pause_task2),
	.comp_col_ex(comp_col_ex),
	.head_pointer_out(head_pointer2),
	.tail_pointer_out(tail_pointer2),
	.crc_old(crc_out_i2),
	.crc_count(crc_count2)
	//.active_task(active_task1)
);

crc_comparator cmp(
	.clk(clk),
	.rst(reset),
	.d_ack(d_ack),
	.crc_in1(crc_out0),
	.crc_in2(crc_out1),
	//.ram_adr_load(ram_addr_load),
	.dir_adr_r(comp_dat_o),
	.spr_we_i1(avs_s0_write),
	.spr_we_i2(avs_s1_write),
	.spr_we_o(comp_spr_we),
	.ch_in_reg1(ch_in_reg_0),
	.ch_in_reg2(ch_in_reg_1),
	.ch_out_reg1(ch_out_reg_0),
	.ch_out_reg2(ch_out_reg_1),
	.comp_col_ex(comp_col_ex),
	.io_release(io_release),
	.head_pointer1(head_pointer1),
	.tail_pointer1(tail_pointer1),
	.head_pointer2(head_pointer2),
	.tail_pointer2(tail_pointer2),
	.pause_in1(pause_en1),
	.pause_in2(pause_en2),
	.pause_task1(pause_task1),
	.pause_task2(pause_task2)
);

crc_pause p(
	.clk(clk),
	.rst(reset),
	.pause1(pause_en1),
	.pause2(pause_en2),
	.pause_strobe1(pause_sel1),
	.pause_strobe2(pause_sel2),
	.unpause_strobe1(unpause_sel1),
	.unpause_strobe2(unpause_sel2),
	.currentstate1(currentstate_reg_0),
	.currentstate2(currentstate_reg_1),
	.pause_task1_out(pause_task1),
	.pause_task2_out(pause_task2),
	.crc_out_i1(crc_out_i1),
	.crc_count1(crc_count1),
	.crc_out_i2(crc_out_i2),
	.crc_count2(crc_count2),
	
	
	.unpause_cs_write1(unpause_cs_write1),				//restore current state
	.unpause_crc_write1(unpause_crc_write1),
	.unpause_counter_write1(unpause_counter_write1),
	.unpause_data_out1(unpause_data1),
	.unpause_cs_write2(unpause_cs_write2),				//restore current state
	.unpause_crc_write2(unpause_crc_write2),
	.unpause_counter_write2(unpause_counter_write2),
	.unpause_data_out2(unpause_data2)
	);
	
	

endmodule

/*
fingerprint_unit #(
		.AUTO_CLOCK_CLOCK_RATE ("100000000")
	) fingerprint_unit_0 (
		.avs_s0_address     (fingerprint_unit_0_s0_translator_avalon_anti_slave_0_address),     //    s0.address
		.avs_s0_read        (fingerprint_unit_0_s0_translator_avalon_anti_slave_0_read),        //      .read
		.avs_s0_readdata    (fingerprint_unit_0_s0_translator_avalon_anti_slave_0_readdata),    //      .readdata
		.avs_s0_write       (fingerprint_unit_0_s0_translator_avalon_anti_slave_0_write),       //      .write
		.avs_s0_writedata   (fingerprint_unit_0_s0_translator_avalon_anti_slave_0_writedata),   //      .writedata
		//.avs_s0_waitrequest (fingerprint_unit_0_s0_translator_avalon_anti_slave_0_waitrequest), //      .waitrequest
		.clk                (clk_clk),                                             // clock.clk
		.reset              (rst_controller_005_reset_out_reset),                               // reset.reset
		.avs_s1_address     (fingerprint_unit_0_s1_translator_avalon_anti_slave_0_address),     //    s1.address
		.avs_s1_read        (fingerprint_unit_0_s1_translator_avalon_anti_slave_0_read),        //      .read
		.avs_s1_readdata    (fingerprint_unit_0_s1_translator_avalon_anti_slave_0_readdata),    //      .readdata
		.avs_s1_write       (fingerprint_unit_0_s1_translator_avalon_anti_slave_0_write),       //      .write
		.avs_s1_writedata   (fingerprint_unit_0_s1_translator_avalon_anti_slave_0_writedata),   //      .writedata
		//.avs_s1_waitrequest (fingerprint_unit_0_s1_translator_avalon_anti_slave_0_waitrequest),  //      .waitrequest
		
		.wrte0				(processor0_0_outgoing_master_write),
		.wrte1				(processor1_0_outgoing_master_write),
		.write_data0		(processor0_0_outgoing_master_writedata),
		.write_data1		(processor1_0_outgoing_master_writedata),
		.address_0			(processor0_0_outgoing_master_address),
		.address_1			(processor1_0_outgoing_master_address),
		.irq_out			(fingerprint_unit_0_irq_irq),
		
		.io_release         (fingerprint_unit_0_to_buffer_io_release),                          // to_buffer.export
		.io_key             (fingerprint_unit_0_to_buffer_io_key),                              //          .export
		.active_task0       (fingerprint_unit_0_to_buffer_active_task0)                         //          .export
	);

*/
