#include "mem_manager.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif

bool requires_manager(int task){
	MemoryManagerStruct *tableEntry = getTaskEntry(task);

	if(tableEntry != NULL) {
		return tableEntry->requires_manager;
	} else {
		return false;
	}
}



MemoryManagerStruct *getTaskEntry(int task){
	int i;
	for(i = 0; i < OS_MAX_TASKS; i++){
		if(memoryManagerTable[i].taskPriority == task){
			return &memoryManagerTable[i];
		}
	}

	return NULL;
}
