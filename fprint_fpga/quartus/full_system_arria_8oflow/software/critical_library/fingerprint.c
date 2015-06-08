/*
 * fingerprint.c
 *
 *  Created on: 2014-02-23
 *      Author: Jonah
 */


#include <stdint.h>
#include <system.h>
#include "fingerprint.h"

//*********************************
//Control register addresses
//*********************************

#define COMPARATOR_BASE_ADDRESS 0x5000000
#define FPRINT_BASE_ADDRESS		0x8100000

void set_task_directory(Directory_Init_Struct* d){

	//Place the start pointer for the given task
	uint32_t* start = (uint32_t*)(COMPARATOR_BASE_ADDRESS \
			+ CRC_DIRECTORY_START_BASE) + d->key + (d->core_id << 6);
	*start = d->start_ptr;
	//Place the end pointer for the given task
	uint32_t* end   = (uint32_t*)(COMPARATOR_BASE_ADDRESS \
			+ CRC_DIRECTORY_END_BASE) + d->key + (d->core_id << 6);
	*end = d->end_ptr;

}


void enable_fprint_task(int key){
	//When enabling, the current state register
	//takes the key with a 1 in the MSB.
	uint32_t* fprint_currentstate = (uint32_t*)(FPRINT_BASE_ADDRESS \
											+ FPRINT_SPR_CURRENTSTATE);
	*fprint_currentstate = 0x10 | key;

}
void disable_fprint_task(int key){

	//When a task is checked in, the task number
	//must be rewritten to the current state register
	//with the MSB set to 0.
	uint32_t* fprint_currentstate = (uint32_t*)(FPRINT_BASE_ADDRESS \
											+ FPRINT_SPR_CURRENTSTATE);
	*fprint_currentstate = key;

}
void fprint_reset_irq(void){
	//In the case of a collision
	//Reset the exception register in the fingerprint unit.
	uint32_t* fprint_collision = (uint32_t*)(COMPARATOR_BASE_ADDRESS \
											+ COMPARATOR_EXCEPTION_OFFSET);
	*fprint_collision = 0;
}

int fprint_status(Fprint_Status* fps){
		Fprint_Status* f = (uint32_t*)(COMPARATOR_BASE_ADDRESS \
												+ COMPARATOR_EXCEPTION_OFFSET);
		fps->status_reg = f->status_reg;
		fps->successful_reg = f->successful_reg;
		fps->failed_reg = f->failed_reg;
		return 0;

}

//int fprint_status(void){
//	uint32_t* fprint_collision = (uint32_t*)(COMPARATOR_BASE_ADDRESS \
//											+ COMPARATOR_EXCEPTION_OFFSET);
//	int x = *fprint_collision;
//	return x;
//
//}

void fprint_set_block_size(int size){
	uint32_t* fprint_blocksize_reg = (uint32_t*)(FPRINT_BASE_ADDRESS \
												+ 0x10);

	*fprint_blocksize_reg = size;

}

void set_core_assignment(int table_column, int core_id, int task_id){
	uint32_t* fprint_core_entry = (uint32_t*)(COMPARATOR_BASE_ADDRESS \
													+ (table_column << 8) + COMPARATOR_CORE_ASSIGNMENT_OFFSET);

    *fprint_core_entry = (task_id << 4) + core_id;
}

void set_core_assignment_table(Core_Assignment_Table* ca){

	int i,j;
	for(i = 0; i < CA_TABLE_MAX_REDUNDANCY; i++){
		for(j = 0; j < CA_TABLE_NUM_TASKS; j++){
			uint32_t a = ca->table[i][j];
			set_core_assignment(i,a,j);
		}
	}
}

void set_maxcount_value(int task_id, int maxcount) {

	uint32_t* fprint_maxcount_reg = (uint32_t*)(COMPARATOR_BASE_ADDRESS \
														 + COMPARATOR_MAXCOUNT_REG_OFFSET);

	*fprint_maxcount_reg = (task_id << 10) + maxcount;
}
