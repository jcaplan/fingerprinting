package codegen.map;

import java.util.ArrayList;
import java.util.Map;

import org.homedns.dade.jcgrid.worker.GridWorkerFeedback;
import org.jgap.FitnessFunction;
import org.jgap.Gene;
import org.jgap.IChromosome;

import codegen.util.Logger;

@SuppressWarnings("serial")
public class MSFitnessFunction extends FitnessFunction{
	
	 Schedule bestSchedule;
	double fittestScore;
	
	
	ArrayList<Task> taskList;
	ArrayList<MapConstraint> constraints;
	Map<Task, ArrayList<Processor>> legalMappings;
	ArrayList<Processor> procList;
	
	public MSFitnessFunction(ArrayList<Task> taskList,
			ArrayList<MapConstraint> constraints, Map<Task, ArrayList<Processor>> legalMappings, ArrayList<Processor> procList) {
		this.taskList = taskList;
		this.constraints = constraints;
		this.legalMappings = legalMappings;
		this.procList = procList;
	}

	@Override
	protected double evaluate(IChromosome chromosome) {
		
		/*
		 * Create a schedule out of the chromosome, taskList and legalMappings
		 */
		Schedule schedule = new Schedule();
		Gene[] geneList = chromosome.getGenes();
		for(int i = 0; i < geneList.length; i++){
			Task t = taskList.get(i);
			int procIndex = (int)geneList[i].getAllele();
			Processor p = legalMappings.get(t).get(procIndex);
			schedule.allocate(t,p);
		}
		
		if(!schedule.checkConstraints(constraints)){
//			Logger.logMessage("schedule illegal");
			return 0;
		} 
//			else {
//			Logger.logMessage("schedule legal");
//		}
		
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
			bestSchedule = schedule;
		}
//		Logger.logMessage("result of schedulability analysis: " + schedResult + ", fitness = " + fitness);
		return fitness;
	}
	
	public Schedule getBestSchedule(){
		return bestSchedule;
	}

}
