package codegen.prof;

import java.util.*;

import codegen.prof.BasicBlock.bbType;

public class Function {

	ArrayList<BasicBlock> bbList = new ArrayList<>();
	int startAddress;
	int endAddress;
	String label;
	ArrayList<Code> code = new ArrayList<>();
	
	public Function(int startAddress, String label){
		this.startAddress = startAddress;
		this.label = label;
	}

	public void addLine(String line) {
		code.add(new Code(line));
		
	}

	public void printcode() {
		System.out.println("******************");
		System.out.println("Function " + label + " code:");
		for(Code l : code){
			System.out.println(l);
		}
		System.out.println("");
		
	}

	public void buildBlocks() {
		
		bbList.add(new BasicBlock());

		//Go through the code until reaching a branch instruction, function call,
		//or final return

		for(Code c : code){
			//Always add the line
			BasicBlock bb = getLastBlock();
			bb.addCode(c);
			//Take different actions depending what the code is
			switch(c.type){
			case callT:
				bb.type = bbType.callT;
				bbList.add(new BasicBlock());
				break;
			case indirectCallT:
				bb.type = bbType.indirectCallT;
				bbList.add(new BasicBlock());
				break;
			case returnT:
				bb.type = bbType.returnT;
				bbList.add(new BasicBlock());
				break;
			case branchT:
				bb.type = bbType.branchT;
				bbList.add(new BasicBlock());
				break;
			case otherT:
				break;
			}
		}
		
		if(getLastBlock().isEmpty())
			bbList.remove(getLastBlock());
		
		endAddress = getLastBlock().getEndAddress();
		
	}
	
	private BasicBlock getLastBlock(){
		return bbList.get(bbList.size()-1);
	}

	public void printBasicBlocks() {
		System.out.println("******************");
		System.out.println("Function " + label + " basic blocks:");
		for(int i = 0; i < bbList.size(); i++){
			BasicBlock bb = bbList.get(i);
			System.out.println("Basic block " + i + "; type: " + bb.type + "; start address: "
					+ bb.getStartAddressHex());
			bb.printCode();
			System.out.println("");
		}
		
		System.out.println("");
		
		
	}
	
	public String getStartAddressHex(){
		return Integer.toHexString(startAddress);
	}
	public String getEndAddresshex(){
		return Integer.toHexString(endAddress);
	}
	
}
