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

	
	enum State {
		OV,OVHI,TF,TFNOHI,TFHI,DONE
	};
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
			

			// start by checking schedulability for the LO mode 
			if(!scheduleLoMode(procTaskList)){
				return false;
			};
						
			//Then do HI mode, assume all LO tasks are dropped in LO mode.
			//If this test fails no point in continuing.
			
			if(!schedHIMode(procTaskList)){
				return false;
			}
			
			//Now a schedule is sure to be found so do more expensive search....
			
			for(Task t : procTaskList){
				if(t.isCritical()){ //skip critical tasks
					continue;
				}
				State state = State.OV;
				while(state != State.DONE){
					boolean result;
					switch(state){
					case OV:
						schedule.setBinding(t, p, modeOV);
						result = schedOVMode(procTaskList);
						if(result){
							state = State.OVHI;
						} else {
							state = State.TFNOHI;
							schedule.removeBinding(t, modeOV);
						}
						break;
					case OVHI:
						schedule.setBinding(t, p, modeHI);
						result = schedOVHIMode(procTaskList);
						schedule.removeBinding(t, modeHI);
						if(result){
							state = State.TF;
						} else {
							state = State.TFNOHI;
						}
						break;
					case TF:
						schedule.setBinding(t, p, modeTF);
						result = schedTFMode(procTaskList);
						if(result){
							state = State.TFHI;
						} else {
							state = State.DONE;
							schedule.removeBinding(t, modeTF);
						}
						break;
					case TFNOHI:
						schedule.setBinding(t, p, modeTF);
						result = schedTFMode(procTaskList);
						if(!result){
							schedule.removeBinding(t, modeTF);
						}
						state = State.DONE;
						break;
					case TFHI:
						schedule.setBinding(t, p, modeHI);
						result = schedTFHIMode(procTaskList);
						if(!result){
							schedule.removeBinding(t, modeHI);
						}
						state = State.DONE;
						break;
					case DONE:
						break;
					
					}
				}
			}
		}

		return true;
	}

	private boolean schedHIMode(ArrayList<Task> procTaskList) {
		for (int i = 0; i < procTaskList.size(); i++) {
			Task t = procTaskList.get(i);
			//no carry overs -> skip LO tasks
			if(t.isCritical()){
				continue;
			}
			double responseTime = t.wcetUpperBound * t.maxNumReexecutions[modeHI];
			while (responseTime < t.period) {
				double rSum = t.wcetUpperBound * t.maxNumReexecutions[modeHI];
				for (int j = i + 1; j < procTaskList.size(); j++) {
					Task hpTask = procTaskList.get(j);

					if(hpTask.criticality == Task.HI){
						rSum += Math.ceil(responseTime / hpTask.period)
								* hpTask.wcetUpperBound * hpTask.maxNumReexecutions[modeHI];
					} else {
						
						rSum += Math.ceil(schedule.getResponseTime(t,modeLO) / hpTask.period)
								* hpTask.wcetLowerBound;
					}
				}
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
	
	
	private boolean schedOVMode(ArrayList<Task> procTaskList){
		for (int i = 0; i < procTaskList.size(); i++) {
			Task t = procTaskList.get(i);
			//always n = 1 for OV
			double responseTime = t.wcetUpperBound;
			while (responseTime < t.period) {
				double rSum = t.wcetUpperBound;
				for (int j = i + 1; j < procTaskList.size(); j++) {
					Task hpTask = procTaskList.get(j);
					
					if(schedule.isScheduled(hpTask,modeOV)){
						rSum += Math.ceil(responseTime / hpTask.period)
								* hpTask.wcetUpperBound * hpTask.maxNumReexecutions[modeHI];
					} else {
						rSum += Math.ceil(schedule.getResponseTime(t,modeLO) / hpTask.period)
								* hpTask.wcetLowerBound;
					}
				}
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
	
	private boolean schedTFMode(ArrayList<Task> procTaskList){
		for (int i = 0; i < procTaskList.size(); i++) {
			Task t = procTaskList.get(i);
			//no carry overs -> skip LO tasks
			if(t.isCritical()){
				continue;
			}
			double responseTime = t.wcetUpperBound * t.maxNumReexecutions[modeTF];
			while (responseTime < t.period) {
				double rSum = t.wcetUpperBound * t.maxNumReexecutions[modeTF];
				for (int j = i + 1; j < procTaskList.size(); j++) {
					Task hpTask = procTaskList.get(j);

					if(schedule.isScheduled(hpTask, modeTF)){
						rSum += Math.ceil(responseTime / hpTask.period)
								* hpTask.wcetLowerBound * hpTask.maxNumReexecutions[modeTF];
					} else {
						
						rSum += Math.ceil(schedule.getResponseTime(t,modeLO) / hpTask.period)
								* hpTask.wcetLowerBound;
					}
				}
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
	
	private boolean schedOVHIMode(ArrayList<Task> procTaskList) {
		for (int i = 0; i < procTaskList.size(); i++) {
			Task t = procTaskList.get(i);
			double responseTime = t.wcetUpperBound * t.maxNumReexecutions[modeHI];
			while (responseTime < t.period) {
				double rSum = t.wcetUpperBound * t.maxNumReexecutions[modeHI];
				for (int j = i + 1; j < procTaskList.size(); j++) {
					Task hpTask = procTaskList.get(j);

					if(schedule.isScheduled(hpTask, modeHI)){
						rSum += Math.ceil(responseTime / hpTask.period)
								* hpTask.wcetUpperBound * hpTask.maxNumReexecutions[modeHI];
					} else if (schedule.isScheduled(hpTask, modeOV)){
						rSum += Math.ceil(schedule.getResponseTime(t, modeOV) / hpTask.period)
								* hpTask.wcetLowerBound;
					} else {
						rSum += Math.ceil(schedule.getResponseTime(t,modeLO) / hpTask.period)
								* hpTask.wcetLowerBound;
					}
				}
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
	
	private boolean schedTFHIMode(ArrayList<Task> procTaskList) {
		for (int i = 0; i < procTaskList.size(); i++) {
			Task t = procTaskList.get(i);
			double responseTime = t.wcetUpperBound * t.maxNumReexecutions[modeHI];
			while (responseTime < t.period) {
				double rSum = t.wcetUpperBound * t.maxNumReexecutions[modeHI];
				for (int j = i + 1; j < procTaskList.size(); j++) {
					Task hpTask = procTaskList.get(j);

					if(schedule.isScheduled(hpTask, modeHI)){
						rSum += Math.ceil(responseTime / hpTask.period)
								* hpTask.wcetUpperBound * hpTask.maxNumReexecutions[modeHI];
					} else if (schedule.isScheduled(hpTask, modeTF)){
						rSum += Math.ceil(schedule.getResponseTime(t, modeTF) / hpTask.period)
								* hpTask.wcetLowerBound;
					} else {
						rSum += Math.ceil(schedule.getResponseTime(t,modeLO) / hpTask.period)
								* hpTask.wcetLowerBound;
					}
				}
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
				double rSum = t.wcetLowerBound;
				for (int j = i + 1; j < procTaskList.size(); j++) {
					Task hpTask = procTaskList.get(j);
					rSum += Math.ceil(responseTime / hpTask.period)
							* hpTask.wcetLowerBound;
				}
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
		double[] results = new double[numModes];
		results[modeLO] = 1;
		int numLoTasks = 0;
		//each mode requires a QoS number
		int[] modes = {modeOV,modeTF,modeHI};
		for(Task t : taskList){
			if(t.isCritical()){
				continue;
			}
			
			numLoTasks++;
			
			for(int mode : modes){
				if(schedule.isScheduled(t, mode)){
					results[mode]++;
				}
			}
		}
		
		double avg = 0;
		for(int mode : modes){
			results[mode] /= (double)numLoTasks;
			avg += results[mode];
		}
//		System.out.println(String.format("OV: %f, TF: %f, HI: %f",results[modeOV],results[modeTF],
//				results[modeHI]));
		
		return avg/3;
	}

}
