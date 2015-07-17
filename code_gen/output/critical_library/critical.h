
#ifndef CRITICAL_H_
#define CRITICAL_H_

#include "fingerprint.h"
#include "AirbagModel.h"
#include "CruiseControlSystem.h"
#include "TractionControl.h"

typedef struct {
	RT_MODEL_AirbagModel_T AirbagModel_M;
	ExtU_AirbagModel_T AirbagModel_U;
	ExtY_AirbagModel_T AirbagModel_Y;
} AirbagModelStruct;

typedef struct {
	RT_MODEL_CruiseControlSystem_T CruiseControlSystem_M;
	ExtU_CruiseControlSystem_T CruiseControlSystem_U;
	ExtY_CruiseControlSystem_T CruiseControlSystem_Y;
} CruiseControlSystemStruct;

typedef struct {
	RT_MODEL_TractionControl_T TractionControl_M;
	ExtU_TractionControl_T TractionControl_U;
	ExtY_TractionControl_T TractionControl_Y;
} TractionControlStruct;

/****************************************************************************
 * This function is a wrapper for the critical task AirbagModel_step()
 ****************************************************************************/
void AirbagModel_CT(int priority, AirbagModelStruct *AirbagModel_struct);
/****************************************************************************
 * This function is a wrapper for the critical task AirbagModel_step()
 ****************************************************************************/
void CruiseControlSystem_CT(int priority, CruiseControlSystemStruct *CruiseControlSystem_struct);
/****************************************************************************
 * This function is a wrapper for the critical task AirbagModel_step()
 ****************************************************************************/
void TractionControl_CT(int priority, TractionControlStruct *TractionControl_struct);
#endif
