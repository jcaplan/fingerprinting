/*
 * alt_sys_init.c - HAL initialization source
 *
 * Machine generated for CPU 'processorM_0_cpuM' in SOPC Builder design 'nios_fprint'
 * SOPC Builder design path: /home/jonah/fingerprinting/code_gen/platform/nios_fprint_4.sopcinfo
 *
 * Generated: Sun Jan 17 14:32:10 EST 2016
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#include "system.h"
#include "sys/alt_irq.h"
#include "sys/alt_sys_init.h"

#include <stddef.h>

/*
 * Device headers
 */

#include "altera_nios2_qsys_irq.h"
#include "altera_avalon_dma.h"
#include "altera_avalon_jtag_uart.h"
#include "altera_avalon_mutex.h"
#include "altera_avalon_sysid_qsys.h"
#include "altera_avalon_timer.h"

/*
 * Allocate the device storage
 */

ALTERA_NIOS2_QSYS_IRQ_INSTANCE ( PROCESSORM_0_CPUM, processorM_0_cpuM);
ALTERA_AVALON_DMA_INSTANCE ( PROCESSOR0_0_DMA_0, processor0_0_dma_0);
ALTERA_AVALON_DMA_INSTANCE ( PROCESSOR1_0_DMA_0, processor1_0_dma_0);
ALTERA_AVALON_DMA_INSTANCE ( PROCESSOR2_0_DMA_0, processor2_0_dma_0);
ALTERA_AVALON_DMA_INSTANCE ( PROCESSOR3_0_DMA_0, processor3_0_dma_0);
ALTERA_AVALON_JTAG_UART_INSTANCE ( PROCESSORM_0_JTAG_UART, processorM_0_jtag_uart);
ALTERA_AVALON_MUTEX_INSTANCE ( MUTEX_0, mutex_0);
ALTERA_AVALON_SYSID_QSYS_INSTANCE ( SYS_ID, sys_id);
ALTERA_AVALON_TIMER_INSTANCE ( PROCESSORM_0_TIMER, processorM_0_timer);
ALTERA_AVALON_TIMER_INSTANCE ( PROCESSORM_0_TIMESTAMP, processorM_0_timestamp);

/*
 * Initialize the interrupt controller devices
 * and then enable interrupts in the CPU.
 * Called before alt_sys_init().
 * The "base" parameter is ignored and only
 * present for backwards-compatibility.
 */

void alt_irq_init ( const void* base )
{
    ALTERA_NIOS2_QSYS_IRQ_INIT ( PROCESSORM_0_CPUM, processorM_0_cpuM);
    alt_irq_cpu_enable_interrupts();
}

/*
 * Initialize the non-interrupt controller devices.
 * Called after alt_irq_init().
 */

void alt_sys_init( void )
{
    ALTERA_AVALON_TIMER_INIT ( PROCESSORM_0_TIMER, processorM_0_timer);
    ALTERA_AVALON_TIMER_INIT ( PROCESSORM_0_TIMESTAMP, processorM_0_timestamp);
    ALTERA_AVALON_DMA_INIT ( PROCESSOR0_0_DMA_0, processor0_0_dma_0);
    ALTERA_AVALON_DMA_INIT ( PROCESSOR1_0_DMA_0, processor1_0_dma_0);
    ALTERA_AVALON_DMA_INIT ( PROCESSOR2_0_DMA_0, processor2_0_dma_0);
    ALTERA_AVALON_DMA_INIT ( PROCESSOR3_0_DMA_0, processor3_0_dma_0);
    ALTERA_AVALON_JTAG_UART_INIT ( PROCESSORM_0_JTAG_UART, processorM_0_jtag_uart);
    ALTERA_AVALON_MUTEX_INIT ( MUTEX_0, mutex_0);
    ALTERA_AVALON_SYSID_QSYS_INIT ( SYS_ID, sys_id);
}
