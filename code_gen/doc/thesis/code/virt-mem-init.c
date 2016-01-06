OS_STK for_loop_100000_0_STACK[FOR_LOOP_100000_0_STACKSIZE] __attribute__ ((section (".stack_bin_0")));

void init() {
	MemoryManagerStruct *entry;
	entry->stackPhysicalAddress = (void*)0x495000;
	entry->stackVirtualAddress = (void*)0x63000;

	entry->tlbStackLine = 1; //uTLB line 1 is only used to translate this task stack
	managerEnableTask(entry); //turn on the uTLB lines
	INT8U perr;
	OSTaskCreateExt(for_loop_100000_0_TASK, NULL,
			(OS_STK *)0x63794,
			FOR_LOOP_100000_0_PRIORITY, FOR_LOOP_100000_0_PRIORITY,
			(OS_STK *)0x63000, FOR_LOOP_100000_0_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
}
