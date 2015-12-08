package codegen.prof.flow;

import codegen.prof.*;
import codegen.prof.Code.CodeType;

import java.util.*;

public class RenameSSA {

	Function root;
	HashMap<String, Integer> count;
	HashMap<String, Stack<Integer>> stack;
	Set<String> vars;
	
	public RenameSSA(Function root, Set<String> vars){
		this.root = root;
		count = new HashMap<>();
		stack = new HashMap<>();
		this.vars = vars;
	}
	
	
	public void analyze(){
		initLists();
		rename(root.getBasicBlockList().get(0));
	}
	
	private void rename(BasicBlock node){
		for(Code c : node.getCode()){
			String[] ops = c.getOperands();
			
			//For each use
			String use;
			switch(c.getType()){
			case STORE:
				use = ops[0];
				ops[0] = getNewId(use);
				break;
			case LOAD:
				use = ops[1];
				ops[1] = getNewId(use);
				break;
			case CUSTOM:
			case BINOP:
			case COMPARE:
				use = ops[1];
				ops[1] = getNewId(use);
				use = ops[2];
				ops[2] = getNewId(use);
				break;
			case COND_BRANCH:
				use = ops[0];
				ops[0] = getNewId(use);
				use = ops[1];
				ops[1] = getNewId(use);
				break;
			case MOVE:
				use = ops[1];
				ops[1] = getNewId(use);
				break;
			default:
				break;
			}
			
			
			String def;
			switch(c.getType()){
			case CALL:
			case INDIRECTCALL:
				update("r2");
				update("r3");
				break;
			case STORE:
				def = ops[1];
				update(def);
				ops[1] = getNewId(def);
				break;
			case LOAD:
			case CUSTOM:
			case BINOP:
			case COMPARE:
			case MOVE:
				def = ops[0];
				update(def);
				ops[0] = getNewId(def);
				break;
			case PHI:
				PhiCode phi = (PhiCode) c;
				def = phi.def;
				update(def);
				phi.def = getNewId(def);
				break;
			default:
				break;
			}
		}
		

		for(BasicBlock y : node.getSuccesors()){
			for(Code c : y.getCode()){
				if(c instanceof PhiCode){
					int predIndex = y.getPredIndex(node);
					PhiCode phi = (PhiCode) c;
					phi.setMergedDef(predIndex, stack.get(phi.getRootString()).peek());
				}
			}
			//replace phi operands
		}
		
		for(BasicBlock child : node.getDomTreeChildren()){
			rename(child);
		}
		
		for(Code c : node.getCode()){
			String def;
			String[] ops = c.getOperands();
			switch(c.getType()){
//			case CALL:
//			case INDIRECTCALL:
//				break;
			case STORE:
				def = ops[1];
				stack.get(getVarName(def)).pop();
				break;
			case LOAD:
			case CUSTOM:
			case BINOP:
			case COMPARE:
			case MOVE:
				def = ops[0];
				stack.get(getVarName(def)).pop();
				break;
			default:
				break;
			}
		}
	}
	
	private String getVarName(String def) {
		return def.split("_")[0];
	}


	public void update(String op){
		int varCount = count.get(op);
		varCount++;
		count.put(op, varCount);
		stack.get(op).push(varCount);
	}
	
	private String getNewId(String op) {
		if(!isNumeric(op) && !op.equals("zero")){ //don't replace constants!
			return op + "_" + stack.get(op).peek();
		} else return op;
	}

	public static boolean isNumeric(String str)
	{
	  return str.matches("-?\\d+(\\.\\d+)?");  //match a number with optional '-' and decimal.
	}
	
	private void initLists(){
		for(String var : vars){
			count.put(var, 0);
			stack.put(var, new Stack<Integer>());
			stack.get(var).push(0);
		}
		String[] args = {"r4","r5","r6","r7"};
		for(String var : args){
			count.put(var, 0);
			stack.put(var, new Stack<Integer>());
			stack.get(var).push(0);		
		}
		
		//also add for loads...
		for(BasicBlock bb : root.getBasicBlockList()){
			for(Code c : bb.getCode()){
				if(c.getType() == CodeType.LOAD){
					String op = c.getOperands()[1];
					count.put(op, 0);
					stack.put(op, new Stack<Integer>());
					stack.get(op).push(0);	
				}
			}
		}
	}
}
