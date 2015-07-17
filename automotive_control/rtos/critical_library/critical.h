/*
 * critical.h
 *
 *  Created on: 2014-03-02
 *      Author: Jonah
 */

#ifndef CRITICAL_H_
#define CRITICAL_H_

#include "fingerprint.h"
#include "AirbagModel.h"
#include "Derivative.h"


typedef struct {
	RT_MODEL_Derivative_T Derivative_M;
	ExtU_Derivative_T Derivative_U;
	ExtY_Derivative_T Derivative_Y;
} DerivativeStruct;

typedef struct {
	RT_MODEL_AirbagModel_T AirbagModel_M;
	ExtU_AirbagModel_T AirbagModel_U;
	ExtY_AirbagModel_T AirbagModel_Y;
} AirbagModelStruct;


/****************************************************************************
 * This function is a wrapper for the critical task AirbagModel_step()
 ****************************************************************************/
void AirbagModel_CT(int priority, AirbagModelStruct *airbagModel_struct);


/****************************************************************************
 * This function is a wrapper for the critical task Derivative_step()
 ****************************************************************************/
void Derivative_CT(int priority,DerivativeStruct *derivative_struct);


#endif /* CRITICAL_H_ */
