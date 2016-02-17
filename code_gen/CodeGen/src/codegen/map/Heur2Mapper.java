package codegen.map;

import java.util.*;

public class Heur2Mapper extends Mapper{
	
	Map<Processor, Double> procUtilizationHI;
	Map<Task, int[]> executionProfiles;
	double[] results;
	public Heur2Mapper(){
		super();
		procUtilizationHI = new HashMap<>();
		executionProfiles = new HashMap<>();
	}
	
	public double[] getResults(){
		return results;
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

	private Comparator<Task> sortTaskDecreasingUtilLO = new Comparator<Task>() {
        @Override
        public int compare(Task t1, Task t2) {
        	double result = t2.getUtilizationLO() - t1.getUtilizationLO();
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
		
		//check all configurations of DMR/lockstep
		ArrayList<Task> hiList = new ArrayList<>();

		ArrayList<Task> loList = new ArrayList<>();
		int[] loProfile = {1,1,1,1};
		for(Task t : app.taskList){
			if(t.isCritical()){
				hiList.add(t);
			} else {
				executionProfiles.put(t,loProfile);
				loList.add(t);
			}
		}
		System.out.println("Num configurations: " + Math.pow(2, hiList.size()));
		results = new double[(int)Math.pow(2,hiList.size())];
		int count = 0;
		for(int i = 0; i < Math.pow(2, hiList.size()); i++){
			Schedule sched = new Schedule();
			
			// binary number -> each task is either dmr (0) or lockstep (1)
			// 2 ^ hi task allocations
			HashMap<Task,Task[]> replicas = new HashMap<>();
			int[] lsProfile = {1,2,1,2};
			for(int j = 0; j < hiList.size(); j++){
				Task t = hiList.get(j);
				if((i & (1 << j)) != 0){
					//DMR
					DMR.makeReplica(t, executionProfiles, replicas);
					//Allocate one to each processing core
					sched.allocate(t, procList.get(0));
					sched.allocate(replicas.get(t)[0], procList.get(1));
				} else {
					//Lockstep
					executionProfiles.put(t, lsProfile);
					sched.allocate(t, procList.get(2));
				}		
			}
			
			ArrayList<Task> fullTaskList = new ArrayList<>(app.taskList);
			for(Task[] r : replicas.values()){
				fullTaskList.add(r[0]);
			}
			
			SchedAnalysis sa = new SchedAnalysis(fullTaskList, sched, procList, executionProfiles);
			boolean success = true;
			for(Processor p : procList){
				if(!sa.schedHIMode(p)){
//					System.out.println("Could not fit all HI tasks");
					success = false;
					break;
				}
			}
			
			if(success){
//				firstFitDecreasingLo(sched,fullTaskList,procList);
				int numPerms = (int) Math.pow(3,loList.size());
				Random rand = new Random();
				for(int j = 0; j < 20; j++){
					int perm = rand.nextInt(numPerms);
					
					for(int k = 0; k < loList.size(); k++){
						sched.allocate(loList.get(k), procList.get(((int)(perm / Math.pow(3, k)) % 3)));
					}
					
					if(sa.schedAnalysis()){
						double qos = sa.qosAnalysis();
						if(qos > results[i]){
							if(results[i] == 0){
								count++;
							}
							results[i] = qos;
						}
						if(qos > fitness){
							fitness = qos;
							bestSchedule = sched;
						}
					}
					
					
				}
				
				
				
			}
			
		}
				
//		System.out.println("Number of successful iterations: " + count);
		
	}
	
	private boolean firstFitDecreasingLo(Schedule sched,
			ArrayList<Task> taskList, ArrayList<Processor> procList) {
		ArrayList<Task> tList = new ArrayList<>();
		//Only use critical tasks
		for(Task t : taskList){
			if(!t.isCritical()){
				tList.add(t);
			}
		}
		

		SchedAnalysis sa = new SchedAnalysis(taskList, sched, procList, executionProfiles);
		Collections.sort(tList,sortTaskDecreasingUtilLO);
		for(Task t : tList){
			//sort proc list by utilization
			boolean success = false;
			for(Processor p : procList){
				//assign a task
				sched.allocate(t, p);
				//check if it is schedulable

				
				if(sa.schedLOMode(p)){
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
			if(success == false){
				return false;
			}
		}	
		return true;
	}

}