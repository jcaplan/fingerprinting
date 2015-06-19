`include "crc_defines.v"
module crc_fingerprint #(
	parameter CORE_ID = 0

)(
	input									clk,							//System clock
	input									rst,							//System reset
	input									waitrequest,					//Processor waitrequest
	input									wrte,							//write data
	input [58:0]							d_in,							//Write data from processor
	input 									slave_spr_read,
	input 									slave_spr_write,				//write for control registers
	input [(`FPRINT_SPR_ADDRESS_WIDTH-1):0]	slave_spr_addr,					//address for control registers
	input [(`NIOS_DATA_WIDTH-1):0]			slave_spr_dat_i,				//data write for control registers
	output[(`NIOS_DATA_WIDTH-1):0]			slave_spr_dat_o,				//data read line for control registers
	output									slave_waitrequest,
	
	output wire [(`NIOS_ADDRESS_WIDTH-1):0]	master_address,
	output wire								master_read,
	input									master_waitrequest,
	input 		[(`NIOS_DATA_WIDTH-1):0]	master_readdata,
	output wire								master_write,
	output wire	[(`NIOS_DATA_WIDTH-1):0]	master_writedata
//	output wire	[3:0]						master_byte_enable
);

/******************************************************
DEFINES
*******************************************************/

`define SB_MUX_CRC								1
`define SB_MUX_PAUSE							2
`define SB_MUX_UNPAUSE							3





/******************************************************
SIGNAL DECLARATIONS
*******************************************************/
wire 										sel;
wire 										en_fcs;
wire 										count_max;
wire 										crc_ready_i;
wire										count_en;
wire										spr_currentstate_sel;
wire										spr_currentstate_sel_a;
wire										pause_task_reg_sel;
wire										pause_task_reg_sel_a;
reg [`CRC_KEY_WIDTH:0]						spr_currentstate;
reg											task_complete;
reg											task_begin;
wire										count_reset;
wire										fingerprint_enable;
wire[`CRC_WIDTH-1:0]						crc_out_i;
wire[`NIOS_ADDRESS_WIDTH-1:0]				sb_write_address;
wire[`NIOS_DATA_WIDTH-1:0]					sb_write_data;
wire										sb_write;
wire[1:0]									sb_mux_sel;
wire										count_mode;
wire										fsm_en_fcs;
wire										wrte_i;
wire										pause_strobe;
wire										unpause_strobe;
reg											pause_reg;
reg											unpause_reg;
wire 										pause_store;
wire										pause_reset;
wire 										pause_waitrequest;
wire 										unpause_load;
wire 										unpause_setfsm_popstack;
wire 										cs_stack_empty_n;
wire[`CRC_KEY_WIDTH:0]						paused_cs_stack_data;
wire[`COUNTER_WIDTH-1:0]					count;
reg [`CRC_KEY_SIZE-1:0]						pause_task_reg;
wire										counter_max_sel_a;
wire										counter_max_sel;
//wire[`CRC_DATA_WIDTH-1:0]                   d_in;
/******************************************************
COMBINATIONAL ASSIGNMENTS
*******************************************************/
//assign d_in = {data_address,writedata};
assign fingerprint_enable		= spr_currentstate[`CRC_KEY_WIDTH];						//1 when Fingerprinting is enabled
assign spr_currentstate_sel_a 	= (slave_spr_addr == `FPRINT_SPR_CURRENTSTATE);			//Slave address points to current state register
assign spr_currentstate_sel 	= spr_currentstate_sel_a & slave_spr_write;				//Write to current state register
assign pause_task_reg_sel_a		= (slave_spr_addr == `FPRINT_PAUSE_TASK_REG);
assign pause_task_reg_sel		= pause_task_reg_sel_a & slave_spr_write;
assign counter_max_sel_a		= (slave_spr_addr == `COUNTER_MAX_OFFSET);
assign counter_max_sel			= (counter_max_sel_a & slave_spr_write);
assign count_en 				= en_fcs &  spr_currentstate[`CRC_KEY_WIDTH];			//Increase counter
assign count_reset 				= rst | task_begin | pause_reset;						//Reset counter
//Need a change here!!!!!!
//
assign slave_spr_dat_o 			= (spr_currentstate & {32{spr_currentstate_sel_a}}) |
									(pause_task_reg & {32{pause_task_reg_sel_a}})	;	//Slave data output
//
//


assign count_mode				= 1'h1; 												//Sets load or store. 1 = store. Only fingerprinting stores for now.													
assign en_fcs 					= fsm_en_fcs;// | task_complete; 							//Register CRC when block is full or task is done
assign sb_write_data			= (sb_mux_sel == `SB_MUX_CRC) ? crc_out_i : spr_currentstate;
assign sb_write_address			= `COMPARATOR_BASE_ADDRESS + (CORE_ID << 6) +
								 ((sb_mux_sel == `SB_MUX_CRC) ? `COMPARATOR_CRC_OFFSET_4x : 
									(sb_mux_sel == `SB_MUX_PAUSE) ? `COMPARATOR_PAUSE_OFFSET_4x :
										(sb_mux_sel == `SB_MUX_UNPAUSE) ? `COMPARATOR_UNPAUSE_OFFSET_4x : 0);			//Base + offset
//assign master_byte_enable		= 4'hf;
assign wrte_i					= wrte & ~waitrequest;									//Only want to read data once. Need to take into account
																						//Variable load times
assign pause_strobe				= (slave_spr_addr == `PAUSE_STROBE_OFFSET) && slave_spr_read;				//Pause strobe = read from address 8;		
assign unpause_strobe			= (slave_spr_addr == `UNPAUSE_STROBE_OFFSET) && slave_spr_read;															
//assign slave_waitrequest		= ~(pause_waitrequest | (pause_reg & ~fingerprint_enable) | (unpause_reg & ~cs_stack_empty_n));
//Also need to have waitrequest go low for writes to current state register. Might not even need a waitrequest signal. Probably just included it
//before I realized it was optional. There are no delays for current state and the pause strobe does not return any meaningful data anyway.
assign slave_waitrequest 		= 0;


																					
/*******************************************************
Sequential assignments
********************************************************/	   
always @ (posedge clk or posedge rst)
begin
	if (rst == 1)begin
		

		task_complete = 0;
		task_begin = 0;
		pause_reg = 0;
		unpause_reg = 0;
	end else begin
		task_complete = spr_currentstate[`CRC_KEY_WIDTH] & ~slave_spr_dat_i[`CRC_KEY_WIDTH]
					& spr_currentstate_sel;			
		task_begin = (~spr_currentstate[`CRC_KEY_WIDTH]) & slave_spr_dat_i[`CRC_KEY_WIDTH]
					& spr_currentstate_sel;
	
		
		if(pause_strobe)
			pause_reg = 1;
		else 
			pause_reg = 0;
			
		if(unpause_strobe)
			unpause_reg = 1;
		else
			unpause_reg = 0;
	end
end

always @ (posedge clk or posedge rst)
begin
	if (rst)begin
		spr_currentstate = 0;
	end else begin	
		if(spr_currentstate_sel)
			spr_currentstate = slave_spr_dat_i[`CRC_KEY_WIDTH:0];
		else if(pause_reset)
			spr_currentstate[`CRC_KEY_WIDTH] = 0;
		else if(unpause_load)
			spr_currentstate = paused_cs_stack_data;
	end
end

always @ (posedge clk or posedge rst)
begin
	if (rst)begin
		pause_task_reg = 0;
	end else begin	
		if(pause_task_reg_sel)
			pause_task_reg = slave_spr_dat_i[`CRC_KEY_SIZE-1:0];
	end
end


lifo 
  #(
    .depth(`PAUSE_DEPTH),
    .width(`CRC_KEY_WIDTH+1)
    )paused_cs_stack
  (
   .clk(clk),
   .reset(rst),
   .push(pause_store),
   .push_data(spr_currentstate),
   .pop(unpause_setfsm_popstack),
   .tos(paused_cs_stack_data), //top of stack
   .empty_n(cs_stack_empty_n)
);
   
   
/*******************************************************
CRC COMPRESSION 
********************************************************/
crc_rtla crc(
	.clk(clk),
	.rst(rst),
	.d_in(d_in),
	.d_latch(wrte_i),
	.sel(sel),
	.en_fcs(en_fcs),
	.count_mode(count_mode),
	.crc_out(crc_out_i),
	.pause_store(pause_store),
	.pause_reset(pause_reset),
	.unpause_load(unpause_load),
	.unpause_popstack(unpause_setfsm_popstack)
);
/*******************************************************
CRC FSM 
********************************************************/
crc_fsm_rtla fsm(
	.clk(clk),
	.rst(rst),
	.wrte(wrte_i),
	.count_max(count_max),
	.en_fprint(fingerprint_enable),
	.en_fcs(fsm_en_fcs),
	.sel(sel),
	.crc_ready(crc_ready_i),
	.pause_reset(pause_reset),
	.unpause_setfsm(unpause_setfsm_popstack),
	.count(count)
	
);
/*******************************************************
CRC COUNTER 
********************************************************/

crc_counter counter(
	.clk(clk),
	.count_reset(count_reset),
	.rst(rst),
	.en(count_en),
	.mode(count_mode),
	.carry(count_max),
	.pause_store(pause_store),
	.unpause_load(unpause_load),
	.unpause_popstack(unpause_setfsm_popstack),
	.q(count),
	.counter_max_set(counter_max_sel),
	.counter_max_value(slave_spr_dat_i[`COUNTER_WIDTH-1:0])
);
/*******************************************************
STORE BUFFER
********************************************************/
crc_store_buffer #(
	.ADDR_WIDTH(`STORE_BUFFER_WIDTH)
) sb (
	.clk(clk),
	.reset(rst),
	
	.write(sb_write),
	.write_data(sb_write_data),
	.write_address(sb_write_address),
	
	.master_address(master_address),
	.master_read(master_read),
	.master_waitrequest(master_waitrequest),
	.master_readdata(master_readdata),
	.master_write(master_write),
	.master_writedata(master_writedata),
	.pause_reset(pause_reset),
	.fprint_enable(fingerprint_enable),
	.crc_ready(crc_ready_i),
	.data_write(wrte_i)
);

/*******************************************************
STORE BUFFER FSM
********************************************************/
crc_sb_fsm sb_fsm(
	.clk(clk), 
	.rst(rst), 
	.task_begin(fingerprint_enable),
	.task_complete(task_complete),
	.count_max(crc_ready_i),
	.wrte(wrte_i),
	.sb_mux_sel(sb_mux_sel),
	.sb_write(sb_write),
	.pause_reset(pause_reset),
	.unpause_setfsm(unpause_load) //need to do this one cycle earlier than the other FSM
);

/*******************************************************
PAUSE FSM
********************************************************/
crc_pause pause(
		
	.clk(clk),
	.rst(rst),
	.pause_reg(pause_reg),
	.unpause_reg(unpause_reg),
	.fprint_enable(fingerprint_enable),
	.pause_store(pause_store),
	.pause_reset(pause_reset),
	.waitrequest(pause_waitrequest),
	.unpause_load(unpause_load),
	.unpause_setfsm_popstack(unpause_setfsm_popstack),
	.empty_n(cs_stack_empty_n),
	.pause_task_reg(pause_task_reg),
	.paused_cs_tos(paused_cs_stack_data[`CRC_KEY_WIDTH-1:0])
);



		   
endmodule

/*
crc_fingerprint #(
		.CORE_ID(0)
	)fingerprint_0 (
		.clk                (clk_clk),                                                    //          clock.clk
		.slave_spr_addr     (mm_interconnect_0_fingerprint_0_avalon_slave_0_address),     // avalon_slave_0.address
		.slave_spr_dat_i    (mm_interconnect_0_fingerprint_0_avalon_slave_0_writedata),   //               .writedata
		.slave_spr_dat_o    (mm_interconnect_0_fingerprint_0_avalon_slave_0_readdata),    //               .readdata
		.slave_spr_read     (mm_interconnect_0_fingerprint_0_avalon_slave_0_read),        //               .read
		.slave_spr_write    (mm_interconnect_0_fingerprint_0_avalon_slave_0_write),       //               .write
		.slave_waitrequest  (mm_interconnect_0_fingerprint_0_avalon_slave_0_waitrequest), //               .waitrequest
		.master_address     (fingerprint_0_avalon_master_address),                        //  avalon_master.address
		.master_waitrequest (fingerprint_0_avalon_master_waitrequest),                    //               .waitrequest
		.master_readdata    (fingerprint_0_avalon_master_readdata),                       //               .readdata
		.master_write       (fingerprint_0_avalon_master_write),                          //               .write
		.master_writedata   (fingerprint_0_avalon_master_writedata),                      //               .writedata
		.master_read        (fingerprint_0_avalon_master_read),                           //               .read
		.rst                (rst_controller_002_reset_out_reset),                         //     reset_sink.reset
		.wrte               (processor0_0_outgoing_master_write),                                                                        //    conduit_end.write
		.d_in               ({processor0_0_outgoing_master_address,processor0_0_outgoing_master_writedata}),                                                                        //               .name
		.waitrequest        (processor0_0_outgoing_master_waitrequest)                                                            //               .writeresponserequest_n
	);

	crc_fingerprint #(
			.CORE_ID(1)
		)fingerprint_1 (
		.clk                (clk_clk),                                                    //          clock.clk
		.slave_spr_addr     (mm_interconnect_0_fingerprint_1_avalon_slave_0_address),     // avalon_slave_0.address
		.slave_spr_dat_i    (mm_interconnect_0_fingerprint_1_avalon_slave_0_writedata),   //               .writedata
		.slave_spr_dat_o    (mm_interconnect_0_fingerprint_1_avalon_slave_0_readdata),    //               .readdata
		.slave_spr_read     (mm_interconnect_0_fingerprint_1_avalon_slave_0_read),        //               .read
		.slave_spr_write    (mm_interconnect_0_fingerprint_1_avalon_slave_0_write),       //               .write
		.slave_waitrequest  (mm_interconnect_0_fingerprint_1_avalon_slave_0_waitrequest), //               .waitrequest
		.master_address     (fingerprint_1_avalon_master_address),                        //  avalon_master.address
		.master_waitrequest (fingerprint_1_avalon_master_waitrequest),                    //               .waitrequest
		.master_readdata    (fingerprint_1_avalon_master_readdata),                       //               .readdata
		.master_write       (fingerprint_1_avalon_master_write),                          //               .write
		.master_writedata   (fingerprint_1_avalon_master_writedata),                      //               .writedata
		.master_read        (fingerprint_1_avalon_master_read),                           //               .read
		.rst                (rst_controller_002_reset_out_reset),                         //     reset_sink.reset
		.wrte               (processor1_0_outgoing_master_write),                                                                        //    conduit_end.write
		.d_in               ({processor1_0_outgoing_master_address,processor1_0_outgoing_master_writedata}),                                                                        //               .name
		.waitrequest        (processor1_0_outgoing_master_waitrequest)    
	);

*/