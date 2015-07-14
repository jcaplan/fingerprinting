package codegen.gen;

import java.io.*;
import java.util.*;

public class Generator {

	Configuration config;

	ArrayList<Function> funcList;
	Platform platform;
	public static final int MONITOR_INDEX = 2;


	public Generator(Configuration config) {
		this.config = config;
		this.funcList = config.funcList;
		platform = new Platform(config);
	}
	
	public void generateCores() throws IOException {

		initHeaders();
		
		getVariableDeclarations();
		getFunctionInitialization();
		//figure out the number of stacks -> the size of the stack bins (4kB pages)
		//This requires compiling the files for Nios...what is the stack size?? profiling!
		//figure out the size of the data variables -> size of global_data region  (4 kB pages)
		//figure out mem manager initialization
		//figure out mpu initialization
		
		
		
		for(Core c : platform.coreList){
			c.printIncludes();
			System.out.println("");
			c.printVarDeclarations();
			System.out.println("");
		}
		for(Function f : funcList){
			f.printInitialization();
			System.out.println("");
		}
		
		
		
		
		/*
		 * the things that need to happen are: 1. get the functions that run on
		 * core 0
		 * 
		 * generate includes 
		 * static model variable 
		 * declarations for non critical tasks stacks (figure out stack sizes) semaphores linker regions and
		 * sections (num of critical stacks?) 
		 * mem manager mpu init
		 * tasks
		 * interrupts
		 * main
		 * 
		 */
		

		
		
	}

	private void getFunctionInitialization() throws IOException {
		for(Function f : funcList){
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
			while(!(line = reader.readLine()).contains("return")){
				line = line.trim();
				if(line.startsWith(f.name)){
					f.initialization.add(line);
				}
			}
			
			reader.close();
		}
		
	}

	private void getVariableDeclarations() throws IOException {
		for(Function f : funcList){
			FileReader fr = new FileReader(f.codeDirectory + "/ert_main.c");
			BufferedReader reader = new BufferedReader(fr);
			String line = "";
			while ((line = reader.readLine()) != null) {
				String dec = "";
				line = line.trim();
				String[] tokens = line.split("[\\s]+");
				if(tokens[0].equals("static")){
					String name = tokens[2];
					if(!name.equals("OverrunFlag")){ /* ignore the OverrunFlag variable */
						dec += line;
						
						/* check that the line ends with a semicolon */
						if(!line.contains(";")){
							while(!(line = reader.readLine()).contains(";")){
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

	private void initHeaders(){
		
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
		
		for(Function f : funcList){
			if(f.critical){
				c = platform.getCore("cpum");
			} else {
				c = platform.getCore(f.cores.get(0));	
			}
			c.addHeader(f + ".h");	
		}	
	}

	public void findFunctionHeaders(){
		for(Function f : funcList){
			File file = new File(f.codeDirectory);
			File[] fileList = file.listFiles();
			for(File fil : fileList){
				if(fil.getName().endsWith(".h")){
					f.includeFiles.add(fil.getName());
				}
			}
		}
	}
	
	void printHeaders(String core) {
		Core c = platform.getCore(core);
		c.printIncludes();
	}
	
	public String getCoreName(int core){
		String name = "";
		if(core == 0){
			name = "cpu0";
		} else if (core == 1){
			name = "cpu1";
		} else if (core == 2){
			name = "cpum";
		}
		return name;
	}
	
}
