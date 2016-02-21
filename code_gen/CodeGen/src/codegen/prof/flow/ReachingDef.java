package codegen.prof.flow;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;

import codegen.prof.BasicBlock;
import codegen.prof.Code;
import codegen.prof.Function;
import codegen.prof.Code.CodeType;

public class ReachingDef  extends IntraForwardAnalysis<HashMap<String,List<Definition>>>{

	
	public ReachingDef(Function root) {
		super(root);
	}
	
	public ReachingDef(List<BasicBlock> workList) {
		super(workList);
	}

	@Override
	protected HashMap<String, List<Definition>> initSet() {
		return new HashMap<>();
	} 
	
	@Override
	protected void analysisPreprocessing() {
		//for every line of code create a bottom
		HashMap<String,List<Definition>> initSet = bbInSet.get(func.getBasicBlockList().get(0));
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
							initSet.put(op, new ArrayList<Definition>());								
						}
					}				
				}
			}
		}
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	@Override
	protected HashMap<String, List<Definition>> merge(
			HashMap<String, List<Definition>> set1, HashMap<String, List<Definition>> set2) {

		//copy and sort the sets...
		HashMap<String, List<Definition>> copySet1 = copy(set1);
		HashMap<String, List<Definition>> copySet2 = copy(set2);
		
		
		
		HashMap<String, List<Definition>> result = new HashMap<>(copySet1);
		
		for(String key : copySet2.keySet()){
			if(copySet1.containsKey(key)){
				//if they both contain key then union lists
				List<Definition> resultList = result.get(key);
				for(Definition exp : copySet2.get(key)){
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
	protected HashMap<String,List<Definition>> copy(
			HashMap<String, List<Definition>> original) {
		HashMap<String, List<Definition>> result = new HashMap<>();
		for(String key : original.keySet()){
			result.put(key, new ArrayList<>(original.get(key)));
		}
		return result;
	}


	@Override
	protected HashMap<String, List<Definition>> caseLoad(Code c,
			HashMap<String, List<Definition>> c_in, BasicBlock succ) {
		HashMap<String, List<Definition>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		
		updateFlow(c_out, ops[0],c.getAddress());
		
		
		return c_out;

	}

	@Override
	protected HashMap<String, List<Definition>> caseStore(Code c,
			HashMap<String, List<Definition>> c_in, BasicBlock succ) {
		HashMap<String, List<Definition>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		
		updateFlow(c_out, ops[1],c.getAddress());
		return c_out;
	}
		


	@Override
	protected HashMap<String, List<Definition>> caseMoveOp(Code c,
			HashMap<String, List<Definition>> c_in, BasicBlock succ) {
		HashMap<String, List<Definition>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		updateFlow(c_out, ops[0],c.getAddress());
		return c_out;
	}

	@Override
	protected HashMap<String, List<Definition>> caseCompareOp(Code c,
			HashMap<String, List<Definition>> c_in, BasicBlock succ) {
		HashMap<String, List<Definition>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		updateFlow(c_out, ops[0],c.getAddress());
		
		return c_out;
	}


	@Override
	protected HashMap<String, List<Definition>> caseBinOp(Code c,
			HashMap<String, List<Definition>> c_in, BasicBlock succ) {
		HashMap<String, List<Definition>> c_out = copy(c_in);
		String[] ops = c.getOperands();
		updateFlow(c_out, ops[0],c.getAddress());
		
		
		return c_out;
	}

	private void updateFlow(HashMap<String, List<Definition>> c_out,
			String op, int line) {
		if(c_out.get(op) != null){
			List<Definition> list = c_out.get(op);
			list.clear();
			list.add(new Definition(line));
		} else {
			ArrayList<Definition> list = new ArrayList<>();
			list.add(new Definition(line));
			c_out.put(op, list);
		}
	}


}
