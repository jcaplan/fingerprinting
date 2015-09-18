package codegen.prof;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;

import javax.management.RuntimeErrorException;

import codegen.prof.BasicBlock.bbType;
import lpsolve.*;

public class Profiler {

	
	CFG cfg;
	ArrayList<Annotation> annotations;
	Parser parser;
	String fileDir;
	String rootName;
	
	public Profiler(String fileDir, String rootName){
		this.fileDir = fileDir;
		this.rootName = rootName;
		parser = new Parser(fileDir,rootName);
		Edge.EdgeCount = Edge.RESET_VALUE;
	}
	
	public void parseFile(String topName){
		try {
			annotations = parser.parseAnnotations();
			cfg = parser.parseCFG(topName,annotations);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	


	public int getWCET(String funcName, boolean cycleAccurate) {
		
		IpetAnalysis ipet = new IpetAnalysis(funcName,cfg);
		int wcet = 0;
		try {
			wcet = ipet.getWCET(cycleAccurate);
		} catch (LpSolveException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return wcet;
	}

	
	public int getMaxStackSize(String name) {
		Function f = cfg.getFunction(name);
		return getMaxStackSize(f);
	}

	private int getMaxStackSize(Function f) {
		int size = 0;
		int maxCalledSize = 0;
		for(BasicBlock bb : f.bbList){
			size += bb.getStackSize();
			if(bb.type == bbType.CALL || bb.type == bbType.JUMP){
				//Don't add up for all functions, only add up for the maximum function that was called
				//(Matters when lots of functions are called)
				if(bb.callee != null && !bb.callee.equals(f)){
					int callSize = getMaxStackSize(bb.callee);
					if(callSize > maxCalledSize)
						maxCalledSize = callSize;
				}
			}
		}
		size += maxCalledSize;
		return size;
	}
	
	
	


	private String getLibs(String entryPoint) {
		String s = "";
		ArrayList<String> oldLibs = new ArrayList<>();
		for(Function f : cfg.getFunction(entryPoint).getAllCalledFunctions()){
			String lib[] = Library.getLibrary(f.label);
			if(lib != null && !oldLibs.contains(lib[1])){
				oldLibs.add(lib[1]);
				s += "*/" + lib[0] + ":" + lib[1] + "\n";
			}
		}
		return s;
	}

	private String getLibFunctions() {
		String s = "";
		for(Function f : cfg.fList){
			if(Library.inLibrary(f.label)){
				s += f.label + ",";
			}
		}
		return s;
	}


	public static void main(String[] args) throws IOException{
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
		System.out.println("*******************************************************************");
		System.out.println("results for " + rootName + " from function " + entryPoint + "\n");
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
//		System.out.println("Max stack height of " + entryPoint + ": " + prof.getMaxStackSize(entryPoint) + " bytes");
		System.out.println("WCET of " + entryPoint + ": " + prof.getWCET(entryPoint,true) + " clock cycles");
//		System.out.println("Library functions are: " + prof.getLibFunctions());
//		System.out.println("Add the following lines to linker file: ");
//		System.out.println(prof.getLibs(entryPoint));
//		System.out.println("\n");
		
		
	}	
	
}
