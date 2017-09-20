package codegen.map;

import java.util.*;

import org.jgap.*;
import org.jgap.impl.IntegerGene;

import codegen.util.Logger;

@SuppressWarnings("serial")
public class RAFitnessFunction extends FitnessFunction {

	
	ArrayList<Processor> procList;
	Application app;
	List<FaultMechanism> faultMechanisms;
	private Schedule bestSchedule;
	double bestFitness;
	private Map<Task, FaultMechanism> bestTechniqueMap;
	public static int count = 0;
	
	public RAFitnessFunction(Mapper mapper) {
		procList = mapper.getProcList();
		app = mapper.getApplication();
		faultMechanisms = mapper.getFtmList();
	}



	public FaultMechanism getDetectionType(IChromosome chromosome, int index) {
		Integer type = (Integer) chromosome.getGene(index).getAllele();
		FaultMechanism mec = faultMechanisms.get(type);
		return mec;
	}

	@SuppressWarnings("unchecked")
	@Override
	protected double evaluate(IChromosome chromosome) {
		
		ArrayList<Task> taskList = (ArrayList<Task>) app.getTaskList().clone();

		int numOriginalTasks = taskList.size();

		Map<Task,Task[]> replicas;
		replicas = new HashMap<>();
		
		//technique map only contains original task set
		Map<Task, FaultMechanism> techniqueMap = new HashMap<>();
		Map<Task, int[]> executionProfiles = new HashMap<>();
		
		int hiCount = 0;
		for (int i = 0; i < numOriginalTasks; i++) {
			Task t = taskList.get(i);
			if (t.criticality == Task.HI) {
				FaultMechanism mec = getDetectionType(chromosome, hiCount);
				hiCount++;
				mec.updateTaskList(taskList, i, replicas, techniqueMap, executionProfiles);
			} else {
				executionProfiles.put(t, new int[] {1,1,1,1});
			}
		}

//		printDetectionTypes(chromosome);

		
		//find the legal mappings for each of the original tasks
		Map<Task, ArrayList<Processor>> legalMappings = new HashMap<>();

		for (Task t : app.getTaskList()) {
			legalMappings.put(t, new ArrayList<Processor>());
		}

		for (Processor p : procList) {
			for (Task t : app.getTaskList()) {
				if (t.criticality == Task.HI) {
					boolean lockstepTask = techniqueMap.get(t).getCommandName()
							.equals("Lockstep");
					boolean lockstepProcessor = (p.faultTolerant == Processor.lockstepType);
					if ((lockstepTask && lockstepProcessor)
							|| (!lockstepTask && !lockstepProcessor)) {
						legalMappings.get(t).add(p);
					}
				} else {
					legalMappings.get(t).add(p);
				}
			}
		}
		
		MapConfiguration.reset();
		MapConfiguration msConfig = new MapConfiguration("msEngine" + count++);
		Chromosome sampleChromosome = null;
		try {
			sampleChromosome = createMSChromosome(msConfig,
					app.getTaskList(), legalMappings, techniqueMap);
		} catch (InvalidConfigurationException e) {
			e.printStackTrace();
		}

		MSFitnessFunction msFF = new MSFitnessFunction(app.getTaskList(), replicas,techniqueMap,
				legalMappings,executionProfiles, procList);
		GAEngine msEngine = new GAEngine(msFF, msConfig,sampleChromosome,true,30);

		msEngine.findSolution();
		double fitness = msEngine.getBestSolutionFitness();
		if(fitness > bestFitness){
			bestFitness = fitness;
			setBestSchedule(msFF.bestSchedule);
			msFF.bestSchedule.executionProfiles = executionProfiles;
			System.out.println(bestSchedule);
			setBestTechniqueMap(techniqueMap);	
		}
		return msEngine.getBestSolutionFitness();
	}
	


	public static Chromosome createMSChromosome(MapConfiguration config,
			ArrayList<Task> taskList,
			Map<Task, ArrayList<Processor>> legalMappings, Map<Task, FaultMechanism> techniqueMap)
			throws InvalidConfigurationException {
		Gene[] sampleGenes = new Gene[taskList.size()];
		
		for (int i = 0; i < sampleGenes.length; i++) {
			Task t = taskList.get(i);
			int numLegalProcessors = legalMappings.get(t).size();
			if(t.isCritical()){
				int lowerBound =  0;
				int upperBound = techniqueMap.get(t).getPermutationSize(numLegalProcessors) - 1;
				if(upperBound < lowerBound){
					System.out.println("wtf");
				}
				sampleGenes[i] = new IntegerGene(config, lowerBound, 
						upperBound);
			} else {
				sampleGenes[i] = new IntegerGene(config, 0, numLegalProcessors-1);
			}
		}
		Chromosome sampleChromosome = null;
		sampleChromosome = new Chromosome(config, sampleGenes);
		config.setSampleChromosome(sampleChromosome);

		return sampleChromosome;
	}

	private void printDetectionTypes(IChromosome chromosome) {
		Gene[] geneList = chromosome.getGenes();
		for (int i = 0; i < geneList.length; i++) {
			Logger.logMessage(faultMechanisms.get((Integer) geneList[i]
					.getAllele()));
		}

	}



	public Map<Task, FaultMechanism> getBestTechniqueMap() {
		return bestTechniqueMap;
	}



	public void setBestTechniqueMap(Map<Task, FaultMechanism> bestTechniqueMap) {
		this.bestTechniqueMap = bestTechniqueMap;
	}



	public Schedule getBestSchedule() {
		return bestSchedule;
	}



	public void setBestSchedule(Schedule bestSchedule) {
		this.bestSchedule = bestSchedule;
	}




}
