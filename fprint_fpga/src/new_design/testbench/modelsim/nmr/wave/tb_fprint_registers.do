onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -label {clk} /tb_fprint_registers/clk
add wave -noupdate -label {reset} /tb_fprint_registers/reset
add wave -noupdate -divider tb
add wave -noupdate -expand -group {tb} -radix hexadecimal /tb_fprint_registers/fprint_address
add wave -noupdate -expand -group {tb} /tb_fprint_registers/fprint_write
add wave -noupdate -expand -group {tb} -radix hexadecimal /tb_fprint_registers/fprint_writedata
add wave -noupdate -expand -group {tb} /tb_fprint_registers/fprint_waitrequest
add wave -noupdate -expand -group {tb} -radix unsigned /tb_fprint_registers/fprint_task_id
add wave -noupdate -expand -group {tb} -radix unsigned /tb_fprint_registers/fprint_physical_core_id
add wave -noupdate -expand -group {tb} -radix unsigned /tb_fprint_registers/fprint_logical_core_id
add wave -noupdate -expand -group {tb} /tb_fprint_registers/fprint_nmr
add wave -noupdate -expand -group {tb} /tb_fprint_registers/fprint_count_inc
add wave -noupdate -expand -group {tb} /tb_fprint_registers/oflow_count_inc_ack
add wave -noupdate -expand -group {tb} /tb_fprint_registers/fprint_inc_head_pointer
add wave -noupdate -expand -group {tb} /tb_fprint_registers/comp_inc_head_pointer_ack
add wave -noupdate -expand -group {tb} -radix unsigned /tb_fprint_registers/fprint_head_pointer
add wave -noupdate -expand -group {tb} -radix unsigned /tb_fprint_registers/comp_tail_pointer_0
add wave -noupdate -expand -group {tb} -radix unsigned /tb_fprint_registers/comp_tail_pointer_1
add wave -noupdate -expand -group {tb} -radix unsigned /tb_fprint_registers/comp_tail_pointer_2
add wave -noupdate -expand -group {tb} -radix hexadecimal /tb_fprint_registers/fprint_0
add wave -noupdate -expand -group {tb} -radix hexadecimal /tb_fprint_registers/fprint_1
add wave -noupdate -expand -group {tb} -radix hexadecimal /tb_fprint_registers/fprint_2
add wave -noupdate -expand -group {tb} -radix unsigned /tb_fprint_registers/comparator_task_id
add wave -noupdate -expand -group {tb} /tb_fprint_registers/fprint_reset_task
add wave -noupdate -expand -group {tb} /tb_fprint_registers/fprint_reset_task_ack
add wave -noupdate -expand -group {tb} /tb_fprint_registers/fprint_checkin
add wave -noupdate -expand -group {tb} /tb_fprint_registers/init_complete
add wave -noupdate -expand -group {tb} -radix unsigned /tb_fprint_registers/cat_mem_0
add wave -noupdate -expand -group {tb} -radix unsigned /tb_fprint_registers/cat_mem_1
add wave -noupdate -expand -group {tb} -radix unsigned /tb_fprint_registers/cat_mem_2
add wave -noupdate -expand -group {tb} /tb_fprint_registers/nmr_mem
add wave -noupdate -expand -group {tb} /tb_fprint_registers/i
add wave -noupdate -expand -group {tb} /tb_fprint_registers/j
add wave -noupdate -expand -group {tb} /tb_fprint_registers/k
add wave -noupdate -divider fprint_registers
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/idle
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/st_fifo_wr_en
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/st_waitrequest
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/st_comparator_regs_write
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/st_fprint_reset_task_ack
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/st_fifo_rd_en
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/st_decode
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/st_fprint_regs_write
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/st_fprint_mem_write
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/st_fprint_inc_head_pointer
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/st_fprint_count_inc
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/clk
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/reset
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_address
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fprint_write
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_writedata
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fprint_waitrequest
add wave -noupdate -expand -group {fprint_registers} -radix unsigned /tb_fprint_registers/fprint_registers/fprint_task_id
add wave -noupdate -expand -group {fprint_registers} -radix unsigned /tb_fprint_registers/fprint_registers/fprint_physical_core_id
add wave -noupdate -expand -group {fprint_registers} -radix unsigned /tb_fprint_registers/fprint_registers/fprint_logical_core_id
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fprint_nmr
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fprint_count_inc
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/oflow_count_inc_ack
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fprint_inc_head_pointer
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/comp_inc_head_pointer_ack
add wave -noupdate -expand -group {fprint_registers} -radix unsigned /tb_fprint_registers/fprint_registers/fprint_head_pointer
add wave -noupdate -expand -group {fprint_registers} -radix unsigned /tb_fprint_registers/fprint_registers/comp_tail_pointer_0
add wave -noupdate -expand -group {fprint_registers} -radix unsigned /tb_fprint_registers/fprint_registers/comp_tail_pointer_1
add wave -noupdate -expand -group {fprint_registers} -radix unsigned /tb_fprint_registers/fprint_registers/comp_tail_pointer_2
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_0
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_1
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_2
add wave -noupdate -expand -group {fprint_registers} -radix unsigned /tb_fprint_registers/fprint_registers/comparator_task_id
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fprint_reset_task
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fprint_reset_task_ack
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fprint_checkin
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/checkout_reg
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/checkin_reg
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_mem_0_0
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_mem_0_1
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_mem_1_0
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_mem_1_1
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_mem_2_0
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_mem_2_1
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_0_0
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_0_1
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_1_0
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_1_1
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_2_0
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_2_1
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fprint_mem_data_in
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fprint_regs_write
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fprint_mem_write
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fprint_block
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/comparator_regs_write
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fifo_rd_en
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fifo_wr_en
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fifo_full
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fifo_empty
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fifo_data_in
add wave -noupdate -expand -group {fprint_registers} -radix hexadecimal /tb_fprint_registers/fprint_registers/fifo_data_out
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/i
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/j
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/checkout_sel
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/checkin_sel
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/fprint_sel
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/checkin_reg_out
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/FIFO_state
add wave -noupdate -expand -group {fprint_registers} /tb_fprint_registers/fprint_registers/state
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {3547 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 380
configure wave -valuecolwidth 316
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
WaveRestoreZoom {0 ps} {4615 ps}
