/*
 * reset_monitor.c
 *
 *  Created on: Jun 23, 2015
 *      Author: jonah.caplan
 */


#include "reset_monitor.h"



void resetMonitorEnable(void){
	INT32U *enableReg = (INT32U *)(RESET_MONITOR_BASE_ADDRESS + RM_ENABLE_REG_OFFSET);
	*enableReg = 1;
}

void resetMonitorDisable(void){
	INT32U *enableReg = (INT32U *)RESET_MONITOR_BASE_ADDRESS + RM_ENABLE_REG_OFFSET;
	*enableReg = 0;
}

void resetMonitorMonReg(INT32U mask){
	INT32U *monReg = (INT32U *)(RESET_MONITOR_BASE_ADDRESS + RM_MONITOR_REG_OFFSET);
	*monReg = mask;
}
void resetMonitorCoreReg(INT32U core){
	INT32U *coreReg = (INT32U *)(RESET_MONITOR_BASE_ADDRESS + RM_CORE_REG_OFFSET);
	*coreReg = core;
}

