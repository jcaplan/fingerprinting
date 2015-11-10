package codegen.prof.flow;

import java.util.HashMap;
import java.util.Map;

import codegen.prof.BasicBlock;
import codegen.prof.Code;
import codegen.prof.Function;

public class ConstantPropagation extends ForwardAnalysis<Map<String,ConstantPropagation.Constant>>{

	public ConstantPropagation(Function root) {
		super(root);
	}

	@Override
	protected void initCodeInSet(Code c) {
		codeInSet.put(c, new HashMap<String,Constant>());
	}

	@Override
	protected void initBasicBlockInSet(BasicBlock bb) {
		bbInSet.put(bb, new HashMap<String,Constant>());
		
	}

	@Override
	protected Map<String, Constant> merge(Map<String, Constant> set1,
			Map<String, Constant> set2) {
		
		HashMap<String, Constant> result = new HashMap<>(set1);
		
		for(String key : set2.keySet()){
			if(!result.containsKey(key)){
				result.put(key, set2.get(key));
			} else if (result.get(key) != set2.get(key)){ //intersection requires both sides equal
				result.put(key, Constant.getNewTop());
			}
		}
		
		return result;
		
	}
	
	@Override 
	protected Map<String, Constant> caseStore(Code c, Map<String, Constant> c_in,
			BasicBlock succ) {
		

		HashMap<String,Constant> c_out = new HashMap<>(c_in);
		String[] ops = c.getOperands();
		
		//remove any old ones
		c_out.remove(ops[1]);
		
		if(ops[0].equals("zero")){				 			// zero is spelled out
			c_out.put(ops[1], Constant.getNewConstant(0));
		} else if(c_out.containsKey(ops[0])){				// op0 already in table
				c_out.put(ops[1], c_out.get(ops[0]));
		} else {												//try parsing op1
			try{
				int value = Integer.parseInt(ops[0]);
				c_out.put(ops[1], Constant.getNewConstant(value));
			} catch (NumberFormatException e){
				c_out.put(ops[1], Constant.getNewTop());
				//not a number, not in table, so nothing...
			}
		}
		
		return c_out;
		
	}


	@Override 
	protected Map<String, Constant> caseLoad(Code c, Map<String, Constant> c_in,
			BasicBlock succ) {
		
		HashMap<String,Constant> c_out = new HashMap<>(c_in);
		String[] ops = c.getOperands();
		
		c_out.remove(ops[0]);
		
		if(c_out.containsKey(ops[1])){
			c_out.put(ops[0], c_out.get(ops[1]));
		} else {
			c_out.put(ops[0], Constant.getNewTop());
		}
		
		
		return c_out;
	}
	
	@Override
	protected Map<String, Constant> caseBinOp(Code c, Map<String, Constant> c_in,
			BasicBlock succ){
	
		HashMap<String,Constant> c_out = new HashMap<>(c_in);
		String ins = c.getInstruction();
		String[] ops = c.getOperands();
		
		switch(ins){
		case "addi":
				int i = Integer.parseInt(ops[2]);   // know that operand is integer
				if(c_out.containsKey(ops[1]) && c_out.get(ops[1]).hasConstantValue()){
					Constant result = Constant.getNewConstant(c_out.get(ops[1]).constValue + i);
					c_out.put(ops[0], result);
				} else {
					c_out.put(ops[0], Constant.getNewTop());
				}
			break;
		default:
			break;
		}
		
		return c_out;
	}
	
	@Override
	protected Map<String, Constant> copy(Map<String, Constant> original) {
		return new HashMap<>(original);
	}
	
	
	protected static class Constant {
		int type;
		static final int TOP = 0;
		static final int CONSTANT = 1;
		static final int BOTTOM = 2;
		
		int constValue;
		
		private Constant(int type, int constValue){
			this.type = type;
			this.constValue = constValue;
		}
		
		@Override
		public String toString(){
			String s = "";
			if(type == TOP){
				s = "T";
			} else if (type == CONSTANT){
				s = Integer.toString(constValue);
			}
			return s;
		}
		
		@Override 
		public boolean equals(Object o){
			if(!(o instanceof Constant))
				return false;
			
			Constant c = (Constant) o;
			
			return c.constValue == this.constValue;
		}
		
		private static Constant getNewTop(){
			return new Constant(TOP, -1);
		}
		
		private static Constant getNewConstant(int value){
			return new Constant(CONSTANT, value);
		}
		
		private boolean hasConstantValue(){
			return type == CONSTANT;
		}
		
		private boolean hasTopType(){
			return type == TOP;
		}
	}
	//Ignore operations on stack and frame pointer,
	//Assume constant for



}
