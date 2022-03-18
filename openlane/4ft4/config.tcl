set ::env(PDK) "sky130A"
set ::env(STD_CELL_LIBRARY) "sky130_fd_sc_hd"

set script_dir [file dirname [file normalize [info script]]]

set ::env(DESIGN_NAME) top_4ft4

set ::env(SYNTH_DEFINES) [list "NO_TRISTATE" "ROM_HEX_FILE=\"${script_dir}/../../verilog/rtl/4ft4_rom.hex\"" "ROM_CAPACITY=128"]

set ::env(VERILOG_FILES) "\
	$::env(CARAVEL_ROOT)/verilog/rtl/defines.v \
	$script_dir/../../verilog/rtl/4ft4_top.v \
	$script_dir/../../4ft4/rtl/wb_system.v \
	$script_dir/../../4ft4/rtl/cpu.v \
	$script_dir/../../4ft4/rtl/cpu_control.v \
	$script_dir/../../4ft4/rtl/alu.v \
	$script_dir/../../4ft4/rtl/datapath.v \
	$script_dir/../../4ft4/rtl/pc_stack.v \
	$script_dir/../../4ft4/rtl/rom.v \
	$script_dir/../../4ft4/rtl/ram.v"

set ::env(DESIGN_IS_CORE) 0

set ::env(CLOCK_PORT) "wb_clock_i"
set ::env(CLOCK_NET) "wb_clock_i"
set ::env(CLOCK_PERIOD) "10"

#set ::env(FP_SIZING) absolute
#set ::env(DIE_AREA) "0 0 900 900"

set ::env(FP_SIZING) relative
set ::env(FP_CORE_UTIL) 10

set ::env(FP_PIN_ORDER_CFG) $script_dir/pin_order.cfg

set ::env(PL_BASIC_PLACEMENT) 0
set ::env(PL_TARGET_DENSITY) 0.12

# Maximum layer used for routing is metal 4.
# This is because this macro will be inserted in a top level (user_project_wrapper)
# where the PDN is planned on metal 5. So, to avoid having shorts between routes
# in this macro and the top level metal 5 stripes, we have to restrict routes to metal4.
# 
# set ::env(GLB_RT_MAXLAYER) 5

set ::env(RT_MAX_LAYER) {met4}

# You can draw more power domains if you need to
set ::env(VDD_NETS) [list {vccd1}]
set ::env(GND_NETS) [list {vssd1}]

set ::env(DIODE_INSERTION_STRATEGY) 4
# If you're going to use multiple power domains, then disable cvc run.
set ::env(RUN_CVC) 1
