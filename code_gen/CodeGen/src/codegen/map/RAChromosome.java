package codegen.map;


import org.jgap.Chromosome;
import org.jgap.Configuration;
import org.jgap.Gene;
import org.jgap.IChromosome;
import org.jgap.InvalidConfigurationException;

import codegen.sw.*;
import codegen.hw.*;
public class RAChromosome {

	int numTasks;
	RAGene[] sampleGenes;
	Configuration config;
	public RAChromosome(int numTasks, Configuration config)
			throws InvalidConfigurationException {
		this.numTasks = numTasks;
		sampleGenes = new RAGene[numTasks];
		for(int i = 0; i < numTasks; i++){
			sampleGenes[i] = new RAGene(config,numTasks);
		}
			this.config = config;
	}
	
	
	public void setAllFDtoLockstep(){
		for(RAGene gene : sampleGenes){
			gene.setDetectionType(RAGene.FD_DMR);
		}
	}
	
	/**
	 * Take an IChromosome and build an RAChromosome with 
	 * an RAGene array
	 * @param chrom the generic IChromosome used by the fitness function
	 * @throws InvalidConfigurationException 
	 */
	public RAChromosome(IChromosome chrom) throws InvalidConfigurationException{
		Gene[] gene = chrom.getGenes();
		sampleGenes = new RAGene[gene.length];
		for(int i = 0; i < gene.length; i++){
			sampleGenes[i] = new RAGene(gene[i], gene.length);
		}
	}
	

	

	public Chromosome getSampleChromosome() throws InvalidConfigurationException{
		return new Chromosome(config, RAGene.getGeneArray(this.sampleGenes));
	}
	
	public static String getChromosomeString(IChromosome chrom){
		Gene[] genes = chrom.getGenes();
		String s = "";
		int count = 0;

		for(Gene g : genes){
			s += "Task " + count++ + ": ";
			s += RAGene.getFDString(g);
			s += RAGene.getFTString(g);
			s += "\n";
		}
		
		
		return s;
	}
	

	public int getTaskFD(Task task){
		return sampleGenes[task.taskID].getDetectionType();
	}
	
	public boolean matchingTechniques(Task task, Processor p){
		return (getTaskFD(task) == RAGene.FD_DMR && p.faultTolerant) ||
				(getTaskFD(task) == RAGene.FD_FPRINT && p.hasFprint);
	}
	
	
}
