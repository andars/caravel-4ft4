set signals [list]

lappend signals "uut.mprj.mprj.wb_clock_i"
lappend signals "uut.mprj.mprj.wb_reset_i"
lappend signals "uut.mprj.mprj.wb_cyc_i"
lappend signals "uut.mprj.mprj.wb_strobe_i"
lappend signals "uut.mprj.mprj.wb_we_i"
lappend signals "uut.mprj.mprj.wb_addr_i"
lappend signals "uut.mprj.mprj.wb_data_i"
lappend signals "uut.mprj.mprj.wb_data_o"
lappend signals "uut.mprj.mprj.wb_ack_o"
lappend signals "uut.mprj_io\[31:16\]"
#lappend signals "tb_system.dut.cpu.datapath.\\registers\[13\]\[3:0\]"
#lappend signals "tb_system.dut.cpu.datapath.\\registers\[14\]\[3:0\]"

set num_added [ gtkwave::addSignalsFromList $signals ]
