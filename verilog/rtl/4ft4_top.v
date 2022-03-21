`default_nettype none

module top_4ft4(
`ifdef USE_POWER_PINS
    inout vccd1, // User area 1 1.8V supply
    inout vssd1, // User area 1 digital ground
`endif
    input wb_clock_i,
    input wb_reset_i,
    input wb_strobe_i,
    input wb_cyc_i,
    input wb_we_i,
    input [3:0] wb_sel_i,
    input [31:0] wb_data_i,
    input [31:0] wb_addr_i,
    output [31:0] wb_data_o,
    output wb_ack_o,
    input  [127:0] la_data_in,
    output [127:0] la_data_out,
    input  [127:0] la_oenb,
    input  [`MPRJ_IO_PADS-1:0] io_in,
    output [`MPRJ_IO_PADS-1:0] io_out,
    output [`MPRJ_IO_PADS-1:0] io_oeb,
    output [2:0] irq
);

wire halt;
assign halt = (~la_oenb[0]) ? la_data_in[0] : 1'b0;

wire la_reset;
wire reset;

assign la_reset = (~la_oenb[32]) ? la_data_in[32] : 1'b0;
assign reset = la_reset | wb_reset_i;

wire [3:0] rom_in;
assign rom_in = io_in[3:0];
assign io_oeb[3:0] = 4'hf;

wire [7:0] ram_out;
assign io_out[11:4] = ram_out;
assign io_oeb[11:4] = {8{reset}};

assign io_out[`MPRJ_IO_PADS-1:12] = 0;
assign io_oeb[`MPRJ_IO_PADS-1:12] = 0;

assign la_data_out = {{56{1'b0}}, ram_out, {64{1'b0}}};

assign irq = 3'b000;

wb_system sys(
    .clock(wb_clock_i),
    .reset(reset),
    .halt(halt),
    .rom_in(rom_in),
    .ram_out(ram_out),

    .wb_data_i(wb_data_i),
    .wb_addr_i(wb_addr_i),
    .wb_cyc_i(wb_cyc_i),
    .wb_strobe_i(wb_strobe_i),
    .wb_we_i(wb_we_i),
    .wb_data_o(wb_data_o),
    .wb_ack_o(wb_ack_o)
);

endmodule
