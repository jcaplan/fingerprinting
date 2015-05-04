package codegen.map;


import org.jgap.*;

import codegen.hw.*;
import codegen.sw.*;

public class RAEngine {
	
	Application app;
	Platform platform;
	static int count = 0;
	public RAEngine(Platform platform, Application app) {
		Schedule.initSchedule();
		this.platform = platform;
		this.app = app;
	}
	
	/**
	 * This is the top level function responsible for doing DSE
	 * @return the fitness of the best solution
	 * @throws InvalidConfigurationException
	 */
	public double findSolution() throws InvalidConfigurationException {

		
		//The MS engine is required to find the best schedule for a given mapping
		
		MSEngine ms = new MSEngine(platform,app);

		//If there is only lockstep, skip the RA stage and set the chromosome
		boolean hasFprint = false;
		for(Processor p : platform.processors){
			if(p.hasFprint){
				hasFprint = true;
				break;
			}
		}

		//Just some random stuff to make JGAP work over several
		//iterations
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
		
		
		//Set up the RA chromosome
		RAChromosome chromosome = new  RAChromosome(numTasks, config);
		//The chromosome takes a sample template
		config.setSampleChromosome(chromosome.getSampleChromosome());
		//The fitness function is defined
		RAFitnessFunction ff = new RAFitnessFunction(ms);
		config.setFitnessFunction(ff);
		//Set the population size
		config.setPopulationSize(50);

		//The main loop that does the GA
		Genotype population = Genotype.randomInitialGenotype(config);
		double lastFitness = 0;
		int sameCount = 0;
		for(int i = 0; i < 30; i++){
			//Check how long the iteration takes
			long time = System.currentTimeMillis();
			//Do the iteration
			population.evolve();
			IChromosome bestSolutionSoFar = population.getFittestChromosome();
			
			//Check if the answer changed from last iteration
			if(lastFitness == bestSolutionSoFar.getFitnessValue()){
				sameCount++;
			} else {
				lastFitness = bestSolutionSoFar.getFitnessValue();
				sameCount = 0;
			}
			
			//Print out diagnostics
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
	


	
}
