
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
# ncsim - auto-generated simulation script

# ----------------------------------------
# initialize variables
TOP_LEVEL_NAME="nios_fprint_tb"
QSYS_SIMDIR="./../"
QUARTUS_INSTALL_DIR="/CMC/tools/altera/13.1/quartus/"
SKIP_FILE_COPY=0
SKIP_DEV_COM=0
SKIP_COM=0
SKIP_ELAB=0
SKIP_SIM=0
USER_DEFINED_ELAB_OPTIONS=""
USER_DEFINED_SIM_OPTIONS="-input \"@run 100; exit\""

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
if [[ `ncsim -version` != *"ncsim(64)"* ]]; then
  :
else
  :
fi

# ----------------------------------------
# create compilation libraries
mkdir -p ./libraries/work/
mkdir -p ./libraries/altera_common_sv_packages/
mkdir -p ./libraries/addr_router_001/
mkdir -p ./libraries/addr_router/
mkdir -p ./libraries/rsp_xbar_mux_001/
mkdir -p ./libraries/rsp_xbar_mux/
mkdir -p ./libraries/rsp_xbar_demux_003/
mkdir -p ./libraries/rsp_xbar_demux/
mkdir -p ./libraries/cmd_xbar_mux_003/
mkdir -p ./libraries/cmd_xbar_mux_001/
mkdir -p ./libraries/cmd_xbar_mux/
mkdir -p ./libraries/cmd_xbar_demux_001/
mkdir -p ./libraries/cmd_xbar_demux/
mkdir -p ./libraries/id_router_003/
mkdir -p ./libraries/id_router/
mkdir -p ./libraries/rsp_xbar_mux_002/
mkdir -p ./libraries/rsp_xbar_demux_001/
mkdir -p ./libraries/cmd_xbar_demux_002/
mkdir -p ./libraries/limiter/
mkdir -p ./libraries/id_router_012/
mkdir -p ./libraries/id_router_010/
mkdir -p ./libraries/id_router_001/
mkdir -p ./libraries/addr_router_002/
mkdir -p ./libraries/sys_id_control_slave_translator_avalon_universal_slave_0_agent_rsp_fifo/
mkdir -p ./libraries/sys_id_control_slave_translator_avalon_universal_slave_0_agent/
mkdir -p ./libraries/processorMonitor_outgoing_master_translator_avalon_universal_master_0_agent/
mkdir -p ./libraries/sys_id_control_slave_translator/
mkdir -p ./libraries/processorMonitor_outgoing_master_translator/
mkdir -p ./libraries/cpu1/
mkdir -p ./libraries/irq_mapper/
mkdir -p ./libraries/mm_interconnect_0/
mkdir -p ./libraries/timestamp/
mkdir -p ./libraries/cpu_irq_0/
mkdir -p ./libraries/fprint_irq_0/
mkdir -p ./libraries/button_pio/
mkdir -p ./libraries/jtag_uart/
mkdir -p ./libraries/timer/
mkdir -p ./libraries/out_system_bridge/
mkdir -p ./libraries/cpu0/
mkdir -p ./libraries/rst_controller/
mkdir -p ./libraries/mm_interconnect_1/
mkdir -p ./libraries/onchip_memory2_2/
mkdir -p ./libraries/processor2_0/
mkdir -p ./libraries/onchip_memory2_3/
mkdir -p ./libraries/Fingerprint_2/
mkdir -p ./libraries/cfpu_0/
mkdir -p ./libraries/mutex_0/
mkdir -p ./libraries/shared_memory/
mkdir -p ./libraries/led_pio/
mkdir -p ./libraries/onchip_memory2_1/
mkdir -p ./libraries/onchip_memory2_0/
mkdir -p ./libraries/processor1_0/
mkdir -p ./libraries/processorMonitor/
mkdir -p ./libraries/sys_id/
mkdir -p ./libraries/nios_fprint_inst_reset_bfm/
mkdir -p ./libraries/nios_fprint_inst_clk_bfm/
mkdir -p ./libraries/nios_fprint_inst/
mkdir -p ./libraries/altera_ver/
mkdir -p ./libraries/lpm_ver/
mkdir -p ./libraries/sgate_ver/
mkdir -p ./libraries/altera_mf_ver/
mkdir -p ./libraries/altera_lnsim_ver/
mkdir -p ./libraries/arriav_ver/
mkdir -p ./libraries/arriav_hssi_ver/
mkdir -p ./libraries/arriav_pcie_hip_ver/

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

# ----------------------------------------
# compile device library files
if [ $SKIP_DEV_COM -eq 0 ]; then
  ncvlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/altera_primitives.v"                    -work altera_ver         
  ncvlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/220model.v"                             -work lpm_ver            
  ncvlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/sgate.v"                                -work sgate_ver          
  ncvlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/altera_mf.v"                            -work altera_mf_ver      
  ncvlog -sv "$QUARTUS_INSTALL_DIR/eda/sim_lib/altera_lnsim.sv"                        -work altera_lnsim_ver   
  ncvlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/cadence/arriav_atoms_ncrypt.v"          -work arriav_ver         
  ncvlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/cadence/arriav_hmi_atoms_ncrypt.v"      -work arriav_ver         
  ncvlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/arriav_atoms.v"                         -work arriav_ver         
  ncvlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/cadence/arriav_hssi_atoms_ncrypt.v"     -work arriav_hssi_ver    
  ncvlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/arriav_hssi_atoms.v"                    -work arriav_hssi_ver    
  ncvlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/cadence/arriav_pcie_hip_atoms_ncrypt.v" -work arriav_pcie_hip_ver
  ncvlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/arriav_pcie_hip_atoms.v"                -work arriav_pcie_hip_ver
fi

# ----------------------------------------
# compile design files in correct order
if [ $SKIP_COM -eq 0 ]; then
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/verbosity_pkg.sv"                                                     -work altera_common_sv_packages                                                   -cdslib ./cds_libs/altera_common_sv_packages.cds.lib                                                  
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_mm_interconnect_0_addr_router_001.sv"        -work addr_router_001                                                             -cdslib ./cds_libs/addr_router_001.cds.lib                                                            
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_mm_interconnect_0_addr_router.sv"            -work addr_router                                                                 -cdslib ./cds_libs/addr_router.cds.lib                                                                
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -work rsp_xbar_mux_001                                                            -cdslib ./cds_libs/rsp_xbar_mux_001.cds.lib                                                           
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_rsp_xbar_mux_001.sv"   -work rsp_xbar_mux_001                                                            -cdslib ./cds_libs/rsp_xbar_mux_001.cds.lib                                                           
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -work rsp_xbar_mux                                                                -cdslib ./cds_libs/rsp_xbar_mux.cds.lib                                                               
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_rsp_xbar_mux.sv"       -work rsp_xbar_mux                                                                -cdslib ./cds_libs/rsp_xbar_mux.cds.lib                                                               
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_rsp_xbar_demux_003.sv" -work rsp_xbar_demux_003                                                          -cdslib ./cds_libs/rsp_xbar_demux_003.cds.lib                                                         
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_rsp_xbar_demux.sv"     -work rsp_xbar_demux                                                              -cdslib ./cds_libs/rsp_xbar_demux.cds.lib                                                             
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -work cmd_xbar_mux_003                                                            -cdslib ./cds_libs/cmd_xbar_mux_003.cds.lib                                                           
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_mux_003.sv"   -work cmd_xbar_mux_003                                                            -cdslib ./cds_libs/cmd_xbar_mux_003.cds.lib                                                           
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -work cmd_xbar_mux_001                                                            -cdslib ./cds_libs/cmd_xbar_mux_001.cds.lib                                                           
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_mux_001.sv"   -work cmd_xbar_mux_001                                                            -cdslib ./cds_libs/cmd_xbar_mux_001.cds.lib                                                           
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -work cmd_xbar_mux                                                                -cdslib ./cds_libs/cmd_xbar_mux.cds.lib                                                               
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_mux.sv"       -work cmd_xbar_mux                                                                -cdslib ./cds_libs/cmd_xbar_mux.cds.lib                                                               
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_demux_001.sv" -work cmd_xbar_demux_001                                                          -cdslib ./cds_libs/cmd_xbar_demux_001.cds.lib                                                         
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_demux.sv"     -work cmd_xbar_demux                                                              -cdslib ./cds_libs/cmd_xbar_demux.cds.lib                                                             
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_id_router_003.sv"      -work id_router_003                                                               -cdslib ./cds_libs/id_router_003.cds.lib                                                              
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_id_router.sv"          -work id_router                                                                   -cdslib ./cds_libs/id_router.cds.lib                                                                  
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_addr_router_001.sv"    -work addr_router_001                                                             -cdslib ./cds_libs/addr_router_001.cds.lib                                                            
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_addr_router.sv"        -work addr_router                                                                 -cdslib ./cds_libs/addr_router.cds.lib                                                                
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -work rsp_xbar_mux                                                                -cdslib ./cds_libs/rsp_xbar_mux.cds.lib                                                               
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_rsp_xbar_mux.sv"                        -work rsp_xbar_mux                                                                -cdslib ./cds_libs/rsp_xbar_mux.cds.lib                                                               
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_rsp_xbar_demux.sv"                      -work rsp_xbar_demux                                                              -cdslib ./cds_libs/rsp_xbar_demux.cds.lib                                                             
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -work cmd_xbar_mux                                                                -cdslib ./cds_libs/cmd_xbar_mux.cds.lib                                                               
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_cmd_xbar_mux.sv"                        -work cmd_xbar_mux                                                                -cdslib ./cds_libs/cmd_xbar_mux.cds.lib                                                               
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_cmd_xbar_demux.sv"                      -work cmd_xbar_demux                                                              -cdslib ./cds_libs/cmd_xbar_demux.cds.lib                                                             
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_id_router.sv"                           -work id_router                                                                   -cdslib ./cds_libs/id_router.cds.lib                                                                  
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_addr_router.sv"                         -work addr_router                                                                 -cdslib ./cds_libs/addr_router.cds.lib                                                                
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -work rsp_xbar_mux_002                                                            -cdslib ./cds_libs/rsp_xbar_mux_002.cds.lib                                                           
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_mux_002.sv"                    -work rsp_xbar_mux_002                                                            -cdslib ./cds_libs/rsp_xbar_mux_002.cds.lib                                                           
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -work rsp_xbar_mux_001                                                            -cdslib ./cds_libs/rsp_xbar_mux_001.cds.lib                                                           
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_mux_001.sv"                    -work rsp_xbar_mux_001                                                            -cdslib ./cds_libs/rsp_xbar_mux_001.cds.lib                                                           
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -work rsp_xbar_mux                                                                -cdslib ./cds_libs/rsp_xbar_mux.cds.lib                                                               
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_mux.sv"                        -work rsp_xbar_mux                                                                -cdslib ./cds_libs/rsp_xbar_mux.cds.lib                                                               
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_demux_003.sv"                  -work rsp_xbar_demux_003                                                          -cdslib ./cds_libs/rsp_xbar_demux_003.cds.lib                                                         
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_demux_001.sv"                  -work rsp_xbar_demux_001                                                          -cdslib ./cds_libs/rsp_xbar_demux_001.cds.lib                                                         
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_demux.sv"                      -work rsp_xbar_demux                                                              -cdslib ./cds_libs/rsp_xbar_demux.cds.lib                                                             
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -work cmd_xbar_mux_003                                                            -cdslib ./cds_libs/cmd_xbar_mux_003.cds.lib                                                           
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_mux_003.sv"                    -work cmd_xbar_mux_003                                                            -cdslib ./cds_libs/cmd_xbar_mux_003.cds.lib                                                           
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -work cmd_xbar_mux_001                                                            -cdslib ./cds_libs/cmd_xbar_mux_001.cds.lib                                                           
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_mux_001.sv"                    -work cmd_xbar_mux_001                                                            -cdslib ./cds_libs/cmd_xbar_mux_001.cds.lib                                                           
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -work cmd_xbar_mux                                                                -cdslib ./cds_libs/cmd_xbar_mux.cds.lib                                                               
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_mux.sv"                        -work cmd_xbar_mux                                                                -cdslib ./cds_libs/cmd_xbar_mux.cds.lib                                                               
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_demux_002.sv"                  -work cmd_xbar_demux_002                                                          -cdslib ./cds_libs/cmd_xbar_demux_002.cds.lib                                                         
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_demux_001.sv"                  -work cmd_xbar_demux_001                                                          -cdslib ./cds_libs/cmd_xbar_demux_001.cds.lib                                                         
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_demux.sv"                      -work cmd_xbar_demux                                                              -cdslib ./cds_libs/cmd_xbar_demux.cds.lib                                                             
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_traffic_limiter.sv"                                     -work limiter                                                                     -cdslib ./cds_libs/limiter.cds.lib                                                                    
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_reorder_memory.sv"                                      -work limiter                                                                     -cdslib ./cds_libs/limiter.cds.lib                                                                    
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_sc_fifo.v"                                              -work limiter                                                                     -cdslib ./cds_libs/limiter.cds.lib                                                                    
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_st_pipeline_base.v"                                     -work limiter                                                                     -cdslib ./cds_libs/limiter.cds.lib                                                                    
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router_012.sv"                       -work id_router_012                                                               -cdslib ./cds_libs/id_router_012.cds.lib                                                              
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router_010.sv"                       -work id_router_010                                                               -cdslib ./cds_libs/id_router_010.cds.lib                                                              
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router_003.sv"                       -work id_router_003                                                               -cdslib ./cds_libs/id_router_003.cds.lib                                                              
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router_001.sv"                       -work id_router_001                                                               -cdslib ./cds_libs/id_router_001.cds.lib                                                              
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router.sv"                           -work id_router                                                                   -cdslib ./cds_libs/id_router.cds.lib                                                                  
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_addr_router_002.sv"                     -work addr_router_002                                                             -cdslib ./cds_libs/addr_router_002.cds.lib                                                            
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_addr_router_001.sv"                     -work addr_router_001                                                             -cdslib ./cds_libs/addr_router_001.cds.lib                                                            
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_addr_router.sv"                         -work addr_router                                                                 -cdslib ./cds_libs/addr_router.cds.lib                                                                
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_sc_fifo.v"                                              -work sys_id_control_slave_translator_avalon_universal_slave_0_agent_rsp_fifo     -cdslib ./cds_libs/sys_id_control_slave_translator_avalon_universal_slave_0_agent_rsp_fifo.cds.lib    
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_slave_agent.sv"                                         -work sys_id_control_slave_translator_avalon_universal_slave_0_agent              -cdslib ./cds_libs/sys_id_control_slave_translator_avalon_universal_slave_0_agent.cds.lib             
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_burst_uncompressor.sv"                                  -work sys_id_control_slave_translator_avalon_universal_slave_0_agent              -cdslib ./cds_libs/sys_id_control_slave_translator_avalon_universal_slave_0_agent.cds.lib             
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_master_agent.sv"                                        -work processorMonitor_outgoing_master_translator_avalon_universal_master_0_agent -cdslib ./cds_libs/processorMonitor_outgoing_master_translator_avalon_universal_master_0_agent.cds.lib
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_slave_translator.sv"                                    -work sys_id_control_slave_translator                                             -cdslib ./cds_libs/sys_id_control_slave_translator.cds.lib                                            
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_master_translator.sv"                                   -work processorMonitor_outgoing_master_translator                                 -cdslib ./cds_libs/processorMonitor_outgoing_master_translator.cds.lib                                
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_jtag_debug_module_wrapper.v"            -work cpu1                                                                        -cdslib ./cds_libs/cpu1.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_oci_test_bench.v"                       -work cpu1                                                                        -cdslib ./cds_libs/cpu1.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_test_bench.v"                           -work cpu1                                                                        -cdslib ./cds_libs/cpu1.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_jtag_debug_module_sysclk.v"             -work cpu1                                                                        -cdslib ./cds_libs/cpu1.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1.vo"                                     -work cpu1                                                                        -cdslib ./cds_libs/cpu1.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_jtag_debug_module_tck.v"                -work cpu1                                                                        -cdslib ./cds_libs/cpu1.cds.lib                                                                       
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_irq_mapper.sv"                               -work irq_mapper                                                                  -cdslib ./cds_libs/irq_mapper.cds.lib                                                                 
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_mm_interconnect_0.v"                         -work mm_interconnect_0                                                           -cdslib ./cds_libs/mm_interconnect_0.cds.lib                                                          
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_jtag_debug_module_tck.v"                -work cpu1                                                                        -cdslib ./cds_libs/cpu1.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_jtag_debug_module_sysclk.v"             -work cpu1                                                                        -cdslib ./cds_libs/cpu1.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1.vo"                                     -work cpu1                                                                        -cdslib ./cds_libs/cpu1.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_oci_test_bench.v"                       -work cpu1                                                                        -cdslib ./cds_libs/cpu1.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_test_bench.v"                           -work cpu1                                                                        -cdslib ./cds_libs/cpu1.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_jtag_debug_module_wrapper.v"            -work cpu1                                                                        -cdslib ./cds_libs/cpu1.cds.lib                                                                       
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_irq_mapper.sv"                           -work irq_mapper                                                                  -cdslib ./cds_libs/irq_mapper.cds.lib                                                                 
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0.v"                     -work mm_interconnect_0                                                           -cdslib ./cds_libs/mm_interconnect_0.cds.lib                                                          
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_timestamp.v"                             -work timestamp                                                                   -cdslib ./cds_libs/timestamp.cds.lib                                                                  
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/cpu_irq.v"                                                            -work cpu_irq_0                                                                   -cdslib ./cds_libs/cpu_irq_0.cds.lib                                                                  
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/fprint_irq.v"                                                         -work fprint_irq_0                                                                -cdslib ./cds_libs/fprint_irq_0.cds.lib                                                               
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_button_pio.v"                            -work button_pio                                                                  -cdslib ./cds_libs/button_pio.cds.lib                                                                 
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_jtag_uart.v"                             -work jtag_uart                                                                   -cdslib ./cds_libs/jtag_uart.cds.lib                                                                  
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_timer.v"                                 -work timer                                                                       -cdslib ./cds_libs/timer.cds.lib                                                                      
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_mm_bridge.v"                                            -work out_system_bridge                                                           -cdslib ./cds_libs/out_system_bridge.cds.lib                                                          
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_jtag_debug_module_sysclk.v"         -work cpu0                                                                        -cdslib ./cds_libs/cpu0.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_test_bench.v"                       -work cpu0                                                                        -cdslib ./cds_libs/cpu0.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0.vo"                                 -work cpu0                                                                        -cdslib ./cds_libs/cpu0.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_jtag_debug_module_tck.v"            -work cpu0                                                                        -cdslib ./cds_libs/cpu0.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_jtag_debug_module_wrapper.v"        -work cpu0                                                                        -cdslib ./cds_libs/cpu0.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_oci_test_bench.v"                   -work cpu0                                                                        -cdslib ./cds_libs/cpu0.cds.lib                                                                       
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_reset_controller.v"                                            -work rst_controller                                                              -cdslib ./cds_libs/rst_controller.cds.lib                                                             
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_reset_synchronizer.v"                                          -work rst_controller                                                              -cdslib ./cds_libs/rst_controller.cds.lib                                                             
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1.v"                                      -work mm_interconnect_1                                                           -cdslib ./cds_libs/mm_interconnect_1.cds.lib                                                          
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0.v"                                      -work mm_interconnect_0                                                           -cdslib ./cds_libs/mm_interconnect_0.cds.lib                                                          
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_2.v"                                       -work onchip_memory2_2                                                            -cdslib ./cds_libs/onchip_memory2_2.cds.lib                                                           
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0.v"                                           -work processor2_0                                                                -cdslib ./cds_libs/processor2_0.cds.lib                                                               
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_3.v"                                       -work onchip_memory2_3                                                            -cdslib ./cds_libs/onchip_memory2_3.cds.lib                                                           
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_fingerprint.v"                                                    -work Fingerprint_2                                                               -cdslib ./cds_libs/Fingerprint_2.cds.lib                                                              
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_counter.v"                                                        -work Fingerprint_2                                                               -cdslib ./cds_libs/Fingerprint_2.cds.lib                                                              
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_fsm_rtla.v"                                                       -work Fingerprint_2                                                               -cdslib ./cds_libs/Fingerprint_2.cds.lib                                                              
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_rtla.v"                                                           -work Fingerprint_2                                                               -cdslib ./cds_libs/Fingerprint_2.cds.lib                                                              
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_sb_fsm.v"                                                         -work Fingerprint_2                                                               -cdslib ./cds_libs/Fingerprint_2.cds.lib                                                              
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_store_buffer.v"                                                   -work Fingerprint_2                                                               -cdslib ./cds_libs/Fingerprint_2.cds.lib                                                              
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/fifo.v"                                                               -work Fingerprint_2                                                               -cdslib ./cds_libs/Fingerprint_2.cds.lib                                                              
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_pause.v"                                                          -work Fingerprint_2                                                               -cdslib ./cds_libs/Fingerprint_2.cds.lib                                                              
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/lifo.v"                                                               -work Fingerprint_2                                                               -cdslib ./cds_libs/Fingerprint_2.cds.lib                                                              
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_buffer.v"                                                         -work cfpu_0                                                                      -cdslib ./cds_libs/cfpu_0.cds.lib                                                                     
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_cfpu.v"                                                           -work cfpu_0                                                                      -cdslib ./cds_libs/cfpu_0.cds.lib                                                                     
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_comparator.v"                                                     -work cfpu_0                                                                      -cdslib ./cds_libs/cfpu_0.cds.lib                                                                     
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_directory.v"                                                      -work cfpu_0                                                                      -cdslib ./cds_libs/cfpu_0.cds.lib                                                                     
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_state_regs.v"                                                     -work cfpu_0                                                                      -cdslib ./cds_libs/cfpu_0.cds.lib                                                                     
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mutex_0.v"                                                -work mutex_0                                                                     -cdslib ./cds_libs/mutex_0.cds.lib                                                                    
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_shared_memory.v"                                          -work shared_memory                                                               -cdslib ./cds_libs/shared_memory.cds.lib                                                              
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/led_pio.v"                                                            -work led_pio                                                                     -cdslib ./cds_libs/led_pio.cds.lib                                                                    
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_defines.v"                                                        -work led_pio                                                                     -cdslib ./cds_libs/led_pio.cds.lib                                                                    
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_1.v"                                       -work onchip_memory2_1                                                            -cdslib ./cds_libs/onchip_memory2_1.cds.lib                                                           
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_0.v"                                       -work onchip_memory2_0                                                            -cdslib ./cds_libs/onchip_memory2_0.cds.lib                                                           
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0.v"                                           -work processor1_0                                                                -cdslib ./cds_libs/processor1_0.cds.lib                                                               
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor.v"                                       -work processorMonitor                                                            -cdslib ./cds_libs/processorMonitor.cds.lib                                                           
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_sys_id.vo"                                                -work sys_id                                                                      -cdslib ./cds_libs/sys_id.cds.lib                                                                     
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_reset_source.sv"                                        -work nios_fprint_inst_reset_bfm                                                  -cdslib ./cds_libs/nios_fprint_inst_reset_bfm.cds.lib                                                 
  ncvlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_clock_source.sv"                                        -work nios_fprint_inst_clk_bfm                                                    -cdslib ./cds_libs/nios_fprint_inst_clk_bfm.cds.lib                                                   
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint.v"                                                        -work nios_fprint_inst                                                            -cdslib ./cds_libs/nios_fprint_inst.cds.lib                                                           
  ncvlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/nios_fprint_tb.v"                                                                                                                                                                                                                                                        
fi

# ----------------------------------------
# elaborate top level design
if [ $SKIP_ELAB -eq 0 ]; then
  ncelab -access +w+r+c -namemap_mixgen $ELAB_OPTIONS $USER_DEFINED_ELAB_OPTIONS $TOP_LEVEL_NAME
fi

# ----------------------------------------
# simulate
if [ $SKIP_SIM -eq 0 ]; then
  eval ncsim -licqueue $SIM_OPTIONS $USER_DEFINED_SIM_OPTIONS $TOP_LEVEL_NAME
fi
