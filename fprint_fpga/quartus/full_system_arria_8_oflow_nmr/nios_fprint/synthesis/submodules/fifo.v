//-----------------------------------------------------
// Design Name : syn_fifo
// File Name   : syn_fifo.v
// Function    : Synchronous (single clock) FIFO
// Coder       : Deepak Kumar Tala
//-----------------------------------------------------
module fifo 
#(
	parameter DATA_WIDTH = 32,
	parameter ADDR_WIDTH = 8
)(
clk      , // Clock input
rst      , // Active high reset
data_in  , // Data input
rd_en    , // Read enable
wr_en    , // Write Enable
data_out , // Data Output
empty    , // FIFO empty
full       // FIFO full
);    
 
// FIFO constants

parameter RAM_DEPTH = (1 << ADDR_WIDTH);
// Port Declarations
input clk ;
input rst ;
input rd_en ;
input wr_en ;
input [DATA_WIDTH-1:0] data_in ;
output full ;
output empty ;
output [DATA_WIDTH-1:0] data_out ;

//-----------Internal variables-------------------
reg [ADDR_WIDTH-1:0] wr_pointer;
reg [ADDR_WIDTH-1:0] rd_pointer;
reg [ADDR_WIDTH :0] status_cnt;
reg [DATA_WIDTH-1:0] data_out ;
reg [DATA_WIDTH-1:0] data_ram [RAM_DEPTH-1:0];


//-----------Variable assignments---------------
assign full = (status_cnt == (RAM_DEPTH-1));
assign empty = (status_cnt == 0);

wire write_data;
wire read_data;
assign write_data = wr_en && !full;
assign read_data = rd_en && !empty;
//-----------Code Start---------------------------
always @ (posedge clk or posedge rst)
begin : WRITE
  if (rst) begin
    wr_pointer <= 0;
  end else if (write_data) begin
    wr_pointer <= wr_pointer + 1;
	data_ram[wr_pointer] = data_in;
  end
end

always @ (posedge clk or posedge rst)
begin : READ
  if (rst) begin
    rd_pointer <= 0;
	data_out = 0;
  end else if (read_data) begin
    rd_pointer <= rd_pointer + 1;
	data_out <= data_ram[rd_pointer];
  end
end

always @ (posedge clk or posedge rst)
begin : STATUS_COUNTER
  if (rst) begin
    status_cnt <= 0;
  // Read but no write.
  end else if (read_data && !(wr_en)) begin
    status_cnt <= status_cnt - 1;
  // Write but no read.
  end else if (write_data && !(rd_en)) begin
    status_cnt <= status_cnt + 1;
  //read while write
  end else if (wr_en && rd_en) begin
	if(empty)
		status_cnt <= 1;
	else if(full)
		status_cnt <= 'hfe;
  end
  
  
end 


endmodule
