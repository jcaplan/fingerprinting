package codegen.gen;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

import org.apache.commons.io.FileUtils;

/**
 * GenCriticalLibrary contains all methods for copying the critical library files, the generate script,
 * and the files critical.c and critical.h
 * @author jonah
 *
 */
public class GenCriticalLibrary {

	

	Configuration config;
	Platform platform;
	ArrayList<Function> funcList;
	ArrayList<Function> fprintList;
	
	/**
	 * Constructor. Should be called from CodeGen after functions have been analyzed.
	 * @param config	Valid configuration
	 * @param funcList	The list of all functions in the application
	 * @param fprintList	The list of fingerprinted functions
	 */
	public GenCriticalLibrary(Configuration config, ArrayList<Function> funcList, ArrayList<Function> fprintList){
		this.config = config;
		this.platform = config.platform;
		this.funcList = funcList;
		this.fprintList = fprintList;
	}

	/**
	 * The main entry point for this class. Copies and generates all files:
	 * fills critical_library/, generates critical.c and critical.h, generate_lib.sh
	 * @throws FileNotFoundException
	 */
	public void generateCriticalLibrary() throws FileNotFoundException {
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
	
	/**
	 * 
	 * @return	Returns the string for generate_lib.sh
	 */
	private String generateCriticalLibraryString() {
		
		
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
	
	/**
	 * Copies files from source directory to critical_library/ and cpu0/.
	 * In the future, cpu0/ should be replaced by logical core 0. 
	 */
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
	
	/**
	 * Copies source files (.c and .h only) from one directory to another
	 * @param srcName	source directory
	 * @param destName	destination directory
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
	 * 
	 * @return String contents of critical.h
	 */
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
	

	/**

	 * @return 	 * String contents of critical.c
	 */
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
			"	fprint_enable_task(priority);\n"+
			"	" + f + "_step(" + f + "_M, " + f + "_U, " + f + "_Y);\n"+
			"	fprint_disable_task(priority);\n"+
			"}\n"+
			"\n";
		}
		return s;
	}
	
	/**
	 * Removes file
	 * @param name	File name
	 */
	private void deleteFile(String name) {

		File file = new File(name);
		try {
			FileUtils.forceDelete(file);
		} catch (IOException e) {
			e.printStackTrace();
		}
		/* need fprint in cpu0 also */
		
	}
	
	
}
