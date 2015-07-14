module fulltlb(
	avm_address,
	avm_byteenable,
	avm_read,
	avm_write,
	avm_writedata,
	avm_waitrequest,
	avm_readdata,

	avs_address,
	avs_address_conduit,
	avs_byteenable,
	avs_read,
	avs_write,
	avs_writedata,
	avs_waitrequest,
	avs_readdata,

	avs_ctrl_reg_address,
	avs_ctrl_reg_writedata,
	avs_ctrl_reg_write,
	avs_ctrl_reg_read,

	avs_ctrl_reg_readdata,
	avs_ctrl_reg_waitrequest,

	fprint_waitrequest,
	fprint_data_address,
	fprint_writedata,
	fprint_write,

	clk,
	reset
);

typedef enum {idle, ind, translate, memory} tlb_state_type;

// parameters
parameter tagwidth 			= 	21;	//20
parameter addresswidth 		=	27;	//26
parameter numentries 		= 	32;

parameter datawidth			=	32;
parameter byteenablewidth 	=	4;

//clock and reset
input clk, reset;

//data_master and instruction_master ports
input [datawidth-1:0] 		avs_writedata;
input [datawidth-1:0]		avm_readdata;
input [addresswidth-1:0] 	avs_address;
input [byteenablewidth-1:0] avs_byteenable;
input avs_read, avs_write;
input avm_waitrequest;

output [datawidth-1:0] 		avm_writedata;
output [datawidth-1:0]		avs_readdata;
output logic [addresswidth-1:0] avm_address;
output [byteenablewidth-1:0] avm_byteenable;
output avm_read, avm_write;
output avs_waitrequest;

//address conduit signal
input [addresswidth-1:0] avs_address_conduit;

//control register ports
input [7:0] avs_ctrl_reg_address;
input [31:0] avs_ctrl_reg_writedata;
input avs_ctrl_reg_write, avs_ctrl_reg_read;

output logic [31:0] avs_ctrl_reg_readdata;
output avs_ctrl_reg_waitrequest;

output						fprint_waitrequest;
output	[addresswidth-1:0]  fprint_data_address;
output  [datawidth-1 : 0]	fprint_writedata;
output                  	fprint_write;

//control registers
wire [tagwidth-1:0] spmtag;
wire [tagwidth-1:0] cputag;
reg [tagwidth-1:0] cputable[numentries-1:0];
reg [numentries-1:0] validReg;
reg [numentries-1:0] enableReg;
logic [31:0] tlb_sel;

wire ram_addr_sel;
reg [tagwidth-1:0] ram_store;

integer vindex;
integer index;
integer i;

tlb_state_type vstate, rstate;

logic v_avm_read, v_avm_write, v_avs_waitrequest;
reg v_avs_ctrl_reg_waitrequest;


assign fprint_write          = avs_write;
assign fprint_writedata      = avs_writedata;
assign fprint_waitrequest    = v_avs_waitrequest;
assign fprint_data_address   = avs_address;

//Forwarding signals
assign avm_writedata = avs_writedata;
assign avm_byteenable = avs_byteenable;
assign avs_readdata = avm_readdata;
assign avm_read = v_avm_read;
assign avm_write = v_avm_write;
assign avs_waitrequest = v_avs_waitrequest;

//intermediate signal computation
assign avs_ctrl_reg_waitrequest = v_avs_ctrl_reg_waitrequest;
assign cputag = avs_address[addresswidth-1:addresswidth-tagwidth];
assign ram_addr_sel = (avs_ctrl_reg_address[7:5] == 3'b011);

always_comb begin
	avm_address = avs_address;
	if(tlb_sel != 32'h00000000 && index != 32 && avs_ctrl_reg_write == 0 && spmtag !== 'x) begin
		avm_address[addresswidth-1:addresswidth-tagwidth] = (index < numentries && index >= 0) ? spmtag : cputag;
	end
end

always_comb begin
	if(tlb_sel != 32'h00000000) begin
		vstate = rstate;
		v_avm_read = '0;
		v_avm_write = '0;
		v_avs_waitrequest = '1;
		case(rstate)
			idle: if (avs_write || avs_read)
						vstate = ind;

			ind: begin
				vstate = translate;
			end

			translate: begin
				v_avm_write = avs_write;
				v_avm_read = avs_read;
				vstate = memory;
			end

			memory: begin
				v_avm_write = avs_write;
				v_avm_read = avs_read;
				if(!avm_waitrequest) begin
					vstate = idle;
					v_avs_waitrequest = '0;
				end
			end
		endcase
	end else begin
		v_avm_write = avs_write;
		v_avm_read = avs_read;
		v_avs_waitrequest = avm_waitrequest;
		vstate = idle;
	end
end

always_ff @(posedge(clk)) begin
    if(reset)
        rstate <= idle; //reset to idle state
    else
        rstate <= vstate;
end

always_comb begin
	vindex = 6'd32;
	for(i = 0; i < numentries; i++) begin
		if ((cputable[i] == cputag) && enableReg[i]) begin
			vindex = i;
		end
	end
end

always_comb begin
	case(avs_ctrl_reg_address[7:5])
		'b000: 	avs_ctrl_reg_readdata = enableReg;

		'b001: 	avs_ctrl_reg_readdata = validReg;

		'b010: 	avs_ctrl_reg_readdata = cputable[avs_ctrl_reg_address[4:0]];

		'b011: 	avs_ctrl_reg_readdata = spmtag;

		'b100: 	avs_ctrl_reg_readdata = tlb_sel;

		default: avs_ctrl_reg_readdata = '0;
	endcase
end

always @ (posedge clk) begin
	index = vindex;
	if(reset) begin
		index = 32;
		tlb_sel = '0;
		enableReg = '0;
		validReg = '0;
		v_avs_ctrl_reg_waitrequest = '1;
	end else begin
		if(avs_ctrl_reg_write) begin
			case(avs_ctrl_reg_address[7:5])
				'b000: 	enableReg = avs_ctrl_reg_writedata;

				'b001: 	validReg = avs_ctrl_reg_writedata;

				'b010: 	cputable[avs_ctrl_reg_address[4:0]] = avs_ctrl_reg_writedata[tagwidth-1:0];

				'b011: 	begin
							index = {27'b0, avs_ctrl_reg_address[4:0]};
							ram_store = avs_ctrl_reg_writedata[tagwidth-1:0];
						end
						
				'b100: 	tlb_sel = avs_ctrl_reg_writedata;
			endcase
			v_avs_ctrl_reg_waitrequest = 0;
		end else if(avs_ctrl_reg_read) begin
			v_avs_ctrl_reg_waitrequest = 0;
		end else begin
			v_avs_ctrl_reg_waitrequest = 1;
		end
	end
end

initial begin
	enableReg = '0;
	validReg = '0;
	tlb_sel = '0;
end

single_port_ram #(tagwidth, numentries, addresswidth) ram(
	.addr(index[addresswidth-1:0]),
	.data(ram_store),
	.we(avs_ctrl_reg_write & ram_addr_sel),
	.clk(clk),
	.q(spmtag)
);

endmodule
