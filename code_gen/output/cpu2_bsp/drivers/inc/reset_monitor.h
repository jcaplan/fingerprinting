/*
 * reset_monitor.h
 *
 *  Created on: Jun 23, 2015
 *      Author: jonah.caplan
 */

#ifndef RESET_MONITOR_H_
#define RESET_MONITOR_H_


#include "includes.h"

#define RESET_MONITOR_BASE_ADDRESS   0x2800000
#define RM_ENABLE_REG_OFFSET            0x0
#define RM_MONITOR_REG_OFFSET           0x4
#define RM_CORE_REG_OFFSET              0x8


void resetMonitorEnable(void);
void resetMonitorDisable(void);
void resetMonitorMonReg(INT32U mask);
void resetMonitorCoreReg(INT32U core);



#endif /* RESET_MONITOR_H_ */
