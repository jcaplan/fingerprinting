onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -label {clk} /tb_oflow_registers/clk
add wave -noupdate -label {reset} /tb_oflow_registers/reset
add wave -noupdate -divider tb
add wave -noupdate -group {tb} /tb_oflow_registers/oflow_write
add wave -noupdate -group {tb} -radix hexadecimal /tb_oflow_registers/oflow_writedata
add wave -noupdate -group {tb} -radix hexadecimal /tb_oflow_registers/oflow_address
add wave -noupdate -group {tb} /tb_oflow_registers/oflow_waitrequest
add wave -noupdate -group {tb} -radix unsigned /tb_oflow_registers/oflow_task_id
add wave -noupdate -group {tb} -radix unsigned /tb_oflow_registers/oflow_logical_core_id
add wave -noupdate -group {tb} -radix unsigned /tb_oflow_registers/oflow_physical_core_id
add wave -noupdate -group {tb} -radix unsigned /tb_oflow_registers/csr_fprint_maxcount
add wave -noupdate -group {tb} /tb_oflow_registers/oflow_nmr
add wave -noupdate -group {tb} -radix unsigned /tb_oflow_registers/fprint_task_id
add wave -noupdate -group {tb} -radix unsigned /tb_oflow_registers/fprint_logical_core_id
add wave -noupdate -group {tb} /tb_oflow_registers/fprint_count_inc
add wave -noupdate -group {tb} /tb_oflow_registers/oflow_count_inc_ack
add wave -noupdate -group {tb} -radix unsigned /tb_oflow_registers/comparator_task_id
add wave -noupdate -group {tb} /tb_oflow_registers/comparator_count_dec
add wave -noupdate -group {tb} /tb_oflow_registers/oflow_count_dec_ack
add wave -noupdate -group {tb} /tb_oflow_registers/oflow_fprints_ready
add wave -noupdate -group {tb} /tb_oflow_registers/oflow_fprints_remaining
add wave -noupdate -group {tb} -radix unsigned /tb_oflow_registers/oflow_fprints_remaining_logical_core_id
add wave -noupdate -group {tb} /tb_oflow_registers/oflow_reset_task
add wave -noupdate -group {tb} /tb_oflow_registers/oflow_reset_task_ack
add wave -noupdate -group {tb} /tb_oflow_registers/init_complete
add wave -noupdate -group {tb} -radix hexadecimal /tb_oflow_registers/bus_task_id
add wave -noupdate -group {tb} -radix hexadecimal /tb_oflow_registers/bus_physical_core_id
add wave -noupdate -group {tb} -radix hexadecimal /tb_oflow_registers/bus_status
add wave -noupdate -group {tb} -radix unsigned /tb_oflow_registers/cat_mem_0
add wave -noupdate -group {tb} -radix unsigned /tb_oflow_registers/cat_mem_1
add wave -noupdate -group {tb} -radix unsigned /tb_oflow_registers/cat_mem_2
add wave -noupdate -group {tb} -radix unsigned -childformat {{{/tb_oflow_registers/fprint_maxcount_mem[15]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[14]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[13]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[12]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[11]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[10]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[9]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[8]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[7]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[6]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[5]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[4]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[3]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[2]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[1]} -radix unsigned} {{/tb_oflow_registers/fprint_maxcount_mem[0]} -radix unsigned}} -subitemconfig {{/tb_oflow_registers/fprint_maxcount_mem[15]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[14]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[13]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[12]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[11]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[10]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[9]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[8]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[7]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[6]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[5]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[4]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[3]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[2]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[1]} {-height 15 -radix unsigned} {/tb_oflow_registers/fprint_maxcount_mem[0]} {-height 15 -radix unsigned}} /tb_oflow_registers/fprint_maxcount_mem
add wave -noupdate -group {tb} /tb_oflow_registers/nmr_mem
add wave -noupdate -group {tb} /tb_oflow_registers/i
add wave -noupdate -group {tb} /tb_oflow_registers/j
add wave -noupdate -group {tb} /tb_oflow_registers/k
add wave -noupdate -divider oflow_registers
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/idle
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/st_count_inc
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/st_count_dec
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/st_fprint_regs_set
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/st_comparator_regs_set
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/st_fifo_oflow_write
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/st_fifo_uflow_write
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/st_oflow_status_set
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/st_oflow_status_reset
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/st_oflow_count_inc_ack
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/st_oflow_count_dec_ack
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/st_reset_task
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/st_oflow_reset_task_ack
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/st_fifo_rd_en
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/st_output_ready
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/clk
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/reset
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/oflow_write
add wave -noupdate -group {oflow_registers} -radix hexadecimal /tb_oflow_registers/oflow_registers/oflow_writedata
add wave -noupdate -group {oflow_registers} -radix hexadecimal /tb_oflow_registers/oflow_registers/oflow_address
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/oflow_waitrequest
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/oflow_task_id
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/oflow_logical_core_id
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/oflow_physical_core_id
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/csr_fprint_maxcount
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/oflow_nmr
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/fprint_task_id
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/fprint_logical_core_id
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/fprint_count_inc
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/oflow_count_inc_ack
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/comparator_task_id
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/comparator_count_dec
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/oflow_count_dec_ack
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/oflow_fprints_ready
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/oflow_fprints_remaining
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/oflow_fprints_remaining_logical_core_id
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/oflow_reset_task
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/oflow_reset_task_ack
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/fprint_oflow_status
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/comparator_oflow_status
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/fprint_count
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/overflow
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/underflow
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/overflow_status_reg_0
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/overflow_status_reg_1
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/overflow_status_reg_2
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/fprint_oflow_status_0
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/fprint_oflow_status_1
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/fprint_oflow_status_2
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/comparator_oflow_status_0
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/comparator_oflow_status_1
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/comparator_oflow_status_2
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/reset_task
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/count_inc
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/count_dec
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/count_logical_id
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/fprint_regs_set
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/comparator_regs_set
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/oflow_status_set
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/oflow_status_reset
add wave -noupdate -group {oflow_registers} -radix unsigned -childformat {{{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[15]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[14]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[13]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[12]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[11]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[10]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[9]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[8]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[7]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[6]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[5]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[4]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[3]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[2]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[1]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[0]} -radix unsigned}} -subitemconfig {{/tb_oflow_registers/oflow_registers/fprint_mem_count_0[15]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[14]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[13]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[12]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[11]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[10]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[9]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[8]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[7]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[6]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[5]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[4]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[3]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[2]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[1]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_0[0]} {-height 15 -radix unsigned}} /tb_oflow_registers/oflow_registers/fprint_mem_count_0
add wave -noupdate -group {oflow_registers} -radix unsigned -childformat {{{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[15]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[14]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[13]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[12]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[11]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[10]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[9]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[8]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[7]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[6]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[5]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[4]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[3]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[2]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[1]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[0]} -radix unsigned}} -subitemconfig {{/tb_oflow_registers/oflow_registers/fprint_mem_count_1[15]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[14]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[13]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[12]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[11]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[10]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[9]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[8]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[7]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[6]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[5]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[4]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[3]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[2]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[1]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_1[0]} {-height 15 -radix unsigned}} /tb_oflow_registers/oflow_registers/fprint_mem_count_1
add wave -noupdate -group {oflow_registers} -radix unsigned -childformat {{{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[15]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[14]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[13]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[12]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[11]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[10]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[9]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[8]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[7]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[6]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[5]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[4]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[3]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[2]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[1]} -radix unsigned} {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[0]} -radix unsigned}} -subitemconfig {{/tb_oflow_registers/oflow_registers/fprint_mem_count_2[15]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[14]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[13]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[12]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[11]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[10]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[9]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[8]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[7]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[6]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[5]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[4]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[3]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[2]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[1]} {-height 15 -radix unsigned} {/tb_oflow_registers/oflow_registers/fprint_mem_count_2[0]} {-height 15 -radix unsigned}} /tb_oflow_registers/oflow_registers/fprint_mem_count_2
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/fprint_count_0
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/fprint_count_1
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/fprint_count_2
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/comparator_count_0
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/comparator_count_1
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/comparator_count_2
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/fifo_uflow_write_done
add wave -noupdate -group {oflow_registers} -radix hexadecimal /tb_oflow_registers/oflow_registers/fifo_data_in
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/fifo_rd_en
add wave -noupdate -group {oflow_registers} -radix hexadecimal /tb_oflow_registers/oflow_registers/fifo_wr_en
add wave -noupdate -group {oflow_registers} -radix hexadecimal /tb_oflow_registers/oflow_registers/fifo_data_out
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/fifo_empty
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/fifo_full
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/state
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/k
add wave -noupdate -group {oflow_registers} /tb_oflow_registers/oflow_registers/l
add wave -noupdate -group {oflow_registers} -radix unsigned /tb_oflow_registers/oflow_registers/fifo_state
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {62960 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 417
configure wave -valuecolwidth 151
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
WaveRestoreZoom {62953 ps} {63227 ps}
