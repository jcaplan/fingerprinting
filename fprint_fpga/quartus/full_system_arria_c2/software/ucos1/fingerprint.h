/*
 * fingerprint.h
 *
 *  Created on: 2013-10-13
 *      Author: Jonah
 */
#include <stdint.h>
#ifndef FINGERPRINT_H_
#define FINGERPRINT_H_

//Struct used for initialization of start and end pointers
typedef struct Directory_Init_Struct{
	uint32_t start_ptr;
	uint32_t end_ptr;
	uint32_t key;
	uint32_t core_id;
}Directory_Init_Struct;

//Macros used to calculate control register offsets
//for comparator and for fingerprint unit
#define FPRINT_SPR_CURRENTSTATE 				0x0
#define CRC_DIRECTORY_START_BASE				0x40
#define CRC_DIRECTORY_END_BASE					0x80
#define COMPARATOR_EXCEPTION_OFFSET				0xc0
#define COMPARATOR_CORE_ASSIGNMENT_OFFSET       0xd0
#define CA_TABLE_MAX_REDUNDANCY					2
#define CA_TABLE_NUM_TASKS						16

//struct used to define the core assignment table
typedef struct Core_Assignment_Table{
	uint32_t table[CA_TABLE_MAX_REDUNDANCY][CA_TABLE_NUM_TASKS];
}Core_Assignment_Table;

//Struct used when getting the status from the comparator
//after an interrupt occurs. Status reg is a 16 bit number.
//If bit n of status_reg is 1, then task n was successful
//Similarly, if bit n of failred_reg is 1, then task n was not successful
//Status reg must be written 0 to disable the IRQ
typedef struct Fprint_Status{
	uint32_t status_reg;
	uint32_t successful_reg;
	uint32_t failed_reg;
} Fprint_Status;

//Turn fingerprinting on. Pass the fingerprint task ID as an argument
void enable_fprint_task(int task);

//Turn fingerprinting off. Pass the fingerprint task ID as an argument
void disable_fprint_task(int task);

//Call when handling an interrupt from the comparator
void fprint_reset_irq(void);

//Use this to set start and end pointers for a given task
void set_task_directory(Directory_Init_Struct* d);

//Use this to retrieve the value of the 3 status registers when an
//interrupt occurs
int fprint_status(Fprint_Status* fps);

//Set the size of a fingerprinting block before starting a task
void fprint_set_block_size(int size);

//Set core assignment table
void set_core_assignment_table(Core_Assignment_Table* ca);

#endif /* FINGERPRINT_H_ */
