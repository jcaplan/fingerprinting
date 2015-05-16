/*
 * driver.h
 *
 *  Created on: Jul 28, 2014
 *      Author: Bryan Gingras
 */

#ifndef DRIVER_H_
#define DRIVER_H_

#include "system.h"

#define ENABLE_REG_OFFSET 	(0)
#define VALID_REG_OFFSET	(0x20)
#define CPUTABLE_REG_OFFSET (0x40)
#define SPMTABLE_REG_OFFSET (0x60)
#define TLB_SEL_OFFSET		(0x80)
#define ADDRESS_SIZE		27
#define TAG_SIZE			15
#define PAGE_SIZE			(ADDRESS_SIZE - TAG_SIZE)
#define TLB_BASE 			0x8300000
//TLB functions
void deactivateTlb();
void activateTlb();
void reset_tlb(int count);
unsigned int tlb_is_on();

//Enable register functions
void enableTlbLine(unsigned int line);
void disableTlbLine(unsigned int line);
void reset_enable();
unsigned int get_enable();

//Valid register functions
void set_valid(unsigned int line);
void reset_valid();
unsigned int get_valid();

//CPU table functions
void set_cputable_entry(int index, unsigned int address);
unsigned int get_cputable_entry(int index);

//SPM table functions
void set_spmtable_entry(int index, unsigned int address);
unsigned int get_spmtable_entry(int index);

#endif /* DRIVER_H_ */
