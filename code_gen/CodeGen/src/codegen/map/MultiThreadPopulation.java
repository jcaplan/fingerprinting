package codegen.map;

import java.util.Iterator;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.jgap.Configuration;
import org.jgap.FitnessEvaluator;
import org.jgap.IChromosome;
import org.jgap.InvalidConfigurationException;
import org.jgap.Population;

public class MultiThreadPopulation extends Population {

	public MultiThreadPopulation(Configuration a_config, int a_size)
			throws InvalidConfigurationException {
		super(a_config, a_size);
	}

	public MultiThreadPopulation(Configuration configuration,
			IChromosome[] chromosomes) throws InvalidConfigurationException {
		super(configuration, chromosomes);
	}

	public MultiThreadPopulation(Configuration configuration,
			List<IChromosome> chromosomes) throws InvalidConfigurationException{
		super(configuration,(IChromosome[]) chromosomes.toArray());
	}

	public IChromosome determineFittestChromosome(int numThreads) {
		if (!m_changed && m_fittestChromosome != null) {
			return m_fittestChromosome;
		}
		Iterator it = m_chromosomes.iterator();
		FitnessEvaluator evaluator = getConfiguration().getFitnessEvaluator();
		double bestFitness;
		if (evaluator.isFitter(2.0d, 1.0d)) {
			bestFitness = -1.0d;
		} else {
			bestFitness = Double.MAX_VALUE;
		}

		ExecutorService executor = Executors.newFixedThreadPool(numThreads);

		while (it.hasNext()) {
			IChromosome chrom = (IChromosome) it.next();
			executor.execute(new Runnable() {
				public void run() {
					chrom.getFitnessValue();
				}
			});
		}
		executor.shutdown();
		while(!executor.isTerminated());
		
		it = m_chromosomes.iterator();
		 double fitness;
		    while (it.hasNext()) {
		      IChromosome chrom = (IChromosome) it.next();
		      fitness = chrom.getFitnessValue();
		      if (evaluator.isFitter(fitness, bestFitness)
		          || m_fittestChromosome == null) {
		        m_fittestChromosome = chrom;
		        bestFitness = fitness;
		      }
		    }
		
		setChanged(false);
		return m_fittestChromosome;
	}

}
