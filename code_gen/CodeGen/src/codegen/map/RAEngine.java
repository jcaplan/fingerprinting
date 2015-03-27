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
	static int count = 0;
	public RAEngine(Platform platform, Application app) {
		Schedule.initSchedule();
		this.platform = platform;
		this.app = app;
	}
	
	public double findSolution() throws InvalidConfigurationException {

		
		
		MSEngine ms = new MSEngine(platform,app);

		//If there is only lockstep, skip the RA stage and set the chromosome
		boolean hasFprint = false;
		for(Processor p : platform.processors){
			if(p.hasFprint){
				hasFprint = true;
				break;
			}
		}

		
		int numTasks = app.getTaskList().size();	
		String name = "RA" + count++;
		RAConfiguration config = new RAConfiguration(name,app);
		
		//If there are no fingerprint units, all cores are lockstep
		//skip straight to a single iteration of MS
		
		if(!hasFprint){
			RAChromosome chrom = new RAChromosome(numTasks,config);
			chrom.setAllFDtoLockstep();
			return ms.getBestSchedule(chrom.getSampleChromosome());
		}
		
		
		
		
		RAChromosome chromosome = new  RAChromosome(numTasks, config);
		config.setSampleChromosome(chromosome.getSampleChromosome());
		RAFitnessFunction ff = new RAFitnessFunction(ms);
		config.setFitnessFunction(ff);
		config.setPopulationSize(50);
		Genotype population = Genotype.randomInitialGenotype(config);
		double lastFitness = 0;
		int sameCount = 0;
		for(int i = 0; i < 30; i++){
			long time = System.currentTimeMillis();
			population.evolve();
			IChromosome bestSolutionSoFar = population.getFittestChromosome();
			
			if(lastFitness == bestSolutionSoFar.getFitnessValue()){
				sameCount++;
			} else {
				lastFitness = bestSolutionSoFar.getFitnessValue();
				sameCount = 0;
			}
			System.out.println("***************************************************");
			System.out.println(RAChromosome.getChromosomeString(bestSolutionSoFar));
			System.out.println("Fitness: " +bestSolutionSoFar.getFitnessValue());
			time = (System.currentTimeMillis() - time)/1000;
			System.out.println("time: " + time);
			System.out.println("generation: " + i);
			if(sameCount > 4){
				System.out.println("Seems to be converging. Quitting early.");
				break;
			}
		}
		return lastFitness;
	}


	public static void main(String[] args) throws IOException, InterruptedException, InvalidConfigurationException{
		Application app = new Application(); 
		DigraphGen g = new DigraphGen(1, 5, 6, 0.25, 0.4);
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
		
			
		Schedule.fileName = "fprint";
		RAEngine engine = new RAEngine(platform,app);
		double fprintSol = engine.findSolution();
		
		System.out.println("best time with fingerprinting: " + fprintSol);
		
		platform = new Platform();
		Schedule.fileName = "no_fprint";
		
		platform.addProcessor(true, false,"FTC0");
		platform.addProcessor(true, false,"FTC1");
		platform.addProcessor(true, false,"FTC2");
		
		engine = new RAEngine(platform,app);
		double sol = engine.findSolution();
		
		double fprintTime = 10000 - fprintSol;
		double nofprintTime = 10000 - sol;
		System.out.println("best time with fingerprinting: " + fprintTime);
		System.out.println("best time without fingerprinting: " + nofprintTime);
		double reduction = 100*(nofprintTime - fprintTime)/nofprintTime;
		System.out.println("reduction of " + reduction + "%");
		
		
		
	}
	
}
