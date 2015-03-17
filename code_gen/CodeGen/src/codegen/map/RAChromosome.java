package codegen.map;

import java.util.ArrayList;

import org.jgap.*;
import org.jgap.impl.*;

public class RAChromosome {

	int numTasks;
	RAGene[] sampleGenes;
	static Configuration config;
	public RAChromosome(int numTasks, Configuration config)
			throws InvalidConfigurationException {
		this.numTasks = numTasks;
		sampleGenes = new RAGene[numTasks];
		for(int i = 0; i < numTasks; i++){
			sampleGenes[i] = new RAGene(config,numTasks);
		}
		if(RAChromosome.config == null) 
			RAChromosome.config = config;
		init(config);
	}

	private void init(Configuration config)
			throws InvalidConfigurationException {
		Chromosome sampleChromosome = new Chromosome(config,
				RAGene.getGeneArray(sampleGenes));
	}

	
	public static SplitGroup getSplitGroups(IChromosome chrom){
		Gene[] genes = chrom.getGenes();
		int[] tasksPerGroup = new int[genes.length];
		ArrayList<Integer> emptyGroups = new ArrayList<Integer>();
		ArrayList<Integer> nonEmptyGroups = new ArrayList<Integer>();
		
		for(int i = 0; i < genes.length; i++){
			int index = (int)((ICompositeGene)genes[i]).geneAt(RAGene.GROUP_INDEX).getAllele();
			tasksPerGroup[index]++;
		}
		//Make a list of empty groups
		for(int i = 0; i < genes.length; i++){
			if(tasksPerGroup[i] == 0){
				emptyGroups.add(i);
			} else {
				nonEmptyGroups.add(i);
			}
		}
		
		if(emptyGroups.size() == 0)
			return null;
		// return an int[] with the index of an empty group
		// and the index of a non-empty group
		// and the number of tasks in a group
		//-------------------------------------------------	
		RandomGenerator generator = config.getRandomGenerator();
		SplitGroup s = new SplitGroup();
		s.emptyGroup = emptyGroups.get(generator.nextInt(emptyGroups.size()));
		s.nonEmptyGroup = nonEmptyGroups.get(generator.nextInt(nonEmptyGroups.size()));
		s.sizeNonEmptyGroup = tasksPerGroup[s.nonEmptyGroup];
		
		// Build a list of genes belonging to that group
		// ---------------------------------------------
		s.group = new ArrayList<Gene>();
		for(int i = 0; i < genes.length; i++){
			if((int)((ICompositeGene)genes[i]).geneAt(RAGene.GROUP_INDEX).getAllele() == s.nonEmptyGroup)
				s.group.add(genes[i]);
		}
		return s;
	}
	
	public static JoinGroup getJoinGroups(IChromosome chrom){
		Gene[] genes =  chrom.getGenes();
		int[] tasksPerGroup = new int[genes.length];
		ArrayList<Integer> emptyGroups = new ArrayList<Integer>();
		ArrayList<Integer> nonEmptyGroups = new ArrayList<Integer>();
		
		for(int i = 0; i < genes.length; i++){
			int index = (int)((ICompositeGene)genes[i]).geneAt(RAGene.GROUP_INDEX).getAllele();
			tasksPerGroup[index]++;
		}
		//Make a list of empty groups
		for(int i = 0; i < genes.length; i++){
			if(tasksPerGroup[i] == 0){
				emptyGroups.add(i);
			} else {
				nonEmptyGroups.add(i);
			}
		}
		
		if(nonEmptyGroups.size() == 0)
			return null;
		// return an int[] with the index of an empty group
		// and the index of a non-empty group
		// and the number of tasks in a group
		//-------------------------------------------------	
		JoinGroup join = new JoinGroup();
		RandomGenerator generator = config.getRandomGenerator();
		join.group0 = nonEmptyGroups.get(generator.nextInt(nonEmptyGroups.size()));
		join.group1 = nonEmptyGroups.get(generator.nextInt(nonEmptyGroups.size()));
		return join;
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
			s += RAGene.getGeneString(g);
			s += "\n";
		}
		
		return s;
	}
	
	public static class SplitGroup{
		Integer emptyGroup;
		int nonEmptyGroup;
		int sizeNonEmptyGroup;
		ArrayList<Gene> group;
	}
	
	public static class JoinGroup {
		int group0;
		int group1;
	}
	
	
}
