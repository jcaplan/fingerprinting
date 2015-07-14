`timescale 1 ns / 1 ns

module monitor_fifo#(
    // --------------------------------------------------
    // Parameters
    // --------------------------------------------------
    parameter DATA_WIDTH            = 32,
    parameter ADDRESS_WIDTH         = 10,
    parameter CSR_ADDRESS_WIDTH     = 5,
    parameter FIFO_DEPTH            = 64, //<=256
    parameter ALMOST_OVERFLOW       = 56
)
(
    // --------------------------------------------------
    // Ports
    // --------------------------------------------------
    input                               clk,
    input                               reset,

    input [DATA_WIDTH-1: 0]             avs_in_writedata,
    input                               avs_in_write,
    input [ADDRESS_WIDTH-1:0]           avs_in_address,
    output                              avs_in_waitrequest,

    output [DATA_WIDTH-1: 0]            avs_out_readdata,
    input                               avs_out_read,
    input [ADDRESS_WIDTH-1:0]           avs_out_address,
    output reg                          avs_out_waitrequest,
    output reg                          empty, //Use this for hardware comparator   

    output reg                          irq,

    input [CSR_ADDRESS_WIDTH-1:0]       csr_address,
    output [DATA_WIDTH-1:0]             csr_readdata,
    input                               csr_write,
    input                               csr_read,
    input [DATA_WIDTH-1 : 0]            csr_writedata

);

parameter MEM_WIDTH = log2ceil(FIFO_DEPTH);
parameter ADR_QUEUE_OFFSET = 0;
parameter DAT_QUEUE_OFFSET = 1;

reg [ MEM_WIDTH -1 : 0 ]                head;
reg [ MEM_WIDTH -1 : 0 ]                tail;
reg [ DATA_WIDTH-1 : 0 ]                data_mem [FIFO_DEPTH - 1 : 0];
reg [ ADDRESS_WIDTH-1 : 0 ]             address_mem [FIFO_DEPTH - 1 : 0];
reg [ 7 : 0 ]                           count;
reg                                     almost_overflow;
//reg                                     empty;
reg                                     full;
reg [DATA_WIDTH-1 : 0]                  data_mem_readdata;
reg [ADDRESS_WIDTH-1 : 0]               address_mem_readdata;
wire                                    pop;
reg                                     overflow_trigger;
assign pop = (avs_out_address == 1);

//Maintain the head and tail

always @(posedge clk or posedge reset)begin
    if(reset)begin
        head = 0;
        tail = 0;
    end else begin
//TODO: Do not write if full
        if(avs_in_write)
            head <= (head == (FIFO_DEPTH - 1)) ? 0 : (head + 1);
//TODO: Do not read if empty
        if(avs_out_read & pop & ~empty)
            tail <= (tail == (FIFO_DEPTH - 1)) ? 0 : (tail + 1);
    end
end


//Memory

always @(posedge clk)begin
    if(avs_in_write)begin
        data_mem[head] = avs_in_writedata;
        address_mem[head] = avs_in_address;
    end
    address_mem_readdata = address_mem[tail];
    data_mem_readdata = data_mem[tail];

end

assign avs_out_readdata = empty ? 0 : (avs_out_address[0] ? data_mem_readdata : address_mem_readdata);
   


//Wait requests not used yet
assign avs_in_waitrequest = 0;

//State registers
always @(posedge clk or posedge reset)begin
    if(reset)begin
        avs_out_waitrequest = 1;
    end else begin
        if(avs_out_read & avs_out_waitrequest)begin
            avs_out_waitrequest <= 0;
        end else begin
            avs_out_waitrequest <= 1;
        end
    end

end

//Count
always @(posedge clk or posedge reset)begin
    if(reset)begin
        count = 0;
    end else begin
        if (avs_in_write & ~avs_out_read)
            count = count + 1;
        else if (avs_out_read & ~avs_in_write & pop & ~empty &~avs_out_waitrequest)
            count = count - 1;
    end
    //if read and write in same clock cycle then count doesn't change
end

//Almost overflow value
always @(posedge clk or posedge reset)begin
    if(reset)begin
        almost_overflow = 0;
        overflow_trigger = 0;
    end else begin
        if((count >= ALMOST_OVERFLOW))begin
            if(~almost_overflow)
                overflow_trigger <= 1;
            else begin
                overflow_trigger <= 0;
            end
            almost_overflow = 1;

        end else begin
            almost_overflow = 0;
        end
    end
end
//Empty
//Full
always @(posedge clk or posedge reset)begin
    if(reset)begin
        empty = 1;
        full = 0;
    end else begin
        empty = (tail == head);
        full  = (head == ((tail == 0) ? (FIFO_DEPTH - 1) : (tail - 1)));
    end
end

always @(posedge clk or posedge reset)begin
    if(reset)begin
       irq = 0;
    end else begin
        if(overflow_trigger)begin
            irq = 1;
        end
        else if(csr_write & (csr_address == 0))begin
            irq = 0;
        end
    end
end
    

//read data
assign csr_readdata = {almost_overflow,empty,full,count};














    // --------------------------------------------------
    // Calculates the log2ceil of the input value
    // --------------------------------------------------
    function integer log2ceil;
        input integer val;
        reg[31:0] i;

        begin
            i = 1;
            log2ceil = 0;

            while (i < val) begin
                log2ceil = log2ceil + 1;
                i = i[30:0] << 1;
            end
        end
    endfunction

endmodule
