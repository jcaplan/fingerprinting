package codegen.test;

import java.io.FileNotFoundException;
import java.io.UnsupportedEncodingException;

import javax.management.RuntimeErrorException;

import codegen.prof.Profiler;
import codegen.prof.flow.LoopAnalysis;

public class LoopAnalysisTest {
public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException{
		
		if(args.length != 3){
			throw new RuntimeErrorException(new Error("Usage java -jar Codegen.jar directory rootName entryFunction"));
			
		}
		
		
		
		/*
		 * Three arguments are required:
		 * 1. The directory where the files to be parsed are located
		 * 2. The file root name. The two files should be called rootName.objdump
		 *    and rootName.annot
		 * 3. The entry function for the analysis.
		 * 
		 * 
		 */
		String fileDir=args[0];
		String rootName = args[1];
		String entryPoint = args[2];
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		prof.getCfg().printDotCFG(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		la.analyze();
	}
}
