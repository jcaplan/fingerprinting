package codegen.prof.flow;

import codegen.prof.*;
import codegen.prof.flow.ExpCompareOp.Type;

import java.util.*;

public class LoopAnalysis {

	Function root;
	ReachingDef reachingDef;

	public LoopAnalysis(Function f) {
		this.root = f;
	}

	public boolean analyze() {
		reachingDef = new ReachingDef(root);
		reachingDef.analyze();
		boolean allPassed = true;
		// First do constant propagation on the entire function
		// ----------------------------------------------------
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
			// Now check other basic block branches besides the head
			for (BasicBlock bb : l.getBody()) {
				if (!bb.equals(bbExitPoint) && bb.isBranch()
						&& bb.isConditional()) {
					checkBranch(l,bb);
				}
			}
		}
		return allPassed;
	}

	private void checkBranch(Loop l, BasicBlock bb) {
		HashMap<String, List<Expression>> conditionOut = (HashMap<String, List<Expression>>) reachingDef
				.getOutSet(bb);
		List<Expression> branchList = conditionOut.get(bb.getLastCode()
				.getCodeKey());
		if (branchList.size() != 1) {
			return;
		}
		ExpBranchCond branchCondition = (ExpBranchCond) branchList.get(0);
		try {
			branchCondition = simplifyBranchCondition(branchCondition);
		} catch (LoopAnalysisException e) {
			System.out.println(e.getMessage());
			return;
		}
		System.out.println(bb + ": condition = " + branchCondition);
		Range threshold = null;
		
		// Get the threshold value
		// -----------------------
		try {
			threshold = getRangeFromExp(branchCondition.getRHS(), conditionOut);
		} catch (LoopAnalysisException e) {
			System.out.println("Could not determine threshold for " + bb);
			return;
		}
		
		//Based on new threshold determine max iterations...
		System.out.println("Found new threshold for " + bb + ": " + threshold);
		

		
		Range newInitValue = null;
		Range newThreshold = null;

		if(l.getIncrValue().isPositive()){
			if(branchCondition.type == Type.LE || branchCondition.type == Type.LT){
				newInitValue = l.getInitValue();
				newThreshold = threshold;
			} else if(branchCondition.type == Type.GE || branchCondition.type == Type.GT){
				//from threshold to l.threshold
				newInitValue = threshold;
				newThreshold = l.getThreshold();
			}
		} else {
			if(branchCondition.type == Type.LE || branchCondition.type == Type.LT){
				//from threshold to l.threshold
				newInitValue = threshold;
				newThreshold = l.getThreshold();
			} else if(branchCondition.type == Type.GE || branchCondition.type == Type.GT){
				//from init to threshold
				newInitValue = l.getInitValue();
				newThreshold = threshold;
			}
		}
		
		System.out.println(String.format("check max iterations: %s,%s,%s",newInitValue,
				newThreshold,l.getIncrValue()));
		int maxIterations = getMaxIterations(newInitValue, newThreshold, l.getIncrValue(),
				branchCondition.type);
		if (maxIterations >= 0) {
			System.out.println("Maximum true branch for " + bb + ": " + maxIterations);
			bb.setMaxTrueBranch(maxIterations);
		} else {
			System.out.println("could not determine maxTrueBranch");
		}
	}

	private void findLoopParameters(Loop l, BasicBlock bbBackwardsEdge,
			BasicBlock bbExitPoint) throws LoopAnalysisException {

		// Get the branch condition, simplified and reduced
		// ------------------------------------------------

		HashMap<String, List<Expression>> conditionOut = (HashMap<String, List<Expression>>) reachingDef
				.getOutSet(bbExitPoint);
		List<Expression> branchList = conditionOut.get(bbExitPoint
				.getLastCode().getCodeKey());
		if (branchList.size() != 1) {
			throw new LoopAnalysisException(
					"More than one reaching def found for branch condition");
		}
		ExpBranchCond branchCondition = (ExpBranchCond) branchList.get(0);
		// Simplify the condition expression
		// ---------------------------------
		branchCondition = simplifyBranchCondition(branchCondition);
		if (!trueCondStaysInLoop(l, bbExitPoint)) {
			branchCondition.invertType();
		}

		// Declare the loop parameters
		// ---------------------------
		String iterator = null;
		Range incrValue = null;
		Range threshold = null;
		Range initValue = null;

		Expression incrExp = null;
		Expression thresholdExp = null;
		
		if (branchCondition.getLHS() instanceof ExpBinOp) {
			
			
			// First pattern ( iterator + increment != threshold )
			// ---------------------------------------------------			
			ExpBinOp lhs = (ExpBinOp) branchCondition.getLHS();

			// Simplify turns all subs int adds
			// --------------------------------
			lhs = (ExpBinOp) simplifyBinOp(lhs);

			// Get the variable name and the increment value from
			// the exit block
			// ---------------------------------------------------
			iterator = ((ExpIdentifier) lhs.getLHS()).id;
			incrExp = lhs.getRHS();
			thresholdExp = branchCondition.getRHS();
		} else {
			// Second pattern ((iterator < threshold != 0)
			iterator = ((ExpIdentifier) branchCondition.getLHS()).id;

			// First find the threshold
			// ------------------------
			thresholdExp = branchCondition.getRHS();

			// Find the increment value
			// ------------------------------------------------------
			// A bit safer to use tail instead of bbExitPoint in case exit point is head with 
			// initial value in list. This way don't have to worry about some corner case 
			// where order of list [incr, initValue] is reversed.
			HashMap<String, List<Expression>> outSet = (HashMap<String, List<Expression>>) reachingDef
					.getOutSet(l.getTail());
			if (conditionOut.get(iterator).size() != 1) {
				throw new LoopAnalysisException(
						"More than one reaching def found for increment value");
			}

			Expression change = outSet.get(iterator).get(0);
			ExpBinOp binOp = null;
			if ((change instanceof ExpBinOp)) {
				binOp = (ExpBinOp) change;
				binOp = (ExpBinOp) simplifyBinOp(binOp);
				if (!((ExpIdentifier) binOp.getLHS()).id.equals(iterator)) {
					throw new LoopAnalysisException(
							"Identifier must match on both sides of increment expression");
				}
			}
			if (binOp != null) {
				incrExp = binOp.getRHS();
			} else {
				throw new LoopAnalysisException(
						"Could not identify binary operation in branch condition");
			}
		}
		
		threshold = getRangeFromExp(thresholdExp, conditionOut);
		incrValue = getRangeFromExp(incrExp, conditionOut);


		// Now find the initial value for iterator
		// ---------------------------------------
		ArrayList<BasicBlock> entryList = l.getEntryBlocks();
		Map<String, List<Expression>> inSet = reachingDef
				.getMergedOutSet(entryList);

		// Need to distinguish between known pairs, take worst case,
		// and unkown branch...
		List<Expression> rhsList = inSet.get(iterator);
		initValue = getRange(rhsList);
		if (initValue == null) {
			throw new LoopAnalysisException(
					"Could not determine initial value range");
		}

		int maxIterations = getMaxIterations(initValue, threshold, incrValue,
				branchCondition.type);
		if (maxIterations < 0) {
			throw new LoopAnalysisException(
					String.format(
							"Constraints have been violated for "
									+ "max iteration calculation: initValue: %s, threshold: %s, incrValue: %s, "
									+ "branchType: %s", initValue, threshold,
							incrValue, branchCondition.type.toString()));
		}
		l.setMaxIterations(maxIterations);
		l.setInductionVar(iterator);
		l.setIncrValue(incrValue);
		l.setInitValue(initValue);
		l.setThreshold(threshold);

		System.out.println("iterator: " + iterator);
		System.out.println("Initial value: " + initValue);
		System.out.println("increment: " + incrValue);
		System.out.println("threshold: " + threshold);
		System.out.println(maxIterations);
	}

	private Range getRangeFromExp(Expression exp, HashMap<String, List<Expression>> defs) throws LoopAnalysisException {
		Range range = null;
		if (exp instanceof ExpConstant) {
			range = new Range(((ExpConstant) exp).value);
		} else if (exp instanceof ExpIdentifier) {
			String name = ((ExpIdentifier) exp).id;
			List<Expression> rhsList = defs
					.get(name);
			range = getRange(rhsList);
			if (range == null) {
				throw new LoopAnalysisException(
						"range not defined on all paths");
			}
		}
		return range;
	}

	private Range getRange(List<Expression> rhsList) {
		Range range = null;
		for (Expression exp : rhsList) {
			if (exp instanceof ExpBinOp) {
				exp = simplifyBinOp((ExpBinOp) exp);
			}
			if (exp instanceof ExpConstant) {
				int value = ((ExpConstant) exp).value;
				if (range == null) {
					range = new Range(value, value);
				} else {
					range.update(value);
				}
			} else {
				return null;
			}
		}
		return range;
	}

	private boolean trueCondStaysInLoop(Loop loop, BasicBlock bbExitPoint) {
		return loop.containsAddress(bbExitPoint.getBranchAddress());
	}

	private ExpBranchCond simplifyBranchCondition(ExpBranchCond cond)
			throws LoopAnalysisException {

		if (cond.getLHS() instanceof ExpBranchCond) {
			simplifyBranchCondition((ExpBranchCond) cond.getLHS());
		} else if (cond.getRHS() instanceof ExpBranchCond) {
			simplifyBranchCondition((ExpBranchCond) cond.getRHS());
		}

		if (cond.getLHS() instanceof ExpBinOp) {
			cond.setLHS(simplifyBinOp((ExpBinOp) cond.getLHS()));
		}
		if (cond.getRHS() instanceof ExpBinOp) {
			cond.setRHS(simplifyBinOp((ExpBinOp) cond.getRHS()));
		}

		if (cond.getLHS() instanceof ExpConstant
				&& (cond.getRHS() instanceof ExpIdentifier || cond.getRHS() instanceof ExpBinOp)) {
			// swap them
			Expression lhs = cond.getLHS();
			cond.setLHS(cond.getRHS());
			cond.setRHS(lhs);
			cond.changeDirection();
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
			switch (binOp.type) {
			case MINUS:
				result = Expression.difference((ExpConstant) binOp.getLHS(),
						(ExpConstant) binOp.getRHS());
				break;
			case PLUS:
				result = Expression.sum((ExpConstant) binOp.getLHS(),
						(ExpConstant) binOp.getRHS());
				break;
			case SLL:
				result = Expression.shiftLeftLogical(
						(ExpConstant) binOp.getLHS(),
						(ExpConstant) binOp.getRHS());
				break;
			case TIMES:
				result = Expression.multiply((ExpConstant) binOp.getLHS(),
						(ExpConstant) binOp.getRHS());
				break;
			case DIV:
				result = Expression.divide((ExpConstant) binOp.getLHS(),
						(ExpConstant) binOp.getRHS());
			default:
				break;

			}

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

	private int getMaxIterations(Range initValueR, Range thresholdR,
			Range incrValueR, ExpCompareOp.Type type) {
		// first check conditions
		int threshold = 0;
		int initValue = 0;
		int incrValue = 0;

		// Get the min or max depending on operation type
		if (type.equals(Type.LT) || type.equals(Type.LE)
				|| (type.equals(Type.NE) && incrValueR.isPositive())) {
			threshold = thresholdR.getMax();
			initValue = initValueR.getMin();
			incrValue = incrValueR.getMin();
		} else if (type.equals(Type.GT) || type.equals(Type.GE)
				|| (type.equals(Type.NE) && incrValueR.isNegative())) {
			threshold = thresholdR.getMin();
			initValue = initValueR.getMax();
			incrValue = incrValueR.getMax(); // largest negative -> smallest
												// step
		}

		// Double check that the constraints are respected
		if (!((thresholdR.getMin() > initValueR.getMax()
				&& incrValueR.getMin() > 0 && (type.equals(Type.LT)
				|| type.equals(Type.LE) || type.equals(Type.NE))) || (thresholdR
				.getMax() < initValueR.getMin() && incrValueR.getMax() < 0 && (type
				.equals(Type.GT) || type.equals(Type.GE) || type
					.equals(Type.NE))))) {
			return -1;
		}

		// Increment if including threshold
		if (type.equals(Type.LE)) {
			threshold++;
		} else if (type.equals(Type.GE)) {
			threshold--;
		}

		// Calculate
		return (int) Math.ceil((threshold - initValue) / ((double) incrValue));
	}
}