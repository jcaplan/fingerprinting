/*
 * critical.c
 *
 *  Created on: 2014-03-02
 *      Author: Jonah
 */
#include "critical.h"


/****************************************************************************
 * This function is a wrapper for the critical task AirbagModel_step()
 ****************************************************************************/
void AirbagModel_CT(int priority, AirbagModelStruct *airbagModel_struct){
	RT_MODEL_AirbagModel_T *const AirbagModel_M = &airbagModel_struct->AirbagModel_M;
	ExtU_AirbagModel_T *AirbagModel_U = &airbagModel_struct->AirbagModel_U;
	ExtY_AirbagModel_T *AirbagModel_Y = &airbagModel_struct->AirbagModel_Y;
	enable_fprint_task(priority);
	AirbagModel_step(AirbagModel_M,AirbagModel_U,AirbagModel_Y);
	disable_fprint_task(priority);
}


/****************************************************************************
 * This function is a wrapper for the critical task Derivative_step()
 ****************************************************************************/
void Derivative_CT(int priority,DerivativeStruct *derivative_struct){
	RT_MODEL_Derivative_T *const Derivative_M = &derivative_struct->Derivative_M;
	ExtU_Derivative_T *Derivative_U = &derivative_struct->Derivative_U;
	ExtY_Derivative_T *Derivative_Y = &derivative_struct->Derivative_Y;
	enable_fprint_task(priority);
	Derivative_step(Derivative_M,Derivative_U,Derivative_Y);
	disable_fprint_task(priority);
}


