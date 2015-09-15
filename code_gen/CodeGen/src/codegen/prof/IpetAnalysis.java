package codegen.prof;

import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;

import javax.management.RuntimeErrorException;

import com.sun.org.apache.bcel.internal.generic.RETURN;

import lpsolve.LpSolve;
import lpsolve.LpSolveException;
import codegen.prof.BasicBlock.bbType;

public class IpetAnalysis {
	private static final double defaultMaxLoop = 15;
	Function rootFunc;
	HashMap<Function, Integer> calledFunctions;
	
	
	CFG cfg;
	private final int LE = 1;
	private final int EQ = 3;
	private final int GE = 2;
	ArrayList<Edge> edges;

	public IpetAnalysis(String funcName, CFG cfg) {
		rootFunc = cfg.getFunction(funcName);
		this.cfg = cfg;
	}

	public int getWCET(boolean clockCycles) throws LpSolveException, FileNotFoundException {
		return getWCET(rootFunc, clockCycles);

	}

	private int getWCET(Function func, boolean clockCycles) throws LpSolveException {

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
		constraint[func.getEntryEdge().index] = 1;
		problem.addConstraint(constraint, EQ, 1);

		ArrayList<Loop> singleBlockLoops = new ArrayList<>();
		double singleBlockLoopBound = 0;

		// ////////////////////////////////////////////////////////////////////////////////////////////////////
		for (Function f : calledFunctions.keySet()) {
			for (BasicBlock bb : f.bbList) {
				constraint = new double[constraintSize];
				if(bb.type != BasicBlock.bbType.RETURN){
					// All blocks have predecessors (root block is empty and
					// ignored)
					// --------------------------------------------------------------
					for (Edge e : bb.predEdges) {
						// Ignore loops consisting of a single BB
						if (!e.isSingleBBLoop()) {
							constraint[e.index] = 1;
						}
					}

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

			// Certain library functions have limits on their loops known in
			// advance for double precision floating poitnt
			// -------------------------------------------------------------
			//TODO check if function has a loop bound after configuration 
			double loopBound = 0;
			switch (f.label) {
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
				loopBound = 1;
				break;
			case "__divdf3":
				loopBound = 61;
				break;
			default:
				loopBound = defaultMaxLoop;
				break;
			}

			for (int i = 0; i < f.loops.size(); i++) {
				Loop l = f.loops.get(i);
				// Deal with single Block loops afterwards if they exist
				if (l.head.equals(l.tail)) {
					singleBlockLoops.add(l);
					singleBlockLoopBound = loopBound;
				} else {
					
					// sum(all edges in) - sum(loopbound* nonbackwardeges) <= 0
					
					constraint = new double[constraintSize];
					for (Edge e : l.head.predEdges) {
						System.out.println(e);
						constraint[e.index] = 1;
						
						if(!l.body.contains(e.startBlock)){ // then it is a backwards edge
							constraint[e.index] -= loopBound;
						}
					}
					problem.addConstraint(constraint, LE, 0);
				}
			}


		}
		// ///////////////////////////////////////////////////////////////////////////////////

		// For each function, get all edges from blocks that call that function
		// and add a constraint to the entry edge for that function
		// --------------------------------------------------------------------
		
		for(Function f : calledFunctions.keySet()){
			boolean called = false;
			constraint = new double[constraintSize];
			for(Function caller : calledFunctions.keySet()){
				for(BasicBlock bb : caller.bbList){
					if(bb.type == bbType.CALL && bb.callee.equals(f)){
						called = true;
						constraint[bb.succEdges.get(0).index] = 1;
					}
				}
			}
			if(called){
				constraint[f.getEntryEdge().index] = -1;
				problem.addConstraint(constraint, EQ, 0);
			}
			
		}
		
		

		// Construct objective function, Each code line is worth 1 for now
		// ---------------------------------------------------------------
		constraint = new double[constraintSize];
		for(Function f : calledFunctions.keySet()){
			for (BasicBlock bb : f.bbList) {
				for (Edge e : bb.predEdges) {
					if(clockCycles){
						constraint[e.index] += bb.getCost(10);
					} else {
						constraint[e.index] += bb.getNumInstructions();
					}
						
				}
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

			// All non loop entries together must be greater than 1
			// ----------------------------------------------------
			problem.addConstraint(constraint, GE, 1);
			
			// The loop must be <= LoopBound *(all the other entry points)
			// -----------------------------------------------------------
			for(int i = 0; i < constraint.length; i++){
				if(constraint[i] > 0){
					constraint[i] -= singleBlockLoopBound;
				}
			}
			constraint[selfEdge.index] = 1;
			problem.addConstraint(constraint, LE, 0);

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


		// Costs: Branch -> 4 (Assume mispredicted)
		// jmp,call,return -> 4
		// load,store -> 1 + memory delay
		// else -> 1
		System.out.println("WCET for func " + func + " = " + wcet);
		return wcet;
	}

	private void buildEdges(Function func) {
		calledFunctions = new HashMap<>();
		for(Function f : func.getAllCalledFunctions()){
			calledFunctions.put(f, 0);
		}
		edges = new ArrayList<Edge>();
		calledFunctions.put(func, 1);
		
		//Doesn't take into account loops!!
		//TODO
		for (Function f : calledFunctions.keySet()) {
			for (BasicBlock bb : f.bbList) {
				if (bb.type == bbType.CALL) {
					if (calledFunctions.containsKey(bb.callee)) {
						int oldValue = calledFunctions.get(bb.callee);
						calledFunctions.put(bb.callee, oldValue + 1);
					} else {
						calledFunctions.put(bb.callee, 1);
					}
				}
			}
			
			// Add the first edge
			edges.add(new Edge(null, f.bbList.get(0)));
			for (BasicBlock bb : f.bbList) {
				for (BasicBlock succ : bb.successors) {
					edges.add(new Edge(bb, succ));
				}	
			}
		}
		
		for(Edge e : edges){
			System.out.println(e);
		}
	}
}
