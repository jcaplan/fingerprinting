package codegen.prof;

import java.util.*;

public class BasicBlock {
	
	ArrayList<BasicBlock> parents = new ArrayList<>();
	ArrayList<BasicBlock> children = new ArrayList<>();
	ArrayList<Code> code = new ArrayList<>();
	enum bbType {
		callT,
		returnT,
		branchT,
		indirectCallT
	};
	
	bbType type;

	public void addCode(Code c) {
		code.add(c);
	}
	
	public int getStartAddress(){
		return code.get(0).address;
	}
	
	public String getStartAddressHex(){
		return Integer.toHexString(code.get(0).address);
	}
	
	public void printCode(){
		for(Code c: code){
			System.out.println(c);
		}
	}

	public boolean isEmpty() {
		return code.isEmpty();
	}

	public int getEndAddress() {
		return code.get(code.size()-1).address;
	}
	
	
}
