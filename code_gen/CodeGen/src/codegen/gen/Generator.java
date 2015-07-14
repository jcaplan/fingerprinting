package codegen.gen;

import java.io.*;
import java.util.*;

import org.apache.commons.io.FileUtils;

import codegen.gen.Function.Type;
import codegen.prof.Profiler;

public class Generator {

	Configuration config;
	Platform platform;
	ArrayList<Function> funcList;
	ArrayList<Function> fprintList;
	ArrayList<StackBin> stackBins;
	public static final int MONITOR_INDEX = 2;

	public Generator(Configuration config) {
		this.config = config;
		this.funcList = config.funcList;
		platform = config.platform;
	}

	public void generateCores() throws IOException {

		initHeaders();
		getFprintList();
		getVariableDeclarations();
		getFunctionInitialization();
		if(config.stackProfilingRequired){
			getMaxStacks();
		}
		getStackBins();
		setStackBinAddresses();
		updateStackBinRegions();
		getVariableSizes();

	
		
		//static analysis complete
		for(Core core : platform.coreList){
			if(core.isMonitor){
				generateMonitor(core);
			} else {
				generateProcessoringCore(core);
			}
		}
		// figure out mem manager initialization
		// location of stack bin on core 1 = virtual address of stack bin on
		// core 0


		// ////////////////////////////////////////////////

		// figure out mpu initialization
		// stick with default for now

		// ////////////////////////////////////////////////

		// ////////////////////////////////////////////////

		// Need a start_hook for each periodic task
		// Easy just postDmaMessage(taskID, true)
		
		// ////////////////////////////////////////////////

		for (Core c : platform.coreList) {
			c.printIncludes();
			System.out.println("");
			c.printVarDeclarations();
			System.out.println("");
		}
		for (Function f : funcList) {
			f.printInitialization();
			System.out.println("");
		}

		System.out.println("\nStack results:");
		for (Function f : funcList) {
			System.out.println(f.name + " max stack: " + f.stackSize + " bytes");
		}

		/*
		 * the things that need to happen are: 1. get the functions that run on
		 * core 0
		 * 
		 * generate includes static model variable declarations for non critical
		 * tasks stacks (figure out stack sizes) semaphores linker regions and
		 * sections (num of critical stacks?) mem manager mpu init tasks
		 * interrupts main
		 */

	}

	private void generateProcessoringCore(Core core) throws IOException {
		// TODO Auto-generated method stub
		File outputDir = new File(config.outputDir + "/" + core.name);
		FileUtils.forceMkdir(outputDir);
		File srcOutput = new File(outputDir + "/" + core.name + ".c");
		File incOutput = new File(outputDir + "/" + core.name + ".h");
		
		
		
		
		
		PrintWriter writer = new PrintWriter(incOutput);
		writer.print(getHeaderFile(core));
		
		
		writer.close();
		
		writer = new PrintWriter(srcOutput);
		
		writer.print(getIncludeString(core)); /* print the includes */
		writer.print(getWhiteSpace(5));
		writer.print(getVarDecString(core));
		writer.print(getWhiteSpace(5));
		writer.print(getStackDeclarations(core));
		writer.print(getWhiteSpace(5));
		writer.print(getSemaphoreDeclarations(core));
		writer.print(getWhiteSpace(5));
		writer.print(getSharedMemoryDeclarations(core));
		writer.close();
		
		
	}

	




	private void generateMonitor(Core core) {
		// TODO Auto-generated method stub
		
	}

	private void updateMainMemorySize() {
		Core c = platform.getCore("cpu0");
		c.mainMemSize -= (stackBins.size() - 1) * StackBin.size;
		c = platform.getCore("cpu1");
		c.mainMemSize -= (stackBins.size()-1) * StackBin.size;
		
	}

	private void setStackBinAddresses() {
		updateMainMemorySize();
		for(int i = 0; i < stackBins.size(); i++){
			StackBin sb = stackBins.get(i);
			Core c = platform.getCore("cpu0");
			sb.startAddress[0] = c.mainMemStartAddress+c.mainMemSize - (i*StackBin.size);
			c = platform.getCore("cpu1");
			sb.startAddress[1] = c.mainMemStartAddress+c.mainMemSize - (i*StackBin.size);
		}
		
	}

	private void updateStackBinRegions() {
		int numBins = 0;
		if((numBins = stackBins.size()) > 1) { /* default num of stack bins */
			Core c = platform.getCore("cpu0");
			config.niosSBT.updateBspStackBins(numBins,c,0,stackBins);
			c = platform.getCore("cpu1");
			config.niosSBT.updateBspStackBins(numBins, c,1,stackBins);
		}
	}

	private void getFprintList() {
		fprintList = new ArrayList<>();
		for (Function f : funcList) {
			if (f.cores.size() > 1) {
				fprintList.add(f);
			}
		}
	}

	
	private void getVariableSizes() {
		// figure out the size of the data variables -> size of global_data
		// region (4 kB pages)

		// ////////////////////////////////////////////////
		for (Function f : funcList) {
			for (String dec : f.varDeclarations) {
				// TODO: parse files to get size of each struct

			}
		}

		// TODO: check that the sum of all functions is less than 4kB.

	}

	private void getStackBins() {
		// MIN_OFFSET = 1256
		// SAFETY = 80
		// MIN_OFFSET + SAFETY + f.stacksize > 2048
		// i.e. f.stacksize < 712
		// TODO : Need separate stack bins to ensure pages align for more than 2
		// cores

		// how do you organize the calculation/storage of these data structures?

		// first fit bin packing
		// sort functions by stack size
		// then stick them in bins

		// ////////////////////////////////////////////////

		
		Collections.sort(fprintList, Function.stackCompareDecreasing);
		if (fprintList.get(0).stackSize > 2760) {
			// TODO error
		}
		stackBins = new ArrayList<>();
		int binIndex = 0;
		int currentBinSize = 0;
		int currentBinIndex = 0;
		for (Function f : fprintList) {
			Function[] bin = null;
			if (currentBinIndex == 1 && (f.stackSize + currentBinSize < 1424)) {
				bin = stackBins.get(binIndex).getBin();
				bin[1] = f;
				f.stackBin = stackBins.get(binIndex);
				binIndex++;
				currentBinIndex = 0;
			} else {
				stackBins.add(new StackBin());
				if(currentBinIndex == 1){
					binIndex++;
				}
				bin = stackBins.get(binIndex).getBin();
				bin[0] = f;
				f.stackBin = stackBins.get(binIndex);
				currentBinSize = f.stackSize;
				currentBinIndex = 1;
			}
		}
		
		for(int i = 0; i < stackBins.size(); i++){
			StackBin sb = stackBins.get(i);
			sb.name = "stack_bin_" + i;
		}

	}

	private void getMaxStacks() throws IOException {
		for (Function f : funcList) {
			System.out.println("Starting profiling for " + f);
			String outputDir = config.outputDir + "/prof";
			copySourceFiles(f.codeDirectory, outputDir);
			Core cpum = platform.getCore("cpum");
			config.niosSBT.generateMakefile(outputDir, cpum.bspDir, "ucos.elf");
			config.niosSBT.updateMakefile(outputDir, f.name);
			config.niosSBT.makeProject(outputDir);

			Profiler prof = new Profiler(outputDir + "/ucos.objdump");
			String funcStep = f.name + "_step";
			prof.parseFile(funcStep);
			int maxStack = prof.getMaxStackSize(funcStep);
			f.stackSize = maxStack;
			deleteDirectory(outputDir);
		}

	}

	private void deleteDirectory(String name) {
		File f = new File(name);
		try {
			FileUtils.deleteDirectory(f);
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	private void copySourceFiles(String srcName, String destName) {

		File[] src = new File(srcName).listFiles();
		File dest = new File(destName);
		for (File file : src) {
			if (file.getName().endsWith(".h") || file.getName().endsWith(".c")) {
				try {
					FileUtils.copyFileToDirectory(file, dest);
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

	}

	private void getFunctionInitialization() throws IOException {
		for (Function f : funcList) {
			FileReader fr = new FileReader(f.codeDirectory + "/ert_main.c");
			BufferedReader reader = new BufferedReader(fr);
			String line = "";
			while (!(line = reader.readLine()).contains("int_T main")) {
				;
			}
			/*
			 * The relevant lines are all inside main and all start with the
			 * function name.
			 */
			while (!(line = reader.readLine()).contains("terminate")
					&& !line.contains("return")) {
				line = line.trim();
				if (line.startsWith(f.name)) {
					f.initialization.add(line);
				}
			}

			reader.close();
		}

	}

	private void getVariableDeclarations() throws IOException {
		for (Function f : funcList) {
			FileReader fr = new FileReader(f.codeDirectory + "/ert_main.c");
			BufferedReader reader = new BufferedReader(fr);
			String line = "";
			while ((line = reader.readLine()) != null) {
				String dec = "";
				line = line.trim();
				String[] tokens = line.split("[\\s]+");
				if (tokens[0].equals("static")) {
					String name = tokens[2];
					if (!name.equals("OverrunFlag")) { /*
														 * ignore the
														 * OverrunFlag variable
														 */
						dec += line;

						/* check that the line ends with a semicolon */
						if (!line.contains(";")) {
							while (!(line = reader.readLine()).contains(";")) {
								dec += line + "\n";
							}
							dec += line;
						}
						f.varDeclarations.add(dec);
					}
				}
			}

			reader.close();
		}

	}

	private void initHeaders() {

		Core c = platform.getCore("cpu0");

		c.addHeader("<stdio.h>");
		c.addHeader("<stddef.h>");
		c.addHeader("includes.h");
		c.addHeader("shared_mem.h");
		c.addHeader("fingerprint.h");
		c.addHeader("gp.h");
		c.addHeader("context_switch.h");
		c.addHeader("tlb.h");
		c.addHeader("critical.h");
		c.addHeader("mpu_utils.h");
		c.addHeader("priv/alt_exception_handler_registry.h");
		c.addHeader("mem_manager.h");
		c.addHeader("cpu0.h");
		c.addHeader("reset_monitor.h");

		c = platform.getCore("cpu1");
		c.addHeader("<stdio.h>");
		c.addHeader("<stddef.h>");
		c.addHeader("includes.h");
		c.addHeader("shared_mem.h");
		c.addHeader("fingerprint.h");
		c.addHeader("gp.h");
		c.addHeader("context_switch.h");
		c.addHeader("tlb.h");
		c.addHeader("critical.h");
		c.addHeader("mpu_utils.h");
		c.addHeader("priv/alt_exception_handler_registry.h");
		c.addHeader("mem_manager.h");
		c.addHeader("cpu1.h");
		c.addHeader("reset_monitor.h");

		c = platform.getCore("cpum");
		c.addHeader("<stdio.h>");
		c.addHeader("<stddef.h>");
		c.addHeader("<stdbool.h>");
		c.addHeader("includes.h");
		c.addHeader("sys/alt_dma.h");
		c.addHeader("<altera_avalon_mutex.h>");
		c.addHeader("shared_mem.h");
		c.addHeader("fingerprint.h");
		c.addHeader("gp.h");
		c.addHeader("context_switch.h");
		c.addHeader("critical.h");
		c.addHeader("mem_manager.h");
		c.addHeader("cpu1.h");
		c.addHeader("reset_monitor.h");
		c.addHeader("repos.h");

		findFunctionHeaders();

		for (Function f : funcList) {
			if (f.critical) {
				c = platform.getCore("cpum");
			} else {
				c = platform.getCore(f.cores.get(0));
			}
			c.addHeader(f + ".h");
		}
	}

	public void findFunctionHeaders() {
		for (Function f : funcList) {
			File file = new File(f.codeDirectory);
			File[] fileList = file.listFiles();
			for (File fil : fileList) {
				if (fil.getName().endsWith(".h")) {
					f.includeFiles.add(fil.getName());
				}
			}
		}
	}

	String getIncludeString(Core core) {
		String s = "";
		s += "/**********************************\n"+
			 " * Includes\n" +
			 " **********************************/\n";
		return s + core.printIncludes();
	}

	private String getVarDecString(Core core) {
		String s = "";
		s += "/**********************************\n"+
			 " * Global variable declarations\n" +
			 " **********************************/\n";
		return s + core.printVarDeclarations();
	}
	
	

	private String getHeaderFile(Core core) {
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
				String.format("%-50s%s\n","#define STACKSIZE_MINOFFSET","314")     				 +
				String.format("%-50s%s\n","#define STACKSIZE_MARGINERROR","20");
		
		for (Function f : core.funcList){
			s += String.format("%-50s%s","#define " + f 
					+ "_STACKSIZE",String.format("(%4d%s )\n",f.stackSize, 
					" + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR")); 
		}
		
		
		s += "/*****************************************************************************\n" +
			 " * Task Priorities\n" + 
			 " *****************************************************************************/\n";
		
		for (Function f : core.funcList){
			s += String.format("%-50s%d\n","#define " + f 
					+ "_PRIORITY",f.priority); 
		}
		
		return s;
	}


	private String getStackDeclarations(Core core) {
		String s = "";
		s += 	"/*****************************************************************************\n"+
				" * Stack Declarations\n"+
				" *****************************************************************************/\n";
		for (Function f : core.funcList){
			s += "OS_STK " + f + "_STACK[" + f + "_STACKSIZE];";
			
			if(f.critical){
				s += "__attribute__ ((section (\"." + f.stackBin.name + "\")))";
			}
			
			s += "\n";
		}
		
		return s;
	}
	

	private String getSemaphoreDeclarations(Core core) {
		String s = "";
		
		s += 	"/*****************************************************************************\n" +
				" * Control Flow declarations\n" + 
				" *****************************************************************************/\n";
		
		for(Function f : funcList){
			if(f.type == Type.eventDriven){
				//TODO
			}
		}
		return s;
	}
	

	private String getSharedMemoryDeclarations(Core core) {
		String s = "";
		s += 	"/*****************************************************************************\n"+
				" * Shared memory interface with monitor\n"+
				" *****************************************************************************/\n"+
				"FunctionTable *functionTable;\n"+
				"CriticalFunctionData *critFuncData;\n"+
				"SharedMemorySymbolTable *stab;\n";
		return s;
	}
	
	public String getCoreName(int core) {
		String name = "";
		if (core == 0) {
			name = "cpu0";
		} else if (core == 1) {
			name = "cpu1";
		} else if (core == 2) {
			name = "cpum";
		}
		return name;
	}
	
	public String getWhiteSpace(int numLines){
		String s = "";
		for(int i = 0; i < numLines; i++){
			s += "\n";
		}
		return s;
	}

}
