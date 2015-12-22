package codegen.map;

import org.jgap.Chromosome;
import org.jgap.Configuration;
import org.jgap.FitnessFunction;
import org.jgap.Genotype;
import org.jgap.IChromosome;
import org.jgap.InvalidConfigurationException;

import codegen.util.Logger;

public class GAEngine {


	private static final int populationSize = 100;
	private int numGenerations = 30;
	private static final double MAX_FITNESS = 1.0;
	private boolean quitEarly = true;
	
	FitnessFunction ff;
	Configuration config;
	Chromosome sampleChromosome;
	Genotype population;

	public GAEngine(FitnessFunction ff, Configuration config,
			Chromosome sampleChromosome, boolean quitEarly, int numGenerations) {
		this.quitEarly = quitEarly;
		this.ff = ff;
		this.config = config;
		this.sampleChromosome = sampleChromosome;
		this.numGenerations = numGenerations;
	}

	public GAEngine(FitnessFunction ff, Configuration config) {
		this.ff = ff;
		this.config = config;
	}

	public void findSolution() {

		try {
			config.setSampleChromosome(sampleChromosome);
			config.setFitnessFunction(ff);
			config.setPopulationSize(populationSize);
			population = Genotype.randomInitialGenotype(config);
		} catch (InvalidConfigurationException e) {
			e.printStackTrace();
			System.exit(1);
		}

		int sameCount = 0;
		double lastFitness = 0;
		for (int i = 0; i < numGenerations; i++) {
			population.evolve();
			IChromosome bestSolutionSoFar = null;

			bestSolutionSoFar = population.getFittestChromosome();
			
			// Check if max
			if(bestSolutionSoFar.getFitnessValue() == MAX_FITNESS){
				Logger.logMessage("Found max fitness in " + i + "iterations.");
				break;
			}
			
			if(quitEarly){
				// Check if the answer changed from last iteration
				if (lastFitness == bestSolutionSoFar.getFitnessValue()) {
					sameCount++;
				} else {
					lastFitness = bestSolutionSoFar.getFitnessValue();
					sameCount = 0;
				}
	
				if (sameCount > 8) {
					Logger.logMessage("Seems to be converging. Quitting after " + i + " iterations.");
					break;
				}
			}
		}
	}

	public double getBestSolutionFitness(){
		return population.getFittestChromosome().getFitnessValue();
	}
}
