

#ifndef RT_MON_H_
#define RT_MON_H_
#include <ucos_ii.h>
#include "shared_mem.h"
#include "system.h"
#include <stdbool.h>
typedef struct rtMonitor_task rtMonitor_task;

struct rtMonitor_task {
	INT8U priority;
	alt_u64 counter;
	INT32U deadline;
	bool running;
	bool critical;
	const char *name;
};


void rtMonitorUpdateTime(void);
void rtMonitorInit(rtMonitor_task *rtMonTaskTableP, INT32U numberTasks);
void rtMonitorStartTask(int taskID);
void rtMonitorEndTask(int taskID);

void rtMonitorPauseTask(int priority);
void rtMonitorUnpauseTask(int priority);
rtMonitor_task* rtMonitorGetTask(int priority);


const char *rtMonitorGetTaskName(int taskID);
void rtMonitorDropLowTasks(void);

#endif /* RT_MON_H_ */
