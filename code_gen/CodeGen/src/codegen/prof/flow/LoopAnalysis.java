package codegen.prof.flow;

import codegen.prof.*;
import codegen.prof.flow.ExpCompareOp.Type;

import java.util.*;

public class LoopAnalysis {

	Function root;
	ConstantPropagation constProp;

	public LoopAnalysis(Function f) {
		this.root = f;
	}

	public boolean analyze() {

		boolean allPassed = true;
		// First do constant propagation on the entire function
		// ----------------------------------------------------
		constProp = new ConstantPropagation(root);
		constProp.analyze();
		for (Loop l : root.getLoopList()) {
			System.out.println(l);
			// The loop must have a single backwards edge
			// ------------------------------------------
			BasicBlock bbBackwardsEdge = null;
			try {
				bbBackwardsEdge = getSingleBackwardsEdge(l);
			} catch (LoopAnalysisException e) {
				e.printStackTrace();
				allPassed = false;
				continue;
			}

			// The loop must have a single exit point
			// --------------------------------------
			BasicBlock bbExitPoint = null;
			try {
				bbExitPoint = getSingleExitPoint(l);

			} catch (LoopAnalysisException e) {
				e.printStackTrace();
				allPassed = false;
				continue;
			}			
			
			try {
				findLoopParameters(l, bbBackwardsEdge, bbExitPoint);
			} catch (LoopAnalysisException e) {
				e.printStackTrace();
				allPassed = false;
				continue;

			}

		}
		return allPassed;
	}
	

	private void findLoopParameters(Loop l, BasicBlock bbBackwardsEdge,
			BasicBlock bbExitPoint) throws LoopAnalysisException {
		
		// Get the branch condition, simplified and reduced
		// ------------------------------------------------
		ReachingDefinition reachingDef = new ReachingDefinition(l.getBody());
		reachingDef.analyze();
		HashMap<String, List<Expression>> conditionOut = (HashMap<String, List<Expression>>) reachingDef
				.getOutSet(bbExitPoint);
		List<Expression> branchList =  conditionOut
		.get(bbExitPoint.getLastCode().getCodeKey());
		if(branchList.size() != 1){
			throw new LoopAnalysisException("More than one reaching def found for branch condition");
		}
		ExpBranchCond branchCondition = (ExpBranchCond) branchList.get(0); 

		
		
		// Simplify the condition expression
		// ---------------------------------
		HashMap<String, Expression> constants;
		constants = (HashMap<String, Expression>) constProp
				.getOutSet(bbExitPoint);
		branchCondition = (ExpBranchCond) replaceConstants(constants,
				branchCondition);
		branchCondition = simplifyBranchCondition(branchCondition);
		if (!trueCondStaysInLoop(l, bbExitPoint)) {
			branchCondition.invertType();
		}
		
		// Declare the loop parameters
		// ---------------------------
		String iterator = null;
		int incrValue = -1;
		int threshold = -1;
		int initValue = 0;
		
		if(branchCondition.getLHS() instanceof ExpBinOp){
			// First pattern ( iterator + increment != threshold )
			// ---------------------------------------------------
			
			ExpBinOp lhs = (ExpBinOp) branchCondition.getLHS();
			if(!(lhs.getLHS() instanceof ExpIdentifier)){
				throw new LoopAnalysisException("Expect identifier on lhs of binary operation in"
						+ "branch condition");
			}
			if(!(lhs.getRHS() instanceof ExpConstant)){
				throw new LoopAnalysisException("Expect constant on rhs of binary operation in"
						+ "branch condition");
			}
			
			// Simplify turns all subs int adds
			// --------------------------------
			lhs = (ExpBinOp) simplifyBinOp(lhs);
			
			// Get the variable name and the increment value from
			// the exit block
			//---------------------------------------------------
			iterator = ((ExpIdentifier) lhs.getLHS()).id;
			incrValue = ((ExpConstant) lhs.getRHS()).value; 
			threshold = ((ExpConstant) branchCondition.getRHS()).value;
		} else {
			// Second pattern ((iterator < threshold != 0)
			iterator = ((ExpIdentifier) branchCondition.getLHS()).id;
			threshold = ((ExpConstant) branchCondition.getRHS()).value;

			// Need to do some extra work to find the increment value
			// ------------------------------------------------------
			
			HashMap<String, List<Expression>> outSet = (HashMap<String, List<Expression>>) reachingDef
					.getOutSet(l.getTail());
			constants = (HashMap<String, Expression>) constProp.getOutSet(l
					.getTail());
			if(outSet.get(iterator).size() != 1){
				throw new LoopAnalysisException("More than one reaching def found for increment value");
			}
			Expression change = outSet.get(iterator).get(0); //TODO check only 1
			change = replaceConstants(constants, change);
			ExpBinOp binOp = null;
			if ((change instanceof ExpBinOp)) {
				binOp = (ExpBinOp) change;
				binOp = (ExpBinOp) simplifyBinOp(binOp);
				if(! ((ExpIdentifier) binOp.getLHS()).id.equals(iterator)){
					throw new LoopAnalysisException("Identifier must match on both sides of increment expression");
				}
			}
			if(binOp != null){
				incrValue = ((ExpConstant) binOp.getRHS()).value;
			} else {
				throw new LoopAnalysisException("Could not parse binary operation in branch condition");
			}
		}

		// Now find the initial value for iterator
		// ---------------------------------------
		ArrayList<BasicBlock> entryList = l.getEntryBlocks();
		Map<String, Expression> inSet = constProp.getMergedOutSet(entryList);
		initValue = ((ExpConstant) inSet.get(iterator)).value;
		
		int maxIterations = getMaxIterations(initValue, threshold, incrValue, branchCondition.type);
		if(maxIterations < 0){
			throw new LoopAnalysisException(String.format("Constraints have been violated for" +
					"max iteration calculation: initValue: %d, threshold: %d, incrValue: %d, "
					+ "branchType: %s",initValue,threshold,incrValue,branchCondition.type.toString()));		
		}
		l.setMaxIterations(maxIterations);
		

		System.out.println("iterator: " + iterator);
		System.out.println("Initial value: " + initValue);
		System.out.println("increment: " + incrValue);
		System.out.println("threshold: " + threshold);
		System.out.println(maxIterations);
	}

	private boolean trueCondStaysInLoop(Loop loop, BasicBlock bbExitPoint) {
		return loop.containsAddress(bbExitPoint.getBranchAddress());
	}

	private ExpBranchCond simplifyBranchCondition(ExpBranchCond cond)
			throws LoopAnalysisException {

		if(cond.getLHS() instanceof ExpConstant && 
				cond.getRHS() instanceof ExpIdentifier){
			//swap them
			Expression lhs = cond.getLHS();
			cond.setLHS(cond.getRHS());
			cond.setRHS(lhs);
			cond.changeDirection();
		}
		
		
		// Throw error of rhs is not a constant
		// ------------------------------------
		if (!(cond.getRHS() instanceof ExpConstant)) {
			throw new LoopAnalysisException(
					"Branch condition rhs is not constant");
		}

		// If the LHS is an identifier than no simplifying required
		// ------------------------------------------------------
		if (cond.getLHS() instanceof ExpIdentifier) {
			return cond;
		}

		// If the LHS is a conditional then map the condition
		// to the branch condition.
		// --------------------------------------------------
		if (cond.getLHS() instanceof ExpCompareOp) {

			if (!cond.getRHS().equals(new ExpConstant(0))
					|| (cond.getType() != ExpCompareOp.Type.EQ && cond
							.getType() != ExpCompareOp.Type.NE)) {
				throw new LoopAnalysisException(
						"LHS of branch is conditional. Expected true/false evaluation"
								+ "but found something else");
			}

			ExpCompareOp lhs = (ExpCompareOp) cond.getLHS();
			Expression newLHS = null;
			if (!(lhs.getLHS() instanceof ExpIdentifier)) {
				throw new LoopAnalysisException(
						"Branch condition LHS cannot be simplified to identifier");
			} else {
				newLHS = lhs.getLHS();
			}

			Expression newRHS = lhs.getRHS();
			if (newRHS instanceof ExpBinOp) {
				newRHS = simplifyBinOp((ExpBinOp) newRHS);
			}

			if (!(newRHS instanceof ExpConstant)) {
				throw new LoopAnalysisException(
						"Branch condition rhs is not constant");
			}

			Type newType;
			if (cond.getType() == Type.EQ) {
				newType = ExpCompareOp.getInvertedType(lhs.getType());
			} else {
				newType = lhs.getType();
			}
			cond = new ExpBranchCond(newType);
			cond.setLHS(newLHS);
			cond.setRHS(newRHS);
		}

		return cond;
	}

	private BasicBlock getSingleExitPoint(Loop l) throws LoopAnalysisException {
		BasicBlock result = null;
		int count = 0;
		ArrayList<BasicBlock> loopBody = l.getBody();
		for (BasicBlock bb : loopBody) {
			for (BasicBlock succ : bb.getSuccesors()) {
				if (!loopBody.contains(succ)) {
					result = bb;
					count++;
				}
				if (count > 1) {
					throw new LoopAnalysisException("Loop ("
							+ l.getHead().getStartAddressHex()
							+ ") cannot be analyzed: more than one exit edge");
				}
			}
		}
		return result;
	}

	private BasicBlock getSingleBackwardsEdge(Loop l)
			throws LoopAnalysisException {
		BasicBlock result = null;
		int count = 0;
		for (BasicBlock bb : l.getBody()) {
			for (BasicBlock succ : bb.getSuccesors()) {
				if (succ.equals(l.getHead())) { // equals for self pointer
					result = bb;
					count++;
				}
				if (count > 1) {
					throw new LoopAnalysisException(
							"Loop ("
									+ l.getHead().getStartAddressHex()
									+ ") cannot be analyzed: more than one backwards edge");
				}
			}
		}
		return result;
	}

	private Expression replaceConstants(HashMap<String, Expression> constants,
			Expression exp) {
		if (exp.getChildren() != null) {
			for (int i = 0; i < exp.numChildren(); i++) {
				exp.setChild(i, replaceConstants(constants, exp.getChild(i)));
			}
		} else {
			if (exp instanceof ExpIdentifier) {
				ExpIdentifier id = (ExpIdentifier) exp;

				Expression constValue = constants.get(id.id);
				if (constValue != null
						&& constValue.getStatus() != Expression.TOP) {
					exp = constValue;
				}
			}
		}
		return exp;
	}

	private Expression simplifyBinOp(ExpBinOp binOp) {

		Expression result = null;

		// propagate down
		if (binOp.getLHS() instanceof ExpBinOp) {
			binOp.setLHS(simplifyBinOp((ExpBinOp) binOp.getLHS()));
		}
		if (binOp.getRHS() instanceof ExpBinOp) {
			binOp.setRHS(simplifyBinOp((ExpBinOp) binOp.getRHS()));
		}

		// Pattern 1: make sure variable is on left

		if (binOp.getLHS() instanceof ExpConstant
				&& !(binOp.getRHS() instanceof ExpConstant)) {
			Expression lhs = binOp.getLHS();
			binOp.setLHS(binOp.getRHS());
			binOp.setRHS(lhs);
		}

		// Pattern 2: constant + constant

		if (binOp.getLHS() instanceof ExpConstant
				&& binOp.getRHS() instanceof ExpConstant) {
			result = Expression.sum((ExpConstant) binOp.getLHS(),
					(ExpConstant) binOp.getRHS());
		}

		// Pattern 3 lhs = exp -> rhs = constant, rhs = constant

		if (binOp.getLHS() instanceof ExpBinOp
				&& binOp.getRHS() instanceof ExpConstant
				&& ((ExpBinOp) binOp.getLHS()).getRHS() instanceof ExpConstant) {
			ExpBinOp lhs = (ExpBinOp) binOp.getLHS();

			Expression newLhs = lhs.getLHS();
			ExpConstant c1 = (ExpConstant) lhs.getRHS();
			if (lhs.type == ExpBinOp.Type.MINUS) {
				c1.value = -c1.value;
			}
			ExpConstant c2 = (ExpConstant) binOp.getRHS();
			if (binOp.type == ExpBinOp.Type.MINUS) {
				c2.value = -c2.value;
			}
			Expression newRhs = Expression.sum(c1, c2);

			binOp = new ExpBinOp(ExpBinOp.Type.PLUS);
			binOp.setLHS(newLhs);
			binOp.setRHS(newRhs);

		}
		if (result == null) {
			result = binOp;
		}
		return result;
	}

	private int getMaxIterations(int initial, int threshold, int increment,
			ExpCompareOp.Type type) {
		// first check conditions
		if (!(
				(threshold > initial && increment > 0 && (type.equals(Type.LT) || type
				.equals(Type.LE) || type.equals(Type.NE))) || 
				(threshold < initial && increment < 0 && (type
				.equals(Type.GT) || type.equals(Type.GE) || type.equals(Type.NE)))
			)
			) {
			return -1;
		}

		if (type.equals(Type.LE)) {
			threshold++;
		} else if (type.equals(Type.GE)) {
			threshold--;
		}

		return (int) Math.ceil((threshold - initial) / ((double) increment));
	}
}