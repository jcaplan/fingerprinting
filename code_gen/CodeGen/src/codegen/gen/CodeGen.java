package codegen.gen;

import java.io.*;
import java.util.*;

/**
 * Codegen is the main class through which the generation flow occurs.
 * 
 * @author jonah
 *
 */

public class CodeGen {

	Configuration config;
	Platform platform;
	ArrayList<Function> funcList;
	ArrayList<Function> fprintList;
	ArrayList<StackBin> stackBins;
	/**
	 * The index of the monitor core
	 */
	public static final int MONITOR_INDEX = 2;

	/**
	 * Constructor for codegen object
	 * @param config	CodeGen must be passed a valid configuration.
	 */
	public CodeGen(Configuration config) {
		this.config = config;
		this.funcList = config.funcList;
		platform = config.platform;
	}

	/**
	 * generates code for all cores.
	 * @throws IOException
	 */
	public void generateCores() throws IOException {


		getFprintList();
		
		
		//Generate necessary scripts and SBT files
		if(config.generateBSPRequired()){
			new GenBSP(config).generateBSPs();
		}
		
		new GenCriticalLibrary(config, funcList, fprintList).generateCriticalLibrary();
		new GenScripts(config).generateScripts();
		new SourceAnalysis(platform, funcList, config,fprintList).doAnalysis();
		new GenStackBin(stackBins,fprintList,platform,config).genStackBins(); 
	
		
		//static analysis complete
		//Generate individual core code
		for(Core core : platform.coreList){
			if(core.isMonitor){
				new GenCoreMon(config, fprintList, platform, funcList).generateCore(core);
			} else {
				new GenCore(config, fprintList, platform).generateCore(core);
			}
		}
		
		File bsp = new File(config.outputDir + "/cpuM_bsp");
		if(!bsp.exists()){
			config.niosSBT.runCommand(new String[]{config.outputDir+"/compile_bsp.sh"});
		}
		config.niosSBT.runCommand(new String[]{config.outputDir+"/create_app.sh"});
		
	}



	/**
	 * Initializes the fprintList. Any function assigned to more than one core is 
	 * considered fingerprinted.
	 */
	private void getFprintList() {
		fprintList = new ArrayList<>();
		for (Function f : funcList) {
			if (f.cores.size() > 1) {
				fprintList.add(f);
			}
		}
	}

	/**
	 * Returns name of a core corresponding to integer index
	 * @param core	index of core
	 * @return		string for core name
	 */
	public String getCoreName(int core) {
		String name = "";
		if (core == 0) {
			name = "cpu0";
		} else if (core == 1) {
			name = "cpu1";
		} else if (core == 2) {
			name = "cpuM";
		}
		return name;
	}

	/**
	 * Shows results of stack analysis
	 */
	public void printStackResults(){
		System.out.println("\nStack results (in bytes):");
		System.out.println("<STACK_PROFILE>");
		for (Function f : funcList) {
			System.out.println(f.name + " " + f.stackSize);
		}
		System.out.println("</STACK_PROFILE>");
	}

	public void printWcetResults() {
		System.out.println("\nWCET results (in clockcycles):");
		System.out.println("<WCET_PROFILE>");
		for (Function f : funcList) {
			System.out.print(f.name + " " + f.wcetLowerBound);
			
			if(f.critical){
				System.out.println(" " + f.wcetUpperBound); 
			} else {
				System.out.println();
			}
		}
		System.out.println("</WCET_PROFILE>");
		
	}
	


	
}
