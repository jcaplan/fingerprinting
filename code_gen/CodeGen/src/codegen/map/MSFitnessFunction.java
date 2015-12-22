package codegen.map;

import java.util.ArrayList;
import java.util.Map;

import org.jgap.FitnessFunction;
import org.jgap.Gene;
import org.jgap.IChromosome;

import codegen.util.Logger;

@SuppressWarnings("serial")
public class MSFitnessFunction extends FitnessFunction{
	
	 Schedule bestSchedule;
	double fittestScore;
	Object lock = new Object();
	
	ArrayList<Task> taskList;
	Map<Task,Task[]> replicas;
	Map<Task, ArrayList<Processor>> legalMappings;
	ArrayList<Processor> procList;
	private Map<Task, FaultMechanism> techniqueMap;
	
//	public MSFitnessFunction(ArrayList<Task> taskList,
//			ArrayList<MapConstraint> constraints, Map<Task, ArrayList<Processor>> legalMappings, ArrayList<Processor> procList) {
//		this.taskList = taskList;
//		this.constraints = constraints;
//		this.legalMappings = legalMappings;
//		this.procList = procList;
//	}

	public MSFitnessFunction(ArrayList<Task> taskList,
			Map<Task,Task[]> replicas, Map<Task, FaultMechanism> techniqueMap,
			Map<Task, ArrayList<Processor>> legalMappings,
			ArrayList<Processor> procList) {
		this.taskList = taskList;
		this.techniqueMap = techniqueMap;
		this.legalMappings = legalMappings;
		this.procList = procList;
		this.replicas = replicas;
	}

	@Override
	protected double evaluate(IChromosome chromosome) {
		
		/*
		 * Create a schedule out of the chromosome, taskList and legalMappings
		 */
		Schedule schedule = new Schedule();
		Gene[] geneList = chromosome.getGenes();
		
		//need to convert chromosome values into core allocations

		for(int i = 0; i < geneList.length; i++){
			Task t = taskList.get(i);
			int permutation = (int)geneList[i].getAllele();
			if(t.isCritical()){
				//go through the FTM...
				techniqueMap.get(t).allocateProcessors(schedule,permutation,legalMappings,t,replicas);
			} else {
				schedule.allocate(t, legalMappings.get(t).get(permutation));
			}
			
		}
		
		
		SchedAnalysis analysis = new SchedAnalysis(taskList, schedule,procList);
		double fitness;
		boolean schedResult;
		if(!(schedResult = analysis.schedAnalysis())){
			fitness = 0;
		} else {
			fitness = analysis.qosAnalysis();
		}
		if(fitness > fittestScore){
			fittestScore = fitness;
			setBestSchedule(schedule);
		}
//		Logger.logMessage("result of schedulability analysis: " + schedResult + ", fitness = " + fitness);
		return fitness;
	}
	
	public Schedule getBestSchedule(){
		synchronized(lock){
			return bestSchedule;
		}
	}
	
	public void setBestSchedule(Schedule schedule){
		synchronized(lock){
			bestSchedule = schedule;
		}
	}

}
