
typedef struct MemoryManagerStruct MemoryManagerStruct;

#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <ucos_ii.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tlb.h"


struct MemoryManagerStruct{
	bool requires_manager;
	int taskPriority;
	void* stackVirtualAddress;
	void* stackPhysicalAddress;
};

MemoryManagerStruct memoryManagerTable[OS_MAX_TASKS];


bool requires_manager(int task);
MemoryManagerStruct *getTaskEntry(int task);










#endif

