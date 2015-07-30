package codegen.gen;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;

public class GenScripts {

	Configuration config;
	
	public GenScripts(Configuration config){
		this.config = config;
	}
	
	public void generateScripts(){
		try {
			generateAppScript();
			generateRunScript();
			generateCompileBSPScript();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}
	
	private void generateAppScript() throws IOException {
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
	
	private void generateRunScript() throws IOException {
		PrintWriter writer;
		File output = new File(config.outputDir + "/RUN.sh");
		writer = new PrintWriter(output);
		writer.print(getRunScriptString());
		
		writer.close();

		
		
		output.setExecutable(true);
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
	
	
}
