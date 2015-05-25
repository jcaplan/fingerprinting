package codegen.prof;

import java.util.*;

import lpsolve.*;
import codegen.prof.BasicBlock.*;

public class IpetAnalysis {
	Function rootFunc;
	CFG cfg;
	
	public IpetAnalysis(String funcName, CFG cfg){
		rootFunc = cfg.getFunction(funcName);
		this.cfg = cfg;
	}

	public int getWCET() throws LpSolveException {
		//Get the rootFunc function
		//---------------------
		System.out.println(rootFunc.bbList.size() + " basic blocks");
		
		
		
		//Just for fun lets see which functions are called from the rootFunc
		//--------------------------------------------------------------
		ArrayList<Function> calledFunctions = new ArrayList<Function>();
		for (BasicBlock bb : rootFunc.bbList){
			if(bb.type == bbType.CALL){
				
				calledFunctions.add(bb.callee);
			}
		}
		
		for(Function f : calledFunctions){
			System.out.println("Function " + f + "is called");
		}
		
		
		//How many edges are there? (set to 1 since there is an entry edge)
		//-----------------------------------------------------------------
		int numEdges = 1;
		for (BasicBlock bb : rootFunc.bbList){
			numEdges += bb.successors.size();
		}
		System.out.println("Number of edges in graph: " + numEdges);
		
		//Build the edges
		ArrayList<Edge> edges = new ArrayList<Edge>();
		//Add the zeroth edge
		edges.add(new Edge(null,rootFunc.bbList.get(0)));
		for(BasicBlock bb : rootFunc.bbList){
			for(BasicBlock succ : bb.successors){
				edges.add(new Edge(bb,succ));
			}
		}
		
		for(Edge e : edges){
			System.out.println(e);
		}
		
		//Set up ILP with one variable per edge and no constraints
		//--------------------------------------------------------
		
		LpSolve problem = LpSolve.makeLp(0,numEdges);
		for(int i = 1; i <= numEdges; i++){
			problem.setInt(i, true);
		}
		
		

		//Edges assigned numbers based on breadth first left-right
		//--------------------------------------------------------
		
		
		
		//First set the entry edge to 1
		//NumEdges egdges as well as the constraint itself
		double[] constraint = new double[numEdges + 1];
		constraint[0] = 1;
		
		problem.setObjFn(constraint);
		
		
		problem.strSetObjFn("2 3 -2 3");
		problem.strAddConstraint("0 4 3 1", LpSolve.GE, 3);
		problem.strAddConstraint("3 2 2 1", LpSolve.LE, 4);
		for(int i = 1; i <= 4; i++){
			problem.setInt(i, true);
		}
		problem.setMaxim();
		problem.solve();
		
		//For each basic block there must be a constraint
		problem.printLp();
		problem.printObjective();
		problem.printSolution(1);
		
		
		//each basic block requires two equations (some overlap will happen)
		
		
		
		// TODO Auto-generated method stub
		// 1. Get the rootFunc block
		// 2. Traverse the successors, calculating the WCET of each block
		// 3. If the block is a call or jump and the callee is known
		//    calculate and add the WCET of the callee
		// 4. If the block is the head of a loop, then calculate the WCET of the loop
		//    The loop may have a specified max iterations later on, for now assume a number
		//    The loop WCET will be MAXITER*WCET. If there are multiple exit points, then the loop is
		//    WCET*(MAXITER - 1) + maxWCET(successors)
		
		//Costs: Branch -> 4 (Assume mispredicted)
		//       jmp,call,return -> 4
		//       load,store -> 1 + memory delay
		//       else -> 1
		
		return 0;
	}
	
}
