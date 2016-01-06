static void handleMonitorInterrupt(void* context) {
	int taskIndex = critFuncData[CORE_ID].tableIndex; //access shared memory
	updateMemoryManagerTable(taskIndex,
			&critFuncData[CORE_ID]);

	int *PROCESSOR0_IRQ = (int *) PROCESSOR0_0_CPU_IRQ_0_BASE;
	*PROCESSOR0_IRQ = 0; //disable IRQ
	OSSemPost(critical_SEM[taskIndex]); //start the task
}
