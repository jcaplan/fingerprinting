package codegen.gen;

import java.io.*;
import java.util.*;

import org.apache.commons.io.FileUtils;

import codegen.gen.Function.Type;

/**
 * Class for generating the processing core code cpuX.c and cpuX.h
 * @author jonah
 *
 */
public class GenCore {

	Configuration config;
	ArrayList<Function> fprintList;
	Platform platform;
	
	/**
	 * Action interface for generation of different sections
	 */
	interface GenString {
		String action(Core core);
	}
	
	/**
	 * Array of actions taken to generate file.
	 */
	private GenString[] genStrings = new GenString[] {
		new GenString() { public String action(Core core) { return getIncludeStringString(core); } },
		new GenString() { public String action(Core core) { return getVarDecStringString(core); } },
		new GenString() { public String action(Core core) { return getStackDeclarationString(core); } },
		new GenString() { public String action(Core core) { return getRuntimeMonitorTableString(core); } },
		new GenString() { public String action(Core core) { return getSemaphoreDeclarations(core); } },
		new GenString() { public String action(Core core) { return getSharedMemoryDeclarationString(core); } },
		new GenString() { public String action(Core core) { return getCPUInterruptString(core); } },
		new GenString() { public String action(Core core) { return getTaskFunctionString(core); } },
		new GenString() { public String action(Core core) { return getMemoryManagerInitString(core); } },
		new GenString() { public String action(Core core) { return getMPUInitString(core); } },
		new GenString() { public String action(Core core) { return getMainString(core); } },
	};
	
	/**
	 * Constructor
	 * @param config
	 * @param fprintList
	 * @param platform
	 */
	public GenCore(Configuration config, ArrayList<Function> fprintList, Platform platform){
		this.config = config;
		this.fprintList = fprintList;
		this.platform = platform;
	}
	
	/**
	 * Generate the files cpuX.c and cpuX.h
	 * @param core
	 * @throws IOException
	 */
	protected void generateCore(Core core) throws IOException {
		
		File outputDir = new File(config.outputDir + "/" + core.name);
		FileUtils.forceMkdir(outputDir);
		File srcOutput = new File(outputDir + "/" + core.name + ".c");
		File incOutput = new File(outputDir + "/" + core.name + ".h");
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
	
	

	/**
	 * 
	 * @param core
	 * @return String for cpuX.h
	 */
	protected String getHeaderFileString(Core core) {
		String s = "";
		s += 	"#include \"shared_mem.h\"\n"+
				"/*****************************************************************************\n"+
				" * Defines\n"+
				" *****************************************************************************/\n"+
				"#define CORE_ID 			(ALT_CPU_CPU_ID_VALUE - 1)\n"+
				"/*****************************************************************************\n"+
				" * Stack sizes\n"+
				" *\n"+
				" * The stack size is composed of three elements A + B + C:\n"+
				" * A: Amount of room that function code requires to execute on stack\n"+
				" *\n"+
				" * B: empirically determined value for amount of stack used with function\n"+
				" * body consisting of empty while loop\n"+
				" *\n"+
				" * C: Safety margin\n"+
				" *\n"+
				" * Note that OS_STK allocates 32 bit words so these numbers represent words\n"+
				" *****************************************************************************/\n" + 
				String.format("%-50s%d\n","#define STACKSIZE_MINOFFSET",StackBin.STACKSIZE_MINOFFSET/4)     				 +
				String.format("%-50s%d\n","#define STACKSIZE_MARGINERROR",StackBin.STACKSIZE_MARGINERROR/4);
		
		s += getStackSizes(core);
		
		
		s += "/*****************************************************************************\n" +
			 " * Task Priorities\n" + 
			 " *****************************************************************************/\n";
		
		for (Function f : core.funcList){
			s += String.format("%-50s%d\n","#define " + f.getPriorityString(),f.priority); 
		}
		
		s += "/*****************************************************************************\n" +
			 " * Task Table indices\n" + 
			 " *****************************************************************************/\n";
			
			for (int i = 0; i < fprintList.size(); i++){
				Function f = fprintList.get(i);
				s += String.format("%-50s%d\n","#define " + f.getTableIndexString(),i); 
			}
			
			s += "/*****************************************************************************\n" +
					 " * Task Periods\n" + 
					 " *****************************************************************************/\n";
			ArrayList<Function> mergeList = getMergedFuncFprintList(core);
			
			s += String.format("%-50s%d\n","#define NUM_TASKS", mergeList.size()); 			
			for (Function f : mergeList){
				s += String.format("%-50s%d\n","#define " + f.getPeriodString(), f.period); 
			}
			s += "/*****************************************************************************\n" +
					 " * Runtime monitor table index\n" + 
					 " *****************************************************************************/\n";
		
			for (int i = 0; i < mergeList.size(); i++){
				Function f = mergeList.get(i);
				s += String.format("%-50s%d\n","#define " + f.getRuntimeIndexString(), i); 
			}
			
		return s;
	}
	

	private ArrayList<Function> getMergedFuncFprintList(Core core) {

		@SuppressWarnings("unchecked")
		ArrayList<Function> mergeList = (ArrayList<Function>) core.funcList.clone();
		for (Function f : fprintList){
			if (!mergeList.contains(f)){
				mergeList.add(f);
			}
		}
		
		return mergeList;
	}

	/**
	 * 
	 * @param core
	 * @return The string for the stack size macros
	 */
	protected String getStackSizes(Core core) {
		String s = "";
		for (Function f : core.funcList){
			/* divide by 4: profile measures in bytes, c code in words */
			s += String.format("%-50s%s","#define " + f.getStackSizeString(),
					String.format("(%4d%s )\n",f.stackSize/4, 
					" + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR")); 
		}
		
		return s;
	}

	/**
	 * 
	 * @param core
	 * @return The string for the header file includes
	 */
	protected String getIncludeStringString(Core core) {
		String s = "";
		s += "/**********************************\n"+
			 " * Includes\n" +
			 " **********************************/\n";
		return s + core.getIncludesString();
	}
	
	/**
	 * 
	 * @param core
	 * @return The string for the global variable declarations for the Simulink functions.
	 */
	protected String getVarDecStringString(Core core) {
		String s = "";
		s += "/**********************************\n"+
			 " * Global variable declarations\n" +
			 " **********************************/\n";
		return s + core.printVarDeclarations();
	}
	
	/**
	 * 
	 * @param core
	 * @return String for the stack declarations for each task
	 */
	protected String getStackDeclarationString(Core core) {
		String s = "";
		s += 	"/*****************************************************************************\n"+
				" * Stack Declarations\n"+
				" *****************************************************************************/\n";
		
		@SuppressWarnings("unchecked")
		ArrayList<Function> stackList = (ArrayList<Function>) core.funcList.clone();
		Collections.sort(stackList, Function.stackCompareDecreasing);
		
		for (Function f : stackList){
			s += "OS_STK " + f + "_STACK[" + f.getStackSizeString() + "]";
			
			if( f.critical){
				s += " __attribute__ ((section (\"." + f.stackBin.name + "\")))";
			}
			
			s += ";\n";
		}
		
		return s;
	}
	
	


	private String getRuntimeMonitorTableString(Core core) {
		String s = "";
		s += "/*****************************************************************************\n" + 
				" * Execution time monitoring table\n" + 
				" *****************************************************************************/\n" + 
				"rtMonitor_task rtMonTaskTable[NUM_TASKS] = {\n";
		
		
		for(Function f : getMergedFuncFprintList(core)){
			s += "	{ " + f.getPriorityString() + ", 0, " + f.getPeriodString() + ", false, " +
						(f.critical ? "true" : "false") + ", \"" + f.name + "\" },\n";  
		}
		
		s = s.substring(0, s.length() - 2); /* remove last comma and new line */
		
		s += "\n};\n";
		
		return s;	
	} 
	
	/**
	 * 
	 * @param core
	 * @return The string for the semaphore array for each critical task
	 */
	String getSemaphoreDeclarations(Core core) {
		String s = "";
		String s2 = "";
		s += 	"/*****************************************************************************\n" +
				" * Control Flow declarations\n" + 
				" *****************************************************************************/\n";

		int count = 0;
		for(Function f : core.funcList){
			if(f.type == Type.eventDriven){
				//TODO
			} else if(f.critical) {
				count++;
			}
		}
		s2 += "OS_EVENT *critical_SEM[" + count+"];\n";
		
		
		

		return s + s2;
	}
	
	/**
	 * @param core
	 * @return Declaration of the shared memory pointers sent by the monitor.
	 */
	protected String getSharedMemoryDeclarationString(Core core) {
		String s = "";
		s += 	"/*****************************************************************************\n"+
				" * Shared memory interface with monitor\n"+
				" *****************************************************************************/\n"+
				"FunctionTable *functionTable;\n"+
				"CriticalFunctionData *critFuncData;\n"+
				"SharedMemorySymbolTable *stab;\n";
		return s;
	}
	
	/**
	 * 
	 * @param core
	 * @return The string for the interrupt handler for beginning critical tasks
	 */
	private String getCPUInterruptString(Core core) {
		String s = "";

		String cpuName = "PROCESSOR" + core.index;
		s += "/*****************************************************************************\n" +
		" * Interrupt from other cores\n" + 
		" *****************************************************************************/\n" +
		"static void handleCPU(void* context) {\n" + 
		"	int taskIndex = critFuncData[CORE_ID].tableIndex;\n"+
		"	updateMemoryManagerTable(taskIndex,\n"+
		"			&critFuncData[CORE_ID]);\n"+
		"\n"+
		"	int *" + cpuName + "_IRQ = (int *) " + cpuName + "_0_CPU_IRQ_0_BASE;\n"+
		"	*" + cpuName + "_IRQ = 0;\n"+
		"	OSSemPost(critical_SEM[taskIndex]);\n" +
		"}\n\n";
		
		s += "static void initCpuIsr(void) {\n"+
				"	alt_ic_isr_register(" + cpuName + "_0_CPU_IRQ_0_IRQ_INTERRUPT_CONTROLLER_ID,\n"+
				"			" + cpuName + "_0_CPU_IRQ_0_IRQ, handleCPU, (void*) NULL, (void*) NULL);\n"+
				"}\n\n";
		
		
		s += "void waitForPartnerCore(int partnerCore) {\n"+
				"	//Synchronize with partner\n"+
				"	//------------------------\n"+
				"	int done = 0, first = 0;\n"+
				"	while (done == 0) {\n"+
				"		if (first == 0) {\n"+
				"			critFuncData[CORE_ID].checkout = 1;\n"+
				"			first = 1;\n"+
				"		}\n"+
				"		if (critFuncData[partnerCore].checkout == 1) {\n"+
				"			critFuncData[partnerCore].checkout = 0;\n"+
				"			done = 1;\n"+
				"		}\n"+
				"	}\n"+
				"}\n";
		return s;
	}

	
	/**
	 * 
	 * @param core
	 * @return String for all function task wrappers. 
	 */
	protected String getTaskFunctionString(Core core) {
		String s = "";
			
		for(Function f : core.funcList){
			if(f.type == Type.periodic){
				s += "/*****************************************************************************\n"+
						" * " + f + "Task wrapper\n"+
						" *****************************************************************************/\n";
				
				if(f.critical){
					s += "void " + f + "_TASK(void* pdata) {\n"+
							"	void *gp = stab->gp_address;\n"+
							"	void (*"+ f + "Func)(int,\n"+
							"			" + f + "Struct*) = functionTable[" + f.getTableIndexString() + "].address;\n";
					int partner = 0;			
					if(core.index == 0){
						partner = 1;
					} 
					s +=	"	int partnerCore = " + partner + "; /* static variable */\n"+
							"\n";
					
					s += "	while (1) {\n"+
							"		INT8U perr;\n"+
							"		OSSemPend(critical_SEM[" + f.getTableIndexString() + "], 0, &perr);\n"+
							"\n"+
							"		rtMonitorStartTask(" + f.getRuntimeIndexString() + ");\n"+
							"\n"+
							"		waitForPartnerCore(partnerCore);\n"+
							"		//Context switch is necessary to clear the callee saved registers\n"+
							"		long registers[8];\n"+
							"		context_switch(registers);\n"+
							"\n"+
							"		//Do the derivative part\n"+
							"\n"+
							"		int priority = critFuncData->priority;\n"+
							"\n"+
							"		//set the flag for the OS context switch\n"+
							"		FprintActive = 1;\n"+
							"		FprintTaskIDCurrent = priority;\n"+
							"\n"+
							"		//Retrieve the arguments before changing the GP\n"+
							"\n"+
							"		void *args = functionTable[" + f.getTableIndexString() + "].args;\n"+
							"		//Set the global pointer in case of compilation issues related\n"+
							"		//to global variables\n"+
							"		set_gp(gp);\n"+
							"\n"+
							"		" + f + "Func(priority, args);\n"+
							"		//call the critical task\n"+
							"		//restore the original global pointer\n"+
							"		restore_gp();\n"+
							"\n"+
							"		//set the flag for the OS context switch\n"+
							"		FprintActive = 0;\n"+
							"\n"+
							"\n"+
							"		rtMonitorEndTask(" + f.getRuntimeIndexString() + ");\n"+
							"	}\n" +
							"}\n";
					
					
				} else {			
						s += "void " + f + "_TASK(void* pdata) {\n";
						
						if(f.preambleFileName != null){
							s += f.getPreambleString();
						}
						s += "\n";
						
						s +=	"\twhile (1) {\n";
						

						s+= "		rtMonitorStartTask(" + f.getRuntimeIndexString() + ");\n";
						
						if(f.printRuntimes){
							s += "		INT32U time = OSTimeGet();\n"; 
									
						}
						
						s += String.format("\t\t%s_step(%s_M, &%s_U,"
								+ "\n\t\t\t&%s_Y);\n", f,f,f,f);
						
						if(f.printRuntimes){
							s += 
								"		time = OSTimeGet() - time;\n" + 
								"		printf(\"runtime task %s: %u\\n\",rtMonitorGetTaskName(" + f.getRuntimeIndexString() +  "),time);\n";
						}
						

						s+= "		rtMonitorEndTask(" + f.getRuntimeIndexString() + ");\n";
						
						
						s+= "\t\tOSTimeDlyHMSM(0, 0, 0, " + f.getPeriodString() + ");\n"; 
						s += "\t}\n}\n";
					
				}
				s += "\n";
			}
		}
		return s;
	}


	/**
	 * 
	 * @param core
	 * @return String for function to initialize memory manager.
	 */
	private String getMemoryManagerInitString(Core core) {
		String s = "";
		s += "/*****************************************************************************\n"+
				" * Memory Manager \n"+
				" *****************************************************************************/\n"+
				"void mem_manager_init(void) {\n";
		s += "// For each critical task set up a position in the table\n"+
				"	MemoryManagerStruct *entry;\n"+
				"\n";
		int tlbDataLine = 0;
		int tlbStackLine = 1;
		for(Function f : core.funcList){
			if(f.critical){
			s += "	// " + f + "\n" +
			"	entry = &memoryManagerTable[" + f.getTableIndexString() + "];\n"+
			"	entry->disablePending = false;\n"+
			"	entry->disablePendSource = 0;\n"+
			"	entry->taskPriority = " + f.getPriorityString() +";\n"+
			"	entry->tlbDataLine = " + tlbDataLine + ";\n"+
			"	entry->tlbStackLine = " + tlbStackLine + ";\n"+
			"	entry->stackPhysicalAddress = (void*)0x" +
			Integer.toHexString(f.stackBin.startAddress[core.index] + platform.mainMemoryBase) + ";\n"+
			"	entry->stackVirtualAddress = (void*)0x";
			

				s += Integer.toHexString(f.stackBin.startAddress[1]);

			s +=";\n"+
			"	entry->dataVirtualAddress = 0; /*get from monitor at interrupt time*/\n"+
			"	entry->dataPhysicalAddress = 0; /*get from monitor at interrupt time*/\n"+
			"\n"+
			"	managerEnableTask(entry);\n";
			
			tlbDataLine += 2;
			tlbStackLine += 2;
			}
		}
		s += "}\n";
		return s;
	}

	

	/**
	 * 
	 * @param core
	 * @return String to initialize default values for MPU data and inst regions.
	 */
	private String getMPUInitString(Core core) {
		String s = "";
		s += "/*****************************************************************************\n"+
				" * MPU Initialization \n"+
				" *****************************************************************************/\n";
		
		s += "alt_exception_result handleMPUException(alt_exception_cause cause,\n"+
				"		alt_u32 exception_pc, alt_u32 badaddr) {\n"+
				"	//TODO: Notify monitor to reset core immediately!!\n"+
				"	int *coreM_IRQ = (int *) PROCESSORM_0_CPU_IRQ_0_BASE;\n"+
				"	if (FprintActive) {\n"+
				"		fprint_disable_task(FprintTaskIDCurrent);\n"+
				"	}\n"+
				"	*coreM_IRQ = 1;\n"+
				"	while (1)\n"+
				"		;\n"+
				"	return 0;\n"+
				"}\n"+
				"\n";
		
		s += "void nios2_mpu_inst_init() {\n"+
				"\n"+
				"	Nios2MPURegion region[NIOS2_MPU_NUM_INST_REGIONS];\n"+
				"\n"+
				"	//catch null pointers\n"+
				"	region[0].index = 0x0;\n"+
				"	region[0].base = 0x0;\n"+
				"	region[0].mask = 0x1000 / 64;\n"+
				"	region[0].c = 0;\n"+
				"	region[0].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;\n"+
				"\n"+
				"\n"+
				"	int index;\n"+
				"	for (index = 1; index < NIOS2_MPU_NUM_INST_REGIONS; index++) {\n"+
				"		region[index].base = 0x0;\n"+
				"		region[index].index = index;\n"+
				"		region[index].mask = 0x2000000;\n"+
				"		region[index].c = 0;\n"+
				"		region[index].perm = MPU_INST_PERM_SUPER_EXEC_USER_EXEC; //No access for user and supervisor\n"+
				"	}\n"+
				"\n"+
				"	nios2_mpu_load_region(region, NIOS2_MPU_NUM_INST_REGIONS, INST_REGION);\n"+
				"}\n";
		
		//TODO: hardcoding data region init for now, needs fixing!!!
		
		s += "void nios2_mpu_data_init() {\n"+
				"	//Data region is scratchpads + this core's main memory region.\n"+
				"	Nios2MPURegion region[NIOS2_MPU_NUM_DATA_REGIONS];\n"+
				"	//monitor core - global_data\n"+
				"	region[0].index = 0x0;\n"+
				"	region[0].base = MEMORY_0_ONCHIP_MEMORYMAIN_BEFORE_RESET_REGION_BASE / 64;\n"+
				"	region[0].mask = (0x432000) / 64;\n"+
				"	region[0].c = 0;\n"+
				"	region[0].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;\n"+
				"\n"+
				"	//other core's region - stack\n" +
				"	region[1].index = 0x1;\n";
		
				int otherCoreIndex = 0;
				if(core.index == 0){
					otherCoreIndex = 1;
				}

//				StackBin lastStackBin =  stackBins.get(stackBins.size()-1);
				Core otherCore = platform.getCore("cpu" + otherCoreIndex);
				s += "	region[1].base = 0x" + Integer.toHexString(platform.mainMemoryBase + otherCore.mainMemStartAddressOffset & 0xFFFFF000) + "/ 64;\n"+
//						"	region[1].mask = 0x" + Integer.toHexString(platform.mainMemoryBase + lastStackBin.startAddress[otherCoreIndex]) + "/ 64;\n"+
						"	region[1].mask = 0x" + Integer.toHexString(platform.mainMemoryBase + 0x32000 + otherCore.mainMemStartAddressOffset & 0xFFFFF000) + "/ 64;\n"+
						"	region[1].c = 0;\n"+
						"	region[1].perm = MPU_DATA_PERM_SUPER_RD_USER_RD;\n";
				
				
				
				s += "\n"+
				"	//catch null pointers\n"+
				"	region[2].index = 0x2;\n"+
				"	region[2].base = 0x0;\n"+
				"	region[2].mask = 0x31000 / 64;\n"+
				"	region[2].c = 0;\n"+
				"	region[2].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;\n"+
				"\n"+
				
				"	//no scratchpad physical address\n" + 
						"	region[3].index = 0x3;\n" + 
						"	region[3].base = 0x4200000 / 64;\n" + 
						"	region[3].mask = 0x4208000 / 64;\n" + 
						"	region[3].c = 0;\n" + 
						"	region[3].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;\n\n"+
				
						
				"	int index;\n"+
				"	for (index = 4; index < NIOS2_MPU_NUM_DATA_REGIONS; index++) {\n"+
				"		region[index].base = 0x0;\n"+
				"		region[index].index = index;\n"+
				"		region[index].mask = 0x2000000;\n"+
				"		region[index].c = 0;\n"+
				"		region[index].perm = MPU_DATA_PERM_SUPER_RW_USER_RW; //No access for user and supervisor\n"+
				"	}\n"+
				"\n"+
				"	nios2_mpu_load_region(region, NIOS2_MPU_NUM_DATA_REGIONS, DATA_REGION);\n"+
				"}\n";
		
		return s;
	}
	

	/**
	 * 
	 * @param core
	 * @return Main function string
	 */
	protected String getMainString(Core core) {
		String s = "";
		s += "/*****************************************************************************\n"+
			" * Main\n"+
			" *****************************************************************************/\n"+
			"\n"+
			"int main() {\n"+
			"	printf(\"starting core %d\\n\", CORE_ID);\n"+
			"\n"+
			"	//Initialize interrupts\n"+
			"	//---------------------\n"+
			"	initCpuIsr();\n"+
			"\n"+
			"	//Initialize OS Context switch flag\n"+
			"	FprintActive = 0;\n"+
			"\n"+
			"	//Symbol table is at known location\n"+
			"	//---------------------------------\n"+
			"	stab = (SharedMemorySymbolTable*) SHARED_MEMORY_BASE;\n"+
			"\n"+
			"	//Wait until the monitor has placed data into shared memory\n"+
			"	//---------------------------------------------------------\n"+
			"	while (stab->init_complete == 0)\n"+
			"		;\n"+
			"\n"+
			"	functionTable = stab->ftable;\n"+
			"	critFuncData = stab->critFuncData;\n"+
			"\n"+
			"	//Put the location of the stack for the task in shared memory\n"+
			"	//-----------------------------------------------------------\n";

		for(Function f : core.funcList){
			if(f.critical){

				s += "	functionTable[" + f.getTableIndexString() + 
						"].stackAddress[CORE_ID] = &" +
						f + "_STACK;\n";
				if(f.cores.get(0).equals(core.name)){
					s += "	functionTable[" + f.getTableIndexString() + 
						"].address = " + f + "_CT;\n";
						;
				}
			}
		}
		s += "\n";
		
		s += core.printInitializations(1);

		for(int i = 0; i < fprintList.size(); i++){
			s += "	critical_SEM[" + i + "] = OSSemCreate(0);\n";
		}
		
		s += "	//Start up the MPU\n"+
				"	//----------------\n"+
				"\n"+
				"	// Register exception handler.\n"+
				"	alt_instruction_exception_register(&handleMPUException);\n"+
				"	// Initialize and start the MPU.\n"+
				"	nios2_mpu_data_init();\n"+
				"	nios2_mpu_inst_init();\n"+
				"	nios2_mpu_enable();\n"+
				"\n"+
				"	//Start up the software memory manager\n"+
				"	mem_manager_init();\n\n";
		
		s += 	"	// Initialize executime time monitor\n" + 
				"	// ---------------------------------\n" + 
				"	rtMonitorInit(&rtMonTaskTable[0],NUM_TASKS);\n\n";
		
		s += 	"	// Declare the OS tasks\n"+
				"	// -------------------\n\n"+
				"	INT8U perr;";
		
		for(Function f : core.funcList){
			if(f.critical){
				/* task stacks are given virtual pointers with the MSB set to 0 (432000 -> 32000) */
				s += "	OSTaskCreateExt(" + f + "_TASK, NULL,\n"+
						"			(OS_STK *)0x"+ Integer.toHexString(getStackEnd(f,1) & 0x3FFFFF) +",\n"+
						"			" + f.getPriorityString() + ", " + f.getPriorityString() + ",\n"+
						"			(OS_STK *)0x" + Integer.toHexString(getStackStart(f,1) & 0x3FFFFF) +", " 
						+ f.getStackSizeString() + ", NULL,\n"+
						"			OS_TASK_OPT_STK_CLR);\n";
				
			} else { 
				s += "	OSTaskCreateExt(" + f + "_TASK, NULL,\n"+
						"			&" + f + "_STACK[" + f.getStackSizeString() + " - 1],\n"+
						"			" + f.getPriorityString() + ", " + f.getPriorityString() + ",\n"+
						"			" + f + "_STACK, " + f.getStackSizeString() + ", NULL,\n"+
						"			OS_TASK_OPT_STK_CLR);\n";
			}
			
			s += "	OSTaskNameSet(" + f.getPriorityString() + ", (INT8U *)\"" + f + "\", &perr);\n";
		}
		
		s += "\n";
		
		s += "	resetMonitorCoreReg(CORE_ID);\n"+
				"\n"+
				"	OSStart();\n"+
				"	return 0;\n"+
				"}\n"+
				"\n";
		return s;
	}
	
	/*************************************************************
	 * HELPERS
	 *************************************************************/
	
	/**
	 * 
	 * @param numLines
	 * @return String with desired number of blank new lines
	 */
	public String getWhiteSpace(int numLines){
		String s = "";
		for(int i = 0; i < numLines; i++){
			s += "\n";
		}
		return s;
	}

	/**
	 * Get the start of the stack for a function on a core
	 * @param f
	 * @param coreID
	 * @return address of start of function stack
	 */
	protected int getStackStart(Function f, int coreID) {
		return platform.mainMemoryBase + f.stackBin.startAddress[coreID] +  f.getStackStart(coreID);
	}

	/**
	 * Get the end of a stack for a function on a core
	 * @param f
	 * @param coreID
	 * @return address of end of function stack.
	 */
	private int getStackEnd(Function f, int coreID) {
		 return platform.mainMemoryBase + f.stackBin.startAddress[coreID] + f.getStackEnd(coreID);
	}

	

}
