
#include <stdint.h>
#include "ucos_ii.h"
#include <stdbool.h>

#ifndef FINGERPRINT_H_
#define FINGERPRINT_H_


#define FPRINT_SPR_CURRENTSTATE 				0x0


/* All indexing and core identification is now done from the data field */

/* HW = SW >> 2 */
#define COMP_EXCEPTION_OFFSET					0x0 	/* resetting IRQ */
#define COMP_STATUS_REG_OFFSET					0x4		/* reserved */
#define COMP_SUCCESS_REG_OFFSET					0x8		/* successful task count */
#define COMP_FAILED_REG_OFFSET					0xC		/* failed task */
#define COMP_CORE_ASSIGNMENT_OFFSET       		0x10	/* core assignment table */
#define COMP_NMR_REG_OFFSET						0x14	/* set DMR or TMR */
#define COMP_SUCCESS_COUNTER_MAX_REG_OFFSET		0x18	/* goal for success reg before sending interrupt */

#define COMP_MAXCOUNT_REG_OFFSET				0xFFFF	/* deprecated */
#define CRC_DIRECTORY_START_BASE				0xFFFF	/* deprecated */
#define CRC_DIRECTORY_END_BASE					0xFFFF	/* deprecated */


/* CPU_IRQ = COMP_CPU_IRQ_BASE | (CORE_ID << CORE_SHIFT) */
#define COMP_CPU_IRQ_BASE 						0x6000000 /* Comp releases core from checkpoint barrier */
#define COMP_CPU_IRQ_CORE_SHIFT					6

#define COMP_BASE_ADDRESS 						0x2400000 /* Monitor to comp interface */
#define FPRINT_BASE_ADDRESS						0x8100000 /* processing core to fprint interface */

 
#define FPRINT_CURRENTSTATE_ENABLE_MASK 		0x10  /* *fprint_currentstate = FPRINT_CURRENTSTATE_ENABLE_MASK | key; */
#define FPRINT_CURRENTSTATE_DISABLE_MASK 		0x00

/* data = (coreID << CORE_SHIFT) | (taskID << TASK_SHIFT) | data */ 
#define COMP_DATA_CORE_SHIFT 					24
#define COMP_DATA_TASKID_SHIFT 					16


#define CA_TABLE_MAX_REDUNDANCY					3
#define CA_TABLE_NUM_TASKS						16




//Struct used for initialization of start and end pointers
typedef struct Directory_Init_Struct{
	uint32_t start_ptr;
	uint32_t end_ptr;
	uint32_t key;
	uint32_t core_id;
}Directory_Init_Struct;


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


void fprint_enable_task(int task_id);
void fprint_disable_task(int task_id);
void comp_reset_irq(void);
int comp_get_status(Fprint_Status* fps);
void comp_set_core_assignment(int table_column, int core_id, int task_id);
void comp_set_core_assignment_table(Core_Assignment_Table* ca);
void comp_set_nmr(int task_id, int nmr);
void comp_checkpoint_irq_reset(int core_id);
int comp_get_success_count(int task);
void comp_set_success_maxcount_value(int task_id, int maxcount);

#endif /* FINGERPRINT_H_ */
