onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -label clk /tb_comparator_maxcount/clk
add wave -noupdate -label reset /tb_comparator_maxcount/reset
add wave -noupdate -divider tb
add wave -noupdate -group tb /tb_comparator_maxcount/idle
add wave -noupdate -group tb /tb_comparator_maxcount/waitforsignal
add wave -noupdate -group tb /tb_comparator_maxcount/st_isr
add wave -noupdate -group tb /tb_comparator_maxcount/clk
add wave -noupdate -group tb /tb_comparator_maxcount/reset
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/fprint_address
add wave -noupdate -group tb /tb_comparator_maxcount/fprint_write
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/fprint_writedata
add wave -noupdate -group tb /tb_comparator_maxcount/fprint_waitrequest
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/csr_address
add wave -noupdate -group tb /tb_comparator_maxcount/csr_read
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/csr_readdata
add wave -noupdate -group tb /tb_comparator_maxcount/csr_write
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/csr_writedata
add wave -noupdate -group tb /tb_comparator_maxcount/csr_waitrequest
add wave -noupdate -group tb /tb_comparator_maxcount/oflow_write
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/oflow_writedata
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/oflow_address
add wave -noupdate -group tb /tb_comparator_maxcount/oflow_waitrequest
add wave -noupdate -group tb /tb_comparator_maxcount/irq
add wave -noupdate -group tb /tb_comparator_maxcount/fsm_fprint_write
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/fsm_fprint_writedata
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/fsm_fprint_address
add wave -noupdate -group tb /tb_comparator_maxcount/fsm_fprint_waitrequest
add wave -noupdate -group tb /tb_comparator_maxcount/fsm_csr_write
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/fsm_csr_writedata
add wave -noupdate -group tb /tb_comparator_maxcount/fsm_csr_read
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/fsm_csr_readdata
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/fsm_csr_address
add wave -noupdate -group tb /tb_comparator_maxcount/fsm_csr_waitrequest
add wave -noupdate -group tb /tb_comparator_maxcount/irq_csr_write
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/irq_csr_writedata
add wave -noupdate -group tb /tb_comparator_maxcount/irq_csr_read
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/irq_csr_readdata
add wave -noupdate -group tb -radix hexadecimal /tb_comparator_maxcount/irq_csr_address
add wave -noupdate -group tb /tb_comparator_maxcount/irq_csr_waitrequest
add wave -noupdate -group tb /tb_comparator_maxcount/state_fprint
add wave -noupdate -group tb /tb_comparator_maxcount/turn_fprint
add wave -noupdate -group tb /tb_comparator_maxcount/state_csr
add wave -noupdate -group tb /tb_comparator_maxcount/turn_csr
add wave -noupdate -group tb /tb_comparator_maxcount/core_start
add wave -noupdate -group tb /tb_comparator_maxcount/core_done
add wave -noupdate -group tb /tb_comparator_maxcount/core_enable_fprints
add wave -noupdate -group tb /tb_comparator_maxcount/core_logical_id
add wave -noupdate -group tb -radix unsigned /tb_comparator_maxcount/core_task_id
add wave -noupdate -group tb -radix unsigned /tb_comparator_maxcount/core_fprint_count
add wave -noupdate -group tb -radix unsigned /tb_comparator_maxcount/core_fprint_total_count
add wave -noupdate -group tb /tb_comparator_maxcount/core_fprint_spd
add wave -noupdate -group tb /tb_comparator_maxcount/task_start
add wave -noupdate -group tb /tb_comparator_maxcount/task_done
add wave -noupdate -group tb -radix unsigned /tb_comparator_maxcount/task_total_fprints
add wave -noupdate -group tb -radix unsigned /tb_comparator_maxcount/task_fprint_mem_maxcount
add wave -noupdate -group tb -radix unsigned /tb_comparator_maxcount/task_physical_core_id_0
add wave -noupdate -group tb -radix unsigned /tb_comparator_maxcount/task_physical_core_id_1
add wave -noupdate -group tb /tb_comparator_maxcount/task_core0_fprint_spd
add wave -noupdate -group tb /tb_comparator_maxcount/task_core1_fprint_spd
add wave -noupdate -group tb /tb_comparator_maxcount/i
add wave -noupdate -group tb /tb_comparator_maxcount/init_complete
add wave -noupdate -divider csr_registers
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/idle
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/st_csr_write
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/st_csr_read
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/st_waitrequest
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/st_comp_write
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/st_comp_ack
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/st_set_head_tail
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/clk
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/reset
add wave -noupdate -group csr_registers -radix hexadecimal /tb_comparator_maxcount/cfpu_block/csr_register_block/csr_address
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/csr_read
add wave -noupdate -group csr_registers -radix hexadecimal /tb_comparator_maxcount/cfpu_block/csr_register_block/csr_readdata
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/csr_write
add wave -noupdate -group csr_registers -radix hexadecimal /tb_comparator_maxcount/cfpu_block/csr_register_block/csr_writedata
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/csr_waitrequest
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/comp_status_write
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/comp_status_ack
add wave -noupdate -group csr_registers -radix hexadecimal /tb_comparator_maxcount/cfpu_block/csr_register_block/comp_task
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/comp_collision_detected
add wave -noupdate -group csr_registers -radix hexadecimal /tb_comparator_maxcount/cfpu_block/csr_register_block/physical_core_id
add wave -noupdate -group csr_registers -radix hexadecimal /tb_comparator_maxcount/cfpu_block/csr_register_block/fprint_task_id
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/logical_core_id
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/csr_maxcount_write
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/csr_maxcount_writedata
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/head_tail_data
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/head_tail_offset
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/set_head_tail
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/head_tail_ack
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/start_pointer_ex
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/end_pointer_ex
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/start_pointer_comp
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/end_pointer_comp
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/irq
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/exception_reg
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/success_reg
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/fail_reg
add wave -noupdate -group csr_registers -radix hexadecimal /tb_comparator_maxcount/cfpu_block/csr_register_block/start_pointer_mem
add wave -noupdate -group csr_registers -radix hexadecimal /tb_comparator_maxcount/cfpu_block/csr_register_block/end_pointer_mem
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/csr_write_reg
add wave -noupdate -group csr_registers -radix unsigned -childformat {{{/tb_comparator_maxcount/cfpu_block/csr_register_block/core_assignment_reg[1]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/csr_register_block/core_assignment_reg[0]} -radix unsigned}} -subitemconfig {{/tb_comparator_maxcount/cfpu_block/csr_register_block/core_assignment_reg[1]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/csr_register_block/core_assignment_reg[0]} {-height 15 -radix unsigned}} /tb_comparator_maxcount/cfpu_block/csr_register_block/core_assignment_reg
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/exception_reg_sel
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/success_reg_sel
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/fail_reg_sel
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/maxcount_reg_sel
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/start_p_sel
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/end_p_sel
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/dir_pointer_offset
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/core_id
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/core_assignment_offset
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/core_assignment_data
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/core_assignment_sel
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/state
add wave -noupdate -group csr_registers /tb_comparator_maxcount/cfpu_block/csr_register_block/comp_reg_write
add wave -noupdate -divider fprint_registers
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/idle
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/st_waitrequest
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/st_read_fifo
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/st_decode
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/st_reg_store
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/st_crc_store_0
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/st_crc_store_1
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/st_increment_head_pointer
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/st_task_verified
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/clk
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/reset
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_address
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_write
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_writedata
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_waitrequest
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/physical_core_id
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_task_id
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/logical_core_id
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_head_pointer
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/increment_head_pointer
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/increment_hp_ack
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/checkin_reg_out
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/comp_tail_pointer0
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/comp_tail_pointer1
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint0
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint1
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/comp_task_verified
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_reg_ack
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/comp_task
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/checkout_reg
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/checkin_reg
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/pause_reg
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_mem_0_0
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_mem_0_1
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_mem_1_0
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_mem_1_1
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/store_reg
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_block
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint0_0
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint0_1
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint1_0
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint1_1
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_data_in
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_rd_en
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_wr_en
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_data_out
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_empty
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_full
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_address_reg
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_writedata_reg
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_fprint_address
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_fprint_writedata
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/checkout_sel
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/checkin_sel
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/fprint_sel
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/pause_sel
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/unpause_sel
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/FIFO_state
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/state
add wave -noupdate -group fprint_registers /tb_comparator_maxcount/cfpu_block/fprint_reg/store_fprint
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/DATA_WIDTH
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/ADDR_WIDTH
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/RAM_DEPTH
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/clk
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/rst
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/rd_en
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/wr_en
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/data_in
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/full
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/empty
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/data_out
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/wr_pointer
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/rd_pointer
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/status_cnt
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/data_ram
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/write_data
add wave -noupdate -group fprint_fifo /tb_comparator_maxcount/cfpu_block/fprint_reg/fifo_block/read_data
add wave -noupdate -divider overflow_registers
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/idle
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/st_overflow_fifo_read
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/st_underflow_fifo_read
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/st_overflow_output
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/st_underflow_output
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/st_output_ready
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/clk
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/reset
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/oflow_write
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/oflow_writedata
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/oflow_address
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/oflow_waitrequest
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/csr_maxcount_write
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/csr_maxcount_writedata
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/checkin_in
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/checkin_out
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/count_inc
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/count_dec
add wave -noupdate -group oflow_registers -radix unsigned /tb_comparator_maxcount/cfpu_block/oflow_reg/count_inc_physical_core_id
add wave -noupdate -group oflow_registers -radix unsigned /tb_comparator_maxcount/cfpu_block/oflow_reg/count_inc_task_id
add wave -noupdate -group oflow_registers -radix unsigned /tb_comparator_maxcount/cfpu_block/oflow_reg/count_dec_task_id
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/count_inc_logical_core_id
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/task_ids_equal
add wave -noupdate -group oflow_registers -radix unsigned /tb_comparator_maxcount/cfpu_block/oflow_reg/physical_id_table
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_status_reg
add wave -noupdate -group oflow_registers -radix unsigned /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_physical_core_id
add wave -noupdate -group oflow_registers -radix unsigned /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_physical_core_id
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_present_on_inc
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_present_on_dec
add wave -noupdate -group oflow_registers -radix unsigned /tb_comparator_maxcount/cfpu_block/oflow_reg/fprint_mem_maxcount
add wave -noupdate -group oflow_registers -radix unsigned /tb_comparator_maxcount/cfpu_block/oflow_reg/fprint_mem_count_0
add wave -noupdate -group oflow_registers -radix unsigned /tb_comparator_maxcount/cfpu_block/oflow_reg/fprint_mem_count_1
add wave -noupdate -group oflow_registers -radix unsigned /tb_comparator_maxcount/cfpu_block/oflow_reg/fprint_mem_count_on_inc_0
add wave -noupdate -group oflow_registers -radix unsigned /tb_comparator_maxcount/cfpu_block/oflow_reg/fprint_mem_count_on_inc_1
add wave -noupdate -group oflow_registers -radix unsigned /tb_comparator_maxcount/cfpu_block/oflow_reg/fprint_mem_count_on_dec_0
add wave -noupdate -group oflow_registers -radix unsigned /tb_comparator_maxcount/cfpu_block/oflow_reg/fprint_mem_count_on_dec_1
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/LC0_overflow
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/LC0_underflow
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/LC1_overflow
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/LC1_underflow
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/fprint_mem_count_inc_0
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/fprint_mem_count_inc_1
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/fprint_mem_count_dec_0
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/fprint_mem_count_dec_1
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/csr_maxcount_task_id
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/csr_maxcount_value
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo_data_in
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo_rd_en
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo_wr_en
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo_data_out
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo_empty
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo_full
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo_data_in
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo_rd_en
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo_wr_en
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo_data_out
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo_empty
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo_full
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/get_overflow_fifo_output
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/get_underflow_fifo_output
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/fifo_output
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/state
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/i
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/j
add wave -noupdate -group oflow_registers /tb_comparator_maxcount/cfpu_block/oflow_reg/k
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/DATA_WIDTH
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/ADDR_WIDTH
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/RAM_DEPTH
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/clk
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/rst
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/rd_en
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/wr_en
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/data_in
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/full
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/empty
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/data_out
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/wr_pointer
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/rd_pointer
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/status_cnt
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/data_ram
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/write_data
add wave -noupdate -group overflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/overflow_fifo/read_data
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/DATA_WIDTH
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/ADDR_WIDTH
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/RAM_DEPTH
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/clk
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/rst
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/rd_en
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/wr_en
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/data_in
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/full
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/empty
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/data_out
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/wr_pointer
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/rd_pointer
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/status_cnt
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/data_ram
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/write_data
add wave -noupdate -group underflow_fifo /tb_comparator_maxcount/cfpu_block/oflow_reg/underflow_fifo/read_data
add wave -noupdate -divider comp_registers
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/idle
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/st_set_pointers
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/st_ack_pointers
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/st_increment_hp_ack
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/st_reset_fprint_ready
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/clk
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/reset
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/head_tail_data
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/head_tail_offset
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/set_head_tail
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/head_tail_ack
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/logical_core_id
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/fprint_task_id
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/fprint_head_pointer
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/increment_head_pointer
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/increment_hp_ack
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/start_pointer_ex
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/end_pointer_ex
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/start_pointer_comp
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/end_pointer_comp
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/comp_task
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/fprints_ready_out
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/head0_matches_head1
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/tail0_matches_head0
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/tail1_matches_head1
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/comp_tail_pointer0
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/comp_tail_pointer1
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/comp_increment_tail_pointer
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/comp_reset_fprint_ready
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/reset_fprint_ack
add wave -noupdate -group comp_registers -group interface /tb_comparator_maxcount/cfpu_block/comp_reg/comp_mismatch_detected
add wave -noupdate -group comp_registers -group FSM -expand -group states /tb_comparator_maxcount/cfpu_block/comp_reg/idle
add wave -noupdate -group comp_registers -group FSM -expand -group states /tb_comparator_maxcount/cfpu_block/comp_reg/st_set_pointers
add wave -noupdate -group comp_registers -group FSM -expand -group states /tb_comparator_maxcount/cfpu_block/comp_reg/st_ack_pointers
add wave -noupdate -group comp_registers -group FSM -expand -group states /tb_comparator_maxcount/cfpu_block/comp_reg/st_increment_hp_ack
add wave -noupdate -group comp_registers -group FSM -expand -group states /tb_comparator_maxcount/cfpu_block/comp_reg/st_reset_fprint_ready
add wave -noupdate -group comp_registers -group FSM -radix unsigned /tb_comparator_maxcount/cfpu_block/comp_reg/state
add wave -noupdate -group comp_registers -group internals -radix unsigned -childformat {{{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[15]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[14]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[13]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[12]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[11]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[10]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[9]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[8]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[7]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[6]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[5]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[4]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[3]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[2]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[1]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[0]} -radix unsigned}} -expand -subitemconfig {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[15]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[14]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[13]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[12]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[11]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[10]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[9]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[8]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[7]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[6]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[5]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[4]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[3]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[2]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[1]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0[0]} {-height 15 -radix unsigned}} /tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_0
add wave -noupdate -group comp_registers -group internals -radix unsigned -childformat {{{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[15]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[14]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[13]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[12]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[11]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[10]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[9]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[8]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[7]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[6]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[5]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[4]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[3]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[2]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[1]} -radix unsigned} {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[0]} -radix unsigned}} -expand -subitemconfig {{/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[15]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[14]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[13]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[12]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[11]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[10]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[9]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[8]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[7]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[6]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[5]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[4]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[3]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[2]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[1]} {-height 15 -radix unsigned} {/tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1[0]} {-height 15 -radix unsigned}} /tb_comparator_maxcount/cfpu_block/comp_reg/head_pointer_mem_1
add wave -noupdate -group comp_registers -group internals -radix unsigned /tb_comparator_maxcount/cfpu_block/comp_reg/tail_pointer_mem_0
add wave -noupdate -group comp_registers -group internals -radix unsigned /tb_comparator_maxcount/cfpu_block/comp_reg/tail_pointer_mem_1
add wave -noupdate -group comp_registers -group internals -radix unsigned /tb_comparator_maxcount/cfpu_block/comp_reg/fprint_head_pointer0
add wave -noupdate -group comp_registers -group internals -radix unsigned /tb_comparator_maxcount/cfpu_block/comp_reg/fprint_head_pointer1
add wave -noupdate -group comp_registers -group internals /tb_comparator_maxcount/cfpu_block/comp_reg/fprints_ready
add wave -noupdate -group comp_registers -group internals -radix unsigned /tb_comparator_maxcount/cfpu_block/comp_reg/comp_head_pointer0
add wave -noupdate -group comp_registers -group internals -radix unsigned /tb_comparator_maxcount/cfpu_block/comp_reg/comp_head_pointer1
add wave -noupdate -group comp_registers -group internals /tb_comparator_maxcount/cfpu_block/comp_reg/reset_task
add wave -noupdate -group comp_registers -group internals -radix unsigned /tb_comparator_maxcount/cfpu_block/comp_reg/state
add wave -noupdate -group comp_registers -group internals /tb_comparator_maxcount/cfpu_block/comp_reg/set_pointers
add wave -noupdate -divider comparator
add wave -noupdate -group comparator -group interface -radix unsigned /tb_comparator_maxcount/cfpu_block/comparator/comp_task
add wave -noupdate -group comparator -group interface /tb_comparator_maxcount/cfpu_block/comparator/head0_matches_head1
add wave -noupdate -group comparator -group interface /tb_comparator_maxcount/cfpu_block/comparator/tail0_matches_head0
add wave -noupdate -group comparator -group interface /tb_comparator_maxcount/cfpu_block/comparator/tail1_matches_head1
add wave -noupdate -group comparator -group interface /tb_comparator_maxcount/cfpu_block/comparator/fprints_ready
add wave -noupdate -group comparator -group interface /tb_comparator_maxcount/cfpu_block/comparator/checkin
add wave -noupdate -group comparator -group interface -radix hexadecimal /tb_comparator_maxcount/cfpu_block/comparator/fprint0
add wave -noupdate -group comparator -group interface -radix hexadecimal /tb_comparator_maxcount/cfpu_block/comparator/fprint1
add wave -noupdate -group comparator -group interface /tb_comparator_maxcount/cfpu_block/comparator/comp_increment_tail_pointer
add wave -noupdate -group comparator -group interface /tb_comparator_maxcount/cfpu_block/comparator/comp_reset_fprint_ready
add wave -noupdate -group comparator -group interface /tb_comparator_maxcount/cfpu_block/comparator/reset_fprint_ack
add wave -noupdate -group comparator -group interface /tb_comparator_maxcount/cfpu_block/comparator/comp_task_verified
add wave -noupdate -group comparator -group interface /tb_comparator_maxcount/cfpu_block/comparator/fprint_reg_ack
add wave -noupdate -group comparator -group interface /tb_comparator_maxcount/cfpu_block/comparator/comp_status_write
add wave -noupdate -group comparator -group interface /tb_comparator_maxcount/cfpu_block/comparator/comp_status_ack
add wave -noupdate -group comparator -group interface /tb_comparator_maxcount/cfpu_block/comparator/comp_mismatch_detected
add wave -noupdate -group comparator -group interface /tb_comparator_maxcount/cfpu_block/comparator/task_check
add wave -noupdate -group comparator -group FSM -group states /tb_comparator_maxcount/cfpu_block/comparator/init
add wave -noupdate -group comparator -group FSM -group states /tb_comparator_maxcount/cfpu_block/comparator/set_task
add wave -noupdate -group comparator -group FSM -group states /tb_comparator_maxcount/cfpu_block/comparator/load_pointer
add wave -noupdate -group comparator -group FSM -group states /tb_comparator_maxcount/cfpu_block/comparator/load_fprint
add wave -noupdate -group comparator -group FSM -group states /tb_comparator_maxcount/cfpu_block/comparator/check_task_status
add wave -noupdate -group comparator -group FSM -group states /tb_comparator_maxcount/cfpu_block/comparator/task_complete
add wave -noupdate -group comparator -group FSM -group states /tb_comparator_maxcount/cfpu_block/comparator/compare_fprints
add wave -noupdate -group comparator -group FSM -group states /tb_comparator_maxcount/cfpu_block/comparator/mismatch_detected
add wave -noupdate -group comparator -group FSM -group states /tb_comparator_maxcount/cfpu_block/comparator/task_verified
add wave -noupdate -group comparator -group FSM -group states /tb_comparator_maxcount/cfpu_block/comparator/increment_tail_pointer
add wave -noupdate -group comparator -group FSM -group states /tb_comparator_maxcount/cfpu_block/comparator/reset_fprint_ready
add wave -noupdate -group comparator -group FSM -group states /tb_comparator_maxcount/cfpu_block/comparator/write_status_reg
add wave -noupdate -group comparator -group FSM -radix unsigned /tb_comparator_maxcount/cfpu_block/comparator/state
add wave -noupdate -group comparator -group internals /tb_comparator_maxcount/cfpu_block/comparator/fprints_match
add wave -noupdate -group comparator -group internals /tb_comparator_maxcount/cfpu_block/comparator/state
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {93108 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 521
configure wave -valuecolwidth 386
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
WaveRestoreZoom {0 ps} {105 ns}
