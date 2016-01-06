void for_loop_100000_0_TASK(void* pdata) {
	void (*for_loop_100000_0Func)(int,
			for_loop_100000_0Struct*) = functionTable[FOR_LOOP_100000_0_TABLE_INDEX].address;
	while (1) {
		INT8U perr;
		OSSemPend(critical_SEM[FOR_LOOP_100000_0_TABLE_INDEX], 0, &perr);

		rtMonitorStartTask(FOR_LOOP_100000_0_RT_PRIO);

		//Context switch is necessary to clear the callee saved registers
		long registers[8];
		context_switch(registers);

		int fprintID = critFuncData->fprintID;

		void *args = functionTable[FOR_LOOP_100000_0_TABLE_INDEX].args;
		//call the critical task
		for_loop_100000_0Func(fprintID, args);

		context_restore(registers);

		rtMonitorEndTask(FOR_LOOP_100000_0_RT_PRIO);
	}
}
