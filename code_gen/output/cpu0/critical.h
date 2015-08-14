
#ifndef CRITICAL_H_
#define CRITICAL_H_

#include "fingerprint.h"
#include "AirbagModel.h"
#include "CruiseControlSystem.h"
#include "RadarTracker.h"

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
	RT_MODEL_RadarTracker_T RadarTracker_M;
	ExtU_RadarTracker_T RadarTracker_U;
	ExtY_RadarTracker_T RadarTracker_Y;
} RadarTrackerStruct;

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
void RadarTracker_CT(int priority, RadarTrackerStruct *RadarTracker_struct);
#endif
