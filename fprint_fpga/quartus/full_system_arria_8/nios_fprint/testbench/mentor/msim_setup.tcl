
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
# Auto-generated simulation script

# ----------------------------------------
# Initialize variables
if ![info exists SYSTEM_INSTANCE_NAME] { 
  set SYSTEM_INSTANCE_NAME ""
} elseif { ![ string match "" $SYSTEM_INSTANCE_NAME ] } { 
  set SYSTEM_INSTANCE_NAME "/$SYSTEM_INSTANCE_NAME"
}

if ![info exists TOP_LEVEL_NAME] { 
  set TOP_LEVEL_NAME "nios_fprint_tb"
}

if ![info exists QSYS_SIMDIR] { 
  set QSYS_SIMDIR "./../"
}

if ![info exists QUARTUS_INSTALL_DIR] { 
  set QUARTUS_INSTALL_DIR "/CMC/tools/altera/13.1/quartus/"
}

# ----------------------------------------
# Initialize simulation properties - DO NOT MODIFY!
set ELAB_OPTIONS ""
set SIM_OPTIONS ""
if ![ string match "*-64 vsim*" [ vsim -version ] ] {
} else {
}

# ----------------------------------------
# Copy ROM/RAM files to simulation directory
alias file_copy {
  echo "\[exec\] file_copy"
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_rf_ram_a.dat ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_rf_ram_a.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_rf_ram_a.mif ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_ic_tag_ram.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_rf_ram_b.mif ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_rf_ram_b.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_rf_ram_b.dat ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_ociram_default_contents.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_ic_tag_ram.dat ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_ociram_default_contents.dat ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_ociram_default_contents.mif ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_ic_tag_ram.mif ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_ic_tag_ram.mif ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_rf_ram_a.mif ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_ic_tag_ram.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_rf_ram_b.dat ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_rf_ram_b.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_rf_ram_b.mif ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_ociram_default_contents.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_ociram_default_contents.mif ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_ic_tag_ram.dat ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_rf_ram_a.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_ociram_default_contents.dat ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_rf_ram_a.dat ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_ic_tag_ram.mif ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_ic_tag_ram.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_rf_ram_b.dat ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_rf_ram_b.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_rf_ram_a.dat ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_ociram_default_contents.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_rf_ram_a.mif ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_rf_ram_a.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_ociram_default_contents.mif ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_ociram_default_contents.dat ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_rf_ram_b.mif ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_ic_tag_ram.dat ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_2.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_3.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_shared_memory.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_1.hex ./
  file copy -force $QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_0.hex ./
}

# ----------------------------------------
# Create compilation libraries
proc ensure_lib { lib } { if ![file isdirectory $lib] { vlib $lib } }
ensure_lib          ./libraries/     
ensure_lib          ./libraries/work/
vmap       work     ./libraries/work/
vmap       work_lib ./libraries/work/
if ![ string match "*ModelSim ALTERA*" [ vsim -version ] ] {
  ensure_lib                     ./libraries/altera_ver/         
  vmap       altera_ver          ./libraries/altera_ver/         
  ensure_lib                     ./libraries/lpm_ver/            
  vmap       lpm_ver             ./libraries/lpm_ver/            
  ensure_lib                     ./libraries/sgate_ver/          
  vmap       sgate_ver           ./libraries/sgate_ver/          
  ensure_lib                     ./libraries/altera_mf_ver/      
  vmap       altera_mf_ver       ./libraries/altera_mf_ver/      
  ensure_lib                     ./libraries/altera_lnsim_ver/   
  vmap       altera_lnsim_ver    ./libraries/altera_lnsim_ver/   
  ensure_lib                     ./libraries/arriav_ver/         
  vmap       arriav_ver          ./libraries/arriav_ver/         
  ensure_lib                     ./libraries/arriav_hssi_ver/    
  vmap       arriav_hssi_ver     ./libraries/arriav_hssi_ver/    
  ensure_lib                     ./libraries/arriav_pcie_hip_ver/
  vmap       arriav_pcie_hip_ver ./libraries/arriav_pcie_hip_ver/
}
ensure_lib                                                                             ./libraries/altera_common_sv_packages/                                                  
vmap       altera_common_sv_packages                                                   ./libraries/altera_common_sv_packages/                                                  
ensure_lib                                                                             ./libraries/addr_router_001/                                                            
vmap       addr_router_001                                                             ./libraries/addr_router_001/                                                            
ensure_lib                                                                             ./libraries/addr_router/                                                                
vmap       addr_router                                                                 ./libraries/addr_router/                                                                
ensure_lib                                                                             ./libraries/rsp_xbar_mux_001/                                                           
vmap       rsp_xbar_mux_001                                                            ./libraries/rsp_xbar_mux_001/                                                           
ensure_lib                                                                             ./libraries/rsp_xbar_mux/                                                               
vmap       rsp_xbar_mux                                                                ./libraries/rsp_xbar_mux/                                                               
ensure_lib                                                                             ./libraries/rsp_xbar_demux_003/                                                         
vmap       rsp_xbar_demux_003                                                          ./libraries/rsp_xbar_demux_003/                                                         
ensure_lib                                                                             ./libraries/rsp_xbar_demux/                                                             
vmap       rsp_xbar_demux                                                              ./libraries/rsp_xbar_demux/                                                             
ensure_lib                                                                             ./libraries/cmd_xbar_mux_003/                                                           
vmap       cmd_xbar_mux_003                                                            ./libraries/cmd_xbar_mux_003/                                                           
ensure_lib                                                                             ./libraries/cmd_xbar_mux_001/                                                           
vmap       cmd_xbar_mux_001                                                            ./libraries/cmd_xbar_mux_001/                                                           
ensure_lib                                                                             ./libraries/cmd_xbar_mux/                                                               
vmap       cmd_xbar_mux                                                                ./libraries/cmd_xbar_mux/                                                               
ensure_lib                                                                             ./libraries/cmd_xbar_demux_001/                                                         
vmap       cmd_xbar_demux_001                                                          ./libraries/cmd_xbar_demux_001/                                                         
ensure_lib                                                                             ./libraries/cmd_xbar_demux/                                                             
vmap       cmd_xbar_demux                                                              ./libraries/cmd_xbar_demux/                                                             
ensure_lib                                                                             ./libraries/id_router_003/                                                              
vmap       id_router_003                                                               ./libraries/id_router_003/                                                              
ensure_lib                                                                             ./libraries/id_router/                                                                  
vmap       id_router                                                                   ./libraries/id_router/                                                                  
ensure_lib                                                                             ./libraries/rsp_xbar_mux_002/                                                           
vmap       rsp_xbar_mux_002                                                            ./libraries/rsp_xbar_mux_002/                                                           
ensure_lib                                                                             ./libraries/rsp_xbar_demux_001/                                                         
vmap       rsp_xbar_demux_001                                                          ./libraries/rsp_xbar_demux_001/                                                         
ensure_lib                                                                             ./libraries/cmd_xbar_demux_002/                                                         
vmap       cmd_xbar_demux_002                                                          ./libraries/cmd_xbar_demux_002/                                                         
ensure_lib                                                                             ./libraries/limiter/                                                                    
vmap       limiter                                                                     ./libraries/limiter/                                                                    
ensure_lib                                                                             ./libraries/id_router_012/                                                              
vmap       id_router_012                                                               ./libraries/id_router_012/                                                              
ensure_lib                                                                             ./libraries/id_router_010/                                                              
vmap       id_router_010                                                               ./libraries/id_router_010/                                                              
ensure_lib                                                                             ./libraries/id_router_001/                                                              
vmap       id_router_001                                                               ./libraries/id_router_001/                                                              
ensure_lib                                                                             ./libraries/addr_router_002/                                                            
vmap       addr_router_002                                                             ./libraries/addr_router_002/                                                            
ensure_lib                                                                             ./libraries/sys_id_control_slave_translator_avalon_universal_slave_0_agent_rsp_fifo/    
vmap       sys_id_control_slave_translator_avalon_universal_slave_0_agent_rsp_fifo     ./libraries/sys_id_control_slave_translator_avalon_universal_slave_0_agent_rsp_fifo/    
ensure_lib                                                                             ./libraries/sys_id_control_slave_translator_avalon_universal_slave_0_agent/             
vmap       sys_id_control_slave_translator_avalon_universal_slave_0_agent              ./libraries/sys_id_control_slave_translator_avalon_universal_slave_0_agent/             
ensure_lib                                                                             ./libraries/processorMonitor_outgoing_master_translator_avalon_universal_master_0_agent/
vmap       processorMonitor_outgoing_master_translator_avalon_universal_master_0_agent ./libraries/processorMonitor_outgoing_master_translator_avalon_universal_master_0_agent/
ensure_lib                                                                             ./libraries/sys_id_control_slave_translator/                                            
vmap       sys_id_control_slave_translator                                             ./libraries/sys_id_control_slave_translator/                                            
ensure_lib                                                                             ./libraries/processorMonitor_outgoing_master_translator/                                
vmap       processorMonitor_outgoing_master_translator                                 ./libraries/processorMonitor_outgoing_master_translator/                                
ensure_lib                                                                             ./libraries/cpu1/                                                                       
vmap       cpu1                                                                        ./libraries/cpu1/                                                                       
ensure_lib                                                                             ./libraries/irq_mapper/                                                                 
vmap       irq_mapper                                                                  ./libraries/irq_mapper/                                                                 
ensure_lib                                                                             ./libraries/mm_interconnect_0/                                                          
vmap       mm_interconnect_0                                                           ./libraries/mm_interconnect_0/                                                          
ensure_lib                                                                             ./libraries/timestamp/                                                                  
vmap       timestamp                                                                   ./libraries/timestamp/                                                                  
ensure_lib                                                                             ./libraries/cpu_irq_0/                                                                  
vmap       cpu_irq_0                                                                   ./libraries/cpu_irq_0/                                                                  
ensure_lib                                                                             ./libraries/fprint_irq_0/                                                               
vmap       fprint_irq_0                                                                ./libraries/fprint_irq_0/                                                               
ensure_lib                                                                             ./libraries/button_pio/                                                                 
vmap       button_pio                                                                  ./libraries/button_pio/                                                                 
ensure_lib                                                                             ./libraries/jtag_uart/                                                                  
vmap       jtag_uart                                                                   ./libraries/jtag_uart/                                                                  
ensure_lib                                                                             ./libraries/timer/                                                                      
vmap       timer                                                                       ./libraries/timer/                                                                      
ensure_lib                                                                             ./libraries/out_system_bridge/                                                          
vmap       out_system_bridge                                                           ./libraries/out_system_bridge/                                                          
ensure_lib                                                                             ./libraries/cpu0/                                                                       
vmap       cpu0                                                                        ./libraries/cpu0/                                                                       
ensure_lib                                                                             ./libraries/rst_controller/                                                             
vmap       rst_controller                                                              ./libraries/rst_controller/                                                             
ensure_lib                                                                             ./libraries/mm_interconnect_1/                                                          
vmap       mm_interconnect_1                                                           ./libraries/mm_interconnect_1/                                                          
ensure_lib                                                                             ./libraries/onchip_memory2_2/                                                           
vmap       onchip_memory2_2                                                            ./libraries/onchip_memory2_2/                                                           
ensure_lib                                                                             ./libraries/processor2_0/                                                               
vmap       processor2_0                                                                ./libraries/processor2_0/                                                               
ensure_lib                                                                             ./libraries/onchip_memory2_3/                                                           
vmap       onchip_memory2_3                                                            ./libraries/onchip_memory2_3/                                                           
ensure_lib                                                                             ./libraries/Fingerprint_2/                                                              
vmap       Fingerprint_2                                                               ./libraries/Fingerprint_2/                                                              
ensure_lib                                                                             ./libraries/cfpu_0/                                                                     
vmap       cfpu_0                                                                      ./libraries/cfpu_0/                                                                     
ensure_lib                                                                             ./libraries/mutex_0/                                                                    
vmap       mutex_0                                                                     ./libraries/mutex_0/                                                                    
ensure_lib                                                                             ./libraries/shared_memory/                                                              
vmap       shared_memory                                                               ./libraries/shared_memory/                                                              
ensure_lib                                                                             ./libraries/led_pio/                                                                    
vmap       led_pio                                                                     ./libraries/led_pio/                                                                    
ensure_lib                                                                             ./libraries/onchip_memory2_1/                                                           
vmap       onchip_memory2_1                                                            ./libraries/onchip_memory2_1/                                                           
ensure_lib                                                                             ./libraries/onchip_memory2_0/                                                           
vmap       onchip_memory2_0                                                            ./libraries/onchip_memory2_0/                                                           
ensure_lib                                                                             ./libraries/processor1_0/                                                               
vmap       processor1_0                                                                ./libraries/processor1_0/                                                               
ensure_lib                                                                             ./libraries/processorMonitor/                                                           
vmap       processorMonitor                                                            ./libraries/processorMonitor/                                                           
ensure_lib                                                                             ./libraries/sys_id/                                                                     
vmap       sys_id                                                                      ./libraries/sys_id/                                                                     
ensure_lib                                                                             ./libraries/nios_fprint_inst_reset_bfm/                                                 
vmap       nios_fprint_inst_reset_bfm                                                  ./libraries/nios_fprint_inst_reset_bfm/                                                 
ensure_lib                                                                             ./libraries/nios_fprint_inst_clk_bfm/                                                   
vmap       nios_fprint_inst_clk_bfm                                                    ./libraries/nios_fprint_inst_clk_bfm/                                                   
ensure_lib                                                                             ./libraries/nios_fprint_inst/                                                           
vmap       nios_fprint_inst                                                            ./libraries/nios_fprint_inst/                                                           

# ----------------------------------------
# Compile device library files
alias dev_com {
  echo "\[exec\] dev_com"
  if ![ string match "*ModelSim ALTERA*" [ vsim -version ] ] {
    vlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/altera_primitives.v"                   -work altera_ver         
    vlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/220model.v"                            -work lpm_ver            
    vlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/sgate.v"                               -work sgate_ver          
    vlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/altera_mf.v"                           -work altera_mf_ver      
    vlog -sv "$QUARTUS_INSTALL_DIR/eda/sim_lib/altera_lnsim.sv"                       -work altera_lnsim_ver   
    vlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/mentor/arriav_atoms_ncrypt.v"          -work arriav_ver         
    vlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/mentor/arriav_hmi_atoms_ncrypt.v"      -work arriav_ver         
    vlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/arriav_atoms.v"                        -work arriav_ver         
    vlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/mentor/arriav_hssi_atoms_ncrypt.v"     -work arriav_hssi_ver    
    vlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/arriav_hssi_atoms.v"                   -work arriav_hssi_ver    
    vlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/mentor/arriav_pcie_hip_atoms_ncrypt.v" -work arriav_pcie_hip_ver
    vlog     "$QUARTUS_INSTALL_DIR/eda/sim_lib/arriav_pcie_hip_atoms.v"               -work arriav_pcie_hip_ver
  }
}

# ----------------------------------------
# Compile the design files in correct order
alias com {
  echo "\[exec\] com"
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/verbosity_pkg.sv"                                                                                  -work altera_common_sv_packages                                                  
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_mm_interconnect_0_addr_router_001.sv"        -L altera_common_sv_packages -work addr_router_001                                                            
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_mm_interconnect_0_addr_router.sv"            -L altera_common_sv_packages -work addr_router                                                                
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -L altera_common_sv_packages -work rsp_xbar_mux_001                                                           
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_rsp_xbar_mux_001.sv"   -L altera_common_sv_packages -work rsp_xbar_mux_001                                                           
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -L altera_common_sv_packages -work rsp_xbar_mux                                                               
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_rsp_xbar_mux.sv"       -L altera_common_sv_packages -work rsp_xbar_mux                                                               
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_rsp_xbar_demux_003.sv" -L altera_common_sv_packages -work rsp_xbar_demux_003                                                         
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_rsp_xbar_demux.sv"     -L altera_common_sv_packages -work rsp_xbar_demux                                                             
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -L altera_common_sv_packages -work cmd_xbar_mux_003                                                           
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_mux_003.sv"   -L altera_common_sv_packages -work cmd_xbar_mux_003                                                           
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -L altera_common_sv_packages -work cmd_xbar_mux_001                                                           
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_mux_001.sv"   -L altera_common_sv_packages -work cmd_xbar_mux_001                                                           
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -L altera_common_sv_packages -work cmd_xbar_mux                                                               
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_mux.sv"       -L altera_common_sv_packages -work cmd_xbar_mux                                                               
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_demux_001.sv" -L altera_common_sv_packages -work cmd_xbar_demux_001                                                         
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_cmd_xbar_demux.sv"     -L altera_common_sv_packages -work cmd_xbar_demux                                                             
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_id_router_003.sv"      -L altera_common_sv_packages -work id_router_003                                                              
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_id_router.sv"          -L altera_common_sv_packages -work id_router                                                                  
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_addr_router_001.sv"    -L altera_common_sv_packages -work addr_router_001                                                            
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0_addr_router.sv"        -L altera_common_sv_packages -work addr_router                                                                
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -L altera_common_sv_packages -work rsp_xbar_mux                                                               
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_rsp_xbar_mux.sv"                        -L altera_common_sv_packages -work rsp_xbar_mux                                                               
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_rsp_xbar_demux.sv"                      -L altera_common_sv_packages -work rsp_xbar_demux                                                             
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -L altera_common_sv_packages -work cmd_xbar_mux                                                               
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_cmd_xbar_mux.sv"                        -L altera_common_sv_packages -work cmd_xbar_mux                                                               
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_cmd_xbar_demux.sv"                      -L altera_common_sv_packages -work cmd_xbar_demux                                                             
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_id_router.sv"                           -L altera_common_sv_packages -work id_router                                                                  
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1_addr_router.sv"                         -L altera_common_sv_packages -work addr_router                                                                
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -L altera_common_sv_packages -work rsp_xbar_mux_002                                                           
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_mux_002.sv"                    -L altera_common_sv_packages -work rsp_xbar_mux_002                                                           
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -L altera_common_sv_packages -work rsp_xbar_mux_001                                                           
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_mux_001.sv"                    -L altera_common_sv_packages -work rsp_xbar_mux_001                                                           
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -L altera_common_sv_packages -work rsp_xbar_mux                                                               
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_mux.sv"                        -L altera_common_sv_packages -work rsp_xbar_mux                                                               
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_demux_003.sv"                  -L altera_common_sv_packages -work rsp_xbar_demux_003                                                         
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_demux_001.sv"                  -L altera_common_sv_packages -work rsp_xbar_demux_001                                                         
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_rsp_xbar_demux.sv"                      -L altera_common_sv_packages -work rsp_xbar_demux                                                             
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -L altera_common_sv_packages -work cmd_xbar_mux_003                                                           
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_mux_003.sv"                    -L altera_common_sv_packages -work cmd_xbar_mux_003                                                           
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -L altera_common_sv_packages -work cmd_xbar_mux_001                                                           
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_mux_001.sv"                    -L altera_common_sv_packages -work cmd_xbar_mux_001                                                           
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_arbitrator.sv"                                          -L altera_common_sv_packages -work cmd_xbar_mux                                                               
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_mux.sv"                        -L altera_common_sv_packages -work cmd_xbar_mux                                                               
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_demux_002.sv"                  -L altera_common_sv_packages -work cmd_xbar_demux_002                                                         
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_demux_001.sv"                  -L altera_common_sv_packages -work cmd_xbar_demux_001                                                         
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_cmd_xbar_demux.sv"                      -L altera_common_sv_packages -work cmd_xbar_demux                                                             
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_traffic_limiter.sv"                                     -L altera_common_sv_packages -work limiter                                                                    
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_reorder_memory.sv"                                      -L altera_common_sv_packages -work limiter                                                                    
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_sc_fifo.v"                                              -L altera_common_sv_packages -work limiter                                                                    
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_st_pipeline_base.v"                                     -L altera_common_sv_packages -work limiter                                                                    
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router_012.sv"                       -L altera_common_sv_packages -work id_router_012                                                              
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router_010.sv"                       -L altera_common_sv_packages -work id_router_010                                                              
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router_003.sv"                       -L altera_common_sv_packages -work id_router_003                                                              
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router_001.sv"                       -L altera_common_sv_packages -work id_router_001                                                              
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_id_router.sv"                           -L altera_common_sv_packages -work id_router                                                                  
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_addr_router_002.sv"                     -L altera_common_sv_packages -work addr_router_002                                                            
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_addr_router_001.sv"                     -L altera_common_sv_packages -work addr_router_001                                                            
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0_addr_router.sv"                         -L altera_common_sv_packages -work addr_router                                                                
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_sc_fifo.v"                                                                           -work sys_id_control_slave_translator_avalon_universal_slave_0_agent_rsp_fifo    
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_slave_agent.sv"                                         -L altera_common_sv_packages -work sys_id_control_slave_translator_avalon_universal_slave_0_agent             
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_burst_uncompressor.sv"                                  -L altera_common_sv_packages -work sys_id_control_slave_translator_avalon_universal_slave_0_agent             
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_master_agent.sv"                                        -L altera_common_sv_packages -work processorMonitor_outgoing_master_translator_avalon_universal_master_0_agent
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_slave_translator.sv"                                    -L altera_common_sv_packages -work sys_id_control_slave_translator                                            
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_merlin_master_translator.sv"                                   -L altera_common_sv_packages -work processorMonitor_outgoing_master_translator                                
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_jtag_debug_module_wrapper.v"                                         -work cpu1                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_oci_test_bench.v"                                                    -work cpu1                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_test_bench.v"                                                        -work cpu1                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_jtag_debug_module_sysclk.v"                                          -work cpu1                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1.vo"                                                                  -work cpu1                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0_cpu1_jtag_debug_module_tck.v"                                             -work cpu1                                                                       
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_irq_mapper.sv"                               -L altera_common_sv_packages -work irq_mapper                                                                 
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_mm_interconnect_0.v"                                                      -work mm_interconnect_0                                                          
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_jtag_debug_module_tck.v"                                             -work cpu1                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_jtag_debug_module_sysclk.v"                                          -work cpu1                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1.vo"                                                                  -work cpu1                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_oci_test_bench.v"                                                    -work cpu1                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_test_bench.v"                                                        -work cpu1                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0_cpu1_jtag_debug_module_wrapper.v"                                         -work cpu1                                                                       
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_irq_mapper.sv"                           -L altera_common_sv_packages -work irq_mapper                                                                 
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_mm_interconnect_0.v"                                                  -work mm_interconnect_0                                                          
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_timestamp.v"                                                          -work timestamp                                                                  
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/cpu_irq.v"                                                                                         -work cpu_irq_0                                                                  
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/fprint_irq.v"                                                                                      -work fprint_irq_0                                                               
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_button_pio.v"                                                         -work button_pio                                                                 
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_jtag_uart.v"                                                          -work jtag_uart                                                                  
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_timer.v"                                                              -work timer                                                                      
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_mm_bridge.v"                                                                         -work out_system_bridge                                                          
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_jtag_debug_module_sysclk.v"                                      -work cpu0                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_test_bench.v"                                                    -work cpu0                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0.vo"                                                              -work cpu0                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_jtag_debug_module_tck.v"                                         -work cpu0                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_jtag_debug_module_wrapper.v"                                     -work cpu0                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor_cpu0_oci_test_bench.v"                                                -work cpu0                                                                       
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_reset_controller.v"                                                                         -work rst_controller                                                             
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_reset_synchronizer.v"                                                                       -work rst_controller                                                             
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_1.v"                                                                   -work mm_interconnect_1                                                          
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mm_interconnect_0.v"                                                                   -work mm_interconnect_0                                                          
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_2.v"                                                                    -work onchip_memory2_2                                                           
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor2_0.v"                                                                        -work processor2_0                                                               
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_3.v"                                                                    -work onchip_memory2_3                                                           
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_fingerprint.v"                                                                                 -work Fingerprint_2                                                              
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_counter.v"                                                                                     -work Fingerprint_2                                                              
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_fsm_rtla.v"                                                                                    -work Fingerprint_2                                                              
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_rtla.v"                                                                                        -work Fingerprint_2                                                              
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_sb_fsm.v"                                                                                      -work Fingerprint_2                                                              
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_store_buffer.v"                                                                                -work Fingerprint_2                                                              
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/fifo.v"                                                                                            -work Fingerprint_2                                                              
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_pause.v"                                                                                       -work Fingerprint_2                                                              
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/lifo.v"                                                                                            -work Fingerprint_2                                                              
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_buffer.v"                                                                                      -work cfpu_0                                                                     
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_cfpu.v"                                                                                        -work cfpu_0                                                                     
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_comparator.v"                                                                                  -work cfpu_0                                                                     
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_directory.v"                                                                                   -work cfpu_0                                                                     
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_state_regs.v"                                                                                  -work cfpu_0                                                                     
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_mutex_0.v"                                                                             -work mutex_0                                                                    
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_shared_memory.v"                                                                       -work shared_memory                                                              
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/led_pio.v"                                                                                         -work led_pio                                                                    
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/crc_defines.v"                                                                                     -work led_pio                                                                    
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_1.v"                                                                    -work onchip_memory2_1                                                           
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_onchip_memory2_0.v"                                                                    -work onchip_memory2_0                                                           
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processor1_0.v"                                                                        -work processor1_0                                                               
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_processorMonitor.v"                                                                    -work processorMonitor                                                           
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint_sys_id.vo"                                                                             -work sys_id                                                                     
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_reset_source.sv"                                        -L altera_common_sv_packages -work nios_fprint_inst_reset_bfm                                                 
  vlog -sv "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/altera_avalon_clock_source.sv"                                        -L altera_common_sv_packages -work nios_fprint_inst_clk_bfm                                                   
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/submodules/nios_fprint.v"                                                                                     -work nios_fprint_inst                                                           
  vlog     "$QSYS_SIMDIR/nios_fprint_tb/simulation/nios_fprint_tb.v"                                                                                                                                                                              
}

# ----------------------------------------
# Elaborate top level design
alias elab {
  echo "\[exec\] elab"
  eval vsim -t ps $ELAB_OPTIONS -L work -L work_lib -L altera_common_sv_packages -L addr_router_001 -L addr_router -L rsp_xbar_mux_001 -L rsp_xbar_mux -L rsp_xbar_demux_003 -L rsp_xbar_demux -L cmd_xbar_mux_003 -L cmd_xbar_mux_001 -L cmd_xbar_mux -L cmd_xbar_demux_001 -L cmd_xbar_demux -L id_router_003 -L id_router -L rsp_xbar_mux_002 -L rsp_xbar_demux_001 -L cmd_xbar_demux_002 -L limiter -L id_router_012 -L id_router_010 -L id_router_001 -L addr_router_002 -L sys_id_control_slave_translator_avalon_universal_slave_0_agent_rsp_fifo -L sys_id_control_slave_translator_avalon_universal_slave_0_agent -L processorMonitor_outgoing_master_translator_avalon_universal_master_0_agent -L sys_id_control_slave_translator -L processorMonitor_outgoing_master_translator -L cpu1 -L irq_mapper -L mm_interconnect_0 -L timestamp -L cpu_irq_0 -L fprint_irq_0 -L button_pio -L jtag_uart -L timer -L out_system_bridge -L cpu0 -L rst_controller -L mm_interconnect_1 -L onchip_memory2_2 -L processor2_0 -L onchip_memory2_3 -L Fingerprint_2 -L cfpu_0 -L mutex_0 -L shared_memory -L led_pio -L onchip_memory2_1 -L onchip_memory2_0 -L processor1_0 -L processorMonitor -L sys_id -L nios_fprint_inst_reset_bfm -L nios_fprint_inst_clk_bfm -L nios_fprint_inst -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L arriav_ver -L arriav_hssi_ver -L arriav_pcie_hip_ver $TOP_LEVEL_NAME
}

# ----------------------------------------
# Elaborate the top level design with novopt option
alias elab_debug {
  echo "\[exec\] elab_debug"
  eval vsim -novopt -t ps $ELAB_OPTIONS -L work -L work_lib -L altera_common_sv_packages -L addr_router_001 -L addr_router -L rsp_xbar_mux_001 -L rsp_xbar_mux -L rsp_xbar_demux_003 -L rsp_xbar_demux -L cmd_xbar_mux_003 -L cmd_xbar_mux_001 -L cmd_xbar_mux -L cmd_xbar_demux_001 -L cmd_xbar_demux -L id_router_003 -L id_router -L rsp_xbar_mux_002 -L rsp_xbar_demux_001 -L cmd_xbar_demux_002 -L limiter -L id_router_012 -L id_router_010 -L id_router_001 -L addr_router_002 -L sys_id_control_slave_translator_avalon_universal_slave_0_agent_rsp_fifo -L sys_id_control_slave_translator_avalon_universal_slave_0_agent -L processorMonitor_outgoing_master_translator_avalon_universal_master_0_agent -L sys_id_control_slave_translator -L processorMonitor_outgoing_master_translator -L cpu1 -L irq_mapper -L mm_interconnect_0 -L timestamp -L cpu_irq_0 -L fprint_irq_0 -L button_pio -L jtag_uart -L timer -L out_system_bridge -L cpu0 -L rst_controller -L mm_interconnect_1 -L onchip_memory2_2 -L processor2_0 -L onchip_memory2_3 -L Fingerprint_2 -L cfpu_0 -L mutex_0 -L shared_memory -L led_pio -L onchip_memory2_1 -L onchip_memory2_0 -L processor1_0 -L processorMonitor -L sys_id -L nios_fprint_inst_reset_bfm -L nios_fprint_inst_clk_bfm -L nios_fprint_inst -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L arriav_ver -L arriav_hssi_ver -L arriav_pcie_hip_ver $TOP_LEVEL_NAME
}

# ----------------------------------------
# Compile all the design files and elaborate the top level design
alias ld "
  dev_com
  com
  elab
"

# ----------------------------------------
# Compile all the design files and elaborate the top level design with -novopt
alias ld_debug "
  dev_com
  com
  elab_debug
"

# ----------------------------------------
# Print out user commmand line aliases
alias h {
  echo "List Of Command Line Aliases"
  echo
  echo "file_copy                     -- Copy ROM/RAM files to simulation directory"
  echo
  echo "dev_com                       -- Compile device library files"
  echo
  echo "com                           -- Compile the design files in correct order"
  echo
  echo "elab                          -- Elaborate top level design"
  echo
  echo "elab_debug                    -- Elaborate the top level design with novopt option"
  echo
  echo "ld                            -- Compile all the design files and elaborate the top level design"
  echo
  echo "ld_debug                      -- Compile all the design files and elaborate the top level design with -novopt"
  echo
  echo 
  echo
  echo "List Of Variables"
  echo
  echo "TOP_LEVEL_NAME                -- Top level module name."
  echo
  echo "SYSTEM_INSTANCE_NAME          -- Instantiated system module name inside top level module."
  echo
  echo "QSYS_SIMDIR                   -- Qsys base simulation directory."
  echo
  echo "QUARTUS_INSTALL_DIR           -- Quartus installation directory."
}
file_copy
h
