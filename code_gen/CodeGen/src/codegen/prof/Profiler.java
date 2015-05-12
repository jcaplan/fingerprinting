package codegen.prof;

import java.io.IOException;
import java.util.ArrayList;

import javax.management.RuntimeErrorException;

import codegen.prof.BasicBlock.bbType;

public class Profiler {

	
	CFG cfg;
	Parser parser;
	public Profiler(String filename){
		parser = new Parser(filename);
	}
	
	public void parseFile(String topName){
		try {
			cfg = parser.parse(topName);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	


	private int getWCET(String string) {
		// TODO Auto-generated method stub
		// 1. Get the root block
		// 2. Traverse the successors, calculating the WCET of each block
		// 3. If the block is a call or jump and the callee is known
		//    calculate and add the WCET of the callee
		// 4. If the block is the head of a loop, then calculate the WCET of the loop
		//    The loop may have a specified max iterations later on, for now assume a number
		//    The loop WCET will be MAXITER*WCET. If there are multiple exit points, then the loop is
		//    WCET*(MAXITER - 1) + maxWCET(successors)
		
		//Costs: Branch -> 4 (Assume mispredicted)
		//       jmp,call,return -> 4
		//       load,store -> 1 + memory delay
		//       else -> 1
		
		
		return 0;
	}

	
	private int getMaxStackSize(String name) {
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
		if(args.length != 2){
			throw new RuntimeErrorException(new Error("Usage java -jar Codegen.jar file entry_function"));
			
		}
		
		String filename=args[0];
		String entryPoint = args[1];
		System.out.println("results for " + filename + " from function " + entryPoint);
		Profiler prof = new Profiler(filename);
		prof.parseFile(entryPoint);
		System.out.println("*******************************************************************");
		System.out.println("Max stack height of " + entryPoint + ": " + prof.getMaxStackSize(entryPoint) + " bytes");
		System.out.println("WCET of " + entryPoint + ": " + prof.getWCET(entryPoint) + " clock cycles");
		System.out.println("Library functions are: " + prof.getLibFunctions());
		System.out.println("Add the following lines to linker file: ");
		System.out.println(prof.getLibs(entryPoint));
		System.out.println("\n");
		
		
	}	
	
}
