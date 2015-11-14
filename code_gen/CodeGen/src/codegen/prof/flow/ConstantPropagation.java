package codegen.prof.flow;

import java.util.HashMap;
import java.util.Map;

import codegen.prof.BasicBlock;
import codegen.prof.Code;
import codegen.prof.Function;

public class ConstantPropagation extends ForwardAnalysis<Map<String,Expression>>{

	public ConstantPropagation(Function root) {
		super(root);
	}

	@Override
	protected void initCodeInSet(Code c) {
		codeInSet.put(c, new HashMap<String,Expression>());
	}

	@Override
	protected void initBasicBlockInSet(BasicBlock bb) {
		bbInSet.put(bb, new HashMap<String,Expression>());
		
	}

	@Override
	protected Map<String, Expression> merge(Map<String, Expression> set1,
			Map<String, Expression> set2) {
		
		HashMap<String, Expression> result = new HashMap<>(set1);
		
		for(String key : set2.keySet()){
			if(!result.containsKey(key)){
				result.put(key, set2.get(key));
			} else if (!result.get(key).equals(set2.get(key))){ //intersection requires both sides equal
				result.put(key, Expression.getNewTop());
			}
		}
		
		return result;
		
	}
	
	@Override 
	protected Map<String, Expression> caseStore(Code c, Map<String, Expression> c_in,
			BasicBlock succ) {
		

		HashMap<String,Expression> c_out = new HashMap<>(c_in);
		String[] ops = c.getOperands();
		
		//remove any old ones
		c_out.remove(ops[1]);
		
		if(ops[0].equals("zero")){				 			// zero is spelled out
			c_out.put(ops[1], new ExpConstant(0));
		} else if(c_out.containsKey(ops[0])){				// op0 already in table
				c_out.put(ops[1], c_out.get(ops[0]));
		} else {												//try parsing op1
			try{
				int value = Integer.parseInt(ops[0]);
				c_out.put(ops[1], new ExpConstant(value));
			} catch (NumberFormatException e){
				c_out.put(ops[1], Expression.getNewTop());
				//not a number, not in table, so nothing...
			}
		}
		
		return c_out;
		
	}


	@Override 
	protected Map<String, Expression> caseLoad(Code c, Map<String, Expression> c_in,
			BasicBlock succ) {
		
		HashMap<String,Expression> c_out = new HashMap<>(c_in);
		String[] ops = c.getOperands();
		
		c_out.remove(ops[0]);
		
		if(c_out.containsKey(ops[1])){
			c_out.put(ops[0], c_out.get(ops[1]));
		} else {
			c_out.put(ops[0], Expression.getNewTop());
		}
		
		
		return c_out;
	}
	
	@Override 
	protected Map<String, Expression> caseMoveOp(Code c, Map<String, Expression> c_in,
			BasicBlock succ) {
		
		HashMap<String,Expression> c_out = new HashMap<>(c_in);
		String[] ops = c.getOperands();
		
		c_out.remove(ops[0]);
		
		if(c_out.containsKey(ops[1])){
			c_out.put(ops[0], c_out.get(ops[1]));
		} else {
			c_out.put(ops[0], getConstant(ops[1]));
		}
		
		
		return c_out;
	}
	
//	@Override
//	protected Map<String,Expression> caseCompareOp(Code c, Map<String,Expression> c_in, BasicBlock succ) {
//		HashMap<String,Expression> c_out = new HashMap<>(c_in);
//		String[] ops = c.getOperands();
//		Expression op1 = c_out.get(ops[1]);
//		Expression op2 = c_out.get(ops[2]);
//		
//		if(op1 == null){
//			op1 = getConstant(ops[1]);
//		}
//		if(op2 == null){
//			op2 = getConstant(ops[2]);
//		}
//		
//		
//		
//		return c_out;
//	};
	
	@Override
	protected Map<String,Expression> caseBinOp(Code c, Map<String,Expression> c_in, BasicBlock succ) {
		HashMap<String,Expression> c_out = new HashMap<>(c_in);
		//TODO only plus right now
		String[] ops = c.getOperands();
		Expression op1 = c_out.get(ops[1]);
		Expression op2 = c_out.get(ops[2]);
		
		if(op1 == null){
			op1 = getConstant(ops[1]);
		}
		if(op2 == null){
			op2 = getConstant(ops[2]);
		}
		
		if(!op1.isTop() && !op2.isTop()){
			Expression result;
			result = Expression.sum((ExpConstant)op1, 
					(ExpConstant)op2); 
			c_out.put(ops[0], result);
		} else {
			c_out.put(ops[0],Expression.getNewTop());
		}
				
		return c_out;
	};
	
	private Expression getConstant(String op) {
		Integer x = null;
		try{
			x = Integer.parseInt(op);
		} catch (NumberFormatException e){
			//not a number;
		}
		
		if(x == null){
			return Expression.getNewTop();
		} else {
			return new ExpConstant(x);
		}
		
	}

//	@Override
//	protected Map<String, Expression> caseBinOp(Code c, Map<String, Expression> c_in,
//			BasicBlock succ){
//	
//		HashMap<String,Expression> c_out = new HashMap<>(c_in);
//		String ins = c.getInstruction();
//		String[] ops = c.getOperands();
//		
//		switch(ins){
//		case "addi":
//				int i = Integer.parseInt(ops[2]);   // know that operand is integer
//				if(c_out.containsKey(ops[1]) && c_out.get(ops[1]).getStatus() != Expression.TOP){
//					Expression result = new ExpConstant(((ExpConstant) c_out.get(ops[1])).value + i);
//					c_out.put(ops[0], result);
//				} else {
//					c_out.put(ops[0], Expression.getNewTop());
//				}
//			break;
//		default:
//			break;
//		}
//		
//		return c_out;
//	}
	
	@Override
	protected Map<String, Expression> copy(Map<String, Expression> original) {
		return new HashMap<>(original);
	}
//	
//	
//	protected static class Expression {
//		int type;
//		static final int TOP = 0;
//		static final int Expression = 1;
//		static final int BOTTOM = 2;
//		
//		int constValue;
//		
//		private Expression(int type, int constValue){
//			this.type = type;
//			this.constValue = constValue;
//		}
//		
//		@Override
//		public String toString(){
//			String s = "";
//			if(type == TOP){
//				s = "T";
//			} else if (type == Expression){
//				s = Integer.toString(constValue);
//			}
//			return s;
//		}
//		
//		@Override 
//		public boolean equals(Object o){
//			if(!(o instanceof Expression))
//				return false;
//			
//			Expression c = (Expression) o;
//			
//			return c.constValue == this.constValue;
//		}
//		
//		private static Expression getNewTop(){
//			return new Expression(TOP, -1);
//		}
//		
//		private static Expression getNewConstant(int value){
//			return new Expression(Expression, value);
//		}
//		
//		private boolean hasConstantValue(){
//			return type == Expression;
//		}
//		
//		private boolean hasTopType(){
//			return type == TOP;
//		}
//	}
	//Ignore operations on stack and frame pointer,
	//Assume Expression for



}
