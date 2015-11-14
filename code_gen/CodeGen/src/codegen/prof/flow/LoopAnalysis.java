package codegen.prof.flow;

import codegen.prof.*;
import codegen.prof.flow.ExpCompareOp.Type;

import java.util.*;

public class LoopAnalysis {

	Function root;
	
	public LoopAnalysis(Function f){
		this.root = f;
	}
	
	
	
	
	public void analyze(){
		
		
		//First do expression builder on the entire function

		
		for(Loop l : root.getLoopList()){
		
			// 1. Only analyze the head. Don't want the effects of merges...
			// Find the iterator
			//--------------------------------------------------------------
			
			
			ArrayList<BasicBlock> head = new ArrayList<>();
			head.add(l.getHead());
			ExpressionBuilder headExpBuilder = new ExpressionBuilder(head);
			
			
			//Get the conditional expression from the outSet of the head
			headExpBuilder.analyze();
			HashMap<String, Expression> headOut = (HashMap<String, Expression>) headExpBuilder.getOutSet(l.getHead());
			
			ExpBranchCond branchCondition = (ExpBranchCond)headOut.get(l.getHead().getLastCode().getCodeKey());
			
			ExpCompareOp compare = (ExpCompareOp)branchCondition.getLHS();
			
			String iterator = ((ExpIdentifier) compare.getLHS()).id;
			System.out.println("Iterator id: " + iterator);
			
			int bound = ((ExpConstant )compare.getRHS()).value;
			System.out.println("loop bound: " + bound);
			System.out.println("loop condition: " + compare.type);
			
			//Now that the iterator has been identified we can find it's initial value and
			//the change that happens over one iteration
			// 2. Initial value
			//-----------------
			
			//analyze from start until loop entry...
			
			ArrayList<BasicBlock> workList = getPrunedTree(root.getBasicBlockList(), l.getHead());
			
			headExpBuilder = new ExpressionBuilder(workList);
			headExpBuilder.analyze();
			
			HashMap<String,Expression> inSet = (HashMap<String, Expression>) headExpBuilder.getInSet(l.getHead());
			int value = ((ExpConstant) inSet.get(iterator)).value;
					
			System.out.println("Initial value: " + value);
			
			//Find expression for iterator after one loop through body
			
			
			workList = new ArrayList<>(l.getBody());
			workList.remove(l.getHead());
			headExpBuilder = new ExpressionBuilder(workList);
			headExpBuilder.analyze();
			
			HashMap<String,Expression> outSet = (HashMap<String, Expression>) headExpBuilder.getOutSet(l.getTail());
			
			//do constant propagation for WHOLE function and then get constants
			//for the last line of the tail
			ConstantPropagation constProp = new ConstantPropagation(root);
			constProp.analyze();
			constProp.prettyPrint();
			HashMap<String, Expression> constants = (HashMap<String, Expression>) constProp.getOutSet(l.getTail());
			
			Expression change = outSet.get(iterator);
			
			change = replaceConstants(constants, change);
			
//			System.out.println(outSet);
			ExpBinOp binOp = null;
			if((change instanceof ExpBinOp)){
				binOp = (ExpBinOp) change;
				binOp = (ExpBinOp) simplifyBinOp(binOp);
				System.out.println("check matching identifier:" +
				((ExpIdentifier) binOp.getLHS()).id.equals(iterator));
				System.out.println("change operator: " + binOp.type);
				System.out.println("change increment: " + binOp.getRHS());
			}
			
			if(binOp != null){
			System.out.println(String.format("for(i = %d; i %s %d; i = i %s %d);",
					 value,
					compare.type.toString(), bound,
					binOp.type,((ExpConstant) binOp.getRHS()).value));
			}
			System.out.println("max iterations: " + getMaxIterations(value, bound, 
					((ExpConstant) binOp.getRHS()).value, compare.type));
//			headExpBuilder.prettyPrint();
			//TODO
			//problems for -O... where is the exit edge assuming only one
			//does the condition take you to the top or the exit?
			//2.... exp = fp + 1 + 1... need to evaluate entire expression
			
		}
	}




	private Expression replaceConstants(HashMap<String, Expression> constants,
			Expression exp) {
		if(exp.getChildren() != null){
			for(int i = 0; i < exp.numChildren(); i++){
				exp.setChild(i,replaceConstants(constants,exp.getChild(i)));
			}
		} else {
			if(exp instanceof ExpIdentifier){
				ExpIdentifier id = (ExpIdentifier)exp;
				 
				Expression constValue = constants.get(id.id);
				if(constValue != null && constValue.getStatus() != Expression.TOP){
					exp = constValue;
				}
			}
		}
		return exp;
	}




	private Expression simplifyBinOp(ExpBinOp binOp) {
		
		Expression result = null;
		
		//propagate down
		if(binOp.getLHS() instanceof ExpBinOp){
			binOp.setLHS(simplifyBinOp((ExpBinOp)binOp.getLHS()));
		}
		if(binOp.getRHS() instanceof ExpBinOp){
			binOp.setRHS(simplifyBinOp((ExpBinOp)binOp.getRHS()));
		}
		
		//Pattern 1: make sure variable is on left
		
		if(binOp.getLHS() instanceof ExpConstant && !(binOp.getRHS() instanceof ExpConstant)){
			Expression lhs = binOp.getLHS();
			binOp.setLHS(binOp.getRHS());
			binOp.setRHS(lhs);
		}
		
		//Pattern 2: constant + constant
		
		if(binOp.getLHS() instanceof ExpConstant && 
				binOp.getRHS() instanceof ExpConstant){
			result = Expression.sum((ExpConstant)binOp.getLHS(), 
					(ExpConstant)binOp.getRHS()); 
		}
		
		
		//Pattern 3     lhs = exp -> rhs = constant, rhs = constant
		
		if(binOp.getLHS() instanceof ExpBinOp && binOp.getRHS() instanceof ExpConstant
				&& ((ExpBinOp)binOp.getLHS()).getRHS() instanceof ExpConstant){
			ExpBinOp lhs = (ExpBinOp)binOp.getLHS();
			
			Expression newLhs = lhs.getLHS();
			ExpConstant c1 = (ExpConstant)lhs.getRHS();
			if(lhs.type == ExpBinOp.Type.MINUS){
				c1.value = -c1.value;
			}
			ExpConstant c2 = (ExpConstant)binOp.getRHS();
			if(binOp.type == ExpBinOp.Type.MINUS){
				c2.value = -c2.value;
			}
			Expression newRhs = Expression.sum(c1,c2);
			
			binOp = new ExpBinOp(ExpBinOp.Type.PLUS);
			binOp.setLHS(newLhs);
			binOp.setRHS(newRhs);
			
		}
		if(result == null){
			result = binOp;
		}
		return result;
	}




	private ArrayList<BasicBlock> getPrunedTree(
			ArrayList<BasicBlock> basicBlockList, BasicBlock head) {
		
		ArrayList<BasicBlock> bbList = new ArrayList<>();
		
		for(BasicBlock bb : basicBlockList){
			if(bb.getAge() < head.getAge()){
				bbList.add(bb);
			}
		}
		bbList.add(head);
		
		return bbList;
	}

	private int getMaxIterations(int initial, int threshold, int increment, 
			ExpCompareOp.Type type){
		//first check conditions
		if(!(
				(threshold > initial && increment > 0 && 
						(type.equals(Type.LT) || type.equals(Type.LE)))	
				||
				(threshold < initial && increment < 0 && 
						(type.equals(Type.GT) || type.equals(Type.GE)))
			)
		){
			return -1;
		}
		
		if(type.equals(Type.LE)){
			threshold++;
		} else if(type.equals(Type.GE)){
			threshold--;
		}
		
		return (int)Math.ceil((threshold - initial)/((double) increment));
	}
}