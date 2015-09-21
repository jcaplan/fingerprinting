package codegen.map;

import java.util.ArrayList;
import java.util.Collections;

public class SchedAnalysis {


	public static final int numModes = 4;
	
	
	ArrayList<Task> taskList;
	Schedule schedule;
	ArrayList<Processor> procList;
	public SchedAnalysis(ArrayList<Task> taskList, Schedule schedule, ArrayList<Processor> procList) {
		this.taskList = taskList;
		this.schedule = schedule;
		this.procList = procList;
	}

	@SuppressWarnings("unchecked")
	public boolean schedAnalysis() {
		//TODO
		
		
		//First a priority must be assigned to each task
		//The priority is assigned to the binding in the schedule
		ArrayList<Task> priorityList = (ArrayList<Task>) taskList.clone();
		Collections.sort(priorityList);
		for(int i = 0; i < priorityList.size(); i++){
			Task t = priorityList.get(i);
			schedule.setPriority(t,i);
		}
		
		//Check the schedulability on each processor
		for(Processor p : procList){
			/*
			 * Build an array of tasks that execute on this processor
			 */
			ArrayList<Task> procTaskList = new ArrayList<>();
			for(Task t : priorityList){
				if(schedule.getProcessor(t).equals(p)){
					procTaskList.add(t);
				}
			}
			System.out.println("hi");
		}
		
		
		return false;
	}
	
	public double qosAnalysis() {
		
		//TODO
		return 0;
	}
	
}
