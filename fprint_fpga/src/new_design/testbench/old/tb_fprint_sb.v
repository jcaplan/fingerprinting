`include "crc_defines.v"


module tb_fprint_sb();

reg 									clk;
reg 									rst;
reg                                     write;                        //write data
reg [`NIOS_DATA_WIDTH-1:0]              writedata;                          //Write data from processor
reg [`NIOS_ADDRESS_WIDTH-1:0]           data_address;


reg 									slave_spr_read;
reg 									slave_spr_write;
reg [`FPRINT_SPR_ADDRESS_WIDTH-1:0] 	slave_spr_addr;
reg [31:0] 								slave_spr_dat_i;
wire[31:0] 								slave_spr_dat_o;

wire[26:0] 								master_address;
wire 									master_read;
reg										master_waitrequest;
reg [31:0] 								master_readdata;
wire									master_write;
wire[31:0] 								master_writedata;
wire									waitrequest;
assign waitrequest = 0;

/******************************
FINGERPRINT COMPONENT
*******************************/
crc_fingerprint #(0) f0(
            .clk(clk),                                      //System clock
            .rst(rst),                                      //System reset
            .write(write),                                 //write data
            .writedata(writedata),
            .data_address(data_address),
            .waitrequest(waitrequest),
            .slave_spr_read(slave_spr_read),
            .slave_spr_write(slave_spr_write),               //write for control registers
            .slave_spr_addr(slave_spr_addr),             //address for control registers
            .slave_spr_dat_i(slave_spr_dat_i),               //data write for control registers
            .slave_spr_dat_o(slave_spr_dat_o),               //data read line for control registers
            
            .master_address(master_address),
            .master_read(master_read),
            .master_waitrequest(master_waitrequest),
            .master_readdata(master_readdata),
            .master_write(master_write),
            .master_writedata(master_writedata)
    ); 


/******************************
Initial Conditions
*******************************/
initial
begin
	clk = 0;
	rst = 1;
    write = 0;
    data_address = 0;
    writedata = 0;
	slave_spr_read = 0;
	slave_spr_write = 0;
	slave_spr_addr = 0;
	slave_spr_dat_i = 0;
	master_waitrequest = 1;
	master_readdata = 0;
	
	#5 rst = 0;
end


/******************************
Enable
*******************************/
task automatic enable_task;
input integer core;
input[31:0] key;
begin
    
    #10
    slave_spr_write = 1;
    slave_spr_addr = `FPRINT_SPR_CURRENTSTATE;
    slave_spr_dat_i = key + (1 << 4);
    #10
    slave_spr_write = 0;
    slave_spr_addr = 0;
    slave_spr_dat_i = 0;
    
end
endtask
/******************************
Disable
*******************************/
task automatic disable_task;
input integer core;
input[31:0] key;
begin
    #10
        slave_spr_write = 1;
        slave_spr_addr = `FPRINT_SPR_CURRENTSTATE;
        slave_spr_dat_i = key;
    #10
        slave_spr_write = 0;
        slave_spr_addr = 0;
        slave_spr_dat_i = 0;
   
end
endtask
/******************************
Write
*******************************/
task automatic write_data;
input integer core;
input[26:0] address;
input[31:0] data;
begin
    #10
    write = 1;
    data_address= address;
    writedata = data;
    #10
    write = 0;
    data_address = 0;
    writedata = 0;
end
endtask


/******************************
Pause
*******************************/
task automatic pause;
input integer core;
begin
    //write ra register
    write_data(core,'hdead,'hdeadbeef);
    #10
    slave_spr_read = 1;
    slave_spr_addr = 8;
    #10 slave_spr_read = 0;
    slave_spr_addr = 0;
        #50;
end
endtask



/******************************
Unpause
*******************************/
task automatic unpause;
input integer core;
begin
    #10
    slave_spr_read = 1;
    slave_spr_addr = 12;
    #10 slave_spr_read = 0;
    slave_spr_addr = 0;
    #50;
end
endtask

task comparator_receive_data;
begin
	#10
	master_waitrequest = 0;
	#20
	master_waitrequest = 1;
end
endtask

/******************************
Write count register
*******************************/
task automatic set_count_reg;
input integer core;
input[31:0]  count;
begin
#10
    slave_spr_write = 1;
    slave_spr_dat_i = count;
    slave_spr_addr = `COUNTER_MAX_OFFSET;
#10
    slave_spr_write = 0;
    slave_spr_dat_i = 0;
    slave_spr_addr = 0;
end
endtask

/******************************
Clock
*******************************/
always begin

	#5 clk = ~clk;
end

/******************************
Comparator release
*******************************/
always @ (negedge rst)
begin
	while(1)begin
		#10;
		if(master_write)
			comparator_receive_data();
	end
end



integer i;
integer file;
/******************************
Tests
*******************************/
always @ (negedge rst)

begin
	//$fmonitor (file,"%x %x", master_address,master_writedata);
	
//	file = $fopen ("../sim_results/crc_fprint_sb.txt", "w");
	
	set_count_reg(0,65);
	//****************
	//First test
	//No burst
	//****************
	enable_task(0,2);
	for(i = 0; i < 10000; i = i + 1)
		write_data(0,0,1);
	#10
	disable_task(0,2);


	
	
end

endmodule

