package codegen.prof.flow;

import codegen.prof.Code;

public class PhiCode extends Code{

	String def;
	String defRoot;
	
	public PhiCode(String def, int address, int numOps) {

		super("");
		this.def = def;
		type = CodeType.PHI;
		this.address = address;
		this.instruction = "phi";
		this.operands = new String[numOps];
		for(int i = 0; i < numOps; i++){
			operands[i] = def;
		}
		defRoot = def;
	}
	
	
	public void setMergedDef(int index, int integer){
		operands[index] = defRoot + "_" + integer;
	}
	
	@Override 
	public String toString(){
		String s = super.toString();
		s += " -> " + def;
		return s;
	}
	
	public String getRootString(){
		return defRoot;
	}
	
	public static String getDefKey(String s,int offset){
		String[] tokens = s.split("_");
		if(tokens.length != 2){
			return null;
		}
		String root = tokens[0] + "_" + (Integer.parseInt(tokens[1]) + offset);
		return root;
	}
	
	public static String getDefRoot(String s){
		String[] tokens = s.split("_");
		if(tokens.length != 2){
			return null;
		}
		return tokens[0];
	}

}
