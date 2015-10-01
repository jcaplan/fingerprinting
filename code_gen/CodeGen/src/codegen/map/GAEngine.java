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
	private static final int numGenerations = 30;
	
	
	FitnessFunction ff;
	Configuration config;
	Chromosome sampleChromosome;
	Genotype population;

	public GAEngine(FitnessFunction ff, Configuration config,
			Chromosome sampleChromosome) {
		this.ff = ff;
		this.config = config;
		this.sampleChromosome = sampleChromosome;
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

			IChromosome bestSolutionSoFar = population.getFittestChromosome();
			// Check if the answer changed from last iteration
			if (lastFitness == bestSolutionSoFar.getFitnessValue()) {
				sameCount++;
			} else {
				lastFitness = bestSolutionSoFar.getFitnessValue();
				sameCount = 0;
			}

			if (sameCount > 4) {
				Logger.logMessage("Seems to be converging. Quitting early.");
				break;
			}
		}
	}

	public double getBestSolutionFitness(){
		return population.getFittestChromosome().getFitnessValue();
	}
}
