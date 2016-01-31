package codegen.map;

import java.util.*;


public class HeurMapper extends Mapper{

	Map<Processor, Double> procUtilizationHI;
	
	public HeurMapper(){
		super();
		procUtilizationHI = new HashMap<>();
	}
	
	private Comparator<Processor> sortProcIncrHIUtil = new Comparator<Processor>() {
        @Override
        public int compare(Processor p1, Processor p2) {
        	double result = procUtilizationHI.get(p1) - procUtilizationHI.get(p2);
        	if(result > 0){
        		return 1;
        	} else if (result < 0){
        		return -1;
        	} else {
        		return 0;
        	}
        }
	};


	private Comparator<Task> sortTaskDecreasingUtilHI = new Comparator<Task>() {
        @Override
        public int compare(Task t1, Task t2) {
        	double result = t2.getUtilizationHI() - t1.getUtilizationHI();
        	if(result > 0){
        		return 1;
        	} else if (result < 0){
        		return -1;
        	} else {
        		return 0;
        	}
        }
	};



	@Override
	public void findSchedule() {
		
		/*
		 * 1. schedule as many HI tasks as possible on lockstep cores using worst fit decreasing
		 * 2. schedule remaining HI tasks using DMR on processing cores
		 * 3. try first fit decreasing for LO tasks
		 * 4. Simulated annealing? Genetic algorithm? see if it improves on FFD? Do comparison?
		 * 
		 */
		
		//not keeping track of processor utilization anywhere. keep running total in map
		for(Processor p : procList){
			procUtilizationHI.put(p, 0.0);
		}
				
		// 1. schedule as many HI tasks as possible on lockstep cores using worst fit decreasing
		Schedule schedule = new Schedule();
		ArrayList<Task> leftoverHI = worstFitDecreasingHiLs(schedule, app.taskList, procList);
		
		// 2. schedule remaining HI tasks using DMR on processing cores
		// for each task create a replica
		HashMap<Task,Task[]> replicas = DMR.makeReplicaList(leftoverHI);
		// Now check for the DMR tasks
		if(!worstFitDecreasingHiDmr(schedule, leftoverHI, procList, replicas)){
			return;
		}
		
		// Now it's time to start working on the LO tasks
		
	}
	
	/**
	 * Try to schedule as many HI tasks as possible on lockstep cores
	 * @param sched
	 * @param taskList
	 * @param procList
	 */
	public ArrayList<Task> worstFitDecreasingHiLs(Schedule sched, 
			ArrayList<Task> taskList,ArrayList<Processor> procList){
		
		ArrayList<Task> tList = new ArrayList<>();
		ArrayList<Processor> pList = new ArrayList<>();
		ArrayList<Task> unassignedTasks = new ArrayList<>();
		//Only use critical tasks
		for(Task t : taskList){
			if(t.isCritical()){
				tList.add(t);
			}
		}
		
		//Only use fault tolerant cores
		for(Processor p : procList){
			if(p.faultTolerant){
				pList.add(p);
			}
		}
		
		SchedAnalysis sa = new SchedAnalysis(tList, sched, pList);
			
		
		//sort tasks by HI utilization
		Collections.sort(tList,sortTaskDecreasingUtilHI);
		
		
		for(Task t : tList){
			//sort proc list by utilization
			Collections.sort(pList, sortProcIncrHIUtil);
			boolean success = false;
			for(Processor p : pList){
				//assign a task
				sched.allocate(t, p);
				//check if it is schedulable
				if(sa.schedHIMode(p)){
					success = true;
					Double util = procUtilizationHI.get(p);
					util += t.getUtilizationHI();
					procUtilizationHI.put(p, util);
					break;
				} else {
					//if not unassign
					sched.deallocate(t,p);
				}
				
			}
			if(!success){
				unassignedTasks.add(t);
			}
		}
		return unassignedTasks;
	}
	
	public boolean worstFitDecreasingHiDmr(Schedule sched, 
			ArrayList<Task> tList,ArrayList<Processor> procList,
			HashMap<Task,Task[]>replicas){
		ArrayList<Processor> pList = new ArrayList<>();
		ArrayList<Task> unassignedTasks = new ArrayList<>();

		//This time task set already contains the correct tasks
		
		//Only use non-fault tolerant cores
		for(Processor p : procList){
			if(!p.faultTolerant){
				pList.add(p);
			}
		}

		SchedAnalysis sa = new SchedAnalysis(tList, sched, pList);
			

		
		//Task list should still be sorted
		

		for(Task t : tList){
			//sort proc list by utilization
			Collections.sort(pList, sortProcIncrHIUtil);
			boolean success = false;
			for(Processor p : pList){
				//assign a task
				sched.allocate(t, p);
				//check if it is schedulable
				if(sa.schedHIMode(p)){
					double util = procUtilizationHI.get(p);
					util += t.getUtilizationHI();
					procUtilizationHI.put(p, util);
					
					//now try the replica
					Task replica = replicas.get(t)[0];
					Collections.sort(pList, sortProcIncrHIUtil);
					for(Processor p2 : pList){
						if(!p2.equals(p)){
							sched.allocate(replica,p2);
							if(sa.schedHIMode(p2)){
								success = true;
								util = procUtilizationHI.get(p2);
								util += replica.getUtilizationHI();
								procUtilizationHI.put(p2, util);
								break;
							} else {
								sched.deallocate(replica, p2);
							}
						} 
					}
					break;
				} else {
					//if not unassign
					sched.deallocate(t,p);
				}
				
			}
			if(!success){
				return false;
			}
		}

		
		return true;
	}
}
