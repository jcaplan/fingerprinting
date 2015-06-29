// (C) 2001-2013 Altera Corporation. All rights reserved.
// Your use of Altera Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License Subscription 
// Agreement, Altera MegaCore Function License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the applicable 
// agreement for further details.


// (C) 2001-2013 Altera Corporation. All rights reserved.
// Your use of Altera Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License Subscription 
// Agreement, Altera MegaCore Function License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the applicable 
// agreement for further details.


// $Id: //acds/rel/13.1/ip/merlin/altera_merlin_router/altera_merlin_router.sv.terp#5 $
// $Revision: #5 $
// $Date: 2013/09/30 $
// $Author: perforce $

// -------------------------------------------------------
// Merlin Router
//
// Asserts the appropriate one-hot encoded channel based on 
// either (a) the address or (b) the dest id. The DECODER_TYPE
// parameter controls this behaviour. 0 means address decoder,
// 1 means dest id decoder.
//
// In the case of (a), it also sets the destination id.
// -------------------------------------------------------

`timescale 1 ns / 1 ns

module nios_fprint_mm_interconnect_1_addr_router_004_default_decode
  #(
     parameter DEFAULT_CHANNEL = 3,
               DEFAULT_WR_CHANNEL = -1,
               DEFAULT_RD_CHANNEL = -1,
               DEFAULT_DESTID = 2 
   )
  (output [91 - 87 : 0] default_destination_id,
   output [30-1 : 0] default_wr_channel,
   output [30-1 : 0] default_rd_channel,
   output [30-1 : 0] default_src_channel
  );

  assign default_destination_id = 
    DEFAULT_DESTID[91 - 87 : 0];

  generate begin : default_decode
    if (DEFAULT_CHANNEL == -1) begin
      assign default_src_channel = '0;
    end
    else begin
      assign default_src_channel = 30'b1 << DEFAULT_CHANNEL;
    end
  end
  endgenerate

  generate begin : default_decode_rw
    if (DEFAULT_RD_CHANNEL == -1) begin
      assign default_wr_channel = '0;
      assign default_rd_channel = '0;
    end
    else begin
      assign default_wr_channel = 30'b1 << DEFAULT_WR_CHANNEL;
      assign default_rd_channel = 30'b1 << DEFAULT_RD_CHANNEL;
    end
  end
  endgenerate

endmodule


module nios_fprint_mm_interconnect_1_addr_router_004
(
    // -------------------
    // Clock & Reset
    // -------------------
    input clk,
    input reset,

    // -------------------
    // Command Sink (Input)
    // -------------------
    input                       sink_valid,
    input  [105-1 : 0]    sink_data,
    input                       sink_startofpacket,
    input                       sink_endofpacket,
    output                      sink_ready,

    // -------------------
    // Command Source (Output)
    // -------------------
    output                          src_valid,
    output reg [105-1    : 0] src_data,
    output reg [30-1 : 0] src_channel,
    output                          src_startofpacket,
    output                          src_endofpacket,
    input                           src_ready
);

    // -------------------------------------------------------
    // Local parameters and variables
    // -------------------------------------------------------
    localparam PKT_ADDR_H = 62;
    localparam PKT_ADDR_L = 36;
    localparam PKT_DEST_ID_H = 91;
    localparam PKT_DEST_ID_L = 87;
    localparam PKT_PROTECTION_H = 95;
    localparam PKT_PROTECTION_L = 93;
    localparam ST_DATA_W = 105;
    localparam ST_CHANNEL_W = 30;
    localparam DECODER_TYPE = 0;

    localparam PKT_TRANS_WRITE = 65;
    localparam PKT_TRANS_READ  = 66;

    localparam PKT_ADDR_W = PKT_ADDR_H-PKT_ADDR_L + 1;
    localparam PKT_DEST_ID_W = PKT_DEST_ID_H-PKT_DEST_ID_L + 1;



    // -------------------------------------------------------
    // Figure out the number of bits to mask off for each slave span
    // during address decoding
    // -------------------------------------------------------
    localparam PAD0 = log2ceil(64'h2000000 - 64'h0); 
    localparam PAD1 = log2ceil(64'h3000400 - 64'h3000000); 
    localparam PAD2 = log2ceil(64'h4000400 - 64'h4000000); 
    localparam PAD3 = log2ceil(64'h4000800 - 64'h4000400); 
    localparam PAD4 = log2ceil(64'h4000c00 - 64'h4000800); 
    localparam PAD5 = log2ceil(64'h4001000 - 64'h4000c00); 
    localparam PAD6 = log2ceil(64'h4001400 - 64'h4001000); 
    localparam PAD7 = log2ceil(64'h4001800 - 64'h4001400); 
    localparam PAD8 = log2ceil(64'h4001c00 - 64'h4001800); 
    localparam PAD9 = log2ceil(64'h4002000 - 64'h4001c00); 
    localparam PAD10 = log2ceil(64'h4002400 - 64'h4002000); 
    localparam PAD11 = log2ceil(64'h4500008 - 64'h4500000); 
    localparam PAD12 = log2ceil(64'h5001000 - 64'h5000000); 
    localparam PAD13 = log2ceil(64'h5100020 - 64'h5100000); 
    localparam PAD14 = log2ceil(64'h5110020 - 64'h5110000); 
    localparam PAD15 = log2ceil(64'h5120020 - 64'h5120000); 
    localparam PAD16 = log2ceil(64'h5130020 - 64'h5130000); 
    localparam PAD17 = log2ceil(64'h5140020 - 64'h5140000); 
    localparam PAD18 = log2ceil(64'h5150020 - 64'h5150000); 
    localparam PAD19 = log2ceil(64'h5160020 - 64'h5160000); 
    localparam PAD20 = log2ceil(64'h5170020 - 64'h5170000); 
    localparam PAD21 = log2ceil(64'h5200008 - 64'h5200000); 
    // -------------------------------------------------------
    // Work out which address bits are significant based on the
    // address range of the slaves. If the required width is too
    // large or too small, we use the address field width instead.
    // -------------------------------------------------------
    localparam ADDR_RANGE = 64'h5200008;
    localparam RANGE_ADDR_WIDTH = log2ceil(ADDR_RANGE);
    localparam OPTIMIZED_ADDR_H = (RANGE_ADDR_WIDTH > PKT_ADDR_W) ||
                                  (RANGE_ADDR_WIDTH == 0) ?
                                        PKT_ADDR_H :
                                        PKT_ADDR_L + RANGE_ADDR_WIDTH - 1;

    localparam RG = RANGE_ADDR_WIDTH-1;
    localparam REAL_ADDRESS_RANGE = OPTIMIZED_ADDR_H - PKT_ADDR_L;

      reg [PKT_ADDR_W-1 : 0] address;
      always @* begin
        address = {PKT_ADDR_W{1'b0}};
        address [REAL_ADDRESS_RANGE:0] = sink_data[OPTIMIZED_ADDR_H : PKT_ADDR_L];
      end   

    // -------------------------------------------------------
    // Pass almost everything through, untouched
    // -------------------------------------------------------
    assign sink_ready        = src_ready;
    assign src_valid         = sink_valid;
    assign src_startofpacket = sink_startofpacket;
    assign src_endofpacket   = sink_endofpacket;
    wire [PKT_DEST_ID_W-1:0] default_destid;
    wire [30-1 : 0] default_src_channel;




    // -------------------------------------------------------
    // Write and read transaction signals
    // -------------------------------------------------------
    wire write_transaction;
    assign write_transaction = sink_data[PKT_TRANS_WRITE];


    nios_fprint_mm_interconnect_1_addr_router_004_default_decode the_default_decode(
      .default_destination_id (default_destid),
      .default_wr_channel   (),
      .default_rd_channel   (),
      .default_src_channel  (default_src_channel)
    );

    always @* begin
        src_data    = sink_data;
        src_channel = default_src_channel;
        src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = default_destid;

        // --------------------------------------------------
        // Address Decoder
        // Sets the channel and destination ID based on the address
        // --------------------------------------------------

    // ( 0x0 .. 0x2000000 )
    if ( {address[RG:PAD0],{PAD0{1'b0}}} == 27'h0   ) begin
            src_channel = 30'b0000000000000000001000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 2;
    end

    // ( 0x3000000 .. 0x3000400 )
    if ( {address[RG:PAD1],{PAD1{1'b0}}} == 27'h3000000   ) begin
            src_channel = 30'b0000000000000000000100;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 29;
    end

    // ( 0x4000000 .. 0x4000400 )
    if ( {address[RG:PAD2],{PAD2{1'b0}}} == 27'h4000000  && write_transaction  ) begin
            src_channel = 30'b0000000000000000010000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 4;
    end

    // ( 0x4000400 .. 0x4000800 )
    if ( {address[RG:PAD3],{PAD3{1'b0}}} == 27'h4000400  && write_transaction  ) begin
            src_channel = 30'b0000000000000000000010;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 7;
    end

    // ( 0x4000800 .. 0x4000c00 )
    if ( {address[RG:PAD4],{PAD4{1'b0}}} == 27'h4000800  && write_transaction  ) begin
            src_channel = 30'b0000000000000000100000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 28;
    end

    // ( 0x4000c00 .. 0x4001000 )
    if ( {address[RG:PAD5],{PAD5{1'b0}}} == 27'h4000c00  && write_transaction  ) begin
            src_channel = 30'b0000000000000001000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 10;
    end

    // ( 0x4001000 .. 0x4001400 )
    if ( {address[RG:PAD6],{PAD6{1'b0}}} == 27'h4001000  && write_transaction  ) begin
            src_channel = 30'b0000000000000010000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 13;
    end

    // ( 0x4001400 .. 0x4001800 )
    if ( {address[RG:PAD7],{PAD7{1'b0}}} == 27'h4001400  && write_transaction  ) begin
            src_channel = 30'b0000000010000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 16;
    end

    // ( 0x4001800 .. 0x4001c00 )
    if ( {address[RG:PAD8],{PAD8{1'b0}}} == 27'h4001800  && write_transaction  ) begin
            src_channel = 30'b0000001000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 19;
    end

    // ( 0x4001c00 .. 0x4002000 )
    if ( {address[RG:PAD9],{PAD9{1'b0}}} == 27'h4001c00  && write_transaction  ) begin
            src_channel = 30'b0000100000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 22;
    end

    // ( 0x4002000 .. 0x4002400 )
    if ( {address[RG:PAD10],{PAD10{1'b0}}} == 27'h4002000  && write_transaction  ) begin
            src_channel = 30'b0010000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 25;
    end

    // ( 0x4500000 .. 0x4500008 )
    if ( {address[RG:PAD11],{PAD11{1'b0}}} == 27'h4500000   ) begin
            src_channel = 30'b0000000000000000000001;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 3;
    end

    // ( 0x5000000 .. 0x5001000 )
    if ( {address[RG:PAD12],{PAD12{1'b0}}} == 27'h5000000   ) begin
            src_channel = 30'b0000000000000100000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 0;
    end

    // ( 0x5100000 .. 0x5100020 )
    if ( {address[RG:PAD13],{PAD13{1'b0}}} == 27'h5100000   ) begin
            src_channel = 30'b0000000000010000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 6;
    end

    // ( 0x5110000 .. 0x5110020 )
    if ( {address[RG:PAD14],{PAD14{1'b0}}} == 27'h5110000   ) begin
            src_channel = 30'b0000000000001000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 9;
    end

    // ( 0x5120000 .. 0x5120020 )
    if ( {address[RG:PAD15],{PAD15{1'b0}}} == 27'h5120000   ) begin
            src_channel = 30'b0000000001000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 12;
    end

    // ( 0x5130000 .. 0x5130020 )
    if ( {address[RG:PAD16],{PAD16{1'b0}}} == 27'h5130000   ) begin
            src_channel = 30'b0000000000100000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 15;
    end

    // ( 0x5140000 .. 0x5140020 )
    if ( {address[RG:PAD17],{PAD17{1'b0}}} == 27'h5140000   ) begin
            src_channel = 30'b0000000100000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 18;
    end

    // ( 0x5150000 .. 0x5150020 )
    if ( {address[RG:PAD18],{PAD18{1'b0}}} == 27'h5150000   ) begin
            src_channel = 30'b0000010000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 21;
    end

    // ( 0x5160000 .. 0x5160020 )
    if ( {address[RG:PAD19],{PAD19{1'b0}}} == 27'h5160000   ) begin
            src_channel = 30'b0001000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 24;
    end

    // ( 0x5170000 .. 0x5170020 )
    if ( {address[RG:PAD20],{PAD20{1'b0}}} == 27'h5170000   ) begin
            src_channel = 30'b0100000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 27;
    end

    // ( 0x5200000 .. 0x5200008 )
    if ( {address[RG:PAD21],{PAD21{1'b0}}} == 27'h5200000  && write_transaction  ) begin
            src_channel = 30'b1000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 1;
    end

end


    // --------------------------------------------------
    // Ceil(log2()) function
    // --------------------------------------------------
    function integer log2ceil;
        input reg[65:0] val;
        reg [65:0] i;

        begin
            i = 1;
            log2ceil = 0;

            while (i < val) begin
                log2ceil = log2ceil + 1;
                i = i << 1;
            end
        end
    endfunction

endmodule

