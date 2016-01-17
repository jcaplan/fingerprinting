
#ifndef CRITICAL_H_
#define CRITICAL_H_

#include "fingerprint.h"
#include "for_loop_100000_0.h"
#include "for_loop_90000_0.h"
#include "for_loop_80000_0.h"
#include "for_loop_60000_0.h"

typedef struct {
	RT_MODEL_for_loop_100000_0_T for_loop_100000_0_M;
	ExtU_for_loop_100000_0_T for_loop_100000_0_U;
	ExtY_for_loop_100000_0_T for_loop_100000_0_Y;
} for_loop_100000_0Struct;

typedef struct {
	RT_MODEL_for_loop_90000_0_T for_loop_90000_0_M;
	ExtU_for_loop_90000_0_T for_loop_90000_0_U;
	ExtY_for_loop_90000_0_T for_loop_90000_0_Y;
} for_loop_90000_0Struct;

typedef struct {
	RT_MODEL_for_loop_80000_0_T for_loop_80000_0_M;
	ExtU_for_loop_80000_0_T for_loop_80000_0_U;
	ExtY_for_loop_80000_0_T for_loop_80000_0_Y;
} for_loop_80000_0Struct;

typedef struct {
	RT_MODEL_for_loop_60000_0_T for_loop_60000_0_M;
	ExtU_for_loop_60000_0_T for_loop_60000_0_U;
	ExtY_for_loop_60000_0_T for_loop_60000_0_Y;
} for_loop_60000_0Struct;

/****************************************************************************
 * This function is a wrapper for the critical task AirbagModel_step()
 ****************************************************************************/
void for_loop_100000_0_CT(int priority, for_loop_100000_0Struct *for_loop_100000_0_struct);
/****************************************************************************
 * This function is a wrapper for the critical task AirbagModel_step()
 ****************************************************************************/
void for_loop_90000_0_CT(int priority, for_loop_90000_0Struct *for_loop_90000_0_struct);
/****************************************************************************
 * This function is a wrapper for the critical task AirbagModel_step()
 ****************************************************************************/
void for_loop_80000_0_CT(int priority, for_loop_80000_0Struct *for_loop_80000_0_struct);
/****************************************************************************
 * This function is a wrapper for the critical task AirbagModel_step()
 ****************************************************************************/
void for_loop_60000_0_CT(int priority, for_loop_60000_0Struct *for_loop_60000_0_struct);
#endif
