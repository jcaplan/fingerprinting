package codegen.gen;

import java.io.*;
import java.util.*;


public class CodeGen {

	Configuration config;
	Platform platform;
	ArrayList<Function> funcList;
	ArrayList<Function> fprintList;
	ArrayList<StackBin> stackBins;
	public static final int MONITOR_INDEX = 2;

	public CodeGen(Configuration config) {
		this.config = config;
		this.funcList = config.funcList;
		platform = config.platform;
	}

	public void generateCores() throws IOException {


		getFprintList();
		
		
		//Generate necessary scripts and SBT files
		if(config.generateBSPRequired()){
			new GenBSP(config).generateBSPs();
		}
		
		new GenCriticalLibrary(config, funcList, fprintList).generateCriticalLibrary();
		new GenScripts(config).generateScripts();
		new SourceAnalysis(platform, funcList, config).doAnalysis();
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




	private void getFprintList() {
		fprintList = new ArrayList<>();
		for (Function f : funcList) {
			if (f.cores.size() > 1) {
				fprintList.add(f);
			}
		}
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

	public void printStackResults(){
		System.out.println("\nStack results (in bytes):");
		System.out.println("<STACK_PROFILE>");
		for (Function f : funcList) {
			System.out.println(f.name + " " + f.stackSize);
		}
		System.out.println("</STACK_PROFILE>");
	}
	


	
}
