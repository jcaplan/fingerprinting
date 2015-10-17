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
	public static final double defaultMaxLoop = 15;
	Function rootFunc;
	HashMap<Function, Integer> calledFunctions;
	ArrayList<Annotation> annotations;
	
	public static final double WCET_RATIO = 1.5;
	
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


			for (int i = 0; i < f.loops.size(); i++) {
				Loop l = f.loops.get(i);
				// sum(all edges in) - sum(loopbound* nonbackwardeges) <= 0
				
				constraint = new double[constraintSize];
				for (Edge e : l.head.predEdges) {
//					System.out.println(e);
					constraint[e.index] = 1;
					
					if(!l.body.contains(e.startBlock)){ 
						// then it is not a backwards edge
						constraint[e.index] = -l.maxIterations;
					}
				}
				problem.addConstraint(constraint, LE, 0);
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

		
		problem.printLp();
		problem.solve();

		problem.printObjective();
		problem.printSolution(1);

		// The value of the objective function is the WCET
		// -----------------------------------------------
		wcet = (int) problem.getObjective();
	


		// Costs: Branch -> 4 (Assume mispredicted)
		// jmp,call,return -> 4
		// load,store -> 1 + memory delay
		// else -> 1
		System.out.println("WCET for func " + func + " = " + wcet);
		return wcet;
	}

	public void buildEdges(Function func) {
		calledFunctions = new HashMap<>();
		for(Function f : func.getAllCalledFunctions()){
			calledFunctions.put(f, 0);
		}
		edges = new ArrayList<Edge>();
		calledFunctions.put(func, 1);
		
		//Doesn't take into account loops!!
		//Values of hash table not needed.. deprecated!

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
