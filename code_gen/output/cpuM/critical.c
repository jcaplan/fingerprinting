#include "critical.h"

/****************************************************************************
 * This function is a wrapper for the critical task for_loop_70000_0_step()
 ****************************************************************************/
void for_loop_70000_0_CT(int priority, for_loop_70000_0Struct *for_loop_70000_0_struct){
	RT_MODEL_for_loop_70000_0_T *const for_loop_70000_0_M = &for_loop_70000_0_struct->for_loop_70000_0_M;
	ExtU_for_loop_70000_0_T *for_loop_70000_0_U = &for_loop_70000_0_struct->for_loop_70000_0_U;
	ExtY_for_loop_70000_0_T *for_loop_70000_0_Y = &for_loop_70000_0_struct->for_loop_70000_0_Y;
	fprint_enable_task(priority);
	for_loop_70000_0_step(for_loop_70000_0_M, for_loop_70000_0_U, for_loop_70000_0_Y);
	fprint_disable_task(priority);
}

/****************************************************************************
 * This function is a wrapper for the critical task for_loop_60000_0_step()
 ****************************************************************************/
void for_loop_60000_0_CT(int priority, for_loop_60000_0Struct *for_loop_60000_0_struct){
	RT_MODEL_for_loop_60000_0_T *const for_loop_60000_0_M = &for_loop_60000_0_struct->for_loop_60000_0_M;
	ExtU_for_loop_60000_0_T *for_loop_60000_0_U = &for_loop_60000_0_struct->for_loop_60000_0_U;
	ExtY_for_loop_60000_0_T *for_loop_60000_0_Y = &for_loop_60000_0_struct->for_loop_60000_0_Y;
	fprint_enable_task(priority);
	for_loop_60000_0_step(for_loop_60000_0_M, for_loop_60000_0_U, for_loop_60000_0_Y);
	fprint_disable_task(priority);
}

