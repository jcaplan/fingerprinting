package codegen.gen;

import java.io.*;
import java.util.*;

import org.apache.commons.io.FileUtils;

import codegen.gen.Function.Type;
import codegen.prof.Profiler;
import codegen.test.CriticalityTest;

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


		getFprintList();
		
		
		//Generate necessary scripts and SBT files
		if(config.generateBSPRequired()){
			generateBSPs();
		}
		generateCriticalLibrary();
		generateAppScript();
		generateRunScript();
		generateCompileBSPScript();
		//Parse source
		initHeaders();
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
		//Generate individual core code
		for(Core core : platform.coreList){
			if(core.isMonitor){
				generateMonitor(core);
			} else {
				generateProcessoringCore(core);
			}
		}
		
		File bsp = new File(config.outputDir + "/cpuM_bsp");
		if(!bsp.exists()){
			config.niosSBT.runCommand(new String[]{config.outputDir+"/compile_bsp.sh"});
		}
		config.niosSBT.runCommand(new String[]{config.outputDir+"/create_app.sh"});
		
	}



	

	private void generateCriticalLibrary() throws FileNotFoundException {
		File critDir = new File(config.outputDir + "/critical_library");
		if(!critDir.exists()){
			copyCriticalLibrary();
		}
		
		File criticalSrcOutput = new File(config.outputDir + "/critical_library/critical.c");
		File criticalIncOutput = new File(config.outputDir + "/critical_library/critical.h");
		

		
		PrintWriter writer = new PrintWriter(criticalIncOutput);
		writer.print(getCriticalIncludeString());
		writer.close();
		
		writer = new PrintWriter(criticalSrcOutput);
		writer.print(getCriticalSourceString());
		writer.close();
		
		File scriptOutput = new File(config.outputDir + "/generate_lib.sh");
		writer = new PrintWriter(scriptOutput);
		writer.print(generateCriticalLibraryString());
		writer.close();
		
		config.niosSBT.generateCriticalLibrary(config.outputDir + "/critical_library", platform.getCore("cpum").bspDir);
	}
	
	public String generateCriticalLibraryString() {
		
		
		String cmd = "";
		cmd += "#!/bin/bash\n"+
				"\n"+
				"OUTPUT_DIR=" + config.outputDir + "\n"+
				"NIOS2COMMANDSHELL=" + config.niosSBT.sbtLocation + "\n" +
				"LIBDIR=${OUTPUT_DIR}/critical_library\n"+
				"\n"+
				"${NIOS2COMMANDSHELL} nios2-lib-generate-makefile --lib-dir ${LIBDIR} \n"+
				"--lib-name \"critical_library\" --bps-dir ${OUTPUT_DIR}/cpuM_bsp --src-dir $ {LIBDIR}\n";

				
		return cmd;
	}

	private void generateAppScript() throws FileNotFoundException {
		PrintWriter writer;
		File output = new File(config.outputDir + "/create_app.sh");		
		writer = new PrintWriter(output);
		writer.print(getAppScriptString());
		writer.close();

		output.setExecutable(true);
		output = new File(config.outputDir + "/compile_app.sh");
		writer = new PrintWriter(output);
		writer.print(getCompileAppScript());
		
		writer.close();

		output.setExecutable(true);
		
	}

	
	private void generateCompileBSPScript() throws IOException{
		String s = "#!/bin/bash\n"+
				"OUTPUT_DIR="+ config.outputDir + "\n"+
				"NIOS2COMMANDSHELL=" + config.niosSBT.sbtLocation + "\n"+
				"\n"+
				"\n"+
				"for i in {0..1} M\n"+
				"do\n"+
				"	bsp_dir=${OUTPUT_DIR}/cpu${i}_bsp\n"+
				"    ${NIOS2COMMANDSHELL} nios2-bsp-generate-files --settings=${bsp_dir}/settings.bsp --bsp-dir=${bsp_dir}\n"+
				"\n"+
				"done\n"+
				"\n"+
				"source ${OUTPUT_DIR}/update_bsps.sh\n"+
				"\n";
		
		PrintWriter writer;
		File output = new File(config.outputDir + "/compile_bsp.sh");		
		writer = new PrintWriter(output);
		writer.print(s);
		writer.close();
		output.setExecutable(true);
	}
	
	private void generateRunScript() throws FileNotFoundException {
		PrintWriter writer;
		File output = new File(config.outputDir + "/RUN.sh");
		writer = new PrintWriter(output);
		writer.print(getRunScriptString());
		
		writer.close();

		output.setExecutable(true);
	}

	private void generateBSPs() throws IOException {

		File output;
		PrintWriter writer;

		FileUtils.forceMkdir(new File(config.outputDir));
		output = new File(config.outputDir + "/update_bsps.sh");
		writer = new PrintWriter(output);
		
		String s = "";
		s += "#!/bin/bash\n"+
				"\n"+
				"if [ -z \"${NIOS_CODEGEN_ROOT}\" ]; then\n"+
				"  echo \"Environment variable NIOS_CODEGEN_ROOT undefined!\"\n"+
				"  exit 1\n"+
				"fi\n"+
				"\n"+
				"\n"+
				"OUTPUT_DIR=" + config.outputDir + "\n"+
				"BSPMOD_DIR=${NIOS_CODEGEN_ROOT}/platform/bsp_mods\n"+
				"OS_DIR=${NIOS_CODEGEN_ROOT}/platform/micrium_uc_osii\n"+
				"\n"+
				"\n"+
				"for i in {0..1} M\n"+
				"do\n"+
				"	cp -f ${BSPMOD_DIR}/os_cpu_c.c            ${OUTPUT_DIR}/cpu${i}_bsp/HAL/src/\n"+
				"	cp -f ${OS_DIR}/inc/os_cfg.h              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/inc/\n"+
				"	cp -f ${OS_DIR}/inc/ucos_ii.h             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/inc/\n"+
				"	cp -f ${OS_DIR}/src/os_dbg.c              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_mem.c              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_task.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_dbg_r.c            ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_mutex.c            ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_time.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_flag.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_q.c                ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_tmr.c              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_core.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_mbox.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_sem.c              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"done\n"+
				"\n"+
				"cp -f ${BSPMOD_DIR}/alt_exception_entry_gp.S ${OUTPUT_DIR}/cpu0_bsp/HAL/src/alt_exception_entry.S\n"+
				"cp -f ${BSPMOD_DIR}/alt_exception_entry_gp.S ${OUTPUT_DIR}/cpu1_bsp/HAL/src/alt_exception_entry.S\n"+
				"cp -f ${BSPMOD_DIR}/os_core.c                ${OUTPUT_DIR}/cpu0_bsp/UCOSII/src/\n"+
				"cp -f ${BSPMOD_DIR}/os_core.c                ${OUTPUT_DIR}/cpu1_bsp/UCOSII/src/\n"+
				"cp -f ${BSPMOD_DIR}/ucos_ii.h                ${OUTPUT_DIR}/cpu0_bsp/UCOSII/inc/\n"+
				"cp -f ${BSPMOD_DIR}/ucos_ii.h                ${OUTPUT_DIR}/cpu1_bsp/UCOSII/inc/\n"+
				"cp -f ${BSPMOD_DIR}/os_cpu_c_MON.c           ${OUTPUT_DIR}/cpuM_bsp/HAL/src/os_cpu_c.c\n"+
				"\n"+
				"\n"+
				"# update system.h for monitor bsp\n"+
				"DIR=.tmp\n"+
				"mkdir -p $DIR\n"+
				"\n"+
				"# fix the comparator interrupt\n"+
				"\n"+
				"sed -r -e 's/(CFPU_0_CSR_IRQ) (-*[0-9]+)/\\1 3/' \\\n"+
				"		-e 's/(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID) (-*[0-9]+)/\\1 0/' \\\n"+
				"		<${OUTPUT_DIR}/cpuM_bsp/system.h >$DIR/test.x\n"+
				"\n"+
				"cp $DIR/test.x ${OUTPUT_DIR}/cpuM_bsp/system.h\n"+
				"\n"+
				"#update the Monitor dma interrupts file for all\n"+
				"for i in {0..1}\n"+
				"do\n"+
				"	irq=$(expr $i + 5)\n"+
				"	sed -r -e \"s/(PROCESSOR${i}_0_DMA_0_IRQ) (-*[0-9]+)/\\1 ${irq}/\" \\\n"+
				"		-e \"s/(PROCESSOR${i}_0_DMA_0_IRQ_INTERRUPT_CONTROLLER_ID) (-*[0-9]+)/\\1 0/\" \\\n"+
				"		<${OUTPUT_DIR}/cpuM_bsp/system.h >$DIR/test.x\n"+
				"	cp $DIR/test.x ${OUTPUT_DIR}/cpuM_bsp/system.h\n"+
				"\n"+
				"done\n"+
				"\n"+
				"\n"+
				"rm -r $DIR\n"+
				"\n";
				
		writer.write(s);
		writer.close();
		
		output.setExecutable(true);
		
		output = new File(config.outputDir + "/create_bsp.sh");
		writer= new PrintWriter(output);
		
		s = "";

		s += "#!/bin/bash\n"+
				"\n"+
				"\n"+
				"if [ -z \"${NIOS_CODEGEN_ROOT}\" ]; then\n"+
				"  echo \"Environment variable NIOS_CODEGEN_ROOT undefined!\"\n"+
				"  exit 1\n"+
				"fi\n"+
				"\n"+
				"NIOS2COMMANDSHELL=" + config.niosSBT.sbtLocation + "\n" +
				"SOPC_BUILDER_PATH=${NIOS_CODEGEN_ROOT}/drivers\n"+
				"SOPC_LOCATION=" + config.sopcinfoFilename + "\n"+
				"OUTPUT_DIR=" + config.outputDir + "\n"+
				"\n"+
				"for i in {0..1} M\n"+
				"do\n"+
				"	${NIOS2COMMANDSHELL} nios2-bsp ucosii ${OUTPUT_DIR}/cpu${i}_bsp ${SOPC_LOCATION} \\\n"+
				"	--cpu-name processor${i}_0_cpu${i}\n"+
				"\n"+
				"	if [ \"$?\" !=  \"0\" ]\n"+
				"	then\n"+
				"		echo \"ERROR: could not generate ${OUTPUT_DIR}/cpu${i}_bsp\"\n"+
				"		exit 1\n"+
				"	fi\n"+
				"done\n"+
				"\n"+
				"\n"+
				"\n"+
				"\n"+
				"# #update linker scripts:\n"+
				"# # update_memory_region Positional Arguments:\n"+
				"# # <name>                  String with the name of the memory region to update.\n"+
				"# # <slave desc>            String with the slave descriptor of the memory device\n"+
				"# #                         for this region.\n"+
				"# # <offset>                String with the byte offset of the memory region from\n"+
				"# #                         the memory device base address.\n"+
				"# # <span>                  String with the span of the memory region in bytes.\n"+
				"\n"+
				"# cpu0\n"+
				"${NIOS2COMMANDSHELL} nios2-bsp-update-settings  --settings ${OUTPUT_DIR}/cpu0_bsp/settings.bsp \\\n"+
				"--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x64020 0x30fe0 \\\n"+
				"--cmd add_memory_region stack_bin_0 memory_0_onchip_memoryMain 0x95000 0x1000 \\\n"+
				"--cmd add_section_mapping .stack_bin_0 stack_bin_0 \\\n"+
				"--cmd enable_sw_package mem_manager \\\n"+
				"--cmd enable_sw_package mpu_utils \\\n"+
				"--cmd enable_sw_package fingerprint \\\n"+
				"--cmd enable_sw_package reset_monitor \\\n"+
				"--cmd set_setting hal.make.bsp_cflags_user_flags -fomit-frame-pointer\n"+
				"\n"+
				"# cpu1\n"+
				"${NIOS2COMMANDSHELL} nios2-bsp-update-settings  --settings ${OUTPUT_DIR}/cpu1_bsp/settings.bsp \\\n"+
				"--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x32020 0x30fe0 \\\n"+
				"--cmd add_memory_region stack_bin_0 memory_0_onchip_memoryMain 0x63000 0x1000 \\\n"+
				"--cmd add_section_mapping .stack_bin_0 stack_bin_0 \\\n"+
				"--cmd update_section_mapping .bss memory_0_onchip_memoryMain \\\n"+
				"--cmd update_section_mapping .text memory_0_onchip_memoryMain \\\n"+
				"--cmd update_section_mapping .stack memory_0_onchip_memoryMain \\\n"+
				"--cmd update_section_mapping .heap memory_0_onchip_memoryMain \\\n"+
				"--cmd update_section_mapping .rodata memory_0_onchip_memoryMain \\\n"+
				"--cmd update_section_mapping .rwdata memory_0_onchip_memoryMain \\\n"+
				"--cmd enable_sw_package mem_manager \\\n"+
				"--cmd enable_sw_package mpu_utils \\\n"+
				"--cmd enable_sw_package fingerprint \\\n"+
				"--cmd enable_sw_package reset_monitor \\\n"+
				"--cmd set_setting hal.make.bsp_cflags_user_flags -fomit-frame-pointer\n"+
				"\n"+
				"# cpum\n"+
				"${NIOS2COMMANDSHELL} nios2-bsp-update-settings  --settings ${OUTPUT_DIR}/cpuM_bsp/settings.bsp \\\n"+
				"--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x20 0x30fe0 \\\n"+
				"--cmd add_memory_region global_data memory_0_onchip_memoryMain 0x31000 0x1000 \\\n"+
				"--cmd add_section_mapping .global_data global_data \\\n"+
				"--cmd add_section_mapping .critical processorM_0_scratchpad \\\n"+
				"--cmd add_section_mapping .shared shared_memory \\\n"+
				"--cmd enable_sw_package repos  \\\n"+
				"--cmd enable_sw_package mpu_utils \\\n"+
				"--cmd enable_sw_package fingerprint \\\n"+
				"--cmd enable_sw_package reset_monitor \\\n"+
				"--cmd enable_sw_package dma \\\n"+
				"--cmd set_setting hal.make.bsp_cflags_user_flags -fomit-frame-pointer\n"+
				"\n"+
				"\n"+
				"for i in {0..1} M\n"+
				"do\n"+
				"	 ${NIOS2COMMANDSHELL} nios2-bsp-generate-files --bsp-dir ${OUTPUT_DIR}/cpu${i}_bsp \\\n"+
				"	 --settings ${OUTPUT_DIR}/cpu${i}_bsp/settings.bsp \n"+
				"done\n"+
				"	\n"+
				"source ${OUTPUT_DIR}/update_bsps.sh\n"+
				"\n";
		writer.write(s);
		writer.close();
		

		output.setExecutable(true);
		File bsp = new File(config.outputDir + "/cpuM_bsp");
		if(!bsp.exists()){
			config.niosSBT.runCommand(new String[]{config.outputDir + "/create_bsp.sh"});
		}
	}

	private void generateProcessoringCore(Core core) throws IOException {
		
		File outputDir = new File(config.outputDir + "/" + core.name);
		FileUtils.forceMkdir(outputDir);
		File srcOutput = new File(outputDir + "/" + core.name + ".c");
		File incOutput = new File(outputDir + "/" + core.name + ".h");
		PrintWriter writer;
		
		
		writer = new PrintWriter(incOutput);
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
		writer.print(getWhiteSpace(5));
		writer.print(getCPUInterruptString(core));
		writer.print(getWhiteSpace(5));
		writer.print(getTaskFunctions(core));
		writer.print(getWhiteSpace(5));
		writer.print(getMemoryManagerInitString(core));
		writer.print(getWhiteSpace(5));
		writer.print(getMPUInitString(core));
		writer.print(getWhiteSpace(5));
		writer.print(getMainString(core));
		writer.close();
		

	}






	private void copyCriticalLibrary() {
		for(Function f: funcList){
			String outputDir = config.outputDir + "/critical_library";
			copySourceFiles(f.codeDirectory, outputDir);
			deleteFile(outputDir + "/ert_main.c");
			
		}
		
		for (Function f : fprintList){
			String outputDir = config.outputDir + "/cpu0";
			copySourceFiles(f.codeDirectory, outputDir);
			deleteFile(outputDir + "/ert_main.c");
		}
		
	}

	private void deleteFile(String name) {

		File file = new File(name);
		try {
			FileUtils.forceDelete(file);
		} catch (IOException e) {
			e.printStackTrace();
		}
		/* need fprint in cpu0 also */
		
	}

	private void generateMonitor(Core core) throws IOException {
		//TODO
		
		//first thing add DMA task
		addDmaFunction(core);
		
		File outputDir = new File(config.outputDir + "/" + "cpuM");
		FileUtils.forceMkdir(outputDir);
		File srcOutput = new File(outputDir + "/" + "cpuM" + ".c");
		File incOutput = new File(outputDir + "/" + "cpuM" + ".h");
		PrintWriter writer;
		
		
		writer = new PrintWriter(incOutput);
		writer.print(getHeaderFile(core));
		writer.close();
		
		
		writer = new PrintWriter(srcOutput);
		writer.print(getIncludeString(core)); /* print the includes */
		writer.print(getWhiteSpace(5));
		writer.print(getVarDecString(core));
		writer.print(getWhiteSpace(5));
		writer.print(getStackDeclarations(core));
		writer.print(getWhiteSpace(5));
		writer.print(getMonitorMiscDecl());
		writer.print(getWhiteSpace(5));
		writer.print(getSharedMemoryDeclarations(core));
		writer.print(getWhiteSpace(5));
		writer.print(getResetMonitorString(core));
		writer.print(getWhiteSpace(5));
		writer.print(getComparatorString(core));
		writer.print(getWhiteSpace(5));
		writer.print(getReposString(core));
		writer.print(getWhiteSpace(5));
		writer.print(getTaskFunctions(core));
		writer.print(getWhiteSpace(5));
		writer.print(getMainStringMonitor(core));
		writer.close();
		
		
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

	private void updateMainMemorySize() {
		Core c = platform.getCore("cpu0");
		c.mainMemSize -= (stackBins.size() - 1) * StackBin.size;
		c = platform.getCore("cpu1");
		c.mainMemSize -= (stackBins.size()-1) * StackBin.size;
		
	}

	private void setStackBinAddresses() {
		for(int i = 0; i < stackBins.size(); i++){
			StackBin sb = stackBins.get(i);
			Core c = platform.getCore("cpu0");
			sb.startAddress[0] = c.mainMemStartAddressOffset+c.mainMemSize - (i*StackBin.size);
			c = platform.getCore("cpu1");
			sb.startAddress[1] = c.mainMemStartAddressOffset+c.mainMemSize - (i*StackBin.size);
		}

		updateMainMemorySize();
	}

	private void updateStackBinRegions() throws IOException {
		int numBins = 0;
		if((numBins = stackBins.size()) > 1) { /* default num of stack bins */
			String cmd = "";
			cmd +=  "#!/bin/bash"+
					"\n"+
					"\n"+
					"OUTPUT_DIR=" + config.outputDir + "\n"+
					"NIOS2COMMANDSHELL=" + config.niosSBT.sbtLocation + "\n"+
					"\n";
			Core c = platform.getCore("cpu0");
			cmd += updateBspStackBins(numBins,c,0,stackBins);
			c = platform.getCore("cpu1");
			cmd += updateBspStackBins(numBins, c,1,stackBins);
			
			if(!cmd.isEmpty()){
			File file = new File(config.outputDir + "/update_bsp_mem_regions.sh");
			PrintWriter writer;
			
			
			writer = new PrintWriter(file);
			writer.print(cmd);
			writer.close();
			
			file.setExecutable(true);

			//Instead of checking inside StackBin,
			//parse once here, check every line for all stack bins,
			
			c = platform.getCore("cpu0");
			config.niosSBT.updateBspStackBins(numBins,c,0,stackBins);
			c = platform.getCore("cpu1");
			config.niosSBT.updateBspStackBins(numBins, c,1,stackBins);
			}
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

		
		@SuppressWarnings("unchecked")
		ArrayList<Function> fprintListCopy = (ArrayList<Function>) fprintList.clone();
		Collections.sort(fprintListCopy, Function.stackCompareDecreasing);
		if (fprintListCopy.get(0).stackSize > 2760) {
			// TODO error
		}
		stackBins = new ArrayList<>();
		int binIndex = 0;
		int currentBinSize = 0;
		int currentBinIndex = 0;
		for (Function f : fprintListCopy) {
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
					
					if (!line.contains(";")) {
						String newLine = "\n";
						while (!(newLine = reader.readLine()).contains(";")) {
							line += "\n	"+ newLine;
						}
						line += "\n	" + newLine + "\n";
					}
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
		c.addHeader("dma.h");
		c.addHeader("<altera_avalon_mutex.h>");
		c.addHeader("shared_mem.h");
		c.addHeader("fingerprint.h");
		c.addHeader("gp.h");
		c.addHeader("critical.h");
		c.addHeader("cpuM.h");
		c.addHeader("reset_monitor.h");
		c.addHeader("repos.h");
		

		findFunctionHeaders();

		for (Function f : funcList) {
			if (f.critical) {
				if(f.cores.contains("cpu0")){
					c = platform.getCore("cpu0");
				} else {
					c = platform.getCore("cpum");
				}
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
				String.format("%-50s%d\n","#define STACKSIZE_MINOFFSET",StackBin.STACKSIZE_MINOFFSET/4)     				 +
				String.format("%-50s%d\n","#define STACKSIZE_MARGINERROR",StackBin.STACKSIZE_MARGINERROR/4);
		
		for (Function f : core.funcList){
			/* divide by 4: profile measures in bytes, c code in words */
			s += String.format("%-50s%s","#define " + f 
					+ "_STACKSIZE",String.format("(%4d%s )\n",f.stackSize/4, 
					" + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR")); 
		}
		if(core.isMonitor){
			for(Function f : fprintList){
				s += String.format("%-50s%s","#define " + f 
						+ "_STACKSIZE",String.format("(%4d%s )\n",f.stackSize/4, 
						" + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR")); 
			
			}
		}
		
		
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


	private String getStackDeclarations(Core core) {
		String s = "";
		s += 	"/*****************************************************************************\n"+
				" * Stack Declarations\n"+
				" *****************************************************************************/\n";
		
		@SuppressWarnings("unchecked")
		ArrayList<Function> stackList = (ArrayList<Function>) core.funcList.clone();
		Collections.sort(stackList, Function.stackCompareDecreasing);
		
		for (Function f : stackList){
			s += "OS_STK " + f + "_STACK[" + f + "_STACKSIZE]";
			
			if(core.isMonitor){
				s += " __attribute__ ((section (\".critical\")))";
					
			} else if( f.critical){
				s += " __attribute__ ((section (\"." + f.stackBin.name + "\")))";
			}
			
			s += ";\n";
		}
		
		return s;
	}
	

	private String getSemaphoreDeclarations(Core core) {
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
	

	private String getSharedMemoryDeclarations(Core core) {
		String s = "";
		if(core.isMonitor){
			s += 	"/*****************************************************************************\n"+
					" * Shared memory interface with other cores\n"+
					" *****************************************************************************/\n"+
					"SharedMemorySymbolTable shared_stab __attribute__ ((section (\".shared\")));\n"+
					"FunctionTable functionTable[" + fprintList.size() + "] __attribute__ ((section (\".shared\")));\n"+
					"CriticalFunctionData critFuncData[NUMCORES] __attribute__ ((section (\".shared\")));\n"+
					"\n";
		} else {
			s += 	"/*****************************************************************************\n"+
					" * Shared memory interface with monitor\n"+
					" *****************************************************************************/\n"+
					"FunctionTable *functionTable;\n"+
					"CriticalFunctionData *critFuncData;\n"+
					"SharedMemorySymbolTable *stab;\n";
			
		}
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

	

	private String getTaskFunctions(Core core) {
		String s = "";
			
		for(Function f : core.funcList){
			if(!f.name.equals("dma")){
				s += "/*****************************************************************************\n"+
						" * " + f + "Task wrapper\n"+
						" *****************************************************************************/\n";
			}
			if(!core.isMonitor && f.critical){
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
						"		// Set default block size for fingerprinting\n"+
						"		fprint_set_block_size(" + f + "_blocksize);\n"+
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
				if(!f.name.equals("dma")){
					s += "void " + f + "_TASK(void* pdata) {\n" +
						"\twhile (1) {\n";
					s += String.format("\t\t%s_step(%s_M, &%s_U,"
							+ "\n\t\t\t&%s_Y);\n", f,f,f,f);
					s+= "\t\tOSTimeDlyHMSM(0, 0, 0, " + f.period + ");\n"; 
					s += "\t}\n}\n";
				}
			}
			s += "\n";
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
			
			if(core.index == 0){
				s += Integer.toHexString(f.stackBin.startAddress[1] + platform.mainMemoryBase);
			} else {
				s += "0";
			}
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
				"		disable_fprint_task(FprintTaskIDCurrent);\n"+
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
				"	region[0].mask = (0x431000) / 64;\n"+
				"	region[0].c = 0;\n"+
				"	region[0].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;\n"+
				"\n"+
				"	//other core's region - stack\n" +
				"	region[1].index = 0x1;\n";
		
				int otherCoreIndex = 0;
				if(core.index == 0){
					otherCoreIndex = 1;
				}

				StackBin lastStackBin =  stackBins.get(stackBins.size()-1);
				Core otherCore = platform.getCore("cpu" + otherCoreIndex);
				s += "	region[1].base = 0x" + Integer.toHexString(platform.mainMemoryBase + otherCore.mainMemStartAddressOffset & 0xFFFFF000) + "/ 64;\n"+
						"	region[1].mask = 0x" + Integer.toHexString(platform.mainMemoryBase + lastStackBin.startAddress[otherCoreIndex]) + "/ 64;\n"+
						"	region[1].c = 0;\n"+
						"	region[1].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;\n";
				
				
				s += "\n"+
				"	//catch null pointers\n"+
				"	region[2].index = 0x2;\n"+
				"	region[2].base = 0x0;\n"+
				"	region[2].mask = (MEMORY_0_ONCHIP_MEMORYMAIN_BEFORE_RESET_REGION_BASE) / 64;\n"+
				"	region[2].c = 0;\n"+
				"	region[2].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;\n"+
				"\n"+
				"	int index;\n"+
				"	for (index = 3; index < NIOS2_MPU_NUM_DATA_REGIONS; index++) {\n"+
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
			if(core.index == 1 || !f.critical){
				s += "	OSTaskCreateExt(" + f + "_TASK, NULL,\n"+
						"			&" + f + "_STACK[" + f + "_STACKSIZE - 1],\n"+
						"			" + f + "_PRIORITY, " + f + "_PRIORITY,\n"+
						"			" + f + "_STACK, " + f + "_STACKSIZE, NULL,\n"+
						"			OS_TASK_OPT_STK_CLR);\n";
			} else { /* critical tasks on core 0 */
				s += "	OSTaskCreateExt(" + f + "_TASK, NULL,\n"+
						"			(OS_STK *)0x"+ Integer.toHexString(getStackEnd(f,1)) +",\n"+
						"			" + f + "_PRIORITY, " + f + "_PRIORITY,\n"+
						"			(OS_STK *)0x" + Integer.toHexString(getStackStart(f,1)) +", " + f + "_STACKSIZE, NULL,\n"+
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
	

	private String getCriticalIncludeString() {
		String s = "";
		s += "\n"+
				"#ifndef CRITICAL_H_\n"+
				"#define CRITICAL_H_\n"+
				"\n"+
				"#include \"fingerprint.h\"\n";
		
		for(Function f : fprintList){
			s += "#include \"" + f + ".h\"\n";
		}
			s +=	"\n";
			
		for(Function f : fprintList){
			s += "typedef struct {\n"+
					"	RT_MODEL_" + f + "_T " + f + "_M;\n"+
					"	ExtU_" + f + "_T " + f + "_U;\n"+
					"	ExtY_" + f + "_T " + f + "_Y;\n"+
					"} " + f + "Struct;\n"+
					"\n";
		}
		
		for (Function f : fprintList){
			s += "/****************************************************************************\n"+
					" * This function is a wrapper for the critical task AirbagModel_step()\n"+
					" ****************************************************************************/\n";
			s += "void " + f + "_CT(int priority, " + f + "Struct *" + f + "_struct);\n";
		
		}
		
		s += "#endif\n";
		
		return s;
	}
	


	private String getCriticalSourceString() {
		String s = "";
		s += "#include \"critical.h\"\n\n";
		
		for(Function f : fprintList){
			s += "/****************************************************************************\n"+
			" * This function is a wrapper for the critical task " + f + "_step()\n"+
			" ****************************************************************************/\n"+
			"void " + f + "_CT(int priority, " + f + "Struct *" + f + "_struct){\n"+
			"	RT_MODEL_" + f + "_T *const " + f + "_M = &" + f + "_struct->" + f + "_M;\n"+
			"	ExtU_" + f + "_T *" + f + "_U = &" + f + "_struct->" + f + "_U;\n"+
			"	ExtY_" + f + "_T *" + f + "_Y = &" + f + "_struct->" + f + "_Y;\n"+
			"	enable_fprint_task(priority);\n"+
			"	" + f + "_step(" + f + "_M, " + f + "_U, " + f + "_Y);\n"+
			"	disable_fprint_task(priority);\n"+
			"}\n"+
			"\n";
		}
		return s;
	}
	
	
	private String getCompileAppScript() {
		String s = "";
		s += "#!/bin/bash\n"+
				"\n"+
				"\n"+
				"NIOS2COMMANDSHELL=" + config.niosSBT.sbtLocation + "\n"+
				"\n"+
				"for i in {0..1} M\n"+
				"do\n"+
				"    pushd cpu${i}\n"+
				"    ${NIOS2COMMANDSHELL} make all\n"+
				"    popd\n"+
				"done\n"+
				"\n";
		return s;
	}
	
	private String getRunScriptString() {
		String s = "";
		s += "#!/bin/bash\n"+
				"\n"+
				"\n"+
				"OUTPUTDIR="+ config.outputDir +
				"\n"+
				"source ${OUTPUTDIR}/compile_bsp.sh\n"+
				"source ${OUTPUTDIR}/compile_app.sh\n"+
				"\n";
		return s;
	}
	
	


	private String getAppScriptString() {
		String s = "";
		
		s += "#!/bin/bash\n"+
				"\n"+
				"\n"+
				"\n"+
				"OUTPUT_DIR=" + config.outputDir + "\n"+
				"NIOS2COMMANDSHELL=" + config.niosSBT.sbtLocation + "\n"+
				"\n"+
				"\n"+
				"for i in {0..1} M\n"+
				"do\n"+
				"	BSPDIR=${OUTPUT_DIR}/cpu${i}_bsp\n"+
				"	APPDIR=${OUTPUT_DIR}/cpu${i}\n"+
				"\n"+
				"	SRCLIST=$(find ${APPDIR} -regex \".*\\.c\")\n"+
				"	${NIOS2COMMANDSHELL} nios2-app-generate-makefile --app-dir ${APPDIR} \\\n"+
				"	--bsp-dir ${BSPDIR} --elf-name cpu${i}.elf --src-files \\\n"+
				" 	$SRCLIST --set APP_CFLAGS_USER_FLAGS -fomit-frame-pointer --set OBJDUMP_INCLUDE_SOURCE 0 \\\n"+
				" 	--use-lib-dir ${OUTPUT_DIR}/critical_library\\\n"+
				"	--set APP_CFLAGS_DEBUG_LEVEL \"-g -gdwarf-2\" \\\n"+
				"	--set APP_INCLUDE_DIRS ${OUTPUT_DIR}/critical_library\n"+

				"\n"+
				"	if [ \"$?\" !=  \"0\" ]\n"+
				"	then\n"+
				"		echo \"Error generating Makefile for ${APPDIR}\"\n"+
				"		exit 1\n"+
				"	fi\n"+
				"\n"+
				"done\n"+
				"\n";
		return s;
	}
	
	
	private String getMonitorMiscDecl() {
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
		
		s += "/*****************************************************************************\n"+
				" * Reset monitor interface\n"+
				" *****************************************************************************/\n"+
				"void resetCores(void) {\n"+
				"	OSTaskDel(dma_PRIORITY);\n"+
				"	int* cpu0_reset = (int*) PROCESSOR0_0_SW_RESET_0_BASE;\n"+
				"	int* cpu1_reset = (int*) PROCESSOR1_0_SW_RESET_0_BASE;\n"+
				"	*cpu0_reset = 1;\n"+
				"	*cpu1_reset = 1;\n"+
				"	coresReady = false;\n"+
				"	taskFailed = true;\n"+
				"	resetMonitorEnable();\n"+
				"}\n"+
				"static void handleResetMonitor(void* context) {\n"+
				"	resetMonitorDisable();\n"+
				"	coresReady = true;\n"+
				"	if (taskFailed) {\n"+
				"		taskFailed = false;\n"+
				"\n"+
				"		postDmaMessage(failedTaskID, true);\n"+
				"\n"+
				"		OSTaskCreateExt(dma_TASK, NULL, &dma_STACK[dma_STACKSIZE - 1], dma_PRIORITY,\n"+
				"				dma_PRIORITY, dma_STACK, dma_STACKSIZE, NULL, OS_TASK_OPT_NONE);\n"+
				"\n"+
				"	}\n"+
				"}\n"+
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
				"	fprint_status(&status);\n"+
				"\n"+
				"	//Assume static mapping of fingeprint tasks for now\n"+
				"	//There is only one possible task that can set this off in this example\n"+
				"	//---------------------------------------------------------------------\n"+
				"	if (status.failed_reg) {\n"+
				"		int i;\n"+
				"		for (i = 0; i < 16; i++) {\n"+
				"			INT32U mask;\n"+
				"			if (status.failed_reg & (mask = 1 << i)) {\n"+
				"				/* assume only one failure possible */\n"+
				"				failedTaskID = REPOSgetTaskID(mask);\n"+
				"				fprint_get_task_count(i); /* make sure the counter resets */\n"+
				"				break;\n"+
				"			}\n"+
				"		}\n"+
				"		resetCores();\n"+
				"		REPOSInit(); /* ORDER MATTERS */\n"+
				"	}\n"+
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
				"				int numFuncs = REPOSTaskTable[taskID].numFuncs;\n"+
				"				/* Here we check that all the functions inside the task have executed,\n"+
				"				 * then we check that the task did not fail (since the same FID may have been used twice\n"+
				"				 * before the handler responds if the functions are tiny).\n"+
				"				 */\n"+
				"				if (((REPOSTaskTable[taskID].funcCompleteCount += fprint_get_task_count(i)) == numFuncs)\n"+
				"						&& (!taskFailed\n"+
				"								|| (taskFailed && taskID != failedTaskID))) { /*function can be decomposed into several chunks, so could be both cases */\n"+
				"					REPOSTaskTable[taskID].funcCompleteCount = 0;\n"+
				"					REPOSTaskComplete(taskID);\n"+
				"					postDmaMessage(taskID, false);\n"+
				"				}\n"+
				"			}\n"+
				"		}\n"+
				"	}\n"+
				"\n"+
				"	fprint_reset_irq();\n"+
				"}\n"+
				"\n"+
				"static void initCompIsr(void) {\n"+
				"	alt_ic_isr_register(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID, CFPU_0_CSR_IRQ,\n"+
				"			handleComp, (void*) NULL, (void*) NULL);\n"+
				"}\n";
		return s;
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
			String core0VirtSp = core1PhysSP;
			String core1VirtSP = core1PhysSP;
			s+= "	task = &REPOSTaskTable[" + funcIndex + "];\n"+
					"\n"+
					"	task->dataAddressPhys = &" + f + "PackageStruct;\n"+
					"\n"+
					"	task->stackAddressPhys[0] = (void *) (0x" + core0PhysSP + ");\n"+
					"	task->stackAddressPhys[1] = (void *) (0x" + core1PhysSP + ");\n"+
					"\n"+
					"	task->stackAddressVirt[0] = (void *) (0x" + core0VirtSp + ");\n"+
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




	private String getMainStringMonitor(Core core) {
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
			s += "	functionTable[" + funcIndex + "].args = &" + f + "PackageStruct;\n"+
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
				"	Directory_Init_Struct d;\n"+
				"	int i,j;\n\n";
		
				
//		s += "	for (i = 0; i < " + fprintList.size() + "; i++) {\n";
		//TODO hard-coded!!
		int increment = 512 /fprintList.size();
		for(int i = 0; i < fprintList.size(); i++){
			s += "	d.core_id = 0;\n"+
					"	d.key = " + i + ";\n"+
					"	d.start_ptr = " + (i * increment) + ";\n"+
					"	d.end_ptr = " + (((i + 1) * increment)-1) + ";\n"+
					"	set_task_directory(&d);\n"+
					"\n"+
					"	d.core_id = 1;\n"+
					"	set_task_directory(&d);\n";

		}
		
//		s += "	}\n";
		
		s += "	Core_Assignment_Table ca;\n"+
				"	//Default table\n"+
				"	for (i = 0; i < CA_TABLE_MAX_REDUNDANCY; i++) {\n"+
				"		for (j = 0; j < CA_TABLE_NUM_TASKS; j++) {\n"+
				"			ca.table[i][j] = i;\n"+
				"		}\n"+
				"	}\n"+
				"	ca.table[0][0] = 0;\n"+
				"	ca.table[1][0] = 1;\n"+
				"	set_core_assignment_table(&ca);\n"+
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

	
	public String updateBspStackBins(int numBins, Core core, int coreID,
			ArrayList<StackBin> sbList) {
		// first shrink size of mainMemory
		// then add more stack regions
		// each function should get a stack region
		String bspSettings = core.name;
		String cmd = "";
		for (int i = 1; i < sbList.size(); i++) {
			StackBin sb = sbList.get(i);
				cmd += ResizeMainMem(bspSettings, core.mainMemStartAddressOffset, core.mainMemSize);
				cmd += addStackBins(bspSettings, sb.startAddress[coreID], sb.name);
		}
		
		return cmd;
	}



	private String addStackBins(String coreID, 
			int binStartAddress, String name) {
		String cmd ="${NIOS2COMMANDSHELL}" + " nios2-bsp-update-settings \\\n" +
				"--settings ${OUTPUT_DIR}/" + coreID + "_bsp/settings.bsp"  + " \\\n" + 
				"--cmd add_memory_region " + name +
				" memory_0_onchip_memoryMain " +
				"0x" + Integer.toString(binStartAddress, 16) +
				" 0x" + Integer.toString(StackBin.size, 16) + " \\\n--cmd" +
				" add_section_mapping " + "." + name + " " + name + "\n\n";

		return cmd;
	}

	private String ResizeMainMem(String coreID, int mainMemStartAddress,
			int newSize) {
		String cmd = "${NIOS2COMMANDSHELL}" + " nios2-bsp-update-settings \\\n"+
				"--settings ${OUTPUT_DIR}/" + coreID + "_bsp/settings.bsp" + " \\\n--cmd update_memory_region" +
				" memory_0_onchip_memoryMain memory_0_onchip_memoryMain "+
				"0x" + Integer.toString(mainMemStartAddress, 16)+
				" 0x" + Integer.toString(newSize, 16) + "\n\n";
		return cmd;

	}

	
	
	/*************************************************************
	 * HELPERS
	 *************************************************************/
	
	
	private int getStackStart(Function f, int coreID) {
		return platform.mainMemoryBase + f.stackBin.startAddress[coreID] +  f.getStackStart(coreID);
	}

	private int getStackEnd(Function f, int coreID) {
		 return platform.mainMemoryBase + f.stackBin.startAddress[coreID] + f.getStackEnd(coreID);
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

	public void printStackResults(){
		System.out.println("\nStack results (in bytes):");
		System.out.println("<STACK_PROFILE>");
		for (Function f : funcList) {
			System.out.println(f.name + " " + f.stackSize);
		}
		System.out.println("</STACK_PROFILE>");
	}
	
}
