package codegen.prof;


public class Code {

	String line;
	int address;
	String instruction;
	String[] operands;
	CodeType type;
	static enum CodeType {
		callT,
		indirectCallT,
		returnT,
		branchT,
		otherT
	};
	public Code(String line) {
		this.line = line;
		//Parse the line immediately
		parseLine();
	}
	
	private void parseLine(){
		String[] tokens = line.split("[ \t]+");
		address = Integer.parseInt(tokens[1].replace(":", ""),16);
		instruction = tokens[3];
		if(tokens.length > 4){
			operands = tokens[4].split(",");
		}
		
		if(instruction.equals("call")){
			type = CodeType.callT;
		} else if(instruction.equals("callr")){
			type = CodeType.indirectCallT;
		} else if(instruction.equals("ret")){	
			type = CodeType.returnT;
		} else if(instruction.startsWith("b") ||
				instruction.contains("jmp")){
			type = CodeType.branchT;
		} else {
			type = CodeType.otherT;
		}
	}
	
	@Override
	public String toString(){
		String s = "";
		s += "Address: " + Integer.toHexString(address) + "; instruction: " + instruction +
				"; ";		
		if(operands != null){
			s += "operands: ";
			for(String op : operands){
				s += op + ",";
			}
			s = s.substring(0, s.length()-1);
		}
		return s;
	}

}
