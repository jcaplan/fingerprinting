package codegen.gen;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

import org.apache.commons.io.FileUtils;

public class GenCriticalLibrary {

	

	Configuration config;
	Platform platform;
	ArrayList<Function> funcList;
	ArrayList<Function> fprintList;
	
	public GenCriticalLibrary(Configuration config, ArrayList<Function> funcList, ArrayList<Function> fprintList){
		this.config = config;
		this.platform = config.platform;
		this.funcList = funcList;
		this.fprintList = fprintList;
	}

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
			"	fprint_enable_task(priority);\n"+
			"	" + f + "_step(" + f + "_M, " + f + "_U, " + f + "_Y);\n"+
			"	fprint_disable_task(priority);\n"+
			"}\n"+
			"\n";
		}
		return s;
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
	
	
}
