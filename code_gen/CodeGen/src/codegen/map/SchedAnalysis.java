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
		// First a priority must be assigned to each task
		// The priority is assigned to the binding in the schedule
		//Sort by decreasing execution time, and increasing priority
		ArrayList<Task> priorityList = (ArrayList<Task>) taskList.clone();
		Collections.sort(priorityList);
		int lowestPriority = priorityList.size() - 1;
		for (int i = 0; i < priorityList.size(); i++) {
			Task t = priorityList.get(i);
			schedule.setPriority(t, lowestPriority - i);
		}

		// Check the schedulability on each processor
		for (Processor p : procList) {
			/*
			 * Build an array of tasks that execute on this processor
			 */
			ArrayList<Task> procTaskList = new ArrayList<>();
			for (Task t : priorityList) {
				if (schedule.getLoModeProcessor(t).equals(p)) {
					procTaskList.add(t);
				}
			}

			/*
			 * each LO task can be assigned to: all 3 modes, a pair of modes, or a
			 * single mode... (OV,TF,HI), (OV,TF), (OV,HI), (TF,HI) , (OV), (TF)
			 */
			
			//5 Response time calculations, LO, OV, TF, HIa,HIb
			//put each one in a different function
			//Different scenarios use different carry over jobs
			//Define a method to return the list of carry over jobs between two modes
			// Pass the analysis function the list of carry over jobs, up to 3 sets
			
			

			// start by checking schedulability for the LO mode 
			if(!scheduleLoMode(procTaskList)){
				return false;
			};
			
			
			
			//Then do HI mode, assume all LO tasks are dropped in LO mode.
			//If this test fails no point in continuing.
			
			if(!scheduleHIMode(procTaskList,modeHI)){
				return false;
			}
						



		}

		return true;
	}

	private boolean scheduleHIMode(ArrayList<Task> procTaskList, int carryOverMode) {

		ArrayList<Task> carryOverJobs = new ArrayList<>();
		ArrayList<Task> carryOverJobsLO = new ArrayList<>();
		
		for(int i = 0 ; i < procTaskList.size(); i++){
			//if a task is critical then it is not a carry over job
			//if a task is lo, then if it is scheduled in the carryOverMode but not the current mode
			//it is a carryOverJob
			//if a task is lo, but was not scheduled in the carryOverMode then it is a carryOverJobLO
			//otherwise it should be scheduled in the current mode
		}
		switch(carryOverMode){
		case modeOV:
			break;
		case modeTF:
			break;
		case modeHI:
			//don't need to do anything here;
			break;
		default:
			break;
		}
		
		
		for (int i = 0; i < procTaskList.size(); i++) {
			Task t = procTaskList.get(i);
			double responseTime = t.wcetUpperBound * t.maxNumReexecutions[modeHI];
			while (responseTime < t.period) {
				double rSum = 0;
				for (int j = i + 1; j < procTaskList.size(); j++) {
					Task hpTask = procTaskList.get(j);

					if(hpTask.criticality == Task.HI){
						rSum += Math.ceil(responseTime / hpTask.period)
								* hpTask.wcetUpperBound * hpTask.maxNumReexecutions[modeHI];
					} else {
						
						rSum += rSum += Math.ceil(schedule.getResponseTime(t,modeLO) / hpTask.period)
								* hpTask.wcetLowerBound;
					}
				}
				rSum += t.wcetLowerBound;
				if (Math.abs(rSum - responseTime) < recursionThreshold) {
					break;
				} else {
					responseTime = rSum;
				}
			}
			
			if (responseTime > t.period) {
				return false;
			}

			schedule.setResponseTime(t, modeHI, responseTime);
		}
		return true;
	}

	private boolean scheduleLoMode(ArrayList<Task> procTaskList) {
		for (int i = 0; i < procTaskList.size(); i++) {
			Task t = procTaskList.get(i);
			double responseTime = t.wcetLowerBound;
			while (responseTime < t.period) {
				double rSum = 0;
				for (int j = i + 1; j < procTaskList.size(); j++) {
					Task hpTask = procTaskList.get(j);
					rSum += Math.ceil(responseTime / hpTask.period)
							* hpTask.wcetLowerBound;
				}
				rSum += t.wcetLowerBound;
				if (Math.abs(rSum - responseTime) < recursionThreshold) {
					break;
				} else {
					responseTime = rSum;
				}
			}
			
			if (responseTime > t.period) {
				return false;
			}

			schedule.setResponseTime(t, modeLO, responseTime);
		}
		return true;
	}

	public double qosAnalysis() {

		// TODO
		return 0;
	}

}
