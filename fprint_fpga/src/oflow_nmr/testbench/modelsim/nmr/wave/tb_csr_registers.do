onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -label clk /tb_csr_registers/clk
add wave -noupdate -label reset /tb_csr_registers/reset
add wave -noupdate -divider tb
add wave -noupdate -group tb /tb_csr_registers/csr_address
add wave -noupdate -group tb /tb_csr_registers/csr_read
add wave -noupdate -group tb /tb_csr_registers/csr_readdata
add wave -noupdate -group tb /tb_csr_registers/csr_write
add wave -noupdate -group tb /tb_csr_registers/csr_writedata
add wave -noupdate -group tb /tb_csr_registers/csr_waitrequest
add wave -noupdate -group tb /tb_csr_registers/comparator_task_id
add wave -noupdate -group tb /tb_csr_registers/comparator_logical_core_id
add wave -noupdate -group tb /tb_csr_registers/comparator_status_write
add wave -noupdate -group tb /tb_csr_registers/comparator_mismatch_detected
add wave -noupdate -group tb /tb_csr_registers/csr_status_ack
add wave -noupdate -group tb /tb_csr_registers/comparator_nmr
add wave -noupdate -group tb -radix unsigned /tb_csr_registers/fprint_task_id
add wave -noupdate -group tb -radix unsigned /tb_csr_registers/fprint_physical_core_id
add wave -noupdate -group tb -radix unsigned /tb_csr_registers/fprint_logical_core_id
add wave -noupdate -group tb -radix unsigned /tb_csr_registers/oflow_task_id
add wave -noupdate -group tb -radix unsigned /tb_csr_registers/oflow_logical_core_id
add wave -noupdate -group tb -radix unsigned /tb_csr_registers/oflow_physical_core_id
add wave -noupdate -group tb -radix unsigned /tb_csr_registers/csr_fprint_maxcount
add wave -noupdate -group tb /tb_csr_registers/oflow_nmr
add wave -noupdate -group tb /tb_csr_registers/csr_pointer_start_write
add wave -noupdate -group tb /tb_csr_registers/csr_pointer_end_write
add wave -noupdate -group tb -radix unsigned /tb_csr_registers/csr_pointer_data
add wave -noupdate -group tb /tb_csr_registers/comp_pointer_ack
add wave -noupdate -group tb /tb_csr_registers/irq
add wave -noupdate -group tb /tb_csr_registers/status_mismatch_detected
add wave -noupdate -group tb /tb_csr_registers/status_mismatch_task_id
add wave -noupdate -group tb /tb_csr_registers/status_mismatch_logical_core_id
add wave -noupdate -group tb /tb_csr_registers/init_complete
add wave -noupdate -group tb /tb_csr_registers/i
add wave -noupdate -group tb /tb_csr_registers/j
add wave -noupdate -group tb /tb_csr_registers/pointer_data
add wave -noupdate -divider csr_registers
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/idle
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/st_csr_regs_write
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/st_csr_cat_write
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/st_csr_maxcount_write
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/st_csr_pointer_start_write
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/st_csr_pointer_end_write
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/st_csr_nmr_write
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/st_csr_read
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/st_waitrequest
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/st_comparator_regs_write
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/st_comparator_status_ack
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_address
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_read
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_readdata
add wave -noupdate -group csr_registers -radix unsigned /tb_csr_registers/csr_registers/state
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_write
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_writedata
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_waitrequest
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/comparator_task_id
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/comparator_logical_core_id
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/comparator_status_write
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/comparator_mismatch_detected
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_status_ack
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/comparator_nmr
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/fprint_task_id
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/fprint_physical_core_id
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/fprint_logical_core_id
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/oflow_task_id
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/oflow_logical_core_id
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/oflow_physical_core_id
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_fprint_maxcount
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/oflow_nmr
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_pointer_start_write
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_pointer_end_write
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_pointer_data
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/comp_pointer_ack
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/irq
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/exception_reg
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/success_reg
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/fail_reg
add wave -noupdate -group csr_registers -radix unsigned -childformat {{{/tb_csr_registers/csr_registers/cat_mem_0[15]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[14]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[13]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[12]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[11]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[10]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[9]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[8]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[7]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[6]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[5]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[4]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[3]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[2]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[1]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_0[0]} -radix unsigned}} -subitemconfig {{/tb_csr_registers/csr_registers/cat_mem_0[15]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[14]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[13]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[12]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[11]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[10]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[9]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[8]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[7]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[6]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[5]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[4]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[3]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[2]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[1]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_0[0]} {-height 15 -radix unsigned}} /tb_csr_registers/csr_registers/cat_mem_0
add wave -noupdate -group csr_registers -radix unsigned -childformat {{{/tb_csr_registers/csr_registers/cat_mem_1[15]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[14]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[13]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[12]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[11]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[10]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[9]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[8]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[7]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[6]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[5]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[4]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[3]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[2]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[1]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_1[0]} -radix unsigned}} -subitemconfig {{/tb_csr_registers/csr_registers/cat_mem_1[15]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[14]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[13]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[12]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[11]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[10]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[9]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[8]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[7]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[6]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[5]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[4]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[3]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[2]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[1]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_1[0]} {-height 15 -radix unsigned}} /tb_csr_registers/csr_registers/cat_mem_1
add wave -noupdate -group csr_registers -radix unsigned -childformat {{{/tb_csr_registers/csr_registers/cat_mem_2[15]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[14]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[13]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[12]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[11]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[10]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[9]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[8]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[7]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[6]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[5]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[4]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[3]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[2]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[1]} -radix unsigned} {{/tb_csr_registers/csr_registers/cat_mem_2[0]} -radix unsigned}} -subitemconfig {{/tb_csr_registers/csr_registers/cat_mem_2[15]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[14]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[13]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[12]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[11]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[10]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[9]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[8]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[7]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[6]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[5]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[4]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[3]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[2]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[1]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/cat_mem_2[0]} {-height 15 -radix unsigned}} /tb_csr_registers/csr_registers/cat_mem_2
add wave -noupdate -group csr_registers -radix unsigned -childformat {{{/tb_csr_registers/csr_registers/fprint_maxcount_mem[15]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[14]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[13]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[12]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[11]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[10]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[9]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[8]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[7]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[6]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[5]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[4]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[3]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[2]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[1]} -radix unsigned} {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[0]} -radix unsigned}} -subitemconfig {{/tb_csr_registers/csr_registers/fprint_maxcount_mem[15]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[14]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[13]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[12]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[11]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[10]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[9]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[8]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[7]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[6]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[5]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[4]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[3]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[2]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[1]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/fprint_maxcount_mem[0]} {-height 15 -radix unsigned}} /tb_csr_registers/csr_registers/fprint_maxcount_mem
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/nmr_mem
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_cat_write
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_maxcount_write
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_nmr_write
add wave -noupdate -group csr_registers -radix unsigned -childformat {{{/tb_csr_registers/csr_registers/csr_maxcount_data[9]} -radix unsigned} {{/tb_csr_registers/csr_registers/csr_maxcount_data[8]} -radix unsigned} {{/tb_csr_registers/csr_registers/csr_maxcount_data[7]} -radix unsigned} {{/tb_csr_registers/csr_registers/csr_maxcount_data[6]} -radix unsigned} {{/tb_csr_registers/csr_registers/csr_maxcount_data[5]} -radix unsigned} {{/tb_csr_registers/csr_registers/csr_maxcount_data[4]} -radix unsigned} {{/tb_csr_registers/csr_registers/csr_maxcount_data[3]} -radix unsigned} {{/tb_csr_registers/csr_registers/csr_maxcount_data[2]} -radix unsigned} {{/tb_csr_registers/csr_registers/csr_maxcount_data[1]} -radix unsigned} {{/tb_csr_registers/csr_registers/csr_maxcount_data[0]} -radix unsigned}} -subitemconfig {{/tb_csr_registers/csr_registers/csr_maxcount_data[9]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/csr_maxcount_data[8]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/csr_maxcount_data[7]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/csr_maxcount_data[6]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/csr_maxcount_data[5]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/csr_maxcount_data[4]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/csr_maxcount_data[3]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/csr_maxcount_data[2]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/csr_maxcount_data[1]} {-height 15 -radix unsigned} {/tb_csr_registers/csr_registers/csr_maxcount_data[0]} {-height 15 -radix unsigned}} /tb_csr_registers/csr_registers/csr_maxcount_data
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_nmr_data
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_task_id
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_logical_core_id
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_physical_core_id
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/comparator_regs_write
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/i
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/j
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/exception_reg_sel
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/success_reg_sel
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/fail_reg_sel
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/cat_sel
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/maxcount_reg_sel
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/pointer_start_sel
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/pointer_end_sel
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/nmr_sel
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/fail_reg_out
add wave -noupdate -group csr_registers /tb_csr_registers/csr_registers/csr_regs_write
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {8505 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 326
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ps
update
WaveRestoreZoom {8483 ps} {8739 ps}
