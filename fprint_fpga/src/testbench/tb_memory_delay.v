module tb_memory_delay();


reg clk;
reg reset;

reg [2:0]  csr_address;
reg        csr_write;
reg [31:0] csr_writedata;
reg m_waitrequest;
wire m_write;
  reg            s_write;

  reg            s_read;
wire s_waitrequest;

 memory_delay dut(
                                      // inputs:
                                       .clk(clk),
                                       .reset(reset),

                                       .csr_address(csr_address),
                                       .csr_write(csr_write),
                                       .csr_writedata(csr_writedata),


                                       .s_write(s_write),
                                       .s_read(s_read),
                                       .s_waitrequest(s_waitrequest),

                                       .m_waitrequest(m_waitrequest),
                                       .m_write(m_write)
                                    );


initial begin
    clk = 0;
    reset = 1;
    csr_address = 0;
    csr_write = 0;
    csr_writedata = 0;
    s_write = 0;
    s_read = 0;
    #5 reset = 0;
    forever begin
        clk = ~clk;
        #5;
    end
end

always@ (posedge clk)begin
	if(m_write)
		m_waitrequest = 0;
	else begin
		m_waitrequest = 1;
	end
end

always @(negedge reset)begin
	//set the counter
	#10
	csr_write = 1;
	csr_address = 4;
	csr_writedata = 'h1f;
	#10;
	csr_write = 0;
	csr_address = 0;
	csr_writedata = 'h0;
	#10;
	
	//enable the counter
	csr_write = 1;
	csr_address = 0;
	csr_writedata = 'h1;
	#10;
	csr_write = 0;
	csr_writedata = 'h0;
	#10;
	
	//try a write
	s_write = 1;
    @ (negedge s_waitrequest) #10
    s_write = 0;
    #10

	//try a reade
	s_read = 1;
    @ (negedge s_waitrequest) #10
    s_read = 0;
    #10
	//disable the counter
	csr_write = 1;
	csr_address = 0;
	csr_writedata = 'h0;
	#10;
	csr_write = 0;
	csr_writedata = 'h0;
	#10;

	//try again
	//try a write
	s_write = 1;
    @ (negedge s_waitrequest) #10
    s_write = 0;
    #10

	//try a reade
	s_read = 1;
    @ (negedge s_waitrequest) #10
    s_read = 0;

end
endmodule
