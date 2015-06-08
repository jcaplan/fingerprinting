module memory_delay (
                                      // inputs:
                                       clk,
                                       reset,

                                       csr_address,
                                       csr_write,
                                       csr_writedata,

                                       m_address,
                                       m_byteenable,
                                       m_chipselect,
                                       m_clken,
                                       m_write,
                                       m_read,
                                       m_writedata,
                                       m_readdata,
                                       m_waitrequest,


                                       s_address,
                                       s_byteenable,
                                       s_chipselect,
                                       s_clken,
                                       s_write,
                                       s_read,
                                       s_writedata,
                                       s_readdata,
                                       s_waitrequest
                                    )
;
  input clk;
  input reset;

  input    [ 31: 0] m_readdata;
  output   [ 22: 0] m_address;
  output   [  3: 0] m_byteenable;
  output            m_chipselect;
  output            m_clken;
  output            m_write;
  output   [ 31: 0] m_writedata;
  input             m_waitrequest;
  output            m_read;
  output  [ 31: 0] s_readdata;
  output           s_waitrequest;
  input   [ 22: 0] s_address;
  input   [  3: 0] s_byteenable;
  input            s_chipselect;
  input            s_clken;
  input            s_write;
  input   [ 31: 0] s_writedata;
  input            s_read;


   input    [2:0]                   csr_address;
   input                            csr_write;
   input  [31:0]  csr_writedata;
   
assign m_address     = s_address;
assign m_byteenable  = s_byteenable;
assign m_chipselect  = s_chipselect;
assign m_clken       = s_clken;
assign m_writedata   = s_writedata;
assign s_readdata    = m_readdata;



//s_write, s_read, s_waitrequest

`define ENABLE_OFFSET 0
`define COUNT_OFFSET  4

reg[31:0] count;
reg[31:0] count_max;
reg       enable;

reg[2:0] state;
parameter init = 0;
parameter wait1 = 1;
parameter execute = 2;
parameter wait2 = 3;
parameter waitrequest = 4;

always@ (posedge clk or posedge reset)begin
   if(reset)begin
      count = 0;
      count_max = 1;
      enable = 0;
   end else begin
      if(state == wait1)
         count <= count + 1;
      if(count == count_max)
         count <= 0;

      if(csr_write & csr_address == `ENABLE_OFFSET)
         enable <= csr_writedata[0];
      if(csr_write &  csr_address == `COUNT_OFFSET)
         count_max <= csr_writedata;
   end
end

always @ (posedge clk or posedge reset)begin
   if(reset)
      state = init;
   else begin
      case(state)
         init:
            if(enable & (s_read | s_write))
               state = wait1;
         wait1:
            if(count == count_max)begin
                if(s_read)
                  state = waitrequest;
                else           
                  state = execute;
            end else begin
                state = wait1;
            end
         execute:
            if(~m_waitrequest)
              state = waitrequest;
            else 
              state = execute;
         waitrequest:
            state = init;
      endcase
   end
end

assign s_waitrequest = enable ? ~(state == waitrequest) : m_waitrequest;
assign m_write       = enable ? (state == execute) : s_write;
assign m_read        = enable ? (state == execute) : s_read;

endmodule
