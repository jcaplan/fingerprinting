    `include "crc_defines.v"


module tb_multicore	();


reg 									clk;
reg 									rst;
reg                                     write[`NUM_CORES-1:0];                        //write data
reg [`NIOS_DATA_WIDTH-1:0]              writedata[`NUM_CORES-1:0];                          //Write data from processor
reg [`NIOS_ADDRESS_WIDTH-1:0]           data_address[`NUM_CORES-1:0];


reg 									slave_spr_read [`NUM_CORES-1:0];
reg 									slave_spr_write[`NUM_CORES-1:0];
reg [`FPRINT_SPR_ADDRESS_WIDTH-1:0] 	slave_spr_addr [`NUM_CORES-1:0];
reg [31:0] 								slave_spr_dat_i[`NUM_CORES-1:0];
wire[31:0] 								slave_spr_dat_o[`NUM_CORES-1:0];

wire[26:0] 								master_address      [`NUM_CORES:0];
wire 									master_read         [`NUM_CORES:0];
wire									master_waitrequest  [`NUM_CORES:0];
wire [31:0] 							master_readdata     [`NUM_CORES:0];
wire									master_write        [`NUM_CORES:0];
wire[31:0] 								master_writedata    [`NUM_CORES:0];

wire[26:0] 								fifo_address;
wire									fifo_write;
wire[31:0] 								fifo_writedata;
wire                                    fifo_waitrequest;

reg[26:0]                              cpu_address;
reg                                    cpu_read;
wire [31:0]                            cpu_readdata;
reg                                    cpu_write;
reg[31:0]                              cpu_writedata;
wire                                    cpu_waitrequest;
wire[26:0]                              fifo_monitor_address;
wire                                    fifo_monitor_read;
wire[31:0]                              fifo_monitor_readdata;
wire                                    fifo_monitor_waitrequest;
wire                                    fifo_monitor_empty;

wire 									io_release;
wire									irq;

wire reset;
wire waitrequest;
assign waitrequest = 0;
assign reset = rst;

    

integer j;
reg init_done;
reg fprint_window_1;
reg fprint_window_2;
reg fprint_window_3;


integer turn;

/******************************
FINGERPRINT COMPONENT
*******************************/
generate
    genvar i;
    for (i = 0; i < `NUM_CORES; i = i + 1)begin : fprint
        crc_fingerprint #(i) f0(
            .clk(clk),                                      //System clock
            .rst(rst),                                      //System reset
            .write(write[i]),                                 //write data
            .writedata(writedata[i]),
            .data_address(data_address[i]),
            .waitrequest(waitrequest),
            .slave_spr_read(slave_spr_read[i]),
            .slave_spr_write(slave_spr_write[i]),               //write for control registers
            .slave_spr_addr(slave_spr_addr[i]),             //address for control registers
            .slave_spr_dat_i(slave_spr_dat_i[i]),               //data write for control registers
            .slave_spr_dat_o(slave_spr_dat_o[i]),               //data read line for control registers
            
            .master_address(master_address[i]),
            .master_read(master_read[i]),
            .master_waitrequest(master_waitrequest[i]),
            .master_readdata(master_readdata[i]),
            .master_write(master_write[i]),
            .master_writedata(master_writedata[i])
    ); 
     assign master_waitrequest[i] = (turn == i) ? fifo_waitrequest : 1;
    end     


endgenerate

/******************************
Arbitration simulation -TDMA
*******************************/

//Just make a simple priority scheme
integer state;

parameter idle = 0;
parameter waitforsignal = 1;
always @(posedge clk or posedge rst)begin
    if(rst)begin
        turn = `NUM_CORES;
    end else begin
        //First if bus is idle assign to highest priority
        case(state)
        idle: begin
            state = waitforsignal;
            if(master_read[0] || master_write[0])
                turn = 0;
            else if(master_read[1] || master_write[1])
                turn = 1;
           else if(master_read[2] || master_write[2])
                turn = 2;
           else if(master_read[3] || master_write[3])
                turn = 3;
            else begin
                turn = `NUM_CORES;
                state = idle;
            end
        end
        waitforsignal:
            if(master_waitrequest[turn] == 0)
                state = idle;
            else begin
                state = waitforsignal;
            end
        default:
            state = idle; 
        endcase  
    end
end
assign fifo_address = master_address[turn];
assign fifo_read = master_read[turn];
assign fifo_readdata = master_readdata[turn];
assign fifo_write = master_write[turn] ;
assign fifo_writedata = master_writedata[turn];

assign master_address[`NUM_CORES] = 0;
assign master_read[`NUM_CORES] = 0;
assign master_readdata[`NUM_CORES] = 0;
assign master_write[`NUM_CORES] = 0;
assign master_writedata[`NUM_CORES] = 0;



crc_cfpu cfpu(
    .clk,
    .reset,

    .fprint_address(fifo_address[11:2]),
    .fprint_write(fifo_write),
    .fprint_writedata(fifo_writedata),
    .fprint_waitrequest(fifo_waitrequest),

    .csr_address(cpu_address),
    .csr_read(cpu_read),
    .csr_readdata(cpu_readdata),
    .csr_write(cpu_write),
    .csr_writedata(cpu_writedata),
    .csr_waitrequest(cpu_waitrequest),
    
    .irq(irq),
    .io_release(io_release)
);

initial begin
    clk = 0;

    rst = 1;
    init_done = 0;
    fprint_window_1 = 0;
    fprint_window_2 = 0;
    fprint_window_3 = 0;
    
    for(j = 0; j < `NUM_CORES; j = j + 1)begin
        write[j] = 0;
        data_address[j] = 0;
        writedata[j] = 0;
        
        slave_spr_read[j] = 0;
        slave_spr_write[j] = 0;
        slave_spr_addr[j] = 0;
        slave_spr_dat_i[j] = 0;
        //master0_waitrequest = 1;
    end

    cpu_write = 0;
    cpu_writedata = 0;
    cpu_address = 0;
    cpu_read = 0;

    #5 rst = 0;
    forever begin
        clk = ~clk;
        #5;
    end
end


/******************************************************************************
Testbench Tasks
*******************************************************************************/


/******************************
Enable
*******************************/
task automatic enable_task;
input integer core;
input[31:0] key;
begin
    
    #10
    slave_spr_write[core] = 1;
    slave_spr_addr[core] = `FPRINT_SPR_CURRENTSTATE;
    slave_spr_dat_i[core] = key + (1 << 4);
    #10
    slave_spr_write[core] = 0;
    slave_spr_addr[core] = 0;
    slave_spr_dat_i[core] = 0;
    
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
        slave_spr_write[core] = 1;
        slave_spr_addr[core] = `FPRINT_SPR_CURRENTSTATE;
        slave_spr_dat_i[core] = key;
    #10
        slave_spr_write[core] = 0;
        slave_spr_addr[core] = 0;
        slave_spr_dat_i[core] = 0;
   
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
    write[core] = 1;
    data_address[core]= address;
    writedata[core] = data;
    #10
    write[core] = 0;
    data_address[core] = 0;
    writedata[core] = 0;
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
    slave_spr_read[core] = 1;
    slave_spr_addr[core] = 8;
    #10 slave_spr_read[core] = 0;
    slave_spr_addr[core] = 0;
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
    slave_spr_read[core] = 1;
    slave_spr_addr[core] = 12;
    #10 slave_spr_read[core] = 0;
    slave_spr_addr[core] = 0;
    #50;
end
endtask



/******************************
Set directory start pointer
*******************************/
task automatic set_directory_start;
input integer core;
input[31:0]  key;
input[31:0] offset;
begin
    #10
    cpu_write = 1;
    cpu_writedata = offset;
    cpu_address = `CRC_DIRECTORY_START_BASE + (core << 6) + key;
  //  while(cpu_waitrequest[core])  
    #10
    cpu_write = 0;
    cpu_writedata = 0;
    cpu_address = 0;
end
endtask

/******************************
Set directory end pointer
*******************************/
task automatic set_directory_end;
input integer core;
input[31:0]  key;
input[31:0] offset;
begin
#10
    cpu_write = 1;
    cpu_writedata = offset ;
    cpu_address = `CRC_DIRECTORY_END_BASE + (core << 6) + key;
  //  while(cpu_waitrequest[core]) 
    #10; 
    cpu_write = 0;
    cpu_writedata = 0;
    cpu_address = 0;
end
endtask

/******************************
Set core assignment
*******************************/
task automatic set_core_assignment;
input integer core;
input[31:0]   key;
input[31:0]   core_id;
begin
#10
    cpu_write = 1;
    cpu_writedata = core_id | (key << 4) ;
    cpu_address = (core << 6) + `COMPARATOR_CORE_ASSIGNMENT_OFFSET;
  //  while(cpu_waitrequest[core]) 
    #10; 
    cpu_write = 0;
    cpu_writedata = 0;
    cpu_address = 0;
end
endtask

/******************************
ISR
*******************************/
always @ (posedge irq)
begin
    #200
    //Read from the successreg
    cpu_read = 1;
    cpu_address = `COMPARATOR_SUCCESS_REG_OFFSET;
    #20
    cpu_address = `COMPARATOR_FAIL_REG_OFFSET;
    #20
    cpu_read = 0;
    cpu_write = 1;
    cpu_writedata = 0;
    cpu_address = `COMPARATOR_EXCEPTION_OFFSET;
#10
    cpu_write = 0;
    cpu_writedata = 0;
    cpu_address = 0;
end

/******************************
Write count register
*******************************/
task automatic set_count_reg;
input integer core;
input[31:0]  count;
begin
#10
    slave_spr_write[core] = 1;
    slave_spr_dat_i[core] = count;
    slave_spr_addr[core] = `COUNTER_MAX_OFFSET;
#10
    slave_spr_write[core] = 0;
    slave_spr_dat_i[core] = 0;
    slave_spr_addr[core] = 0;
end
endtask

task automatic os_pause;
input integer core;
input[31:0] key;
begin
    #10
    slave_spr_write[core] = 1;
    slave_spr_addr[core] = `FPRINT_PAUSE_TASK_REG;
    slave_spr_dat_i[core] = key;
    #10
    slave_spr_write[core] = 0;
    slave_spr_addr[core] = 0;
    slave_spr_dat_i[core] = 0;
end
endtask



always @ (negedge rst)

begin
//  $fmonitor (file,"%x %x", master0_address,master0_writedata);
    
//  file = $fopen ("../sim_results/crc_fprint_sb.txt", "w");
    for(j = 0; j < `NUM_CORES; j = j + 1)begin
        set_directory_start(j,1,0);
        set_directory_end(j,1,8);
        set_directory_start(j,2,21);
        set_directory_end(j,2,30);
        set_directory_start(j,0,12);
        set_directory_end(j,0,20);
        set_directory_start(j,5,31);
        set_directory_end(j,5,40);
    end

    
    // for(j = 0; j < 15; j = j + 1)begin
    //     set_core_assignment(0,j,'hF);
    //     set_core_assignment(1,j,'hF);
    // end
    //     set_core_assignment(0,0,0);
    //     set_core_assignment(1,0,1);
    //     set_core_assignment(0,1,2);
    //     set_core_assignment(1,1,3);
    //     set_core_assignment(0,2,3);    
    //     set_core_assignment(1,2,2);
    //     set_core_assignment(0,5,0);    
    //     set_core_assignment(1,5,1);


    // init_done = 1;
    // $display("Test 1 begins at time %d",$time);
    // while(init_done ==1)
    // #5000;
    // while(irq)#10;

    //     set_core_assignment(0,0,0);
    //     set_core_assignment(1,0,2);
    //     set_core_assignment(0,1,1);
    //     set_core_assignment(1,1,3);
    //     set_core_assignment(0,2,3);    
    //     set_core_assignment(1,2,1);
    //     set_core_assignment(0,5,0);    
    //     set_core_assignment(1,5,2);   

    // fprint_window_1 = 1;
    // $display("Test 2 begins at time %d",$time);
    // while(fprint_window_1 ==1)
    // #5000;
    // while(irq)#10;
        set_core_assignment(0,0,0);
        set_core_assignment(1,0,2);
        set_core_assignment(0,1,1);
        set_core_assignment(1,1,3);
        set_core_assignment(0,2,3);    
        set_core_assignment(1,2,2);
        set_core_assignment(0,5,0);    
        set_core_assignment(1,5,1);   
    fprint_window_2 = 1;
    $display("Test 3 begins at time %d",$time);
    while(fprint_window_2 ==1)
    #5000;
    while(irq)#10;
        set_core_assignment(0,0,0);
        set_core_assignment(1,0,1);
        set_core_assignment(0,1,0);
        set_core_assignment(1,1,1);
        set_core_assignment(0,2,0);    
        set_core_assignment(1,2,1);
        set_core_assignment(0,5,0);    
        set_core_assignment(1,5,1);   

        $display("Test 4 begins at time %d",$time);
    fprint_window_3 = 1;

    
end


generate
    for(i = 0; i < 2; i = i + 1)begin : test
        integer k;
        always @(posedge init_done)begin


            set_count_reg(i,'hff);
            //start task 1
            enable_task(i,0);
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(i,0,k);
            end
            //pause task 1
            #100
            disable_task(i,0);


            #1200;

        
            set_count_reg(i,'hff);
            //start task 1
            enable_task(i,5);
            for(k = 0; k < 1200; k = k + 1)begin
                #10 write_data(i,0,k);
            end
            //pause task 1
            #100
            disable_task(i,5);

            #800;


            //********************************************************
            //start task 1
            enable_task(i,5);
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(i,0,k);
            end
            

            
            //pause and change count register
            pause(i);
            #50;
            os_pause(i,'b100000);
            #50;
            unpause(i);
            #50;
            
            //start second task
                set_count_reg(i,'hff);
            //start task 1
            enable_task(i,0);
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(i,0,k);
            end
            
            #100
            disable_task(i,0);
            
            #200
            //resume old task
            
            os_pause(i,0);
            #50;
            unpause(i);
            #50;
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(i,0,k);
            end
            
                
            #100
            disable_task(i,5);
            init_done = 0;
         
        end
    end
endgenerate
 
 generate
    genvar m;
    for(m = 2; m < 4; m = m + 1)begin : test1
        integer k;
        always @(posedge init_done)begin

        
            set_count_reg(m,'hff);
            //start task 1
            enable_task(m,1);
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(m,0,k*2);
            end
            //pause task 1
            #100
            disable_task(m,1);


            #800;
            //TEST 2*************************************************
                //start task 2
            set_count_reg(m,'hfff);
            enable_task(m,2);
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(m,0,k);
            end
            

            
            //pause and change count register

            pause(m);
            #50;
            os_pause(m,'b100);
            #50;
            unpause(m);
            #50;
            
            //start second task
                set_count_reg(m,'hff);
            //start task 1
            enable_task(m,1);
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(m,0,k);
            end
            
            #100
            disable_task(m,1);
            
            #200
            //resume old task
            
            os_pause(m,0);
            #50;
            unpause(m);
            #50;
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(m,0,k);
            end
            
                
            #100
            disable_task(m,2);        

        end
    end
endgenerate

/*********************************************
*NEW FPRINT WINDOW - DIFFERENT COUPLINGS
**********************************************/

generate
    for(i = 0; i < 3; i = i + 2)begin : test2
        integer k;
        always @(posedge fprint_window_1)begin

        
            set_count_reg(i,'hff);
            //start task 1
            enable_task(i,0);
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(i,0,k);
            end
            //pause task 1
            #100
            disable_task(i,0);


            #1200;

        
            set_count_reg(i,'hff);
            //start task 1
            enable_task(i,5);
            for(k = 0; k < 1200; k = k + 1)begin
                #10 write_data(i,0,k);
            end
            //pause task 1
            #100
            disable_task(i,5);

            #800;


            //********************************************************
            //start task 1
            enable_task(i,5);
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(i,0,k);
            end
            

            
            //pause and change count register
            pause(i);
            #50;
            os_pause(i,'b100000);
            #50;
            unpause(i);
            #50;
            
            //start second task
                set_count_reg(i,'hff);
            //start task 1
            enable_task(i,0);
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(i,0,k);
            end
            
            #100
            disable_task(i,0);
            
            #200
            //resume old task
            
            os_pause(i,0);
            #50;
            unpause(i);
            #50;
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(i,0,k);
            end
            
                
            #100
            disable_task(i,5);
            fprint_window_1 = 0;
         
        end
    end
endgenerate
 
 generate
    for(m = 1; m < 4; m = m + 2)begin : test3
        integer k;
        always @(posedge fprint_window_1)begin

        
            set_count_reg(m,'hff);
            //start task 1
            enable_task(m,1);
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(m,0,k*2);
            end
            //pause task 1
            #100
            disable_task(m,1);


            #800;
            //TEST 2*************************************************
                //start task 2
            set_count_reg(m,'hfff);
            enable_task(m,2);
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(m,0,k);
            end
            

            
            //pause and change count register

            pause(m);
            #50;
            os_pause(m,'b100);
            #50;
            unpause(m);
            #50;
            
            //start second task
                set_count_reg(m,'hff);
            //start task 1
            enable_task(m,1);
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(m,0,k);
            end
            
            #100
            disable_task(m,1);
            
            #200
            //resume old task
            
            os_pause(m,0);
            #50;
            unpause(m);
            #50;
            for(k = 0; k < 1000; k = k + 1)begin
                #10 write_data(m,0,k);
            end
            
                
            #100
            disable_task(m,2);        

        end
    end
endgenerate


/***********************************************************
*NEW FPRINT WINDOW - DYNAMIC COUPLINGS WITHIN SAME WINDOW
************************************************************
/*********************************************
*TEST 3
* Core 0 does task 0 and 5
* Core 1 does task 1 and 5
* Core 2 does task 0 and 2
* Core 3 does task 1 and 2

x y
0 2
1 3
3 2
0 1
**********************************************/
/*********************************************
*NEW FPRINT WINDOW - DIFFERENT COUPLINGS
**********************************************/

//Each core separately
/*********************************************
CORE 0
**********************************************/
always @(posedge fprint_window_2)begin : test4 

    integer c;
    integer k;
    c = 0;
    //#5000
    set_count_reg(c,'hff);
    //start task 1
    enable_task(c,0);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,0);

    #400
    
    set_count_reg(c,'hff);
    //start task 1

    enable_task(c,5);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,5);


    /**********************************************
    *Pause then change couplings
    ***********************************************/

    enable_task(c,5);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100

 
    pause(c);
    #50;
    os_pause(c,'b100000);
    #50;
    unpause(c);
    #50;


    enable_task(c,0);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,0);

    os_pause(c,0);
    #50;
    unpause(c);

    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,5);
end
        
/*********************************************
*CORE 1
**********************************************/
always @(posedge fprint_window_2)begin : test5 
    integer c;
    integer k;
    c = 1;
    set_count_reg(c,'hff);
    //start task 1
    enable_task(c,1);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,1);

    #400
    
    set_count_reg(c,'hff);
    //start task 1

    enable_task(c,5);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,5);

    /**********************************************
    *Pause then change couplings
    ***********************************************/

    enable_task(c,5);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100

 
    pause(c);
    #50;
    os_pause(c,'b100000);
    #50;
    unpause(c);
    #50;


    enable_task(c,1);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,1);

    os_pause(c,0);
    #50;
    unpause(c);

    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,5);
 
end
/*********************************************
*CORE 2
**********************************************/


always @(posedge fprint_window_2)begin : test6
    integer c;
    integer k;
    c = 2;
    set_count_reg(c,'hff);
    //start task 1
    enable_task(c,0);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,0);

    #400
    
    set_count_reg(c,'hff);
    //start task 1

    enable_task(c,2);
    for(k = 0; k < 1000; k = k + 1)begin 
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,2);



    /**********************************************
    *Pause then change couplings
    ***********************************************/
 

    enable_task(c,2);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100

 
    pause(c);
    #50;
    os_pause(c,'b100);
    #50;
    unpause(c);
    #50;


    enable_task(c,0);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,0);

    os_pause(c,0);
    #50;
    unpause(c);

    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,2);
end

/*********************************************
*CORE 3
**********************************************/
always @(posedge fprint_window_2)begin : test7
    integer c;
    integer k;
    c = 3;
    set_count_reg(c,'hff);
    //start task 1
    enable_task(c,1);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,1);

    #400
    
    set_count_reg(c,'hff);
    //start task 1

    enable_task(c,2);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,2);

    /**********************************************
    *Pause then change couplings
    ***********************************************/
 

    enable_task(c,2);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100

 
    pause(c);
    #50;
    os_pause(c,'b100);
    #50;
    unpause(c);
    #50;


    enable_task(c,1);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,1);

    os_pause(c,0);
    #50;
    unpause(c);

    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,2);

    fprint_window_2 = 0;
end

/***************************************************
TEST 4
****************************************************/

/***********************************************************
*Task 0 finishes on one core only, and then a fingerprint is sent for the next task
***********************************************************/


always @(posedge fprint_window_3)begin : test8 
    integer c;
    integer k;
    c = 0;
    set_count_reg(c,'hff);



    /**********************************************
    *Pause then change couplings
    ***********************************************/

    enable_task(c,5);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100

 
    pause(c);
    #50;
    os_pause(c,'b100000);
    #50;
    unpause(c);
    #50;


    enable_task(c,0);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100

    //extra wait before task ending
    #2000;
    disable_task(c,0);

    os_pause(c,0);
    #50;
    unpause(c);

    
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,5);
end
        

always @(posedge fprint_window_3)begin : test9
    integer c;
    integer k;
    c = 1;
    /**********************************************
    *Pause then change couplings
    ***********************************************/
    set_count_reg(c,'hff);
    enable_task(c,5);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100

 
    pause(c);
    #50;
    os_pause(c,'b100000);
    #50;
    unpause(c);
    #50;


    enable_task(c,0);
    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,0);

    os_pause(c,0);
    #50;
    unpause(c);

    for(k = 0; k < 1000; k = k + 1)begin
        #10 write_data(c,0,k);
    end
    //pause task 1
    #100
    disable_task(c,5);
 
end



endmodule
