package codegen.prof;

import java.util.*;

import javax.management.RuntimeErrorException;

import codegen.prof.Code.CodeType;

public class BasicBlock {
		
		ArrayList<BasicBlock> predecessors;
		ArrayList<BasicBlock> successors;
		ArrayList<Edge> succEdges;
		ArrayList<Edge> predEdges;
		ArrayList<BasicBlock> dominators;
		ArrayList<BasicBlock> domFrontier;
		ArrayList<BasicBlock> domTreeChildren;
		BasicBlock immDominator;
		Function callee;
		int maxFalseBranch = -1;
		int age = 0;
		BbType type;
		ArrayList<Code> code;
		private Loop innerLoop;
		int numLinesCode = 0;
		
	enum BbType {
		CALL,
		RETURN,
		BRANCH,
		INDIRECT_CALL,
		JUMP,
		OTHER
	};
	
	public BasicBlock(){
		predecessors = new ArrayList<>();
		successors = new ArrayList<>();
		succEdges = new ArrayList<>();
		predEdges = new ArrayList<>();
		code = new ArrayList<>();
		dominators = new ArrayList<>();
		domFrontier = new ArrayList<>();
		domTreeChildren = new ArrayList<>();
		type = BbType.OTHER;
	}
	

	
	public BasicBlock(BasicBlock bb){
		this.type = bb.type;
		this.predecessors = new ArrayList<>(bb.successors);
		this.successors = new ArrayList<>(bb.predecessors);
		this.succEdges = new ArrayList<>(bb.succEdges);
		this.predEdges = new ArrayList<>(bb.predEdges);
		this.dominators = new ArrayList<>(bb.dominators);
		this.domFrontier = new ArrayList<>(bb.domFrontier);
		this.domTreeChildren = new ArrayList<>(bb.domTreeChildren);
		if(callee != null){
			this.callee = bb.callee;
		}
		this.age = bb.age;
		this.code = new ArrayList<>(bb.code);
	}
	

	public void addCode(Code c) {
		code.add(c);
		if(c.type != CodeType.PHI){
			numLinesCode++;
		}
	}
	
	public void addCodeToFront(Code c){
		code.add(0, c);
		if(c.type != CodeType.PHI){
			numLinesCode++;
		}
	}
	
	public int getStartAddress(){
		return code.get(0).address;
	}
	
	public String getStartAddressHex(){
		return code.get(0).getAddressHex();
	}
	
	public void printCode(){
		for(Code c: code){
			System.out.println(c);
		}
	}

	public boolean isEmpty() {
		return code.isEmpty();
	}

	public Code getLastCode(){
		return code.get(code.size()-1);
	}
	
	public int getEndAddress() {
		return getLastCode().address;
	}
	
	public boolean containsCode(Code c){
		return c.address >= getStartAddress() && c.address <= getEndAddress() ;
	}
	
	public boolean containsCode(int address){
		return address >= getStartAddress() && address <= getEndAddress() ;
	}

	/**
	 * 
	 * @return an array of size two, first the address, then the function name
	 */
	public String[] getCalleeString() {
		if(this.type != BbType.CALL){
			throw new RuntimeErrorException(new Error("Attempted to find a callee for non-call basic block"));
		}
		
		return getLastCode().operands;
		
	}
	
	public String[] getJumpDest() {
		if(this.type != BbType.JUMP){
			throw new RuntimeErrorException(new Error("Attempted to find a destination for non-jump basic block"));
		}
		
		return getLastCode().operands;
	}

	public void addSuccessor(BasicBlock nextBlock) {
		successors.add(nextBlock);
		nextBlock.predecessors.add(this);
	}

	public boolean isConditional() {
		if(this.type != BbType.BRANCH){
			throw new RuntimeErrorException(new Error("Check if block is branch before using this method"));
		}
		return (getLastCode().type == CodeType.COND_BRANCH);
	}
	
	public int getBranchAddress() {
		if(this.type != BbType.BRANCH){
			throw new RuntimeErrorException(new Error("Check if block is branch before using this method"));
		}
		
		Code c = getLastCode();
		int address = 0;
		if(c.type == CodeType.COND_BRANCH){
			address = Integer.parseInt(c.operands[2],16);
		} else {
			address = Integer.parseInt(c.operands[0],16);
		}
		
		return address;
	}
	
	public String getPredecessorString() {
		return buildAddressString(predecessors);
	}

	public String getSuccessorString() {
		return buildAddressString(successors);
	}
	
	private String buildAddressString(ArrayList<BasicBlock> bbList){
		String s = "";
		for(BasicBlock bb : bbList){
			s += bb.getStartAddressHex() + ",";
		}
		if(s.length() > 0) 
			s = s.substring(0, s.length()-1);
		return s;
	}
	
	@Override
	public String toString(){
		return "BB @" + getStartAddressHex();
	}

	public void setAges() {
		if(!predecessors.isEmpty()){
			throw new RuntimeErrorException(new Error("Set age should be from root node only (no predecessors)"));
		}
		//this is root node. Set age to 1
		age = 1;
		ArrayList<BasicBlock> bbList = new ArrayList<>();
		bbList.add(this);
		setSuccessorAges(age + 1,bbList);
		
	}

	private void setSuccessorAges(int age, ArrayList<BasicBlock> bbList) {
		
		for(BasicBlock succ : successors){
			//If this bb has already been seen on the current path (DFS)
			//don't bother changing it's age
			//If it has been seen on a different path through the tree,
			//then the oldest age wins
			if( !bbList.contains(succ) && succ.age < this.age){
				bbList.add(succ);
				succ.age = age;
				//Add the successor to the list of seen blocks and repeat

				succ.setSuccessorAges(age+1,bbList);
				bbList.remove(succ);
			}
		}
		
	}

	public String getDotNodeString() {
		String s = "";
		s += "\tnode_" + getStartAddressHex();
		s += " [label = \"@0x" + getStartAddressHex();
		switch(type){
		case BRANCH:
			s += "(BB)";
			break;
		case CALL:
			s += "(Call " + callee + ")";
			break;
		case INDIRECT_CALL:
			s += "(Call ?)";
			break;
		case JUMP:
			s += "(Jump " + callee + ")";
			break;
		case OTHER:
			s += "(BB)";
			break;
		case RETURN:
			s += "(Return)";
			break;
		default:
			break;
		
		}
		s += "\"";
		if(type == BbType.JUMP && callee == null){
			s += ", color=red";
		}
		if(predecessors.isEmpty()){
			s += ", color=blue";
		}
				
		s += "];\n";
		
		return s;
	}

	public int getStackSize() {
		Code c = code.get(0); 
		int size = 0;
		if(c.instruction.equals("addi") && c.operands[1].equals("sp")){
			size = -Integer.parseInt(c.operands[2]);
		}
		return size;
	}

	public int getNumInstructions(){
		//exclude the PHI code
		return numLinesCode;
	}
	
	public int getCost(int memoryDelay) {
		
		int cost = 0;
		for(Code c : code){
			
			switch(c.type){
			case COND_BRANCH:
			case UNCOND_BRANCH:
			case CALL:
			case RETURN:
			case JUMP:
				cost += 4;
				break;
			case STORE:
				cost += memoryDelay;
				break;
			case LOAD:
				cost += memoryDelay;
				break;
			case CUSTOM:
				switch(c.customInstructionID){
				case 255:
					cost += 16;
					break;
				case 254:
				case 253:
					cost += 5;
					break;
				case 252:
				case 250:
					cost += 4;
					break;
				case 251:
					cost += 8;
					break;
				case 249:
				case 248:
					cost += 2;
				default:
					cost += 1;
				}
				
				break;
			default:
				cost += 1;
				break;
				
			}
			
			
		}
		return cost;
		//The cost of instructions
		
		// Costs: Branch -> 4 (Assume mispredicted)
		// jmp,call,return -> 4
		// load,store -> 1 + memory delay
		// else -> 1
	}
	
	public ArrayList<Code> getCode(){
		return new ArrayList<>(code);
	}

	public ArrayList<BasicBlock> getSuccesors() {
		return successors;
	}
	
	public boolean isReturn(){
		return type == BbType.RETURN;
	}

	public int getAge() {
		return age;
	}

	public void removeSuccessor(BasicBlock bb) {
		successors.remove(bb);
	}

	public void removePredecessor(BasicBlock bb) {
		predecessors.remove(bb);
		
	}



	public boolean isBranch() {
		return this.type == BbType.BRANCH;
	}



	public void setMaxFalseBranch(int maxIterations) {
		maxFalseBranch = maxIterations;
	}



	public int getMaxFalseBranch() {
		return maxFalseBranch;
	}





	public int getFalseEdgeIndex() {
		for(Edge e : succEdges){
			if(e.endBlock.getStartAddress() == this.getEndAddress() + 4){
				return e.index;
			}
		}
		return -1;
	}

	public void setInnerLoop(Loop l) {
		innerLoop = l;
		
	}
	public Loop getInnerLoop(){
		return innerLoop;
	}



	public void setDominators(List<BasicBlock> list) {
		dominators = (ArrayList<BasicBlock>) list;
	}



	public List<BasicBlock> getDomFrontier() {
		return domFrontier;
	}
	

	public void setDomFrontier(List<BasicBlock> list) {
		domFrontier = (ArrayList<BasicBlock>) list;
	}



	public List<BasicBlock> getDomTreeChildren() {
		return domTreeChildren;
	}

	public void setDomTreeChildren(List<BasicBlock> list) {
		domTreeChildren = (ArrayList<BasicBlock>) list;
	}



	public List<BasicBlock> getDominators() {
		return dominators;
	}



	public void setImmDominator(BasicBlock idom) {
		immDominator = idom;
	}
	
	public BasicBlock getImmDominator(){
		return immDominator;
	}



	public void addToDomFrontier(BasicBlock succ) {
		if(!domFrontier.contains(succ)){
			domFrontier.add(succ);
		}
	}



	public List<BasicBlock> getPredecessors() {
		return predecessors;
	}



	public int getPredIndex(BasicBlock node) {
		return predecessors.indexOf(node);
	}
}
