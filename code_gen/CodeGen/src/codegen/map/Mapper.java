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
		
		Configuration.reset();
		RAConfiguration reliabilityConfig = new RAConfiguration("raConfig");
		Chromosome sampleChromosome = null;
		RAFitnessFunction raFF = new RAFitnessFunction(this);
		try {
			sampleChromosome = createRAChromosome(reliabilityConfig);
		} catch (InvalidConfigurationException e) {
			e.printStackTrace();
		}

		GAEngine raEngine = new GAEngine(raFF,reliabilityConfig,sampleChromosome);		
		raEngine.findSolution();
		bestSchedule = raFF.getBestSchedule();
		fitness = raEngine.getBestSolutionFitness();
		bestTechniqueMap = raFF.getBestTechniqueMap();
		
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
