/*
 * fingerprint.h
 *
 *  Created on: 2013-10-13
 *      Author: Jonah
 */
#include <stdint.h>
#include "ucos_ii.h"

#ifndef FINGERPRINT_H_
#define FINGERPRINT_H_

typedef struct Directory_Init_Struct{
	uint32_t start_ptr;
	uint32_t end_ptr;
	uint32_t key;
	uint32_t core_id;
}Directory_Init_Struct;


#define FPRINT_SPR_CURRENTSTATE 				0x0
#define CRC_DIRECTORY_START_BASE				0x40
#define CRC_DIRECTORY_END_BASE					0x80
#define COMPARATOR_EXCEPTION_OFFSET				0xc0
#define COMPARATOR_MAXCOUNT_REG_OFFSET			0xcc
#define COMPARATOR_CORE_ASSIGNMENT_OFFSET       0xd0
#define COMPARATOR_NMR_REG_OFFSET				0xd8
#define CA_TABLE_MAX_REDUNDANCY					3
#define CA_TABLE_NUM_TASKS						16

typedef struct Core_Assignment_Table{
	uint32_t table[CA_TABLE_MAX_REDUNDANCY][CA_TABLE_NUM_TASKS];
}Core_Assignment_Table;

typedef struct Fprint_Status{
	uint32_t status_reg;
	uint32_t successful_reg;
	uint32_t failed_reg;
} Fprint_Status;
void enable_fprint_task(int);
void disable_fprint_task(int);
void fprint_reset_irq(void);
void set_task_directory(Directory_Init_Struct* d);
int fprint_status(Fprint_Status* fps);
void fprint_set_block_size(int size);
void set_core_assignment_table(Core_Assignment_Table* ca);
void set_maxcount_value(int task_id, int logical_core_id, int maxcount);
void set_nmr(int task_id, int nmr);
#endif /* FINGERPRINT_H_ */
