package codegen.map;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.jgap.BulkFitnessFunction;
import org.jgap.Configuration;
import org.jgap.IChromosome;
import org.jgap.Population;
import org.jgap.impl.GABreeder;

public class MulitThreadGABreeder extends GABreeder {

	private static final int NUM_THREADS = 20;

	@Override
	protected void updateChromosomes(Population a_pop, Configuration a_conf) {
		int currentPopSize = a_pop.size();
		// Ensure all chromosomes are updated.
		// -----------------------------------
		BulkFitnessFunction bulkFunction = a_conf.getBulkFitnessFunction();
		boolean bulkFitFunc = (bulkFunction != null);
		if (!bulkFitFunc) {
			ExecutorService executor = Executors
					.newFixedThreadPool(NUM_THREADS);
			for (int i = 0; i < currentPopSize; i++) {
				final IChromosome chrom = a_pop.getChromosome(i);
				executor.execute(new Runnable() {
					public void run() {
						chrom.getFitnessValue();
					}
				});
			}
			executor.shutdown();
			while(!executor.isTerminated());
		}
	}

}
