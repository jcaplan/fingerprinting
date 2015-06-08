package codegen.prof;

import java.io.*;
import java.util.*;

import javax.management.RuntimeErrorException;

import lpsolve.LpSolve;
import lpsolve.LpSolveException;
import codegen.prof.BasicBlock.bbType;

public class IpetAnalysis {
	private static final double defaultMaxLoop = 15;
	Function rootFunc;
	HashMap<Function, Integer> visitedList;
	CFG cfg;
	private final int LE = 1;
	private final int EQ = 3;
	private final int GE = 2;
	ArrayList<Edge> edges;

	public IpetAnalysis(String funcName, CFG cfg) {
		rootFunc = cfg.getFunction(funcName);
		this.cfg = cfg;
		this.visitedList = new HashMap<>();
	}

	public int getWCET() throws LpSolveException, FileNotFoundException {
		return getWCET(rootFunc);

	}

	private int getWCET(Function func) throws LpSolveException {
		if (visitedList.containsKey(func)) {
			return visitedList.get(func);
		}

		System.out.println("calculating WCET for function " + func);
		int wcet = 0;

		// Build the edges
		// ---------------
		buildEdges(func);

		// Set up ILP with one variable per edge and no constraints
		// --------------------------------------------------------
		LpSolve problem = LpSolve.makeLp(0, edges.size());
		for (int i = 1; i <= edges.size(); i++) {
			problem.setInt(i, true);
		}

		// First set the entry edge to 1
		// NumEdges egdges as well as the constraint itself
		// Array must be indexed from offset 1...
		// ------------------------------------------------
		int constraintSize = edges.size() + 1;
		double[] constraint = new double[constraintSize];
		constraint[1] = 1;
		problem.addConstraint(constraint, EQ, 1);

		for (BasicBlock bb : func.bbList) {
			constraint = new double[constraintSize];

			// All blocks have predecessors (root block is empty and ignored)
			// --------------------------------------------------------------
			for (Edge e : bb.predEdges) {
				// Ignore loops consisting of a single BB
				if (!e.isSingleBBLoop()) {
					constraint[e.index] = 1;
				}
			}
			// Return types don't have successors, and need a LE constraint
			// Otherwise add successors and do sum_in - sum_out = 0
			// ------------------------------------------------------------
			if (bb.type == bbType.RETURN) {
				problem.addConstraint(constraint, LE, 1);
			} else {
				for (Edge e : bb.succEdges) {
					// Ignore loops consisting of a single BB
					if (!e.isSingleBBLoop()) {
						constraint[e.index] = -1;
					}
				}
				problem.addConstraint(constraint, EQ, 0);
			}

		}

		// Determine the number of times loop executes
		// For now, either hard-set measured number for library functions
		// or else default guess
		// --------------------------------------------------------------

		ArrayList<Loop> singleBlockLoops = new ArrayList<>();

		// Certain library functions have limits on their loops known in
		// advance for double precision floating poitnt
		// -------------------------------------------------------------
		double loopBound = 0;
		switch (func.label) {
		case "__muldf3":
			loopBound = 4;
			break;
		case "__mulsi3":
			loopBound = 32;
			break;
		case "__unpack_d":
			loopBound = 1;
			break;
		case "_fpadd_parts":
			loopBound = 14;
			break;
		case "__divdf3":
			loopBound = 61;
			break;
		default:
			loopBound = defaultMaxLoop;
			break;
		}

		for (int i = 0; i < func.loops.size(); i++) {
			Loop l = func.loops.get(i);
			// Deal with single Block loops afterwards if they exist
			if (l.head.equals(l.tail)) {
				singleBlockLoops.add(l);
			} else {
				constraint = new double[constraintSize];
				for (Edge e : l.head.predEdges) {
					System.out.println(e);
					constraint[e.index] = 1;
				}
				problem.addConstraint(constraint, LE, loopBound);
			}
		}

		// Construct objective function, Each code line is worth 1 for now
		// ---------------------------------------------------------------
		constraint = new double[constraintSize];
		for (BasicBlock bb : func.bbList) {
			for (Edge e : bb.predEdges) {
				constraint[e.index] += bb.code.size(); // All instructions same
														// cost = 1
			}
		}
		problem.setMaxim();
		problem.setObjFn(constraint);

		if (!singleBlockLoops.isEmpty()) { // Single block loops were found
			int numConstraints = problem.getNrows();
			constraint = new double[constraintSize];
			// If more than one self loop, throw an error for now
			if (singleBlockLoops.size() > 1) {
				throw new RuntimeErrorException(new Error(
						"Multiple single block loops not supported yet"));
			}

			// For each loop, need to consider the two cases:
			// 1. loop > 0 and other entries > 0
			Edge selfEdge = new Edge();
			Loop loop = singleBlockLoops.get(0);
			for (Edge e : loop.head.predEdges) {
				if (e.isSingleBBLoop()) {
					selfEdge = e;
				} else {
					constraint[e.index] = 1;
				}

			}

			int maxBranch;

			problem.addConstraint(constraint, GE, 1);
			constraint[selfEdge.index] = 1;
			problem.addConstraint(constraint, LE, loopBound);

			problem.solve();
			maxBranch = (int) problem.getObjective();

			problem.printObjective();
			problem.printSolution(1);
			// 2. all = 0

			// Remove the two constraints
			problem.delConstraint(numConstraints + 2);
			problem.delConstraint(numConstraints + 1);

			for (Edge e : loop.head.predEdges) {
				constraint = new double[constraintSize];
				constraint[e.index] = 1;
				problem.addConstraint(constraint, EQ, 0);
			}

			problem.solve();
			int otherBranch = (int) problem.getObjective();
			if (otherBranch > maxBranch) {
				maxBranch = otherBranch;
			}

			problem.printObjective();
			problem.printSolution(1);

			wcet = maxBranch;

		} else { // There were no single block loops
			problem.printLp();
			problem.solve();

			problem.printObjective();
			problem.printSolution(1);

			// The value of the objective function is the WCET
			// -----------------------------------------------
			wcet = (int) problem.getObjective();
		}

		// Now for each function call, the WCET of that function
		// must be added to the overall WCET. This must be applied
		// recursively. Recursive function calls cannot be handled.
		// --------------------------------------------------------

		// 1. which functions are called?
		// 2. How many times are they called?

		HashMap<Function, Integer> calledFunctions = new HashMap<>();
		for (BasicBlock bb : func.bbList) {
			if (bb.type == bbType.CALL) {
				if (calledFunctions.containsKey(bb.callee)) {
					int oldValue = calledFunctions.get(bb.callee);
					calledFunctions.put(bb.callee, oldValue + 1);
				} else {
					calledFunctions.put(bb.callee, 1);
				}
			}
		}

		for (Function f : calledFunctions.keySet()) {
			System.out.println("Function " + f + " is called "
					+ calledFunctions.get(f) + " times");
		}

		// For each function repeat analysis, keep track of which functions have
		// already been called. Do not repeat functions
		// ----------------------------------------------------------------------
		visitedList.put(func, wcet);
		for (Function f : calledFunctions.keySet()) {
			wcet += getWCET(f) * calledFunctions.get(f);
		}

		// Costs: Branch -> 4 (Assume mispredicted)
		// jmp,call,return -> 4
		// load,store -> 1 + memory delay
		// else -> 1
		System.out.println("WCET for func " + func + " = " + wcet);
		return wcet;
	}

	private void buildEdges(Function func) {
		Edge.EdgeCount = 1;
		edges = new ArrayList<Edge>();
		// Add the first edge
		edges.add(new Edge(null, func.bbList.get(0)));
		for (BasicBlock bb : func.bbList) {
			for (BasicBlock succ : bb.successors) {
				edges.add(new Edge(bb, succ));
			}
		}
	}
}
