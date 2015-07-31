package codegen.gen;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

import org.apache.commons.io.FileUtils;

import codegen.prof.Profiler;

/**
 * SourceAnalysis contains methods for parsing and retrieving information from Simulink files.
 * @author jonah
 *
 */
public class SourceAnalysis {

	Platform platform;
	ArrayList<Function> funcList;
	Configuration config;

	/**
	 * Constructor
	 * @param platform
	 * @param funcList
	 * @param config
	 */
	public SourceAnalysis(Platform platform, ArrayList<Function> funcList, Configuration config){
		this.platform = platform;
		this.funcList = funcList;
		this.config = config;
	}
	
	/**
	 * This method does the analysis.
	 * First the header file includes are determined, then variable declarations and model
	 * initialization is parsed from ert_main.c, then the max stack height is determined if necessary.
	 * Finally the variable size is checked (not implemented yet).
	 */
	public void doAnalysis(){
		//Parse source
		initHeaders();
		try {
			getVariableDeclarations();
			getFunctionInitialization();
			if(config.stackProfilingRequired){
				getMaxStacks();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		getVariableSizes();
	}
	
	
	/**
	 * Adds the header strings to the Core objects. 
	 * Default list as well as extra headers for Simulink functions.
	 */
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
	
	
	/**
	 * Adds header files to function object.
	 */
	private void findFunctionHeaders() {
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
	
	/**
	 * Parses the ert_main.c file for variable declarations.
	 * @throws IOException
	 */
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
	
	/**
	 * Parses the ert_main.c file for function initialization.
	 * @throws IOException
	 */
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
	
	/**
	 * Uses codege.prof.Profiler to find maximum stack height for each function
	 * (uses cpuM BSP by default. Should depend on which core function is running on
	 * because of FPU).
	 * @throws IOException
	 */
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

	/**
	 * Force delete an entire directory
	 * @param name	Name of directory
	 */
	private void deleteDirectory(String name) {
		File f = new File(name);
		try {
			FileUtils.deleteDirectory(f);
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	/**
	 * Copy between folders
	 * @param srcName	Source folder name
	 * @param destName	Destination folder name
	 */
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
	
	/**
	 * Determines the size of the static variables for each function.
	 * Not currently implemented.
	 */
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


	
	
}
