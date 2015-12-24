package codegen.map;

import java.util.*;

import org.jgap.*;
import org.jgap.impl.IntegerGene;

import codegen.util.Logger;

public class Mapper {

	Application app;
	ArrayList<Processor> procList;
	protected Schedule bestSchedule;
	protected double fitness;
	protected Map<Task, FaultMechanism> bestTechniqueMap;
	List<FaultMechanism> ftms;
	public static int count;
	public Mapper(){
		app = new Application();
		procList = new ArrayList<>();
		ftms = new ArrayList<FaultMechanism>();
		initDefaultMechs();
	}
	
	public Mapper(Application app, ArrayList<Processor> procList){
		this.app = app;
		this.procList = procList;
	}
	
	public void setApplication(Application app){
		this.app = app;
	}

	public void setProcList(ArrayList<Processor> pList){
		this.procList = pList;
	}
	
	public void printApp() {
		Logger.logMessage(app);
		
	}

	public void printProcessors() {
		for(Processor p : procList){
			Logger.logMessage(p);
		} 
		
	}

	public void findSchedule() {
		if (ftms.size() == 1 && ftms.get(0) instanceof Lockstep) {
			// Skip the first stage, go around mapper
			Map<Task, ArrayList<Processor>> legalMappings = new HashMap<>();
			Map<Task, FaultMechanism> techniqueMap = new HashMap<>();
			Lockstep ls = new Lockstep();
			for (Task t : app.getTaskList()) {
				legalMappings.put(t, procList);
				techniqueMap.put(t, ls);
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
					replicas, techniqueMap, legalMappings, procList);
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
			ArrayList<Task> taskList = (ArrayList<Task>) app.getTaskList().clone();
			int numOriginalTasks = taskList.size();
			for (int i = 0; i < numOriginalTasks; i++) {
				Task t = taskList.get(i);
				if (t.criticality == Task.HI) {
					FaultMechanism mec = dmr;
					mec.updateTaskList(taskList, i, replicas, techniqueMap);
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
					replicas, techniqueMap, legalMappings, procList);
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
	
	public Application getApplication(){
		return app;
	}
	
	public ArrayList<Processor> getProcList(){
		return procList;
	}
	
	public Schedule getBestSchedule(){
		return bestSchedule;
	}
	
	public double getBestScheduleFitness(){
		return fitness;
	}
	
	public Map<Task, FaultMechanism> getBestFTMs(){
		return bestTechniqueMap;
	}

	public void clearFTMs() {
		ftms.clear();
	}

	public void addFTM(FaultMechanism mech) {
		if(!ftms.contains(mech)){
			ftms.add(mech);
		}
	}

	public List<FaultMechanism> getFtmList() {
		return ftms;
	}
	
	private void initDefaultMechs() {
		ftms.add(new DMR());
		ftms.add(new TMR());
		ftms.add(new PassiveReplication());
		ftms.add(new Lockstep());
	}

	public void setFTMS(ArrayList<FaultMechanism> ftms) {
		this.ftms = ftms;
	}
	
}
