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

void fprint_enable_task(int task_id){
	//When enabling, the current state register
	//takes the task_id with a 1 in the MSB.
	uint32_t* fprint_currentstate = (uint32_t*)(FPRINT_BASE_ADDRESS 
		+ FPRINT_SPR_CURRENTSTATE);
	*fprint_currentstate = FPRINT_CURRENTSTATE_ENABLE_MASK | task_id;
}

void fprint_disable_task(int task_id){
	//When a task is checked in, the task number
	//must be rewritten to the current state register
	//with the MSB set to 0.
	uint32_t* fprint_currentstate = (uint32_t*)(FPRINT_BASE_ADDRESS 
		+ FPRINT_SPR_CURRENTSTATE);
	*fprint_currentstate = FPRINT_CURRENTSTATE_DISABLE_MASK | task_id;
}


void comp_reset_irq(void){
	//In the case of a collision
	//Reset the exception register in the fingerprint unit.
	uint32_t* fprint_collision = (uint32_t*)(COMP_BASE_ADDRESS 
		+ COMP_EXCEPTION_OFFSET);
	*fprint_collision = 0;
}

int comp_get_status(Fprint_Status* fps){
		Fprint_Status* f = (Fprint_Status*)(COMP_BASE_ADDRESS + 
			COMP_STATUS_REG_OFFSET);
		fps->successful_reg = f->successful_reg;
		fps->failed_reg = f->failed_reg;
		return 0;
}


void comp_set_core_assignment(int table_column, int core_id, int task_id){
	uint32_t* fprint_core_entry = (uint32_t*)(COMP_BASE_ADDRESS + 
		COMP_CORE_ASSIGNMENT_OFFSET);

    *fprint_core_entry = (table_column << COMP_DATA_CORE_SHIFT) 
    + (task_id << COMP_DATA_TASKID_SHIFT) +  core_id;
}

void comp_set_core_assignment_table(Core_Assignment_Table* ca){

	int i,j;
	for(i = 0; i < CA_TABLE_MAX_REDUNDANCY; i++){
		for(j = 0; j < CA_TABLE_NUM_TASKS; j++){
			uint32_t a = ca->table[i][j];
			comp_set_core_assignment(i,a,j);
		}
	}
}

void comp_set_success_maxcount_value(int task_id, int maxcount) {

	uint32_t* fprint_maxcount_reg = (uint32_t*)(COMP_BASE_ADDRESS 
		+ COMP_SUCCESS_COUNTER_MAX_REG_OFFSET);
	*fprint_maxcount_reg =
		+ (task_id << COMP_DATA_TASKID_SHIFT) + maxcount;
}

void comp_set_nmr(int task_id, int nmr) {

	uint32_t* nmr_reg = (uint32_t*)(COMP_BASE_ADDRESS 
		+ COMP_NMR_REG_OFFSET);

	*nmr_reg = (task_id << COMP_DATA_TASKID_SHIFT) + nmr;
}


void comp_checkpoint_irq_reset(int core_id) {
	int *checkpoint_irq = (int *) (COMP_CPU_IRQ_BASE 
		+ (core_id << COMP_CPU_IRQ_CORE_SHIFT));
	*checkpoint_irq = 0;
}

bool comp_check_task_failed(int task, int failed_reg){
	int mask = (0x3 << task);
	return ((failed_reg & mask) != mask);
}

int comp_get_failed_core(int task, int failed_reg){
	int mask = (0x3 << task);
	return ((failed_reg & mask) >> task);
}

bool comp_check_task_success(int task, int success_reg){
	return (success_reg & (1 << task));
}

bool comp_success_any_task(int success_reg){
	return success_reg;
}

bool comp_fail_any_task(int failed_reg){
	return (failed_reg != 0xFFFFFFFF);
}