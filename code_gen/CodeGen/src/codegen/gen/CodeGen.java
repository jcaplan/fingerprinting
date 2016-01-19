package codegen.gen;

import java.io.*;
import java.util.*;

import codegen.map.Application;
import codegen.map.DMR;
import codegen.map.Lockstep;
import codegen.map.Mapper;
import codegen.map.Processor;
import codegen.map.Schedule;
import codegen.map.Task;

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
		fprintList = new ArrayList<>();
	}

	/**
	 * generates code for all cores.
	 * @throws IOException
	 * @throws ConfigurationException 
	 */
	public void generateCores() throws IOException, ConfigurationException {


		
		//Generate necessary scripts and SBT files
		if(config.generateBSPRequired()){
			new GenBSP(config,platform).generateBSPs();
		}
		SourceAnalysis sa = new SourceAnalysis(platform, funcList, config,fprintList);
		sa.doSourceProfiling();
		if(config.mappingRequired){
			System.out.println("Starting mapping");
			doMapping();
			System.out.println("finished mapping");
			for(Function f : funcList){
				System.out.print(f + ": ");
				for(Core c : f.cores){
					System.out.print(c + ",");
				}
				System.out.println("");
			}
		}

		platform.addFunctionsToCores();
		getFprintList();
		
		new GenCriticalLibrary(config, funcList, fprintList).generateCriticalLibrary();
		sa.doSourceParsing();
		new GenScripts(config,platform.numProcessingCores).generateScripts();
		new GenStackBin(fprintList,platform,config).genStackBins(); 
	
		
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



	private void doMapping() throws ConfigurationException {
			Application app = new Application();
			for(Function func : config.funcList){
				app.addTask(new Task(func.getCloMS(), func.getChiMS(), func.period, func.critical, func.name));
			}
			
			ArrayList<Processor> pList = new ArrayList<>();
			for(Core core : config.platform.coreList){
				pList.add(new Processor(core.name, core.isMonitor));
			}
			
			Mapper map = new Mapper();
			map.setProcList(pList);
			map.setApplication(app);
			
			//only use lockstep and dmr for now
			map.clearFTMs();
			map.addFTM(new Lockstep());
			map.addFTM(new DMR());
			
			//Solve the mapping problem
			map.findSchedule();
			
			Schedule sched = map.getBestSchedule();
			if(sched == null){
				throw new ConfigurationException("Could not find legal mapping of tasks onto platform");
			}
			
			//The schedule now tells a bunch of things...
			//for the functions in the configuration
				//update the priority
				//for each binding add to processor list
			for(Function f : config.funcList){
				Task t = app.getTask(f.name);
				f.priority = sched.getPriority(t) + 1; //DMA gets 0
				for(Processor p : sched.getProcessorsForAllPreplicas(t)){
					f.cores.add(platform.getCore(p.getName()));
				}
			}
	}

	/**
	 * Initializes the fprintList. Any function assigned to more than one core is 
	 * considered fingerprinted.
	 */
	private void getFprintList() {
		for (Function f : funcList) {
			if (f.cores.size() > 1) {
				fprintList.add(f);
			}
			if (f.cores.size() > 2) {
				f.tmr = true;
			}
		}
	}


	/**
	 * Shows results of stack analysis
	 */
	public void printStackResults(){
		System.out.println("\n#Stack results (in bytes):");
		System.out.println("<STACK_PROFILE>");
		for (Function f : funcList) {
			System.out.println(f.name + " " + f.stackSize);
		}
		System.out.println("</STACK_PROFILE>");
	}

	public void printWcetResults() {
		System.out.println("\n#WCET results (in ms):");
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

	public void printMappingResults() {
		System.out.println("\n#Mapping results");
		System.out.println("<MAPPING>");
		for (Function f : funcList){
			System.out.print(f + " ");
			for(Core c : f.cores){
				System.out.print(c + " ");
			}
			System.out.println("");
		}
		System.out.println("</MAPPING>");
		
	}
	


	
}
