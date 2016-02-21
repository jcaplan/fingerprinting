package codegen.prof.flow;

import java.util.*;

import codegen.prof.BasicBlock;
import codegen.prof.Code;
import codegen.prof.Function;
import codegen.prof.Code.CodeType;

public class InterReachingDef  extends InterForwardAnalysis<HashMap<String,List<InterDefinition>>>{
	
	public InterReachingDef(Function root) {
		super(root);
	}
	
	public InterReachingDef(List<BasicBlock> workList) {
		super(workList);
	}

	@Override
	protected HashMap<String, List<InterDefinition>> initSet() {
		return new HashMap<>();
	} 
	
	@Override
	protected void analysisPreprocessing() {
		//for every line of code create a bottom
		HashMap<String,List<InterDefinition>> initSet = bbInSet.get(func.getBasicBlockList().get(0));
		for(BasicBlock bb : bbList){
			for(Code c : bb.getCode()){
				if(c.getType() == CodeType.COND_BRANCH || c.getType() == CodeType.UNCOND_BRANCH){
					continue;
				}
				
				String[] operands = c.getOperands();
				
				if(operands != null){
					for(String op : operands){
						if(op.startsWith("r") || op.contains("sp") || 
								op.contains("fp")){
							initSet.put(op, new ArrayList<InterDefinition>());								
						}
					}				
				}
			}
		}
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	@Override
	protected HashMap<String, List<InterDefinition>> merge(
			HashMap<String, List<InterDefinition>> set1, HashMap<String, List<InterDefinition>> set2) {

		//copy and sort the sets...
		HashMap<String, List<InterDefinition>> copySet1 = copy(set1);
		HashMap<String, List<InterDefinition>> copySet2 = copy(set2);
		
		
		
		HashMap<String, List<InterDefinition>> result = new HashMap<>(copySet1);
		
		for(String key : copySet2.keySet()){
			if(copySet1.containsKey(key)){
				//if they both contain key then union lists
				List<InterDefinition> resultList = result.get(key);
				for(InterDefinition exp : copySet2.get(key)){
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
	protected HashMap<String,List<InterDefinition>> copy(
			HashMap<String, List<InterDefinition>> original) {
		HashMap<String, List<InterDefinition>> result = new HashMap<>();
		for(String key : original.keySet()){
			result.put(key, new ArrayList<>(original.get(key)));
		}
		return result;
	}


	@Override
	protected HashMap<String, List<InterDefinition>> caseLoad(Code c,
			HashMap<String, List<InterDefinition>> c_in, BasicBlock succ) {
		HashMap<String, List<InterDefinition>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		
		updateFlow(c_out, ops[0],c.getAddress());
		
		
		return c_out;

	}

	@Override
	protected HashMap<String, List<InterDefinition>> caseStore(Code c,
			HashMap<String, List<InterDefinition>> c_in, BasicBlock succ) {
		HashMap<String, List<InterDefinition>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		
		updateFlow(c_out, ops[1],c.getAddress());
		return c_out;
	}
		


	@Override
	protected HashMap<String, List<InterDefinition>> caseMoveOp(Code c,
			HashMap<String, List<InterDefinition>> c_in, BasicBlock succ) {
		HashMap<String, List<InterDefinition>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		updateFlow(c_out, ops[0],c.getAddress());
		return c_out;
	}

	@Override
	protected HashMap<String, List<InterDefinition>> caseCompareOp(Code c,
			HashMap<String, List<InterDefinition>> c_in, BasicBlock succ) {
		HashMap<String, List<InterDefinition>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		updateFlow(c_out, ops[0],c.getAddress());
		
		return c_out;
	}


	@Override
	protected HashMap<String, List<InterDefinition>> caseBinOp(Code c,
			HashMap<String, List<InterDefinition>> c_in, BasicBlock succ) {
		HashMap<String, List<InterDefinition>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		updateFlow(c_out, ops[0],c.getAddress());
		
		
		return c_out;
	}

	private void updateFlow(HashMap<String, List<InterDefinition>> c_out,
			String op, int line) {
		if(c_out.get(op) != null){
			List<InterDefinition> list = c_out.get(op);
			list.clear();
			list.add(new InterDefinition(context,line));
		} else {
			ArrayList<InterDefinition> list = new ArrayList<>();
			list.add(new InterDefinition(context,line));
			c_out.put(op, list);
		}
	}


}
