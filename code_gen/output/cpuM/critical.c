#include "critical.h"

/****************************************************************************
 * This function is a wrapper for the critical task AirbagModel_step()
 ****************************************************************************/
void AirbagModel_CT(int priority, AirbagModelStruct *AirbagModel_struct){
	RT_MODEL_AirbagModel_T *const AirbagModel_M = &AirbagModel_struct->AirbagModel_M;
	ExtU_AirbagModel_T *AirbagModel_U = &AirbagModel_struct->AirbagModel_U;
	ExtY_AirbagModel_T *AirbagModel_Y = &AirbagModel_struct->AirbagModel_Y;
	fprint_enable_task(priority);
	AirbagModel_step(AirbagModel_M, AirbagModel_U, AirbagModel_Y);
	fprint_disable_task(priority);
}

/****************************************************************************
 * This function is a wrapper for the critical task CruiseControlSystem_step()
 ****************************************************************************/
void CruiseControlSystem_CT(int priority, CruiseControlSystemStruct *CruiseControlSystem_struct){
	RT_MODEL_CruiseControlSystem_T *const CruiseControlSystem_M = &CruiseControlSystem_struct->CruiseControlSystem_M;
	ExtU_CruiseControlSystem_T *CruiseControlSystem_U = &CruiseControlSystem_struct->CruiseControlSystem_U;
	ExtY_CruiseControlSystem_T *CruiseControlSystem_Y = &CruiseControlSystem_struct->CruiseControlSystem_Y;
	fprint_enable_task(priority);
	CruiseControlSystem_step(CruiseControlSystem_M, CruiseControlSystem_U, CruiseControlSystem_Y);
	fprint_disable_task(priority);
}

/****************************************************************************
 * This function is a wrapper for the critical task RadarTracker_step()
 ****************************************************************************/
void RadarTracker_CT(int priority, RadarTrackerStruct *RadarTracker_struct){
	RT_MODEL_RadarTracker_T *const RadarTracker_M = &RadarTracker_struct->RadarTracker_M;
	ExtU_RadarTracker_T *RadarTracker_U = &RadarTracker_struct->RadarTracker_U;
	ExtY_RadarTracker_T *RadarTracker_Y = &RadarTracker_struct->RadarTracker_Y;
	fprint_enable_task(priority);
	RadarTracker_step(RadarTracker_M, RadarTracker_U, RadarTracker_Y);
	fprint_disable_task(priority);
}

