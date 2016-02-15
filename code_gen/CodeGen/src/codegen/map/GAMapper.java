package codegen.map;

import java.util.*;

import org.jgap.*;
import org.jgap.impl.IntegerGene;


public class GAMapper extends Mapper{


	public static int count;
	
	public void findSchedule() {
		if (ftms.size() == 1 && ftms.get(0) instanceof Lockstep) {
			// Skip the first stage, go around mapper
			Map<Task, ArrayList<Processor>> legalMappings = new HashMap<>();
			Map<Task, FaultMechanism> techniqueMap = new HashMap<>();
			Lockstep ls = new Lockstep();
			Map<Task,int[]> executionProfiles = new HashMap<>();
			
			for (Task t : app.getTaskList()) {
				legalMappings.put(t, procList);
				techniqueMap.put(t, ls);
				if(t.isCritical()){
					executionProfiles.put(t, new int[]{1,2,1,2});
				} else {
					executionProfiles.put(t, new int[]{1,1,1,1});
				}
			}

			MapConfiguration.reset();
			MapConfiguration msConfig = new MapConfiguration("msEngine"
					+ RAFitnessFunction.count++);
			Chromosome sampleChromosome = null;
			
			try {
				sampleChromosome = RAFitnessFunction.createMSChromosome(
						msConfig, app.getTaskList(), legalMappings,techniqueMap);
			} catch (InvalidConfigurationException e) {
				e.printStackTrace();
			}
			Map<Task,Task[]> replicas = new HashMap<>();
			MSFitnessFunction msFF = new MSFitnessFunction(app.getTaskList(),
					replicas, techniqueMap, legalMappings, executionProfiles, procList);
			GAEngine msEngine = new GAEngine(msFF, msConfig, sampleChromosome,false,30);

			msEngine.findSolution();
			bestSchedule = msFF.getBestSchedule();
			fitness = msEngine.getBestSolutionFitness();
			bestTechniqueMap = null;
		} else 	if (ftms.size() == 1 && ftms.get(0) instanceof DMR) {
			// Skip the first stage, go around mapper
			Map<Task, ArrayList<Processor>> legalMappings = new HashMap<>();
			Map<Task, FaultMechanism> techniqueMap = new HashMap<>();
			DMR dmr = new DMR();
			for (Task t : app.getTaskList()) {
				legalMappings.put(t, procList);
				techniqueMap.put(t, dmr);
			}
			Map<Task,Task[]> replicas = new HashMap<>();
			Map<Task,int[]> executionProfiles = new HashMap<>();
			ArrayList<Task> taskList = (ArrayList<Task>) app.getTaskList().clone();
			int numOriginalTasks = taskList.size();
			for (int i = 0; i < numOriginalTasks; i++) {
				Task t = taskList.get(i);
				if (t.criticality == Task.HI) {
					FaultMechanism mec = dmr;
					mec.updateTaskList(taskList, i, replicas, techniqueMap, executionProfiles);
				} else {
					executionProfiles.put(t, new int[]{1,1,1,1});
				}
			}
			MapConfiguration.reset();
			MapConfiguration msConfig = new MapConfiguration("msEngine"
					+ RAFitnessFunction.count++);
			Chromosome sampleChromosome = null;			
			try {
				sampleChromosome = RAFitnessFunction.createMSChromosome(
						msConfig, app.getTaskList(), legalMappings,techniqueMap);
			} catch (InvalidConfigurationException e) {
				e.printStackTrace();
			}

			MSFitnessFunction msFF = new MSFitnessFunction(app.getTaskList(),
					replicas, techniqueMap, legalMappings, executionProfiles, procList);
			GAEngine msEngine = new GAEngine(msFF, msConfig, sampleChromosome,false,30);

			msEngine.findSolution();
			bestSchedule = msFF.getBestSchedule();
			fitness = msEngine.getBestSolutionFitness();
			bestTechniqueMap = null;
		} else {
			RAConfiguration.reset();
			RAConfiguration reliabilityConfig = new RAConfiguration("raConfig" + count++);
			Chromosome sampleChromosome = null;
			RAFitnessFunction raFF = new RAFitnessFunction(this);
			try {
				sampleChromosome = createRAChromosome(reliabilityConfig);
			} catch (InvalidConfigurationException e) {
				e.printStackTrace();
			}
	
			GAEngine raEngine = new GAEngine(raFF,reliabilityConfig,sampleChromosome,false,30);		
			raEngine.findSolution();
			bestSchedule = raFF.getBestSchedule();
			fitness = raEngine.getBestSolutionFitness();
			bestTechniqueMap = raFF.getBestTechniqueMap();
		}
	}

	public Chromosome createRAChromosome(Configuration config) throws InvalidConfigurationException {
		Gene[] sampleGenes = new Gene[app.getNumHiTasks()];
		int geneSize = ftms.size()-1;
		for(int i = 0; i < app.getNumHiTasks(); i++){
				sampleGenes[i] = new IntegerGene(config,0,geneSize);

		}
		Chromosome sampleChromosome = null;
		sampleChromosome = new Chromosome(config, sampleGenes);
		config.setSampleChromosome(sampleChromosome);
		
		return sampleChromosome;
	}
	
	
	
	
	
}
