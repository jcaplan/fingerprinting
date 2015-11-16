package codegen.prof.flow;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import codegen.prof.BasicBlock;
import codegen.prof.Code;
import codegen.prof.Function;

public class ExpressionBuilder extends ForwardAnalysis<Map<String, Expression>>{

	

	public ExpressionBuilder(Function root) {
		super(root);
	}

	public ExpressionBuilder(List<BasicBlock> workList) {
		super(workList);
	}

	@Override
	protected HashMap<String,Expression> initSet() {
		return new HashMap<String,Expression>();
	}

	@Override
	protected Map<String,Expression> caseLoad(Code c, Map<String,Expression> c_in, BasicBlock succ) {
		HashMap<String,Expression> c_out = new HashMap<>(c_in);
		String[] ops = c.getOperands();
		
		c_out.remove(ops[0]);
		
		if(c_out.containsKey(ops[1])){
			c_out.put(ops[0], c_out.get(ops[1]));
		} else {
			c_out.put(ops[0], convertOpToExp(ops[1]));
		}
		
		
		return c_out;
		
	};
	
	@Override 
	protected Map<String,Expression> caseMoveOp(Code c, Map<String,Expression> c_in, BasicBlock succ) {
		HashMap<String,Expression> c_out = new HashMap<>(c_in);
		String[] ops = c.getOperands();
		
		c_out.remove(ops[0]);
		
		if(c_out.containsKey(ops[1])){
			c_out.put(ops[0], c_out.get(ops[1]));
		} else {
			c_out.put(ops[0], convertOpToExp(ops[1]));
		}
		
		
		return c_out;
	}
	@Override
	protected Map<String,Expression> caseStore(Code c, Map<String,Expression> c_in, BasicBlock succ) {
		HashMap<String,Expression> c_out = new HashMap<>(c_in);
		String[] ops = c.getOperands();
		
		c_out.remove(ops[1]);
		
		if(c_out.containsKey(ops[0])){
			c_out.put(ops[1], c_out.get(ops[0]));
		} else {
			c_out.put(ops[1], convertOpToExp(ops[0]));
		}
		
		
		return c_out;
		
	};
	
	@Override
	protected Map<String,Expression> caseCompareOp(Code c, Map<String,Expression> c_in, BasicBlock succ) {
		HashMap<String,Expression> c_out = new HashMap<>(c_in);
		String[] ops = c.getOperands();
		Expression op1 = c_out.get(ops[1]);
		Expression op2 = c_out.get(ops[2]);
		
		if(op1 == null){
			op1 = convertOpToExp(ops[1]);
		}
		if(op2 == null){
			op2 = convertOpToExp(ops[2]);
		}
		
		ExpCompareOp exp = new ExpCompareOp(c.getInstruction());
		exp.setLHS(op1);
		exp.setRHS(op2);
		c_out.put(ops[0], exp);
		
		return c_out;
	};
	
	@Override
	protected Map<String,Expression> caseBinOp(Code c, Map<String,Expression> c_in, BasicBlock succ) {
		HashMap<String,Expression> c_out = new HashMap<>(c_in);
		String[] ops = c.getOperands();
		Expression op1 = c_out.get(ops[1]);
		Expression op2 = c_out.get(ops[2]);
		
		if(op1 == null){
			op1 = convertOpToExp(ops[1]);
		}
		if(op2 == null){
			op2 = convertOpToExp(ops[2]);
		}
		
		ExpBinOp exp = new ExpBinOp(c.getInstruction());
		exp.setLHS(op1);
		exp.setRHS(op2);
		c_out.put(ops[0], exp);
		
		return c_out;
	};
	
	@Override
	protected Map<String,Expression> caseCondBranch(Code c, Map<String,Expression> c_in, BasicBlock succ) {
		HashMap<String,Expression> c_out = new HashMap<>(c_in);
		String[] ops = c.getOperands();
		Expression op1 = c_out.get(ops[0]);
		Expression op2 = c_out.get(ops[1]);
		
		if(op1 == null){
			op1 = convertOpToExp(ops[0]);
		}
		if(op2 == null){
			op2 = convertOpToExp(ops[1]);
		}
		
		ExpBranchCond exp = new ExpBranchCond(c.getInstruction());
		exp.setLHS(op1);
		exp.setRHS(op2);
		String key = c.getCodeKey();
		c_out.put(key, exp);
		
		return c_out;
	}



	@Override
	protected Map<String, Expression> merge(Map<String, Expression> set1,
			Map<String, Expression> set2) {
		if(set2 == null){
			return set1;
		} else if (set1 == null){
			return set2;
		}

		HashMap<String,Expression> out = new HashMap<>(set1);
		
		for(String key : set2.keySet()){
			if(!set1.containsKey(key)){ //bottom is implied by no key stored yet
				out.put(key,set2.get(key));
			} else if(!set1.get(key).equals(set2.get(key))){ //if the keys are equal all is good, otherwise wipe it out
				out.put(key, Expression.getNewTop());
			}
		}
		
		return out;
	}
	



	@Override
	protected Map<String, Expression> copy(Map<String, Expression> original) {
		return new HashMap<String, Expression>(original);
	}
	

	private Expression convertOpToExp(String op) {
		if(op.equals("zero")){
			return new ExpConstant(0); 
		}
		
		try{
			int value;
			value = Integer.parseInt(op);
			return new ExpConstant(value);
		} catch (NumberFormatException e){
			//not a number
		}
		
		return new ExpIdentifier(op);
			
			
		
	}


	
}
