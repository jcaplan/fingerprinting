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

			// if block is both head and tail then it's pretty easy
			// ---------------------------------------------------------
			try {
				if (bbBackwardsEdge.equals(bbExitPoint)) {
					optimizedPattern(l, bbBackwardsEdge, bbExitPoint);
				} else {
					unoptimizedPattern(l, bbBackwardsEdge, bbExitPoint);
				}

			} catch (LoopAnalysisException e) {
				e.printStackTrace();
				allPassed = false;
				continue;

			}

		}
		return allPassed;
	}

	private void unoptimizedPattern(Loop l, BasicBlock bbBackwardsEdge,
			BasicBlock bbExitPoint) throws LoopAnalysisException {

		// Analyze exit edge to get branch condition
		// -----------------------------------------
		ArrayList<BasicBlock> bExitEdge = new ArrayList<>();
		bExitEdge.add(bbExitPoint);

		ExpressionBuilder headExpBuilder = new ExpressionBuilder(bExitEdge);

		headExpBuilder.analyze();
		HashMap<String, Expression> conditionOut = (HashMap<String, Expression>) headExpBuilder
				.getOutSet(bbExitPoint);
		ExpBranchCond branchCondition = (ExpBranchCond) conditionOut
				.get(bbExitPoint.getLastCode().getCodeKey());

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

		// If we make it here then the branch condition should be
		// of the form x <> c...

		String iterator = ((ExpIdentifier) branchCondition.getLHS()).id;
		System.out.println("Iterator id: " + iterator);

		int bound = ((ExpConstant) branchCondition.getRHS()).value;
		System.out.println("loop bound: " + bound);
		System.out.println("loop condition: " + branchCondition.type);

		// Now that the iterator has been identified we can find it's initial
		// value and
		// the change that happens over one iteration
		// 2. Initial value
		// -----------------

		// Get iterator from merged outset of entry blocks
		// (excluding backwards edge)
		// -----------------------------------------------
		ArrayList<BasicBlock> entryList = l.getEntryBlocks();
		Map<String, Expression> inSet = constProp.getMergedOutSet(entryList);
		int value = ((ExpConstant) inSet.get(iterator)).value;

		System.out.println("Initial value: " + value);

		// Find expression for iterator after one loop through body

		ArrayList<BasicBlock> workList = new ArrayList<>(l.getBody());
		workList.remove(l.getHead());
		headExpBuilder = new ExpressionBuilder(workList);
		headExpBuilder.analyze();

		HashMap<String, Expression> outSet = (HashMap<String, Expression>) headExpBuilder
				.getOutSet(l.getTail());

		constants = (HashMap<String, Expression>) constProp.getOutSet(l
				.getTail());

		Expression change = outSet.get(iterator);

		change = replaceConstants(constants, change);
		ExpBinOp binOp = null;
		if ((change instanceof ExpBinOp)) {
			binOp = (ExpBinOp) change;
			binOp = (ExpBinOp) simplifyBinOp(binOp);
			System.out.println("check matching identifier:"
					+ ((ExpIdentifier) binOp.getLHS()).id.equals(iterator));
			System.out.println("change operator: " + binOp.type);
			System.out.println("change increment: " + binOp.getRHS());
		}

		if (binOp != null) {
			System.out.println(String.format(
					"for(i = %d; i %s %d; i = i %s %d);", value,
					branchCondition.type.toString(), bound, binOp.type,
					((ExpConstant) binOp.getRHS()).value));
		} else {
			throw new LoopAnalysisException("Could not parse branch condition");
		}
		l.setMaxIterations(getMaxIterations(value, bound,
				((ExpConstant) binOp.getRHS()).value, branchCondition.type));
		System.out.println("max iterations: " + l.getMaxIterations());
	}

	
	private boolean optimizedPattern(Loop l, BasicBlock bbBackwardsEdge,
			BasicBlock bbExitPoint) throws LoopAnalysisException {

		// Analyze exit edge to get branch condition
		// -----------------------------------------

		if (l.getHead().equals(l.getTail())) {
			// If there is only a single block then remove the backwards edge
			bbExitPoint = new BasicBlock(bbExitPoint);
			bbExitPoint.removeSuccessor(l.getHead());
			bbExitPoint.removePredecessor(l.getHead());
		}

		ArrayList<BasicBlock> bExitEdge = new ArrayList<>();
		bExitEdge.add(bbExitPoint);

		ExpressionBuilder headExpBuilder = new ExpressionBuilder(bExitEdge);

		headExpBuilder.analyze();
		HashMap<String, Expression> conditionOut = (HashMap<String, Expression>) headExpBuilder
				.getOutSet(bbExitPoint);
		ExpBranchCond branchCondition = (ExpBranchCond) conditionOut
				.get(bbExitPoint.getLastCode().getCodeKey());

		// The branch condition should already be cleaned up
		// by the compiler. Check the format
		// -------------------------------------------------
		if (!(branchCondition.getLHS() instanceof ExpBinOp)) {
			throw new LoopAnalysisException(
					"Expect simple binary operation on lhs of branch condition");
		}

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
		String iterator = ((ExpIdentifier) lhs.getLHS()).id;
		int incrValue = ((ExpConstant) lhs.getRHS()).value; 
		HashMap<String, Expression> constants;
		constants = (HashMap<String, Expression>) constProp
				.getOutSet(bbExitPoint);
		branchCondition = (ExpBranchCond) replaceConstants(constants,
				branchCondition);
		int threshold = ((ExpConstant) branchCondition.getRHS()).value;
		
		
		ArrayList<BasicBlock> entryList = l.getEntryBlocks();
		Map<String, Expression> inSet = constProp.getMergedOutSet(entryList);
		int value = ((ExpConstant) inSet.get(iterator)).value;

		System.out.println("Initial value: " + value);
		System.out.println("iterator: " + iterator);
		System.out.println("increment: " + incrValue);
		System.out.println("threshold: " + threshold);
		// Get the initial value
		int maxIterations = getMaxIterations(value, threshold, incrValue, branchCondition.type);
		l.setMaxIterations(maxIterations);
		System.out.println(maxIterations);
		return false;
	}

	private boolean trueCondStaysInLoop(Loop loop, BasicBlock bbExitPoint) {
		return loop.containsAddress(bbExitPoint.getBranchAddress());
	}

	private ExpBranchCond simplifyBranchCondition(ExpBranchCond cond)
			throws LoopAnalysisException {

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

	private ArrayList<BasicBlock> getPrunedTree(
			ArrayList<BasicBlock> basicBlockList, BasicBlock head) {

		ArrayList<BasicBlock> bbList = new ArrayList<>();

		for (BasicBlock bb : basicBlockList) {
			if (bb.getAge() < head.getAge()) {
				bbList.add(bb);
			}
		}
		bbList.add(head);

		return bbList;
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