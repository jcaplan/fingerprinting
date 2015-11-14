package codegen.prof;

import java.util.*;

import javax.management.RuntimeErrorException;

import codegen.prof.Code.CodeType;

public class BasicBlock {
		
		ArrayList<BasicBlock> predecessors = new ArrayList<>();
		ArrayList<BasicBlock> successors = new ArrayList<>();
		ArrayList<Edge> succEdges = new ArrayList<>();
		ArrayList<Edge> predEdges = new ArrayList<>();
		
	ArrayList<Code> code = new ArrayList<>();
	enum bbType {
		CALL,
		RETURN,
		BRANCH,
		INDIRECT_CALL,
		JUMP,
		OTHER
	};
	
	public BasicBlock(){
		type = bbType.OTHER;
	}
	
	Function callee;
	int age = 0;
	bbType type;

	public void addCode(Code c) {
		code.add(c);
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
		if(this.type != bbType.CALL){
			throw new RuntimeErrorException(new Error("Attempted to find a callee for non-call basic block"));
		}
		
		return getLastCode().operands;
		
	}
	
	public String[] getJumpDest() {
		if(this.type != bbType.JUMP){
			throw new RuntimeErrorException(new Error("Attempted to find a destination for non-jump basic block"));
		}
		
		return getLastCode().operands;
	}

	public void addSuccessor(BasicBlock nextBlock) {
		successors.add(nextBlock);
		nextBlock.predecessors.add(this);
	}

	public boolean isConditional() {
		if(this.type != bbType.BRANCH){
			throw new RuntimeErrorException(new Error("Check if block is branch before using this method"));
		}
		return (getLastCode().type == CodeType.COND_BRANCH);
	}
	
	public int getBranchAddress() {
		if(this.type != bbType.BRANCH){
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
				ArrayList<BasicBlock> newList = new ArrayList<>(bbList);
				succ.age = age;
				//Add the successor to the list of seen blocks and repeat
				newList.add(succ);
				succ.setSuccessorAges(age+1,newList);
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
		if(type == bbType.JUMP && callee == null){
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
		return code.size();
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
		return type == bbType.RETURN;
	}

	public int getAge() {
		return age;
	}
}
