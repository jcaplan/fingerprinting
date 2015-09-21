package codegen.map;

import java.util.ArrayList;
import java.util.Map;

import org.jgap.FitnessFunction;
import org.jgap.Gene;
import org.jgap.IChromosome;

@SuppressWarnings("serial")
public class MSFitnessFunction extends FitnessFunction{

	ArrayList<Task> taskList;
	ArrayList<MapConstraint> constraints;
	Map<Task, ArrayList<Processor>> legalMappings;
	Schedule bestSchedule;
	ArrayList<Processor> procList;
	
	public MSFitnessFunction(ArrayList<Task> taskList,
			ArrayList<MapConstraint> constraints, Map<Task, ArrayList<Processor>> legalMappings,
			Schedule bestSchedule, ArrayList<Processor> procList) {
		this.taskList = taskList;
		this.constraints = constraints;
		this.legalMappings = legalMappings;
		this.bestSchedule = bestSchedule;
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
			System.out.println("schedule illegal");
			return 0;
		} else {
			System.out.println("schedule legal");
		}
		
		SchedAnalysis analysis = new SchedAnalysis(taskList, schedule,procList);
		double fitness;
		if(!analysis.schedAnalysis()){
			fitness = 0;
		} else {
			fitness = analysis.qosAnalysis();
		}
		
		return fitness;
	}
	


}
