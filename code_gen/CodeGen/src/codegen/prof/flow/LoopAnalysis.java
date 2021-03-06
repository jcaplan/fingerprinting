package codegen.prof.flow;

import codegen.prof.*;
import codegen.prof.flow.ExpCompareOp.Type;

import java.util.*;

public class LoopAnalysis {

	Function root;
	ReachingExp reachingExp;
	public LoopAnalysis(Function f) {
		this.root = f;
	}

	public boolean analyze() {
		DomFrontier domFrontier = new DomFrontier(root);
		domFrontier.analyze();
		PhiInsertion phiInsert = new PhiInsertion(root);
		phiInsert.analyze();
		RenameSSA ssa = new RenameSSA(root,phiInsert.getVarSet());
		ssa.analyze();
		reachingExp = new ReachingExp(root);
		reachingExp.analyze();
		reachingExp.prettyPrint();
		boolean allPassed = true;
		// First do constant propagation on the entire function
		// ----------------------------------------------------
		ArrayList<BasicBlock> backwardEdges = new ArrayList<>();
		ArrayList<BasicBlock> exitPoints = new ArrayList<>();
		ArrayList<Loop> successfulLoops = new ArrayList<>();
		ArrayList<Loop> unsuccessfulLoops = new ArrayList<>();
 		
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
				unsuccessfulLoops.add(l);
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
				unsuccessfulLoops.add(l);
				continue;
			}

			try {
				findLoopParameters(l, bbBackwardsEdge, bbExitPoint);
			} catch (LoopAnalysisException e) {
				e.printStackTrace();
				allPassed = false;
				unsuccessfulLoops.add(l);
				continue;

			}
			
			backwardEdges.add(bbBackwardsEdge);
			exitPoints.add(bbExitPoint);
			successfulLoops.add(l);
		}
		
		//TODO if unsuccessful loops > 0, check for annotation.. if neither error
		for(Loop l : unsuccessfulLoops){
			if (l.getMaxIterations() < 0){
				throw new RuntimeException("LoopAnalysis:: " + l + ": does not have annotation and loop analysis failed");
			}
		}
		
		//Now check branches.t..
		// Now check other basic block branches besides the head

		ArrayList<BasicBlock> branchesToTest = new ArrayList<>();
		
		for(Loop l : successfulLoops){
			for(BasicBlock bb : l.getBody()){
				if(bb.isBranch() && bb.isConditional()){
					branchesToTest.add(bb);
				}
			}
		}
		
		//remove exit edges and backwards edges
		branchesToTest.removeAll(exitPoints);
		branchesToTest.removeAll(backwardEdges);
		
		for(BasicBlock bb : branchesToTest){
			//find the oldest containing loop
			Loop innerLoop = null;
			for(Loop l : successfulLoops){
				if(innerLoop == null){
					innerLoop = l;
				} else {
					if(l.getHead().getAge() > innerLoop.getHead().getAge()){
						innerLoop = l;
					}
				}
			}
			if(innerLoop != null){
				checkBranch(innerLoop,bb);
			}
		}
		
		
		return allPassed;
	}

	private void checkBranch(Loop l, BasicBlock bb) {
		HashMap<String, List<Expression>> conditionOut = (HashMap<String, List<Expression>>) reachingExp
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
		

		String id = getLeftmostIdentifier(branchCondition);
		if(!id.equals(l.getInductionVar())){
			System.out.println("Expression not operating on induction variable");
			return;
		}
		Range threshold = null;
		
		// Get the threshold value
		// -----------------------
		try {
			threshold = getRangeFromExp(branchCondition.getRHS());
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
			bb.setMaxFalseBranch(l.getMaxIterations() - maxIterations);
			bb.setInnerLoop(l);
		} else {
			System.out.println("could not determine maxTrueBranch");
		}
	}

	private String getLeftmostIdentifier(Expression exp) {
		String name = "";
		while(exp.children != null && exp.children.length == 2){
			exp = exp.children[0];
		}
		if(exp instanceof ExpIdentifier){
			name = ((ExpIdentifier)exp).id;
		}
		return name;
	}

	private void findLoopParameters(Loop l, BasicBlock bbBackwardsEdge,
			BasicBlock bbExitPoint) throws LoopAnalysisException {

		// Get the branch condition, simplified and reduced
		// ------------------------------------------------

		HashMap<String, List<Expression>> conditionOut = (HashMap<String, List<Expression>>) reachingExp
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
		Expression initExp = null;
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
			HashMap<String, List<Expression>> outSet = (HashMap<String, List<Expression>>) reachingExp
					.getOutSet(l.getTail());
			
			//Index should be the index in the outset			
			String root = PhiCode.getDefRoot(iterator);
			String incrVar = null;
			for(String key : outSet.keySet()){
				if(key.startsWith(root)){
					incrVar = key;
					break;
				}
			}
			if (reachingExp.getSymbolExpList(incrVar).size() != 1) {
				throw new LoopAnalysisException(
						"More than one reaching def found for increment value");
			}

			Expression change = reachingExp.getSymbolExpList(incrVar).get(0);
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

		// Now find the initial value for iterator
		// ---------------------------------------
		List<Expression> rhsList  = getMergedOutList(l,iterator);

		boolean nonConstantInit = false;
		for(Expression exp : rhsList){
			if(!(exp instanceof ExpConstant)){
				nonConstantInit = true;
				break;
			}
		}
		
		if(nonConstantInit){
			if(rhsList.size() > 1){			
				throw new LoopAnalysisException("Found multiple non-constant reaching expressions for induction initialization");
			}
			initExp = rhsList.get(0);
			
			String initId = getLeftmostIdentifier(initExp);
			String thresId = getLeftmostIdentifier(thresholdExp);
			
			if(!initId.isEmpty() && initId.equals(thresId)){
				if(initExp instanceof ExpIdentifier){
					initExp = new ExpConstant(0);
				} else if(initExp instanceof ExpBinOp){
					((ExpBinOp) initExp).setLHS(new ExpConstant(0));
					initExp = simplifyBinOp((ExpBinOp) initExp);
				}
				if(thresholdExp instanceof ExpIdentifier){
					thresholdExp = new ExpConstant(0);
				} else if(thresholdExp instanceof ExpBinOp){
					((ExpBinOp) thresholdExp).setLHS(new ExpConstant(0));
					thresholdExp = simplifyBinOp((ExpBinOp) thresholdExp);
				}			
			}
		}
		initValue = getRange(rhsList);
		if (initValue == null) {
			throw new LoopAnalysisException(
					"Could not determine initial value range");
		}
		threshold = getRangeFromExp(thresholdExp);
		incrValue = getRangeFromExp(incrExp);
		if(threshold == null){
			throw new LoopAnalysisException(
					"Could not determine range value range");
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

	private List<Expression> getMergedOutList(
			Loop l, String iterator) {
		
		BasicBlock head = l.getHead();
		
		//find the predecessors not in the body and get their indices
		ArrayList<Integer> entryIndices = new ArrayList<>();
		for(BasicBlock pred : head.getPredecessors()){
			if(!l.getBody().contains(pred)){
				entryIndices.add(head.getPredIndex(pred));
			}
		}
		
		if(entryIndices.size() == 1){
			//the last variable holds the interesting value
			String initVar = PhiCode.getDefKey(iterator,-1);
			//get the output of the last basic block
			//find the predecessor
			BasicBlock pred = head.getPredecessors().get(entryIndices.get(0));
			return reachingExp.getOutSet(pred).get(initVar);
		}
		
		//more than one predecessor 
		
		//get the phi function for the iterator
		PhiCode phi = null;
		for(Code c : head.getCode()){
			if(c instanceof PhiCode){
				PhiCode cc = (PhiCode) c;
				if(cc.def.equals(iterator)){
					phi = cc;
					break;
				}
			}
		}
		
		ArrayList<Expression> result = new ArrayList<>();
		// Now build a list using the info from the Phi
		for(int i : entryIndices){
			String def = phi.getOperands()[i];
			List<Expression> list = (reachingExp.getInSet(phi)).get(def);
			for(Expression e : list){
				if(!result.contains(e)){
					result.add(e);
				}
			}
			
		}
		return result;
	}

	private Range getRangeFromExp(Expression exp) throws LoopAnalysisException {
		Range range = null;
		if (exp instanceof ExpConstant) {
			range = new Range(((ExpConstant) exp).value);
		} else if (exp instanceof ExpIdentifier) {
			String name = ((ExpIdentifier) exp).id;
			List<Expression> rhsList = reachingExp.getSymbolExpList(name);
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
			case SRL:
				result = Expression.shiftRightLogical(
						(ExpConstant) binOp.getLHS(),
						(ExpConstant) binOp.getRHS());
				break;
			case SRA:
				result = Expression.shiftRightArithmetic(
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
		// if (!(cond1 || cond2))
		if (!((thresholdR.getMin() >= initValueR.getMax()
				&& incrValueR.getMin() > 0 && (type.equals(Type.LT)
				|| type.equals(Type.LE) || type.equals(Type.NE))) || (thresholdR
				.getMax() <= initValueR.getMin() && incrValueR.getMax() < 0 && (type
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