package codegen.map;

import java.io.IOException;

import codegen.hw.Platform;
import codegen.sw.Application;
import codegen.sw.DigraphGen;
import codegen.sw.Task;

public class Mapper {
	
	public Mapper(){
	};
	
	
	public static void main(String[] args) throws IOException, InterruptedException{
		Application a = new Application(); 
		DigraphGen g = new DigraphGen(1, 5, 6, 0.3, 0.25);
		g.buildGraph(a);
		Qsort.sort(a.getTaskList());
		for(Task t : a.getTaskList()){
			t.printTaskStats();
		}
		
		a.getGraphs("appT");
		
		/* 
		 * Now there is a list of tasks with data dependencies.
		 * Assume all tasks have equal period of Tms. 
		 * The tasks must be mapped onto an architecture.
		 * A non-critical task can be run on any core.
		 * A critical task must be run either on a single FTC
		 * or two PCs *at the same time*. 
		 * 
		 *
		 */
		
		//Mapper can have a platform...
		
		Platform platform0 = new Platform();
		platform0.addProcessor(true, false,"FTC");
		platform0.addProcessor(false, true,"PC0");
		platform0.addProcessor(false, true,"PC1");
		platform0.addProcessor(false, true,"PC2");
		platform0.addProcessor(false, true,"PC3");
		
		//Rank will be helpful to get off to a good start
		//Need a data structure that connects tasks and processors...
		//How to represent the schedule?
	}
	
}
