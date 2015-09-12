
# (C) 2001-2014 Altera Corporation. All rights reserved.
# Your use of Altera Corporation's design tools, logic functions and 
# other software and tools, and its AMPP partner logic functions, and 
# any output files any of the foregoing (including device programming 
# or simulation files), and any associated documentation or information 
# are expressly subject to the terms and conditions of the Altera 
# Program License Subscription Agreement, Altera MegaCore Function 
# License Agreement, or other applicable license agreement, including, 
# without limitation, that your use is for the sole purpose of 
# programming logic devices manufactured by Altera and sold by Altera 
# or its authorized distributors. Please refer to the applicable 
# agreement for further details.

# ACDS 13.1 162 linux 2014.07.12.16:45:11

# ----------------------------------------
# vcs - auto-generated simulation script

# ----------------------------------------
# initialize variables
TOP_LEVEL_NAME="nios_fprint_tb"
QSYS_SIMDIR="./../../"
QUARTUS_INSTALL_DIR="/CMC/tools/altera/13.1/quartus/"
SKIP_FILE_COPY=0
SKIP_ELAB=0
SKIP_SIM=0
USER_DEFINED_ELAB_OPTIONS=""
USER_DEFINED_SIM_OPTIONS="+vcs+finish+100"
# ----------------------------------------
# overwrite variables - DO NOT MODIFY!
# This block evaluates each command line argument, typically used for 
# overwriting variables. An example usage:
#   sh <simulator>_setup.sh SKIP_ELAB=1 SKIP_SIM=1
for expression in "$@"; do
  eval $expression
  if [ $? -ne 0 ]; then
    echo "Error: This command line argument, \"$expression\", is/has an invalid expression." >&2
    exit $?
  fi
done

# ----------------------------------------
# initialize simulation properties - DO NOT MODIFY!
ELAB_OPTIONS=""
SIM_OPTIONS=""
if [[ `vcs -platform` != *"amd64"* ]]; then
  :
else
  :
fi

# ----------------------------------------
# copy RAM/ROM files to simulation directory
if [ $SKIP_FILE_COPY -eq 0 ]; then
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_rf_ram_a.dat ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_rf_ram_a.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_rf_ram_a.mif ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_ic_tag_ram.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_rf_ram_b.mif ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_rf_ram_b.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_rf_ram_b.dat ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_ociram_default_contents.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_ic_tag_ram.dat ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_ociram_default_contents.dat ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_ociram_default_contents.mif ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_ic_tag_ram.mif ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_ic_tag_ram.mif ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_rf_ram_a.mif ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_ic_tag_ram.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_rf_ram_b.dat ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_rf_ram_b.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_rf_ram_b.mif ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_ociram_default_contents.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_ociram_default_contents.mif ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_ic_tag_ram.dat ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_rf_ram_a.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_ociram_default_contents.dat ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_rf_ram_a.dat ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_ic_tag_ram.mif ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_ic_tag_ram.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_rf_ram_b.dat ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_rf_ram_b.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_rf_ram_a.dat ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_ociram_default_contents.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_rf_ram_a.mif ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_rf_ram_a.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_ociram_default_contents.mif ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_ociram_default_contents.dat ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_rf_ram_b.mif ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_ic_tag_ram.dat ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_2.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_3.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_shared_memory.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_1.hex ./
  cp -f $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_0.hex ./
fi

vcs -lca -timescale=1ps/1ps -sverilog +verilog2001ext+.v -ntb_opts dtm $ELAB_OPTIONS $USER_DEFINED_ELAB_OPTIONS \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/altera_primitives.v \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/220model.v \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/sgate.v \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/altera_mf.v \
  $QUARTUS_INSTALL_DIR/eda/sim_lib/altera_lnsim.sv \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/synopsys/arriav_atoms_ncrypt.v \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/synopsys/arriav_hmi_atoms_ncrypt.v \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/arriav_atoms.v \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/synopsys/arriav_hssi_atoms_ncrypt.v \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/arriav_hssi_atoms.v \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/synopsys/arriav_pcie_hip_atoms_ncrypt.v \
  -v $QUARTUS_INSTALL_DIR/eda/sim_lib/arriav_pcie_hip_atoms.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/verbosity_pkg.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_mm_interconnect_0_addr_router_001.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_mm_interconnect_0_addr_router.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_rsp_xbar_mux_001.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_rsp_xbar_mux.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_rsp_xbar_demux_003.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_rsp_xbar_demux.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_mux_003.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_mux_001.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_mux.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_demux_001.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_demux.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_id_router_003.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_id_router.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_addr_router_001.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_addr_router.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_rsp_xbar_mux.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_rsp_xbar_demux.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_cmd_xbar_mux.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_cmd_xbar_demux.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_id_router.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_addr_router.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_mux_002.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_mux_001.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_mux.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_demux_003.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_demux_001.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_demux.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_mux_003.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_mux_001.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_mux.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_demux_002.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_demux_001.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_demux.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_traffic_limiter.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_reorder_memory.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_sc_fifo.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_st_pipeline_base.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router_012.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router_010.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router_003.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router_001.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_addr_router_002.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_addr_router_001.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_addr_router.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_slave_agent.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_burst_uncompressor.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_master_agent.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_slave_translator.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_master_translator.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_jtag_debug_module_wrapper.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_oci_test_bench.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_test_bench.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_jtag_debug_module_sysclk.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1.vo \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_jtag_debug_module_tck.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_irq_mapper.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_mm_interconnect_0.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_jtag_debug_module_tck.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_jtag_debug_module_sysclk.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1.vo \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_oci_test_bench.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_test_bench.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_jtag_debug_module_wrapper.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_irq_mapper.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_timestamp.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/cpu_irq.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/fprint_irq.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_button_pio.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_jtag_uart.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_timer.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_mm_bridge.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_jtag_debug_module_sysclk.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_test_bench.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0.vo \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_jtag_debug_module_tck.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_jtag_debug_module_wrapper.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_oci_test_bench.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_reset_controller.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_reset_synchronizer.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_2.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_3.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_fingerprint.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_counter.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_fsm_rtla.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_rtla.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_sb_fsm.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_store_buffer.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/fifo.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_pause.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/lifo.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_buffer.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_cfpu.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_comparator.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_directory.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_state_regs.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mutex_0.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_shared_memory.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/led_pio.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_defines.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_1.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_0.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_sys_id.vo \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_reset_source.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_clock_source.sv \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint.v \
  $QSYS_SIMDIR/nios_fprint_tb/simulation/nios_fprint_tb.v \
  -top $TOP_LEVEL_NAME
# ----------------------------------------
# simulate
if [ $SKIP_SIM -eq 0 ]; then
  ./simv $SIM_OPTIONS $USER_DEFINED_SIM_OPTIONS
fi
