package codegen.map;

import java.util.ArrayList;
import java.util.Map;

import org.jgap.*;
import org.jgap.impl.IntegerGene;

import codegen.util.Logger;

public class Mapper {

	Application app;
	ArrayList<Processor> procList;
	Schedule bestSchedule;
	double fitness;
	Map<Task, FaultMechanism> bestTechniqueMap;
	
	
	public Mapper(){
		app = new Application();
		procList = new ArrayList<>();
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
		MapConfiguration reliabilityConfig = new MapConfiguration("raConfig");
		Chromosome sampleChromosome = null;
		RAFitnessFunction raFF = new RAFitnessFunction(this);
		try {
			sampleChromosome = createRAChromosome(reliabilityConfig);
		} catch (InvalidConfigurationException e) {
			e.printStackTrace();
		}

		GAEngine raEngine = new GAEngine(raFF,reliabilityConfig,sampleChromosome);		
		raEngine.findSolution();
		bestSchedule = raFF.bestSchedule;
		fitness = raEngine.getBestSolutionFitness();
		bestTechniqueMap = raFF.bestTechniqueMap;
		
	}

	private Chromosome createRAChromosome(MapConfiguration config) throws InvalidConfigurationException {
		Gene[] sampleGenes = new Gene[app.getNumHiTasks()];
		int geneSize = FaultMechanism.getNumMechanisms()-1;
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
	
}
