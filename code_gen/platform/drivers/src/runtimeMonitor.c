#include "runtimeMonitor.h"
#include <stdio.h>


static INT32U numTasks;
static rtMonitor_task *rtMonTaskTable;

void rtMonitorUpdateTime(void){
	int i;
	for(i = 0; i < numTasks; i++){
		rtMonitor_task *task = &rtMonTaskTable[i];
  		if(task->running){
  			task->counter++;
  			if (task->counter == task->deadline){
				printf("time to drop low criticality task because of overrun: %s\n",task->name);
				rtMonitorDropLowTasks();
			} else {
				printf("task %s runtime %d\n",task->name,task->counter);
			}
		}
	}
}


void rtMonitorStartTask(int taskID){
	rtMonitor_task *task = &rtMonTaskTable[taskID];
	task->running = true;
	task->counter = 0;
}

void rtMonitorEndTask(int taskID){
	rtMonitor_task *task = &rtMonTaskTable[taskID];
	task->running = false;
}

void rtMonitorInit(rtMonitor_task *rtMonTaskTableP, INT32U numberTasks){
	rtMonTaskTable = rtMonTaskTableP;
	numTasks = numberTasks;
}


const char *rtMonitorGetTaskName(int taskID){
	return rtMonTaskTable[taskID].name;
}

void rtMonitorDropLowTasks(void){
	int i;
	for(i = 0; i < numTasks; i++){
		rtMonitor_task *task = &rtMonTaskTable[i];
		if(!task->critical){
			OSTaskSuspend (task->priority);
			task->running = false;
		}
	}
}