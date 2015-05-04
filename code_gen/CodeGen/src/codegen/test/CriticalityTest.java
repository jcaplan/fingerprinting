package codegen.test;

import java.io.IOException;
import java.util.Random;

import org.jgap.InvalidConfigurationException;

import codegen.hw.Platform;
import codegen.map.RAConfiguration;
import codegen.map.RAEngine;
import codegen.map.Schedule;
import codegen.sw.Application;
import codegen.sw.DigraphGen;
import codegen.sw.Task;



public class CriticalityTest {
	
	static double doSingleTrial(double propCritical) throws IOException, InterruptedException, InvalidConfigurationException{

		//Build the random task graph
		
		Application app = new Application(); 
		DigraphGen g = new DigraphGen(1, 5, 6, 0.25, propCritical);
		g.buildGraph(app);
		app.build();
		
		//Assign execution times to each task in the task graph
		
		Random rand = new Random();
		//give each task an execution time of 50-100ms
		for(Task t : app.getTaskList()){
			t.setExecutionTime(50 + rand.nextInt(50));
		}
		
		
		//Build the platform
		Platform platform = new Platform();
		platform.addProcessor(true, false,"FTC");
		platform.addProcessor(false, true,"PC0");
		platform.addProcessor(false, true,"PC1");
		platform.addProcessor(false, true,"PC2");
		platform.addProcessor(false, true,"PC3");
		
			
		//Set up Schedule for printing best schedule
		Schedule.fileName = "fprint";
		
		//Start the RA DSE engine
		RAEngine engine = new RAEngine(platform,app);
		double fprintSol;

		fprintSol = engine.findSolution();
	
		//Repeat for second platform
		platform = new Platform();
		Schedule.fileName = "no_fprint";
		
		platform.addProcessor(true, false,"FTC0");
		platform.addProcessor(true, false,"FTC1");
		platform.addProcessor(true, false,"FTC2");
		
		engine = new RAEngine(platform,app);
		double sol = engine.findSolution();
		
		
		//Print out results for this trial
		double fprintTime = 10000 - fprintSol;
		double nofprintTime = 10000 - sol;
		System.out.println("best time with fingerprinting: " + fprintTime);
		System.out.println("best time without fingerprinting: " + nofprintTime);
		double reduction = 100*(nofprintTime - fprintTime)/nofprintTime;
		System.out.println("reduction of " + reduction + "%");
		return reduction;
	}
	
	/**
	 * Main function for test of RA DSE. Loops through DSE for given criticality factor
	 * 
	 * @param args
	 * @throws IOException
	 * @throws InterruptedException
	 * @throws InvalidConfigurationException
	 */
	public static void main(String[] args) throws IOException, InterruptedException, InvalidConfigurationException {

		double score = 0;
		for(int i = 0; i < 25; i++){
			double newScore = doSingleTrial(0.4); 
			score += newScore;
			System.out.println("trial" + i + ": " + newScore);
			RAConfiguration.reset();
			System.gc();
		}
		System.out.println("average score for 0 criticality: " + score/25.0);
		
		
		
	}
}
