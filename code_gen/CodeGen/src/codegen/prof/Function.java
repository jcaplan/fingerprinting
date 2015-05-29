package codegen.prof;

import java.util.*;

import codegen.prof.BasicBlock.bbType;
import codegen.prof.Code.CodeType;

public class Function {

	ArrayList<BasicBlock> bbList = new ArrayList<>();
	int startAddress;
	int endAddress;
	String label;
	ArrayList<Code> code = new ArrayList<>();
	ArrayList<Loop> loops = new ArrayList<>();
	
	public Function(int startAddress, String label) {
		this.startAddress = startAddress;
		this.label = label;
	}

	public void addLine(String line) {
		code.add(new Code(line));

	}

	public void printcode() {
		System.out.println("******************");
		System.out.println("Function " + label + " code:");
		for (Code l : code) {
			System.out.println(l);
		}
		System.out.println("");

	}

	public void buildBlocks() {

		bbList.add(new BasicBlock());

		// Go through the code until reaching a branch instruction, function
		// call,
		// or final return

		for (Code c : code) {
			// Always add the line
			BasicBlock bb = getLastBlock();
			bb.addCode(c);
			// Take different actions depending what the code is
			switch (c.type) {
			case CALL:
				bb.type = bbType.CALL;
				bbList.add(new BasicBlock());
				break;
			case INDIRECTCALL:
				bb.type = bbType.INDIRECT_CALL;
				bbList.add(new BasicBlock());
				break;
			case RETURN:
				bb.type = bbType.RETURN;
				bbList.add(new BasicBlock());
				break;
			case COND_BRANCH:
			case UNCOND_BRANCH:
				bb.type = bbType.BRANCH;
				bbList.add(new BasicBlock());
				break;
			case JUMP:
				bb.type = bbType.JUMP;
				bbList.add(new BasicBlock());
				break;
			case OTHER:
				break;
			}
		}
		if (getLastBlock().isEmpty()){
			bbList.remove(getLastBlock());
		}
		endAddress = getLastBlock().getEndAddress();
	}

	public void splitBasicBlock() {
		// Now split blocks that contain destinations of branches
		for (Code c : code) {
			if (c.type == CodeType.COND_BRANCH || 
					c.type == CodeType.UNCOND_BRANCH) {
				// Find the basic block that contains the destination
				for (BasicBlock bb : bbList) {
					if (bb.containsCode(c.getBranchDest())) {
						splitBasicBlock(bb, c.getBranchDest());
						break;
					}
				}
			} 
		}
	}

	private void splitBasicBlock(BasicBlock bb, int cutoff) {
	
		//If branch is to beginning of basic block then there is
		//nothing to do
		if(bb.getStartAddress() == cutoff){
			return;
		}
	
		BasicBlock newBlock1 = new BasicBlock();
		BasicBlock newBlock2 = new BasicBlock();
		
		for (Code cd : bb.code) {
			if (cd.address < cutoff) {
				newBlock1.addCode(cd);
			} else {
				newBlock2.addCode(cd);
			}
		}
		//block 1 is just a plain block, it is separated
		//because it contained a branch destination,
		//it's last instruction is type other -> no action
		
		//block 2 is the same type as the old one
		newBlock2.type = bb.type;
		newBlock2.callee = bb.callee;
		
		int index = bbList.indexOf(bb);
		bbList.remove(bb);
		// Might as well place them in the right position to keep things in
		// order
		bbList.add(index, newBlock1);
		bbList.add(index + 1, newBlock2);
		
		
	}

	private BasicBlock getLastBlock() {
		return bbList.get(bbList.size() - 1);
	}

	public void printBasicBlocks() {
		System.out.println("******************");
		System.out.println("Function " + label + " basic blocks:");
		for (int i = 0; i < bbList.size(); i++) {
			BasicBlock bb = bbList.get(i);
			System.out.println("Basic block " + i + "; type: " + bb.type
					+ "; start address: " + bb.getStartAddressHex());
			System.out.println("Age: " + bb.age);
			System.out.println("Successors: " + bb.getSuccessorString());
			System.out.println("Predecessors: " + bb.getPredecessorString());
			bb.printCode();
			System.out.println("");
		}

		System.out.println("");

	}



	public String getStartAddressHex() {
		return Integer.toHexString(startAddress);
	}

	public String getEndAddresshex() {
		return Integer.toHexString(endAddress);
	}

	@Override
	public boolean equals(Object o) {
		if (o instanceof String) {
			return this.label.equals(o);
		}

		if (!(o instanceof Function)) {
			return false;
		}
		Function f = (Function) o;
		return (f.label.equals(this.label));
	}

	@Override
	public String toString() {
		return this.label;
	}

	public ArrayList<Function> getAllCalledFunctions() {
		ArrayList<Function> fList = new ArrayList<>();
		fList.add(this);
		for (BasicBlock bb : bbList) {
			if (bb.type == bbType.CALL && bb.callee != this) {
				fList.add(bb.callee);
				for (Function f : bb.callee.getAllCalledFunctions()) {
					if (!fList.contains(f)) {
						fList.add(f);
					}
				}
			}
		}
		return fList;
	}

	public BasicBlock getNextBlock(BasicBlock bb) {
		//There is no next block if bb is the last one
		int index = bbList.indexOf(bb);
		if(index == bbList.size()-1)
			return null;
		//otherwise
		return bbList.get(index+1);
	}

	public BasicBlock getBranchDest(BasicBlock bb) {
		int address = bb.getBranchAddress();
		for(BasicBlock block : bbList){
			if(block.getStartAddress() == address){
				return block;
			}
		}
		return null;
	}

	public void connectBlocks() {
		for(BasicBlock bb : bbList){
			/*
			 * call -> successor is next block (not the function call)
			 * return -> no successors
			 * branch -> conditional -> next block and destination
			 * 		  -> unconditional -> only destination
			 * indirect_call -> next block
			 * other -> next block
			 * jumps -> no successors
			 */
			switch(bb.type){
			case CALL:
			case INDIRECT_CALL:
			case OTHER:
				bb.addSuccessor(getNextBlock(bb));
				break;
			case RETURN:
			case JUMP:
				//Return and jump have no successors
				break;
			case BRANCH:
				//add destination					
				bb.addSuccessor(getBranchDest(bb));				
				if(bb.isConditional()){
					//add next block
					bb.addSuccessor(getNextBlock(bb));
				} 
				break;
			}				
		}
		
	}

	public String printDot() {
		String s = "";
		s += "subgraph cluster_" + label + " {\n";
		s += "\tgraph [label = \"" + label + "\"];\n";
		
		ArrayList<BasicBlock> blocks = new ArrayList<>(bbList);
		s += "\tnode_" + bbList.get(0).getStartAddressHex();
		s += " [peripheries=2]\n";
		for(Loop l : loops){
			blocks.removeAll(l.body);
			s += l.getDotGraphString();
		}
		for(BasicBlock bb : blocks){
			s += bb.getDotNodeString();
		}	
		s += "}\n";
		
		for(BasicBlock bb : bbList){
			for(BasicBlock successor : bb.successors){
				s += "node_" + bb.getStartAddressHex() + " -> node_" + successor.getStartAddressHex();
				if(isHeadAndTail(successor,bb)){
					s += "[color=red]";
				}
				s +=  ";\n";
			}
		}
		s +="\n";
		return s;
	}

	private boolean isHeadAndTail(BasicBlock head, BasicBlock tail) {
		for(Loop l : loops){
			if(l.head == head && l.tail == tail)
				return true;
		}
		return false;
	}

	public void findLoops() {

		
		BasicBlock root = bbList.get(0);
		//Each basic block receives an "age" representing its depth in call graph
		root.setAges();
		
		//If the predecessor of a basic block is older, then there is a loop
		for(BasicBlock bb : bbList){
			for(BasicBlock pred : bb.predecessors){
				if(bb.age < pred.age){
					loops.add(new Loop(bb,pred));
				}
			}
			if(bb.predecessors.contains(bb)){
				loops.add(new Loop(bb,bb));
			}
		}
		
	} 
	
	public void printLoops() {
		for(Loop l : loops){
			System.out.println("function " + this + ";");
			System.out.println(l);
		}
	}

}
