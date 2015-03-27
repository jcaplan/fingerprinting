package codegen.map;

import java.io.IOException;
import java.util.Random;

import org.jgap.*;

import codegen.hw.*;
import codegen.sw.*;
import codegen.util.*;

public class RAEngine {
	
	Application app;
	Platform platform;
	
	public RAEngine(Platform platform, Application app) {
		this.platform = platform;
		this.app = app;
	}
	
	public void findSolution() throws InvalidConfigurationException {

		MSEngine ms = new MSEngine(platform,app);

		int numTasks = app.getTaskList().size();	  
		
		RAConfiguration config = new RAConfiguration(app);
		
		RAChromosome chromosome = new  RAChromosome(numTasks, config);
		
		config.setFitnessFunction(new RAFitnessFunction(ms));
		config.setSampleChromosome(chromosome.getSampleChromosome());
		config.setPopulationSize(100);
		Genotype population = Genotype.randomInitialGenotype(config);
		for(int i = 0; i < 30; i++){
			population.evolve();
			IChromosome bestSolutionSoFar = population.getFittestChromosome();
			System.out.println("***************************************************");
			System.out.println(RAChromosome.getChromosomeString(bestSolutionSoFar));
			System.out.println(bestSolutionSoFar.getFitnessValue());
		}

	}


	public static void main(String[] args) throws IOException, InterruptedException, InvalidConfigurationException{
		Application app = new Application(); 
		DigraphGen g = new DigraphGen(1, 5, 6, 0.3, 0.25);
		g.buildGraph(app);
		app.build();
		
		
		Random rand = new Random();
		//give each task an execution time of 50-100ms
		for(Task t : app.getTaskList()){
			t.setExecutionTime(50 + rand.nextInt(50));
		}
		

		Platform platform = new Platform();
		platform.addProcessor(true, false,"FTC");
		platform.addProcessor(false, true,"PC0");
		platform.addProcessor(false, true,"PC1");
		platform.addProcessor(false, true,"PC2");
		platform.addProcessor(false, true,"PC3");
		
		RAEngine engine = new RAEngine(platform,app);
		engine.findSolution();
		
	}
	
}
