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
		
		// Copy the source files into the folder for the first core in the list
		// --------------------------------------------------------------------
		
		for(Core core : platform.coreList){
			for(Function f : core.funcList){	
				String outputDir = config.outputDir + "/" + core.name;
				copySourceFiles(f.codeDirectory,outputDir);
				deleteFile(outputDir + "/ert_main.c");
			}
			
			if(core.isMonitor){
				for(Function f : fprintList){	
					String outputDir = config.outputDir + "/" + core.name;
					copySourceFiles(f.codeDirectory,outputDir);
					deleteFile(outputDir + "/ert_main.c");
				}	
			}
		}
		

		
		
		for(Core core : platform.coreList){
			ArrayList<Function> critList = new ArrayList<>();
			
			if(core.isMonitor){
				critList = fprintList;
			} else {
				for(Function f : core.funcList){
					if(f.critical){
						critList.add(f);
					}
				}
			}
			if(!critList.isEmpty()){
				core.requiresCriticalHeader = true;
				String outputDir = config.outputDir + "/" + core.name;
				File criticalSrcOutput = new File(outputDir + "/critical.c");
				File criticalIncOutput = new File(outputDir + "/critical.h");
						
				PrintWriter writer = new PrintWriter(criticalIncOutput);
				writer.print(getCriticalIncludeString(critList));
				writer.close();
				
				writer = new PrintWriter(criticalSrcOutput);
				writer.print(getCriticalSourceString(critList));
				writer.close();
			}
			
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
	 * @param critList 
	 * @return String contents of critical.h
	 */
	private String getCriticalIncludeString(ArrayList<Function> critList) {
		String s = "";
		s += "\n"+
				"#ifndef CRITICAL_H_\n"+
				"#define CRITICAL_H_\n"+
				"\n"+
				"#include \"fingerprint.h\"\n";
		
		for(Function f : critList){
			s += "#include \"" + f + ".h\"\n";
		}
			s +=	"\n";
			
		for(Function f : critList){
			s += "typedef struct {\n"+
					"	RT_MODEL_" + f + "_T " + f + "_M;\n"+
					"	ExtU_" + f + "_T " + f + "_U;\n"+
					"	ExtY_" + f + "_T " + f + "_Y;\n"+
					"} " + f + "Struct;\n"+
					"\n";
		}
		
		for (Function f : critList){
			s += "/****************************************************************************\n"+
					" * This function is a wrapper for the critical task AirbagModel_step()\n"+
					" ****************************************************************************/\n";
			s += "void " + f + "_CT(int priority, " + f + "Struct *" + f + "_struct);\n";
		
		}
		
		s += "#endif\n";
		
		return s;
	}
	

	/**

	 * @param critList 
	 * @return 	 * String contents of critical.c
	 */
	private String getCriticalSourceString(ArrayList<Function> critList) {
		String s = "";
		s += "#include \"critical.h\"\n\n";
		
		for(Function f : critList){
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
