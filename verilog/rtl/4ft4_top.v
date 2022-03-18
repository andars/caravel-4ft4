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

wb_system sys(
    .clock(wb_clock_i),
    .reset(wb_reset_i),
    .rom_out(),

    .wb_data_i(wb_data_i),
    .wb_addr_i(wb_addr_i),
    .wb_cyc_i(wb_cyc_i),
    .wb_strobe_i(wb_strobe_i),
    .wb_we_i(wb_we_i),
    .wb_data_o(wb_data_o),
    .wb_ack_o(wb_ack_o)
);

endmodule
