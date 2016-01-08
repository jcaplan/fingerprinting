#include "runtimeMonitor.h"
#include "sys/alt_timestamp.h"
#include <stdio.h>


static INT32U numTasks;
static rtMonitor_task *rtMonTaskTable;

void rtMonitorUpdateTime(void){
	int i;
	for(i = 0; i < numTasks; i++){
		rtMonitor_task *task = &rtMonTaskTable[i];
  		if(task->running){
  			task->counter += alt_timestamp();
  			alt_timestamp_start();
  			if (task->counter >= task->wcetLowerBound){
				printf("time to drop low criticality task because of overrun: %s\n",task->name); 
				rtMonitorDropLowTasks();
			} else {
				// printf("task %s runtime %llu\n",task->name,task->counter);
			}
		}
	}
}
 

void rtMonitorStartTask(int taskID){
	rtMonitor_task *task = &rtMonTaskTable[taskID];
	task->running = true;
	alt_timestamp_start();
	task->counter = 0;
}

void rtMonitorEndTask(int taskID){
	rtMonitor_task *task = &rtMonTaskTable[taskID];
	task->running = false;
}

void rtMonitorPauseTask(int priority){
		rtMonitor_task *task = rtMonitorGetTask(priority);
		if(task != NULL && task->running){
				task->counter += alt_timestamp();
		}
}

void rtMonitorUnpauseTask(int priority){
	alt_timestamp_start();
}

rtMonitor_task *rtMonitorGetTask(int priority){
	int i;
	for (i = 0; i < numTasks; i++){
		rtMonitor_task *task = &rtMonTaskTable[i];
		if(task->priority == priority){
			return task;
		}
	}
	return NULL;
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