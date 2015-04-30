package codegen.prof;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.management.RuntimeErrorException;

import codegen.prof.BasicBlock.bbType;


public class Code {

	String line;
	int address;
	String instruction;
	String[] operands;
	CodeType type;
	static enum CodeType {
		CALL,
		INDIRECTCALL,
		JUMP,
		RETURN,
		COND_BRANCH,
		UNCOND_BRANCH,
		OTHER
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
			type = CodeType.CALL;
			String[] ops = {operands[0], tokens[5]};
			operands = ops;
			operands[1] = parseFuncName(operands[1]);
		} else if(instruction.equals("callr")){
			type = CodeType.INDIRECTCALL;
		} else if(instruction.equals("ret")){	
			type = CodeType.RETURN;
		} else if(instruction.startsWith("b") &&
				!instruction.equals("br")){
			type = CodeType.COND_BRANCH;
		} else if (instruction.equals("br")) {
			type = CodeType.UNCOND_BRANCH;
		} else if (instruction.equals("jmp")){
			type = CodeType.JUMP;
		} else if (instruction.equals("jmpi")){
			type = CodeType.JUMP;
			String[] ops = {operands[0], new String()};
			operands = ops;
			operands[1] = parseFuncName(tokens[5]);
		} else {
			type = CodeType.OTHER;
		}
	}
	
	private String parseFuncName(String name) {
		Pattern pattern = Pattern.compile("<(.*)>");
		Matcher matcher = pattern.matcher(name);
		if (matcher.find()) {
			return matcher.group(1);
		}
		return null;
	}

	public int getBranchDest(){
		if(this.type != CodeType.COND_BRANCH &&
				this.type != CodeType.UNCOND_BRANCH){
			throw new RuntimeErrorException(new Error("Attempted to find a destination for non-branch basic block"));
		}
		return Integer.parseInt(operands[operands.length-1],16);
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
