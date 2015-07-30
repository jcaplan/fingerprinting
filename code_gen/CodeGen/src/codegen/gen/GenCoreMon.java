package codegen.gen;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;

import org.apache.commons.io.FileUtils;

import codegen.gen.Function.Type;

public class GenCoreMon extends GenCore{

	ArrayList<Function> funcList;
	
	private GenString[] genStrings = new GenString[] {
			new GenString() { public String action(Core core) { return getIncludeStringString(core); } },
			new GenString() { public String action(Core core) { return getVarDecStringString(core); } },
			new GenString() { public String action(Core core) { return getStackDeclarationString(core); } },
			new GenString() { public String action(Core core) { return getMiscDeclString(core); } },
			new GenString() { public String action(Core core) { return getSharedMemoryDeclarationString(core); } },
			new GenString() { public String action(Core core) { return getTaskRelocationString(core); } },
			new GenString() { public String action(Core core) { return getResetMonitorString(core); } },
			new GenString() { public String action(Core core) { return getComparatorString(core); } },
			new GenString() { public String action(Core core) { return getReposString(core); } },
			new GenString() { public String action(Core core) { return getTaskFunctionString(core); } },
			new GenString() { public String action(Core core) { return getMainString(core); } },
		};
		
	
	public GenCoreMon(Configuration config, ArrayList<Function> fprintList,
			Platform platform, ArrayList<Function> funcList) {
		super(config, fprintList, platform);
		this.funcList = funcList;
	}
	
	void generateCore(Core core) throws IOException {

		//first thing add DMA task
		addDmaFunction(core);
		
		File outputDir = new File(config.outputDir + "/" + "cpuM");
		FileUtils.forceMkdir(outputDir);
		File srcOutput = new File(outputDir + "/" + "cpuM" + ".c");
		File incOutput = new File(outputDir + "/" + "cpuM" + ".h");
		PrintWriter writer;
		
		
		writer = new PrintWriter(incOutput);
		writer.print(getHeaderFileString(core));
		writer.close();
		
		
		writer = new PrintWriter(srcOutput);
		for (GenString gen : genStrings){
			writer.print(gen.action(core));
			writer.print(getWhiteSpace(5));
			
		}
		writer.close();
	}
	

	private String getReposString(Core core) {
		String s = "";
		s += "/*****************************************************************************\n"+
				" * REPOS configuration functions\n"+
				" *****************************************************************************/\n"+
				"\n"+
				" void startHook(void *args) {\n"+
				"	postDmaMessage((int)args,true);\n"+
				"}\n"+
				"\n"+
				"\n"+
				"void initializeTaskTable(void) {\n";
				s += "	REPOS_task *task;\n"+
				"\n";
		for(Function f : fprintList){
			String funcIndex = f.toString().toUpperCase() + "_TABLE_INDEX";
			//TODO hard coded!!!
			String core0PhysSP = Integer.toHexString(getStackStart(f,0));
			String core1PhysSP = Integer.toHexString(getStackStart(f,1));
			String core0VirtSp = Integer.toHexString(getStackStart(f,1) & 0x3FFFFF);
			String core1VirtSP = Integer.toHexString(getStackStart(f,1) & 0x3FFFFF);
			s+= "	task = &REPOSTaskTable[" + funcIndex + "];\n"+
					"\n"+
					"	task->dataAddressPhys = &" + f + "PackageStruct;\n"+
					"	task->dataAddressVirt = (void *)((int)&" + f + "PackageStruct & 0x3FFFFF);"+
					"\n"+
					"	task->stackAddressPhys[0] = (void *) (0x" + core0PhysSP + ");\n"+
					"	task->stackAddressPhys[1] = (void *) (0x" + core1PhysSP + ");\n"+
					"\n"+
					"	task->stackAddressVirt[0] = (void *) (0x" + core0VirtSp  + ");\n"+
					"	task->stackAddressVirt[1] = (void *) (0x" + core1VirtSP + ");\n"+
					"\n"+
					"	task->dataSize = sizeof(" + f + "PackageStruct" + ");\n"+
					"	task->stackSize = (" + f + "_STACKSIZE" + " * 4);\n"+
					"\n";
		}
		s += 	"}\n"+
				"\n";
						
		s += "void REPOSInit(void) {\n"+
				"\n"+
				"	memset(REPOSCoreTable, 0, NUMCORES * sizeof(REPOS_core));\n"+
				"	memset(REPOSTaskTable, 0, OS_MAX_TASKS * sizeof(REPOS_task));\n"+
				"\n";
		
		s += "	REPOS_task *task;\n";
		for(Function f : fprintList){
			String funcIndex = f.toString().toUpperCase() + "_TABLE_INDEX";
			s += 	"	task = &REPOSTaskTable[" + funcIndex +"];\n"+
					"	task->status = PENDING;\n";
					//TODO hard coded!
			s += 	"	task->kind = PERIODIC_K;\n"+
					"	task->data.periodic.period = " + f.toString().toUpperCase() + "_PERIOD" + ";\n"+
			"	task->core[0] = 0;\n"+
			"	task->core[1] = 1;\n"+
			"	task->numFuncs = 1;\n"+
			"	task->funcTableFirstIndex = 0;\n"+
			"	task->taskID = " + funcIndex + ";\n"+
			"	task->startHook = startHook;\n"+
			"	task->startHookArgs = (void*)" + funcIndex + ";\n"+
			"\n";

		}
		s += 	"\n"+
				"	fprintIDFreeList = 0xFFFF;\n"+
				"	int i,j;\n"+
				"	for (i = 0; i < 2; i++) {\n"+
				"		for (j = 0; j < 4; j++) {\n"+
				"			pageTable[i][j] = 0x4200000 + ((i * 4 + j) << 12);\n"+
				"		}\n"+
				"	}\n"+
				"	/*\n"+
				"	 * pageTable\n"+
				"	 * = {\n"+
				"	 {\n"+
				"	 0x4200000,\n"+
				"	 0x4201000,\n"+
				"	 0x4202000,\n"+
				"	 0x4203000\n"+
				"	 },\n"+
				"	 {\n"+
				"	 0x4204000,\n"+
				"	 0x4205000,\n"+
				"	 0x4206000,\n"+
				"	 0x4207000\n"+
				"	 }\n"+
				"	 };\n"+
				"	 */\n"+
				"\n"+
				"	initializeTaskTable();\n"+
				"}\n"+
				"\n";	
	
		return s;
	}
	

	private void addDmaFunction(Core core) {
		Function dmaFunction = new Function();
		dmaFunction.codeDirectory = null;
		dmaFunction.critical = false; /* evade special treatment */
		dmaFunction.cores.add("cpum");
		dmaFunction.name = "dma";
		dmaFunction.type = Type.eventDriven;
		
		/* add the variable declarations */
	
		for(Function f : fprintList){
			String s = "typedef struct {\n";
			s += "\t" + f + "Struct " + f + "_STRUCT;\n" +
					"	P_" + f  + "_T " + f + "_P;\n";
			
			for(String dec : f.varDeclarations){
				if(dec.contains("DW")){
					s += "	DW_" + f + "_T " + f + "_DW;\n"; 
					break;
				}
			}
			
			s += "} DMA_" + f + "PackageStruct;\n"+
					"\n"+
					"DMA_" + f + "PackageStruct " + f + "PackageStruct __attribute__ ((section (\".global_data\")));\n"+
					"\n"+
					"\n";

			
			dmaFunction.varDeclarations.add(s);
		}
		
		String s = "";
		s += "OS_EVENT *dmaQ;\n"+
				"OS_FLAG_GRP *dmaReadyFlag;\n"+
				"#define DMA_Q_SIZE 12\n"+
				"INT32U dmaQMem[DMA_Q_SIZE];\n";
		dmaFunction.varDeclarations.add(s);
		
		dmaFunction.stackSize = 0x2000;
		core.funcList.add(dmaFunction);
	}
	
	
	private String getMiscDeclString(Core c) {
		String s = "";
		s += "/*****************************************************************************\n"+
		" * Pointers to interrupt other cores\n"+
		" *****************************************************************************/\n"+
		"\n"+
		"int *core_IRQ[NUMCORES] = { (int *) PROCESSOR0_0_CPU_IRQ_0_BASE,\n"+
		"		(int *) PROCESSOR1_0_CPU_IRQ_0_BASE };\n"+
		"\n"+
		"/*****************************************************************************\n"+
		" * Hardware mutex\n"+
		" *****************************************************************************/\n"+
		"alt_mutex_dev* mutex;\n"+
		"\n"+
		"/*****************************************************************************\n"+
		" * Task control flow conditions\n"+
		" *****************************************************************************/\n"+
		"\n"+
		"#define NUM_CRITICAL_TASKS 						2\n"+
		"\n"+
		"bool coresReady = false;\n"+
		"bool taskFailed = false;\n"+
		"int failedTaskID = 0;\n"+
		"\n";
		
		
		return s;
	}
	
	private String getResetMonitorString(Core core) {
		String s = "";
		
		s += "/*****************************************************************************\n" + 
				" * Reset monitor interface\n" + 
				" *****************************************************************************/\n" + 
				"void resetCores(void) {\n" + 
				"	int* cpu0_reset = (int*) PROCESSOR0_0_SW_RESET_0_BASE;\n" + 
				"	int* cpu1_reset = (int*) PROCESSOR1_0_SW_RESET_0_BASE;\n" + 
				"	*cpu0_reset = 1;\n" + 
				"	*cpu1_reset = 1;\n" + 
				"	coresReady = false;\n" + 
				"	taskFailed = true;\n" + 
				"	resetMonitorEnable();\n" + 
				"}\n" + 
				"static void handleResetMonitor(void* context) {\n" + 
				"	resetMonitorDisable();\n" + 
				"	coresReady = true;\n" + 
				"	if (taskFailed) {\n" + 
				"		taskFailed = false;\n" + 
				"\n" + 
				"		postDmaMessage(failedTaskID, true);\n" + 
				"\n" + 
				"	}\n" + 
				"}\n" + 
				"\n"+
				"static void initResetMonitorIsr(void) {\n"+
				"	alt_ic_isr_register(\n"+
				"			PROCESSORM_0_RESET_MONITOR_0_IRQ_INTERRUPT_CONTROLLER_ID,\n"+
				"			PROCESSORM_0_RESET_MONITOR_0_IRQ, handleResetMonitor, (void*) NULL,\n"+
				"			(void*) NULL);\n"+
				"}\n";
		return s;
	}
	

	private String getComparatorString(Core core) {
		String s = "";
		s += "\n"+
				"/*****************************************************************************\n"+
				" * Comparator ISR\n"+
				" *****************************************************************************/\n"+
				"\n"+
				"static void handleComp(void* context) {\n"+
				"	int result = 0;\n"+
				"	Fprint_Status status;\n"+
				"	comp_get_status(&status);\n"+
				"\n"+
				"	//Assume static mapping of fingeprint tasks for now\n"+
				"	//There is only one possible task that can set this off in this example\n"+
				"	//---------------------------------------------------------------------\n"+
				"	if (status.failed_reg) {\n" + 
				"		int i;\n" + 
				"		for (i = 0; i < 16; i++) {\n" + 
				"			INT32U mask;\n" + 
				"			if (status.failed_reg & (mask = 1 << i)) {\n" + 
				"				/* assume only one failure possible */\n" + 
				"				failedTaskID = REPOSgetTaskID(mask);\n" + 
				"				REPOSTaskReset(failedTaskID);" + 
				"				postDmaMessage(failedTaskID, true);\n" + 
				"				break;\n" + 
				"			}\n" + 
				"		}\n" + 
				"		// resetCores();\n" + 
				"		// REPOSInit(); \n" + 
				"	}"+
				"\n"+
				"	if ((result = status.successful_reg)) {\n"+
				"\n"+
				"		//figure out what task is complete\n"+
				"		int i;\n"+
				"		int taskID = -1;\n"+
				"		for (i = 0; i < 16; i++) {\n"+
				"			INT32U mask;\n"+
				"			if (result & (mask = 1 << i)) {\n"+
				"				taskID = REPOSgetTaskID(mask);\n"+
				"				/* Here we check that all the functions inside the task have executed,\n"+
				"				 * then we check that the task did not fail (since the same FID may have been used twice\n"+
				"				 * before the handler responds if the functions are tiny).\n"+
				"				 */\n"+
				"				if (!taskFailed || (taskFailed && taskID != failedTaskID)) { /*function can be decomposed into several chunks, so could be both cases */\n"+
				"					REPOSTaskTable[taskID].funcCompleteCount = 0;\n"+
				"					REPOSTaskComplete(taskID);\n"+
				"					postDmaMessage(taskID, false);\n"+
				"				}\n"+
				"			}\n"+
				"		}\n"+
				"	}\n"+
				"\n"+
				"	comp_reset_irq();\n"+
				"}\n"+
				"\n"+
				"static void initCompIsr(void) {\n"+
				"	alt_ic_isr_register(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID, CFPU_0_CSR_IRQ,\n"+
				"			handleComp, (void*) NULL, (void*) NULL);\n"+
				"}\n";
		return s;
	}
	
	private String getMainString(Core core) {
		String s = "";
		
		
		s += "/*****************************************************************************\n"+
				" * Main entry point\n"+
				" *****************************************************************************/\n"+
				"int main(void) {\n"+
				"	printf(\"starting monitor\\n\");\n"+
				"\n"+
				"	//Initialize the hardware mutex\n"+
				"	//-----------------------------\n"+
				"	mutex = altera_avalon_mutex_open(MUTEX_0_NAME);\n"+
				"\n"+
				"	//Initialize the shared memory symbol table\n"+
				"	//-----------------------------------------\n"+
				"	shared_stab.ftable = functionTable;\n"+
				"	shared_stab.critFuncData = critFuncData;\n"+
				"\n";
		
		s += "	//Initialize the Function Table\n"+
				"	//-----------------------------\n";
		for(Function f : fprintList){

			String funcIndex = f.toString().toUpperCase() + "_TABLE_INDEX";
			s += "	functionTable[" + funcIndex + "].args =  (void *)((int)&" + f + "PackageStruct & 0x3FFFFF);\n"+
					"	functionTable[" + funcIndex + "].blocksize = 0xfff;\n";			 
		}
		
		s += "	//Initialize the runtime interface\n"+
				"	REPOSInit();\n"+
				"\n"+
				"	// Initialize the reset monitor\n"+
				"	resetMonitorMonReg(3);\n"+
				"	resetMonitorEnable();\n"+
				"	initResetMonitorIsr();\n"+
				"\n"+
				"	//Pass information to processing cores and notify them to begin their startup\n"+
				"	//---------------------------------------------------------------------------\n"+
				"	altera_avalon_mutex_lock(mutex, 1);\n"+
				"	{\n"+
				"		//Synchronize clocks\n"+
				"		//Give other cores the global pointer offset\n"+
				"		//Notify them to continue startup\n"+
				"		//------------------------------------------\n"+
				"		OSTimeSet(0);\n"+
				"		shared_stab.gp_address = get_gp();\n"+
				"		shared_stab.init_complete = 1;\n"+
				"	}\n"+
				"	altera_avalon_mutex_unlock(mutex);\n"+
				"\n"+
				"	//Set up the fingerprinting hardware\n"+
				"	//Only one task is being fingerprinted\n"+
				"	//Assign FID=0\n"+
				"	//------------------------------------\n"+
				"	int i,j;\n\n";
		s += 	"	for (i = 0; i < CA_TABLE_NUM_TASKS; i++){\n" + 
				"		comp_set_success_maxcount_value(i,1);\n" + 
				"	}";
		//TODO: no need to change maxcount without dataflow
		
		s += "	Core_Assignment_Table ca;\n"+
				"	//Default table\n"+
				"	for (i = 0; i < CA_TABLE_MAX_REDUNDANCY; i++) {\n"+
				"		for (j = 0; j < CA_TABLE_NUM_TASKS; j++) {\n"+
				"			ca.table[i][j] = i;\n"+
				"		}\n"+
				"	}\n"+
				"	ca.table[0][0] = 0;\n"+
				"	ca.table[1][0] = 1;\n"+
				"	comp_set_core_assignment_table(&ca);\n"+
				"\n"+
				"	initCompIsr();\n"+
				"\n"+
				"	//Initialize the DMA hardware\n"+
				"	//---------------------------\n"+
				"	initDMA();\n"+
				"\n";
		
		for(Function f : core.funcList){
			s += f.printInitialization(1);
		}
		for(Function f : fprintList){
			s += f.printInitializationWithStruct(1);
		}
				
		
		s += 	//Initialize the control flow data structures\n"+
				"	//-------------------------------------------\n"+
				"	INT8U perr;\n"+
				"	dmaReadyFlag = OSFlagCreate(0, &perr); /* 0x3 = both cores are ready */\n"+
				"	dmaQ = OSQCreate((void**) dmaQMem, DMA_Q_SIZE);\n"+
				"\n\n\n";
		
		
		s += 	"	//Declare the OS tasks\n"+
				"	//-------------------\n";
		
		for(Function f : core.funcList){
			s += "	OSTaskCreateExt(" + f + "_TASK, NULL,\n"+
					"			&" + f + "_STACK[" + f + "_STACKSIZE - 1],\n"+
					"			" + f + "_PRIORITY, " + f + "_PRIORITY,\n"+
					"			" + f + "_STACK, " + f + "_STACKSIZE, NULL,\n"+
					"			OS_TASK_OPT_STK_CLR);\n";
			s += "	OSTaskNameSet(" + f + "_PRIORITY, (INT8U *)\"" + f + "\", &perr);\n";

		}
		
		s += "\n\n";
		
		s += 	"	//Wait for confirmation that other cores have completed their startup routines\n"+
				"	//----------------------------------------------------------------------------\n"+
				"	while (!coresReady)\n"+
				"		;\n"+
				"\n"+
				"	//Start the OS\n"+
				"	//------------\n"+
				"	OSStart();\n"+
				"\n"+
				"	//Never get here\n"+
				"	//--------------\n"+
				"	return 0;\n"+
				"}\n"+
				"\n";
		
		return s;
	}


	private String getTaskRelocationString(Core core) {
		String s = "/*****************************************************************************\n"+
				" * Pointer relocation functions\n"+
				" *****************************************************************************/\n";
		
		for(Function f : fprintList){
			s += 	"void " + f + "UpdatePointers" + "(INT32U baseAddress, RT_MODEL_" + f + "_T *" + f + "_M){\n"+
					"	" + f + "_M->ModelData.defaultParam = (P_" + f + "_T *)(baseAddress + sizeof(" + f  + "Struct));\n";
			for(String dec : f.varDeclarations){
				if(dec.contains("DW")){
					s += "	" + f + "_M->ModelData.dwork = (DW_" + f + "_T *)(baseAddress + sizeof(" + f + "Struct) + sizeof(P_" + f + "_T));\n";
					break;
				}
			}
		
		s += "}\n\n";
		}
		
		return s;
	}

	
	protected String getStackDeclarationString(Core core) {
		String s = "";
		s += 	"/*****************************************************************************\n"+
				" * Stack Declarations\n"+
				" *****************************************************************************/\n";
		
		@SuppressWarnings("unchecked")
		ArrayList<Function> stackList = (ArrayList<Function>) core.funcList.clone();
		Collections.sort(stackList, Function.stackCompareDecreasing);
		
		for (Function f : stackList){
			s += "OS_STK " + f + "_STACK[" + f + "_STACKSIZE]";
			
		
			s += " __attribute__ ((section (\".critical\")))";
			s += ";\n";
		}
		
		return s;
	}
	
	protected String getSharedMemoryDeclarationString(Core core) {
		String s = "";
		s += 	"/*****************************************************************************\n"+
				" * Shared memory interface with other cores\n"+
				" *****************************************************************************/\n"+
				"SharedMemorySymbolTable shared_stab __attribute__ ((section (\".shared\")));\n"+
				"FunctionTable functionTable[" + fprintList.size() + "] __attribute__ ((section (\".shared\")));\n"+
				"CriticalFunctionData critFuncData[NUMCORES] __attribute__ ((section (\".shared\")));\n"+
				"\n";
		
		return s;
	}
	
	
	protected String getTaskFunctionString(Core core) {
		String s = "";
			
		for(Function f : core.funcList){	
			if(f.type == Type.periodic){
				s += "/*****************************************************************************\n"+
						" * " + f + "Task wrapper\n"+
						" *****************************************************************************/\n";

				s += "void " + f + "_TASK(void* pdata) {\n" +
					"\twhile (1) {\n";
				s += String.format("\t\t%s_step(%s_M, &%s_U,"
						+ "\n\t\t\t&%s_Y);\n", f,f,f,f);
				s+= "\t\tOSTimeDlyHMSM(0, 0, 0, " + f.period + ");\n"; 
				s += "\t}\n}\n";
			}
			s += "\n";
		}
		return s;
	}
	
	protected String getStackSizes(Core core) {
		String s = "";
		s += super.getStackSizes(core);
		for(Function f : fprintList){
			s += String.format("%-50s%s","#define " + f 
					+ "_STACKSIZE",String.format("(%4d%s )\n",f.stackSize/4, 
					" + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR")); 
		
		}
		
		return s;
	}

}
