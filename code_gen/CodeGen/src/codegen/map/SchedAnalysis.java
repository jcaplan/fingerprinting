package codegen.map;

import java.util.ArrayList;
import java.util.Collections;

public class SchedAnalysis {

	public static final int numModes = 4;
	public static final int modeLO = 0;
	public static final int modeTF = 1;
	public static final int modeOV = 2;
	public static final int modeHI = 3;
	
	public static final double recursionThreshold = 0.01;
	
	ArrayList<Task> taskList;
	Schedule schedule;
	ArrayList<Processor> procList;

	public SchedAnalysis(ArrayList<Task> taskList, Schedule schedule,
			ArrayList<Processor> procList) {
		this.taskList = taskList;
		this.schedule = schedule;
		this.procList = procList;
	}

	@SuppressWarnings("unchecked")
	public boolean schedAnalysis() {
		// TODO

		// First a priority must be assigned to each task
		// The priority is assigned to the binding in the schedule
		ArrayList<Task> priorityList = (ArrayList<Task>) taskList.clone();
		Collections.sort(priorityList);
		for (int i = 0; i < priorityList.size(); i++) {
			Task t = priorityList.get(i);
			schedule.setPriority(t, i);
		}

		// Check the schedulability on each processor
		for (Processor p : procList) {
			/*
			 * Build an array of tasks that execute on this processor
			 */
			ArrayList<Task> procTaskList = new ArrayList<>();
			for (Task t : priorityList) {
				if (schedule.getProcessor(t).equals(p)) {
					procTaskList.add(t);
				}
			}
			System.out.println("hi");

			/*
			 * each task can be assigned to: all 3 modes, a pair of modes, or a
			 * single mode... (OV,TF,HI), (OV,TF), (OV,HI), (TF,HI) , (OV), (TF)
			 */

			//start by checking schedulability for the LO mode
			
			for(int i = 0; i < procTaskList.size(); i++){
				Task t = procTaskList.get(i);
				double responseTime = t.wcetLowerBound;
				while(responseTime < t.period){
					double rSum = 0;
					for(int j = i+1; j < procTaskList.size(); j++ ){
						Task hpTask = procTaskList.get(j);
						rSum += Math.ceil(responseTime/hpTask.period) * hpTask.wcetLowerBound;
					}
					rSum += t.wcetLowerBound;
					if(Math.abs(rSum - responseTime) < recursionThreshold){
						break;
					} else {
						responseTime = rSum;
					}
				}
				if(responseTime > t.period){
					return false;
				} 
				
				schedule.setResponseTime(t, modeLO,responseTime);
				return true;
			}
			//then proceed to the other three modes
			
			//Make this a bit more general... same thing 4 times with some catches...
			
		}

		return false;
	}

	public double qosAnalysis() {

		// TODO
		return 0;
	}

}
