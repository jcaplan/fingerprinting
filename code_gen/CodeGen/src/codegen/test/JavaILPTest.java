package codegen.test;

import net.sf.javailp.*;

public class JavaILPTest {

	public static void main(String[] args) {
		SolverFactory factory = new SolverFactoryLpSolve(); // use lp_solve

		
		/**
		* Constructing a Problem:
		* Maximize: 143x+60y
		* Subject to:
		* 120x+210y <= 15000
		* 110x+30y <= 4000
		* x+y <= 75
		*
		* With x,y being integers
		*
		*/
		
		factory.setParameter(Solver.VERBOSE, 0);

		factory.setParameter(Solver.TIMEOUT, 100); // set timeout to 100 seconds

		Problem problem = new Problem();

		Linear linear = new Linear();
		linear.add(143, "x");
		linear.add(60, "y");

		problem.setObjective(linear, OptType.MAX);

		linear = new Linear();
		linear.add(120, "x");
		linear.add(210, "y");

		problem.add(linear, "<=", 15000);

		linear = new Linear();
		linear.add(110, "x");
		linear.add(30, "y");

		problem.add(linear, "<=", 4000);

		linear = new Linear();
		linear.add(1, "x");
		linear.add(1, "y");

		problem.add(linear, "<=", 75);

		problem.setVarType("x", Integer.class);
		problem.setVarType("y", Integer.class);

		Solver solver = factory.get(); // you should use this solver only once
										// for one problem
		Result result = solver.solve(problem);

		System.out.println(result);

		/**
		 * Extend the problem with x <= 16 and solve it again
		 */
		problem.setVarUpperBound("x", 16);

		solver = factory.get();
		result = solver.solve(problem);

		System.out.println(result);
	}
}
