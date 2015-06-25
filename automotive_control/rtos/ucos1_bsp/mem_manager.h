
typedef struct MemoryManagerStruct MemoryManagerStruct;

#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <ucos_ii.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tlb.h"
#include "ucos1.h"


#define derivate_airbagModel_memoryTableIndex 0


struct MemoryManagerStruct{
	bool disablePending;
	int disablePendSource;
	int tlbDataLine;
	int tlbStackLine;
	int taskPriority;
	void* stackVirtualAddress;
	void* stackPhysicalAddress;
	void* dataVirtualAddress;
	void* dataPhysicalAddress;
};

MemoryManagerStruct memoryManagerTable[OS_MAX_TASKS];


bool requires_manager(int task);
MemoryManagerStruct *getTaskEntry(int task);
void mem_manager_init(void);

void updateMemoryManagerTable(int taskPriority,CriticalFunctionData *data);

void managerDisableCurrentTask(INT8U OSPrioCur);
void managerEnableNextTask(INT8U OSPrioHighRdy);
void  managerCheckPendingDisabled(int OSPrioCur);

#endif

