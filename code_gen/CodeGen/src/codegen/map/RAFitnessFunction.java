package codegen.map;

import java.util.*;

import org.jgap.*;
import org.jgap.impl.IntegerGene;

import codegen.util.Logger;

@SuppressWarnings("serial")
public class RAFitnessFunction extends FitnessFunction {

	
	ArrayList<Processor> procList;
	Application app;
	String[] faultMechanisms;
	ArrayList<MapConstraint> constraints;

	public RAFitnessFunction(Mapper mapper) {
		procList = mapper.getProcList();
		app = mapper.getApplication();
		faultMechanisms = FaultMechanism.getAvailableMechanismsArray();
	}

	public FaultMechanism getDetectionType(IChromosome chromosome, int index) {
		Integer type = (Integer) chromosome.getGene(index).getAllele();
		FaultMechanism mec = FaultMechanism
				.getMechanismInstance(faultMechanisms[type]);

		return mec;
	}

	public String getDetectionTypeString(IChromosome chromosome, int index) {
		Integer type = (Integer) chromosome.getGene(index).getAllele();
		return faultMechanisms[type];

	}

	@SuppressWarnings("unchecked")
	@Override
	protected double evaluate(IChromosome chromosome) {
		
		ArrayList<Task> taskList = (ArrayList<Task>) app.getTaskList().clone();

		int numOriginalTasks = taskList.size();

		constraints = new ArrayList<>();
		
		Map<Task, FaultMechanism> techniqueMap = new HashMap<>();
		int hiCount = 0;
		for (int i = 0; i < numOriginalTasks; i++) {
			Task t = taskList.get(i);
			if (t.criticality == Task.HI) {
				FaultMechanism mec = getDetectionType(chromosome, hiCount);
				hiCount++;
				mec.updateTaskList(taskList, i, constraints, techniqueMap);
			}
		}

		printDetectionTypes(chromosome);

		for (int i = 0; i < taskList.size(); i++) {
			Task t = taskList.get(i);
			Logger.logMessage(t);
		}

		/*
		 * Want to maximize mutations, so anything that doesn't change should
		 * not be in the gene... build list of legal processors. should work to
		 * have gene size of 1, skip most of the heavy library stuff. too tricky
		 * to try and leave out genes of size 1.
		 */

		Map<Task, ArrayList<Processor>> legalMappings = new HashMap<>();

		for (Task t : taskList) {
			ArrayList<Processor> legalProcList = new ArrayList<>();
			legalMappings.put(t, legalProcList);
		}

		for (Processor p : procList) {
			for (Task t : taskList) {
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
		/*
		 * Note: library will break if MapConfiguration is given a non-blank name.
		 * Otherwise, name must be unique every time.
		 */
		MapConfiguration.reset();
		MapConfiguration msConfig = new MapConfiguration("");
		Chromosome sampleChromosome = null;
		try {
			sampleChromosome = createMSChromosome(msConfig,
					taskList, legalMappings);
		} catch (InvalidConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Schedule bestSchedule = new Schedule();
		MSFitnessFunction msFF = new MSFitnessFunction(taskList, constraints,legalMappings,bestSchedule,procList);
		GAEngine msEngine = new GAEngine(msFF, msConfig,sampleChromosome);

		msEngine.findSolution();
		return msEngine.getBestSolutionFitness();
	}

	private Chromosome createMSChromosome(MapConfiguration config,
			ArrayList<Task> taskList,
			Map<Task, ArrayList<Processor>> legalMappings)
			throws InvalidConfigurationException {
		Gene[] sampleGenes = new Gene[taskList.size()];
		
		for (int i = 0; i < sampleGenes.length; i++) {
			Task t = taskList.get(i);
			int numLegalProcessors = legalMappings.get(t).size();
			sampleGenes[i] = new IntegerGene(config, 0, numLegalProcessors - 1);

		}
		Chromosome sampleChromosome = null;
		sampleChromosome = new Chromosome(config, sampleGenes);
		config.setSampleChromosome(sampleChromosome);

		return sampleChromosome;
	}

	private void printDetectionTypes(IChromosome chromosome) {
		Gene[] geneList = chromosome.getGenes();
		for (int i = 0; i < geneList.length; i++) {
			Logger.logMessage(faultMechanisms[(Integer) geneList[i]
					.getAllele()]);
		}

	}

}
