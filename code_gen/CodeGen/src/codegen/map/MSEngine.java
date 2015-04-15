package codegen.map;

import java.io.IOException;
import java.util.ArrayList;

import org.jgap.*;
import org.jgap.impl.*;

import codegen.hw.*;
import codegen.sw.*;

public class MSEngine {

	Platform platform;
	Application app;
	static int count = 0;
	public MSEngine(Platform platform,Application application){
		this.platform = platform;
		this.app = application;
	}
	
	/**
	 * This function finds the best schedule for a given technique chromosome. 
	 * @param chromosome
	 * @return
	 * @throws InvalidConfigurationException
	 */
	public double getBestSchedule(IChromosome chromosome) throws InvalidConfigurationException {
		
		RAChromosome chrom = new RAChromosome(chromosome);
		
		//chrom can be used to extract the state
		//Ignore correction type for now, only use detection type
		
		
		
		//Now each task has a mechanism
		//Each task should have a list of legal processors that it can run on.
		
		//Make sure any transformations on the application task graph were cleaned
		app.cleanTransformation();
		
		
		//Build the list of legal processors for each task
		for(Task task : app.getTaskList()){
			task.resetLegalProcessors();
			if(task.isCritical()){
				
				for(Processor p : platform.processors){
					if(chrom.matchingTechniques(task, p))
						task.addLegalProcessor(p);
				}
			} else {
				for(Processor p : platform.processors){
					task.addLegalProcessor(p);
				}
			}
		}
		
		
		//How many tasks are there?
		//Create a copy of any tasks that require FTC and 
		//add them to the task List
		app.transformTasks();

		
		//Now the task list has been updated
		String name = "MS" + count++;
		MSConfiguration.reset();
		MSConfiguration config = new MSConfiguration(name,null);
		Gene[] sampleGenes = new Gene[app.getTaskList().size()];
		
		//Build the sample gene, each gene for each task is mapped to the possible legal processors only
		for(int i = 0; i < sampleGenes.length; i++){
			sampleGenes[i] = new IntegerGene(
					config,0,app.getTaskList().get(i).legalProcessors.size()-1);
		}
		//Use GP to map applications onto cores
		//Each task is an integer gene 
		//Each integer is limited to the size of the legal processors
		//The integer points to the index in the legalProcessor ArrayList,
		//*not* to a specific processor.
		
		
		config.setPopulationSize( 50 );

		//Set up the JGAP library and do the DSE
		Chromosome sampleChromosome = new Chromosome(config, sampleGenes );
		config.setSampleChromosome( sampleChromosome );
		MSFitnessFunction ff = new MSFitnessFunction(platform,app);
		config.setFitnessFunction(ff);
		Genotype population = Genotype.randomInitialGenotype( config );
		IChromosome bestSolutionSoFar;
		double lastFitness = 0;
		int sameCount = 0;
		for( int i = 0; i < 30; i++ )
		{
		    population.evolve();
		    double newFitness = population.getFittestChromosome().getFitnessValue();
		    if(lastFitness == newFitness){
		    	sameCount++;
		    } else {
		    	lastFitness = newFitness;
		    	sameCount = 0;
		    }
		    
		    if(sameCount > 4){
				break;
			}
		}
		
		
		return population.getFittestChromosome().getFitnessValue();
	}
	
}
