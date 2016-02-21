package codegen.prof.flow;

import java.util.*;

import codegen.prof.BasicBlock;
import codegen.prof.Code;
import codegen.prof.Function;

public class ReachingExp extends IntraForwardAnalysis<HashMap<String,List<Expression>>>{

	//Keep a symbol table for easy lookup of defs
	
	Map<String,List<Expression>> symTable;
	
	
	public ReachingExp(Function root) {
		super(root);
		symTable = new HashMap<>();
	}
	
	public ReachingExp(List<BasicBlock> workList) {
		super(workList);
	}

	@Override
	protected HashMap<String, List<Expression>> initSet() {
		return new HashMap<>();
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
		updateFlow(c_out, ops[0], op1);
		
		return c_out;

	}

	private void updateFlow(HashMap<String, List<Expression>> c_out,
			String key, Expression value) {
		List<Expression> list;
		//clear out the previous definition if it exists
		String lastDef = PhiCode.getDefKey(key,-1);
		if(lastDef != null && c_out.containsKey(lastDef)){
			c_out.remove(lastDef);
		}
		if(c_out.containsKey(key)){
			 list = c_out.get(key);
			list.clear();
			list.add(value);
		} else {
			list = new ArrayList<>();
			list.add(value);
			c_out.put(key, list);
		}
		symTable.put(key, list);
	}

	@Override
	protected HashMap<String, List<Expression>> caseStore(Code c,
			HashMap<String, List<Expression>> c_in, BasicBlock succ) {
		HashMap<String, List<Expression>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		
		Expression op0 = getOpExpression(c_out, ops[0]);

		updateFlow(c_out, ops[1], op0);
		return c_out;
	}
		
	@Override
	protected HashMap<String, List<Expression>> caseMoveOp(Code c,
			HashMap<String, List<Expression>> c_in, BasicBlock succ) {
		HashMap<String, List<Expression>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		
		Expression op1 = getOpExpression(c_out, ops[1]);
		if(c.getInstruction().equals("movhi")){
			ExpBinOp shift = new ExpBinOp(ExpBinOp.Type.SLL);
			shift.setLHS(op1);
			shift.setRHS(new ExpConstant(16));
			op1 = shift;
		}
		updateFlow(c_out, ops[0], op1);
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
		updateFlow(c_out, ops[0], exp);
		
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
		updateFlow(c_out, ops[0], exp);
		
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
		updateFlow(c_out, key, exp);
		
		return c_out;
	}
	
	@Override
	protected HashMap<String, List<Expression>> casePhiOp(Code c,
			HashMap<String, List<Expression>> c_in, BasicBlock succ) {
		
		
		HashMap<String, List<Expression>> c_out = copy(c_in);
		
		String[] ops = c.getOperands();
		String def = ((PhiCode) c).def;
		
		List<Expression> expList = new ArrayList<>();
		c_out.put(def, expList);
		for(String op : ops){
			List<Expression> opList = c_out.get(op);
			if(opList != null){
				for(Expression opexp : opList){
					if(!(opexp.isBottom() && opList.size() > 1)){
						if(!expList.contains(opexp)){
							expList.add(opexp);
						}
					}
				}
			}
			c_out.remove(op);
		}
		symTable.put(def, expList);
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
			result = list.get(0).copy();
		}
		
		if(result == null || result.getStatus() == Expression.BOTTOM){
			result = convertOpToExp(op);
		}
		return result;
	}
	
	public List<Expression> getSymbolExpList(String var){
		return symTable.get(var);
	}


}
