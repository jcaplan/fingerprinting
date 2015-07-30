package codegen.gen;

import java.io.*;
import java.util.*;

import org.apache.commons.io.FileUtils;

import codegen.gen.Function.Type;

public class GenCore {

	Configuration config;
	ArrayList<Function> fprintList;
	Platform platform;
	
	interface GenString {
		String action(Core core);
	}
	
	private GenString[] genStrings = new GenString[] {
		new GenString() { public String action(Core core) { return getIncludeStringString(core); } },
		new GenString() { public String action(Core core) { return getVarDecStringString(core); } },
		new GenString() { public String action(Core core) { return getStackDeclarationString(core); } },
		new GenString() { public String action(Core core) { return getSemaphoreDeclarations(core); } },
		new GenString() { public String action(Core core) { return getSharedMemoryDeclarationString(core); } },
		new GenString() { public String action(Core core) { return getCPUInterruptString(core); } },
		new GenString() { public String action(Core core) { return getTaskFunctionString(core); } },
		new GenString() { public String action(Core core) { return getMemoryManagerInitString(core); } },
		new GenString() { public String action(Core core) { return getMPUInitString(core); } },
		new GenString() { public String action(Core core) { return getMainString(core); } },
	};
	
	
	public GenCore(Configuration config, ArrayList<Function> fprintList, Platform platform){
		this.config = config;
		this.fprintList = fprintList;
		this.platform = platform;
	}
	

	void generateCore(Core core) throws IOException {
		
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
			s += String.format("%-50s%d\n","#define " + f 
					+ "_PRIORITY",f.priority); 
		}
		
		s += "/*****************************************************************************\n" +
			 " * Task Table indices\n" + 
			 " *****************************************************************************/\n";
			
			for (int i = 0; i < fprintList.size(); i++){
				Function f = fprintList.get(i);
				String upperCaseName = f.toString().toUpperCase();
				s += String.format("%-50s%d\n","#define " + upperCaseName 
						+ "_TABLE_INDEX",i); 
			}
			
			s += "/*****************************************************************************\n" +
					 " * Task Periods\n" + 
					 " *****************************************************************************/\n";
			for (Function f : fprintList){

				String upperCaseName = f.toString().toUpperCase();
				s += String.format("%-50s%d\n","#define " + upperCaseName + "_PERIOD", f.period); 
			}
		return s;
	}
	

	protected String getStackSizes(Core core) {
		String s = "";
		for (Function f : core.funcList){
			/* divide by 4: profile measures in bytes, c code in words */
			s += String.format("%-50s%s","#define " + f 
					+ "_STACKSIZE",String.format("(%4d%s )\n",f.stackSize/4, 
					" + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR")); 
		}
		
		return s;
	}


	String getIncludeStringString(Core core) {
		String s = "";
		s += "/**********************************\n"+
			 " * Includes\n" +
			 " **********************************/\n";
		return s + core.printIncludes();
	}
	
	protected String getVarDecStringString(Core core) {
		String s = "";
		s += "/**********************************\n"+
			 " * Global variable declarations\n" +
			 " **********************************/\n";
		return s + core.printVarDeclarations();
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
			
			if( f.critical){
				s += " __attribute__ ((section (\"." + f.stackBin.name + "\")))";
			}
			
			s += ";\n";
		}
		
		return s;
	}
	

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

	

	protected String getTaskFunctionString(Core core) {
		String s = "";
			
		for(Function f : core.funcList){
			if(f.type == Type.periodic){
				s += "/*****************************************************************************\n"+
						" * " + f + "Task wrapper\n"+
						" *****************************************************************************/\n";
				
				if(f.critical){
					String upperCaseName = f.toString().toUpperCase();
					s += "void " + f + "_TASK(void* pdata) {\n"+
							"	void *gp = stab->gp_address;\n"+
							"	int " + f + "_blocksize =\n"+
							"			functionTable[" + upperCaseName + "_TABLE_INDEX].blocksize;\n"+
							"	void (*"+ f + "Func)(int,\n"+
							"			" + f + "Struct*) = functionTable[" + upperCaseName + "_TABLE_INDEX].address;\n";
					int partner = 0;			
					if(core.index == 0){
						partner = 1;
					} 
					s +=	"	int partnerCore = " + partner + "; /* static variable */\n"+
							"\n";
					
					s += "	while (1) {\n"+
							"		INT8U perr;\n"+
							"		OSSemPend(critical_SEM[" + upperCaseName + "_TABLE_INDEX], 0, &perr);\n"+
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
							"		void *args = functionTable[" + upperCaseName + "_TABLE_INDEX].args;\n"+
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
							"	}\n" +
							"}\n";
					
					
				} else {			
						s += "void " + f + "_TASK(void* pdata) {\n" +
							"\twhile (1) {\n";
						s += String.format("\t\t%s_step(%s_M, &%s_U,"
								+ "\n\t\t\t&%s_Y);\n", f,f,f,f);
						s+= "\t\tOSTimeDlyHMSM(0, 0, 0, " + f.period + ");\n"; 
						s += "\t}\n}\n";
					
				}
				s += "\n";
			}
		}
		return s;
	}



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
			String upperCaseName = f.toString().toUpperCase();
			s += "	// " + f + "\n" +
			"	entry = &memoryManagerTable[" + upperCaseName + "_TABLE_INDEX];\n"+
			"	entry->disablePending = false;\n"+
			"	entry->disablePendSource = 0;\n"+
			"	entry->taskPriority = " + f + "_PRIORITY;\n"+
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
						"	region[1].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;\n";
				
				
				
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
	


	private String getMainString(Core core) {
		String s = "";
		s += "/*****************************************************************************\n"+
			" * Main\n"+
			" *****************************************************************************/\n"+
			"\n"+
			"int main() {\n"+
			"	printf(\"starting core %d\", CORE_ID);\n"+
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

				String upperCaseName = f.toString().toUpperCase();
				s += "	functionTable[" + upperCaseName + 
						"_TABLE_INDEX].stackAddress[CORE_ID] = &" +
						f + "_STACK;\n" + 
						"	functionTable[" + upperCaseName + 
						"_TABLE_INDEX].address = " + f + "_CT;\n";
						;
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
				"	mem_manager_init();\n";
		
		s += 	"	// Declare the OS tasks\n"+
				"	// -------------------\n\n"+
				"	INT8U perr;";
		
		for(Function f : core.funcList){
			if(!f.critical){
				s += "	OSTaskCreateExt(" + f + "_TASK, NULL,\n"+
						"			&" + f + "_STACK[" + f + "_STACKSIZE - 1],\n"+
						"			" + f + "_PRIORITY, " + f + "_PRIORITY,\n"+
						"			" + f + "_STACK, " + f + "_STACKSIZE, NULL,\n"+
						"			OS_TASK_OPT_STK_CLR);\n";
			} else { /* critical tasks on core 0 */
				s += "	OSTaskCreateExt(" + f + "_TASK, NULL,\n"+
						"			(OS_STK *)0x"+ Integer.toHexString(getStackEnd(f,1) & 0x3FFFFF) +",\n"+
						"			" + f + "_PRIORITY, " + f + "_PRIORITY,\n"+
						"			(OS_STK *)0x" + Integer.toHexString(getStackStart(f,1) & 0x3FFFFF) +", " + f + "_STACKSIZE, NULL,\n"+
						"			OS_TASK_OPT_STK_CLR);\n";
			}
			
			s += "	OSTaskNameSet(" + f + "_PRIORITY, (INT8U *)\"" + f + "\", &perr);\n";
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
	
	public String getWhiteSpace(int numLines){
		String s = "";
		for(int i = 0; i < numLines; i++){
			s += "\n";
		}
		return s;
	}

	
	protected int getStackStart(Function f, int coreID) {
		return platform.mainMemoryBase + f.stackBin.startAddress[coreID] +  f.getStackStart(coreID);
	}

	private int getStackEnd(Function f, int coreID) {
		 return platform.mainMemoryBase + f.stackBin.startAddress[coreID] + f.getStackEnd(coreID);
	}

	

}