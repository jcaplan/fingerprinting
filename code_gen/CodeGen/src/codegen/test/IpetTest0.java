package codegen.test;

import lpsolve.LpSolve;
import lpsolve.LpSolveException;

public class IpetTest0 {
	static final int numEdges = 9;
	
	public static void main(String[] args) throws LpSolveException{
		
		
		LpSolve problem = LpSolve.makeLp(0, numEdges);
		for (int i = 1; i <= numEdges; i++) {
			problem.setInt(i, true);
		}
		

		problem.strAddConstraint(" 1  0  0  0  0  0  0  0  0", LpSolve.EQ, 1); // x1 = 1
		//problem.strAddConstraint(" 0  0  0  0  0  1  0  0  0", LpSolve.EQ, 1); //return the same number of times as entered
		
		
		
		problem.strAddConstraint(" 1 -1 -1  0  0  0  1  0  0", LpSolve.EQ, 0); // x1 + x7 - x2 - x3 = 0
		problem.strAddConstraint(" 0  1  0 -1  0  0  0  0  0", LpSolve.EQ, 0); // x2 - x4 = 0
		problem.strAddConstraint(" 0  0  1  0 -1  0  0  0  0", LpSolve.EQ, 0); // x3 - x5 = 0
		problem.strAddConstraint(" 0  0  0  1  1 -1 -1  0  0", LpSolve.EQ, 0); // x4 + x5 - x6 -x7 = 0
		
		
		//Did the loops wrong! need to identify backwards edge(s)
		problem.strAddConstraint("-9  0  0  0  0  0  1  0  0", LpSolve.LE, 0); // x1 + x7 -10x1 <= 0
		
		
		//Function calls are ok
		problem.strAddConstraint(" 0  0  0  0 -1  0  0  1  0", LpSolve.EQ, 0); // x8 - x5 = 0
		problem.strAddConstraint(" 0  0  0  0  0  0  0 -1  1", LpSolve.EQ, 0); // x9 - x8 = 0
		
		
		problem.strSetObjFn(" 1 2 1 1 2 1 1 6 3");

		problem.setMaxim();		
		problem.printLp();		
		problem.solve();
		problem.printObjective();
		problem.printSolution(1);
	}
}
