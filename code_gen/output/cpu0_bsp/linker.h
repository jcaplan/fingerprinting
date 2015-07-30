/*
 * linker.h - Linker script mapping information
 *
 * Machine generated for CPU 'processor0_0_cpu0' in SOPC Builder design 'nios_fprint'
 * SOPC Builder design path: /home/jonah/fingerprinting/code_gen/platform/nios_fprint.sopcinfo
 *
 * Generated: Thu Jul 30 17:51:39 EDT 2015
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

#ifndef __LINKER_H_
#define __LINKER_H_


/*
 * BSP controls alt_load() behavior in crt0.
 *
 */

#define ALT_LOAD_EXPLICITLY_CONTROLLED


/*
 * Base address and span (size in bytes) of each linker region
 *
 */

#define MEMORY_0_ONCHIP_MEMORYMAIN_BEFORE_RESET_REGION_BASE 0x400000
#define MEMORY_0_ONCHIP_MEMORYMAIN_BEFORE_RESET_REGION_SPAN 204800
#define MEMORY_0_ONCHIP_MEMORYMAIN_REGION_BASE 0x464020
#define MEMORY_0_ONCHIP_MEMORYMAIN_REGION_SPAN 196576
#define PROCESSOR0_0_SCRATCHPAD_0_REGION_BASE 0x4200000
#define PROCESSOR0_0_SCRATCHPAD_0_REGION_SPAN 16384
#define PROCESSOR0_0_SCRATCHPAD_1_REGION_BASE 0x4204000
#define PROCESSOR0_0_SCRATCHPAD_1_REGION_SPAN 16384
#define RESET_REGION_BASE 0x464000
#define RESET_REGION_SPAN 32
#define SHARED_MEMORY_REGION_BASE 0x2500000
#define SHARED_MEMORY_REGION_SPAN 1024
#define STACK_BIN_0_REGION_BASE 0x495000
#define STACK_BIN_0_REGION_SPAN 4096
#define STACK_BIN_1_REGION_BASE 0x494000
#define STACK_BIN_1_REGION_SPAN 4096


/*
 * Devices associated with code sections
 *
 */

#define ALT_EXCEPTIONS_DEVICE MEMORY_0_ONCHIP_MEMORYMAIN
#define ALT_RESET_DEVICE MEMORY_0_ONCHIP_MEMORYMAIN
#define ALT_RODATA_DEVICE MEMORY_0_ONCHIP_MEMORYMAIN
#define ALT_RWDATA_DEVICE MEMORY_0_ONCHIP_MEMORYMAIN
#define ALT_TEXT_DEVICE MEMORY_0_ONCHIP_MEMORYMAIN


/*
 * Initialization code at the reset address is allowed (e.g. no external bootloader).
 *
 */

#define ALT_ALLOW_CODE_AT_RESET


/*
 * The alt_load() facility is called from crt0 to copy sections into RAM.
 *
 */

#define ALT_LOAD_COPY_RWDATA

#endif /* __LINKER_H_ */
