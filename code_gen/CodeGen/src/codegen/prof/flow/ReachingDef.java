package codegen.prof.flow;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;

import codegen.prof.BasicBlock;
import codegen.prof.Code;
import codegen.prof.Function;
import codegen.prof.Code.CodeType;

public class ReachingDef extends ForwardAnalysis<HashMap<String,List<Expression>>>{

	public ReachingDef(Function root) {
		super(root);
	}
	
	public ReachingDef(List<BasicBlock> workList) {
		super(workList);
	}

	@Override
	protected HashMap<String, List<Expression>> initSet() {
		return new HashMap<>();
	}
	
	@Override
	protected void analysisPreprocessing() {
		//for every line of code create a bottom
		HashMap<String,List<Expression>> initSet = bbInSet.get(func.getBasicBlockList().get(0));
		for(BasicBlock bb : bbList){
			for(Code c : bb.getCode()){
				if(c.getType() == CodeType.COND_BRANCH || c.getType() == CodeType.UNCOND_BRANCH){
					continue;
				}
				ArrayList<Expression> expList = new ArrayList<>();
				expList.add(Expression.getNewBottom());
				String[] operands = c.getOperands();
				if(operands != null){
					initSet.put(c.getOperands()[0],expList	);
					if(c.getType() == CodeType.STORE){
						expList = new ArrayList<>();
						expList.add(Expression.getNewBottom());
						initSet.put(c.getOperands()[1], expList);
						
					}
				}
			}
		}
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	@Override
	protected HashMap<String, List<Expression>> merge(
			HashMap<String, List<Expression>> set1, HashMap<String, List<Expression>> set2) {

		//copy and sort the sets...
		HashMap<String, List<Expression>> copySet1 = copy(set1);
		HashMap<String, List<Expression>> copySet2 = copy(set2);
		
		//sort both sets
		for(List l : copySet1.values()){
			Collections.sort(l);
		}
		for(List l : copySet2.values()){
			Collections.sort(l);
		}
		
		
		HashMap<String, List<Expression>> result = new HashMap<>(copySet1);
		
		for(String key : copySet2.keySet()){
			if(copySet1.containsKey(key)){
				//if they both contain key then union lists
				List<Expression> resultList = result.get(key);
				for(Expression exp : copySet2.get(key)){
					if(!resultList.contains(exp)){
						resultList.add(exp);
					}
				}
			} else {
				result.put(key,copySet2.get(key));
			}
		}
		
		return result;
	}

	@Override
	protected HashMap<String, List<Expression>> copy(
			HashMap<String, List<Expression>> original) {
		HashMap<String, List<Expression>> result = new HashMap<>();
		for(String key : original.keySet()){
			result.put(key, new ArrayList<>(original.get(key)));
		}
		return result;
	}


	@Override
	protected HashMap<String, List<Expression>> caseLoad(Code c,
			HashMap<String, List<Expression>> c_in, BasicBlock succ) {
		HashMap<String, List<Expression>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		
		Expression op1 = getOpExpression(c_out, ops[1]);
		
		if(c_out.containsKey(ops[0])){
			List<Expression> list = c_out.get(ops[0]);
			list.clear();
			list.add(op1);
		} else {
			ArrayList<Expression> list = new ArrayList<>();
			list.add(op1);
			c_out.put(ops[0], list);
		}
		return c_out;

	}

	@Override
	protected HashMap<String, List<Expression>> caseStore(Code c,
			HashMap<String, List<Expression>> c_in, BasicBlock succ) {
		HashMap<String, List<Expression>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		
		Expression op0 = getOpExpression(c_out, ops[0]);

		if(c_out.containsKey(ops[1])){
			List<Expression> list = c_out.get(ops[1]);
			list.clear();
			list.add(op0);
		} else {
			ArrayList<Expression> list = new ArrayList<>();
			list.add(op0);
			c_out.put(ops[1], list);
		}
		return c_out;
	}
		
	@Override
	protected HashMap<String, List<Expression>> caseMoveOp(Code c,
			HashMap<String, List<Expression>> c_in, BasicBlock succ) {
		HashMap<String, List<Expression>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		
		Expression op1 = getOpExpression(c_out, ops[1]);
		
		if(c_out.containsKey(ops[0])){
			List<Expression> list = c_out.get(ops[0]);
			list.clear();
			list.add(op1);
		} else {
			ArrayList<Expression> list = new ArrayList<>();
			list.add(op1);
			c_out.put(ops[0], list);
		}
		return c_out;
	}

	@Override
	protected HashMap<String, List<Expression>> caseCompareOp(Code c,
			HashMap<String, List<Expression>> c_in, BasicBlock succ) {
		HashMap<String, List<Expression>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		
		Expression op1 = getOpExpression(c_out, ops[1]);
		Expression op2 = getOpExpression(c_out,ops[2]);
		
		
		ExpCompareOp exp = new ExpCompareOp(c.getInstruction());
		exp.setLHS(op1);
		exp.setRHS(op2);
		if(c_out.get(ops[0]) != null){
			List<Expression> list = c_out.get(ops[0]);
			list.clear();
			list.add(exp);
		} else {
			ArrayList<Expression> list = new ArrayList<>();
			list.add(exp);
			c_out.put(ops[0], list);
		}
		
		return c_out;
	}
	

	@Override
	protected HashMap<String, List<Expression>> caseBinOp(Code c,
			HashMap<String, List<Expression>> c_in, BasicBlock succ) {
		HashMap<String, List<Expression>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		
		Expression op1 = getOpExpression(c_out, ops[1]);
		Expression op2 = getOpExpression(c_out, ops[2]);
		
		ExpBinOp exp = new ExpBinOp(c.getInstruction());
		exp.setLHS(op1);
		exp.setRHS(op2);
		if(c_out.get(ops[0]) != null){
			List<Expression> list = c_out.get(ops[0]);
			list.clear();
			list.add(exp);
		} else {
			ArrayList<Expression> list = new ArrayList<>();
			list.add(exp);
			c_out.put(ops[0], list);
		}
		
		return c_out;
	}

	@Override
	protected HashMap<String, List<Expression>> caseCondBranch(Code c,
			HashMap<String, List<Expression>> c_in, BasicBlock succ) {
		HashMap<String, List<Expression>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		
		Expression op1 = getOpExpression(c_out, ops[0]);
		Expression op2 = getOpExpression(c_out, ops[1]);
		
		ExpBranchCond exp = new ExpBranchCond(c.getInstruction());
		exp.setLHS(op1);
		exp.setRHS(op2);
		String key = c.getCodeKey();
		if(c_out.get(key) != null){
			List<Expression> list = c_out.get(key);
			list.clear();
			list.add(exp);
		} else {
			ArrayList<Expression> list = new ArrayList<>();
			list.add(exp);
			c_out.put(key, list);
		}
		
		return c_out;
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
	
	private Expression getOpExpression(HashMap<String, List<Expression>> c_out,
			String op) {
		Expression result = null;
		List<Expression> list = c_out.get(op);
		if(list != null && list.size() == 1){
			result = list.get(0);
		}
		
		if(result == null || result.getStatus() == Expression.BOTTOM){
			result = convertOpToExp(op);
		}
		return result;
	}


}
