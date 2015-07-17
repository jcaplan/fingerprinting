#include "critical.h"

/****************************************************************************
 * This function is a wrapper for the critical task AirbagModel_step()
 ****************************************************************************/
void AirbagModel_CT(int priority, AirbagModelStruct *AirbagModel_struct){
	RT_MODEL_AirbagModel_T *const AirbagModel_M = &AirbagModel_struct->AirbagModel_M;
	ExtU_AirbagModel_T *AirbagModel_U = &AirbagModel_struct->AirbagModel_U;
	ExtY_AirbagModel_T *AirbagModel_Y = &AirbagModel_struct->AirbagModel_Y;
	enable_fprint_task(priority);
	AirbagModel_step(AirbagModel_M, AirbagModel_U, AirbagModel_Y);
	disable_fprint_task(priority);
}

/****************************************************************************
 * This function is a wrapper for the critical task CruiseControlSystem_step()
 ****************************************************************************/
void CruiseControlSystem_CT(int priority, CruiseControlSystemStruct *CruiseControlSystem_struct){
	RT_MODEL_CruiseControlSystem_T *const CruiseControlSystem_M = &CruiseControlSystem_struct->CruiseControlSystem_M;
	ExtU_CruiseControlSystem_T *CruiseControlSystem_U = &CruiseControlSystem_struct->CruiseControlSystem_U;
	ExtY_CruiseControlSystem_T *CruiseControlSystem_Y = &CruiseControlSystem_struct->CruiseControlSystem_Y;
	enable_fprint_task(priority);
	CruiseControlSystem_step(CruiseControlSystem_M, CruiseControlSystem_U, CruiseControlSystem_Y);
	disable_fprint_task(priority);
}

/****************************************************************************
 * This function is a wrapper for the critical task TractionControl_step()
 ****************************************************************************/
void TractionControl_CT(int priority, TractionControlStruct *TractionControl_struct){
	RT_MODEL_TractionControl_T *const TractionControl_M = &TractionControl_struct->TractionControl_M;
	ExtU_TractionControl_T *TractionControl_U = &TractionControl_struct->TractionControl_U;
	ExtY_TractionControl_T *TractionControl_Y = &TractionControl_struct->TractionControl_Y;
	enable_fprint_task(priority);
	TractionControl_step(TractionControl_M, TractionControl_U, TractionControl_Y);
	disable_fprint_task(priority);
}

