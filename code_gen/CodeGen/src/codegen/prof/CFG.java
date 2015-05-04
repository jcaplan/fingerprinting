package codegen.prof;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.*;

import javax.management.RuntimeErrorException;

import codegen.prof.BasicBlock.bbType;

public class CFG {

	ArrayList<Function> fList = new ArrayList<>();

	public void addFunction(Function f) {
		fList.add(f);
	}

	public void addFunction(int startAddress, String label) {
		fList.add(new Function(startAddress, label));
	}

	public Function getLastFunction() {
		return fList.get(fList.size() - 1);
	}

	@Override
	public String toString() {
		String s = "";

		s += "**********************\n";
		for (Function f : fList) {
			s += "Function " + f.label;
			s += " (" + Integer.toHexString(f.startAddress) + ")\n";

		}
		return s;
	}

	public void printFunctions() {
		for (Function f : fList) {
			f.printBasicBlocks();
		}
	}
	
	public void printDotCFG(String fileName) throws FileNotFoundException, UnsupportedEncodingException {
		PrintWriter writer = new PrintWriter(fileName + ".dot", "UTF-8");
		writer.println(getDotCFG());
		writer.close();
	}
	
	private String getDotCFG() {
		String s = "digraph d {\n";
		s += "size=\"8.5,7\";\n\n";
//		s += "splines=ortho\n;";
		for (Function f : fList) {
			s += f.printDot();
		}
		s += "}";
		return s;
	}

	public void printFunctionBounds(){
		for(Function f : fList){
			System.out.println("Function " + f.label + "; start address: " + f.getStartAddressHex() +
					"; end address: " + f.getEndAddresshex());
		}
	}

	

	public void build(String fName) {
		//Identify basic blocks
		for(Function f : fList){
			f.buildBlocks();
		}
		
		Function mainF = getFunction(fName);
		
		//Build the tree from main, this should naturally prune the tree
		//of all the extra functions 
		buildCallTree(mainF);
		
		//Delete functions not part of the main call tree
		//buildCallTree is a recursive function so pruning must 
		//be done separately afterwards
		pruneCFG(mainF);
		
		//Do branches after pruning to avoid weird jmp instructions
		//for the preamble stuff
		for(Function f: fList){
			f.splitBasicBlock();
		}
		//TODO
		
		//Connect basic blocks 
		for(Function f : fList){
			f.connectBlocks();
		}
		
		
		//Identify loops
		for(Function f : fList){
			f.findLoops();
		}
		
	}

	public void printLoops(){
		for(Function f : fList){
			f.printLoops();
		}
	}

	private void pruneCFG(Function root) {
		ArrayList<Function> rootList = root.getAllCalledFunctions();
		ArrayList<Function> removeList = new ArrayList<>();
		for(Function f : fList){
			if(!rootList.contains(f)){
				removeList.add(f);
			}
		}
		
		fList.removeAll(removeList);
	}

	private void buildCallTree(Function root) {
		for(BasicBlock bb : root.bbList){
			if(bb.type == bbType.CALL){
				//Need to establish a reference to callee function
				//has to be done at this level
				String[] ops = bb.getCalleeString();
					bb.callee = getFunction(ops[1]);	
					if(!bb.callee.equals(root))
						buildCallTree(bb.callee);
			} else if(bb.type == bbType.JUMP){
				String[] ops = bb.getJumpDest();
				if(ops.length > 1){
					bb.callee = getFunction(ops[1]);
				}
			}
		}
	}

	public Function getFunction(String name) {
		for(Function f : fList){
			if(f.equals(name)){
				return f;
			}
		}
		throw new RuntimeErrorException(new Error(), "Function " + name + " not found in table");
	}
}
