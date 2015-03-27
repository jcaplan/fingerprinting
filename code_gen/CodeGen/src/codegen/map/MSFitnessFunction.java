package codegen.map;

import java.io.IOException;
import java.util.ArrayList;

import codegen.sw.Task;

import org.jgap.*;

import codegen.hw.*;
import codegen.sw.*;
import codegen.util.Qsort;

public class MSFitnessFunction extends FitnessFunction {
	public Platform platform;
	public Application app;

	int currentTime;
	boolean[] readyList;
	boolean[] scheduledTasks;
	boolean[] completedTasks;
	int[] coreAssignments;
	int[] startTimes;
	boolean[] coreBusy;
	boolean[] coreReserved;
	int[] coreJob;
	int numTasks;
	int numCores;
	IChromosome chromosome;
	public MSFitnessFunction() {

	}

	public MSFitnessFunction(Platform platform, Application app) {
		this.platform = platform;
		this.app = app;
	}

	private boolean checkLegal(IChromosome chrom) {
		// for every task that has a copy,
		// check that the copy is on a different core
		boolean legal = true;
		for (int i = 0; i < numTasks; i++) {
			Task t = app.getTaskList().get(i);
			if (t.copy != null) {
				int pIndex = (Integer) chrom.getGene(i).getAllele();
				int iCopy = app.getTaskList().indexOf(t.copy);
				if(iCopy == -1){
					throw new RuntimeException("no copy found!!");
				}
				int pIndexCopy = (Integer) chrom.getGene(iCopy).getAllele();
				if (pIndex == pIndexCopy) {
					legal = false;
					break;
				}
			}
		}
		return legal;
	}

	@Override
	protected double evaluate(IChromosome chrom) {
		chromosome = chrom;
		// Check that copies aren't on the same core
		// Chromosome contains the mapping of task to core
		// Don't have to figure that out.
		currentTime = 0;
		numTasks = app.getTaskList().size();
		if (!checkLegal(chrom))
			return 0;
		readyList = new boolean[numTasks];
		scheduledTasks = new boolean[numTasks];
		completedTasks = new boolean[numTasks];
		coreAssignments = new int[numTasks];
		startTimes = new int[numTasks];
		numCores = platform.processors.size();
		coreBusy = new boolean[numCores];
		coreJob = new int[numCores];
		coreReserved = new boolean[numCores];
		for (int i = 0; i < numTasks; i++) {
			scheduledTasks[i] = false;
			completedTasks[i] = false;
			readyList[i] = false;
			//Translate the relative index of each processor
			//to the absolute index in the platform for each task
			int index = (Integer) chrom.getGene(i).getAllele();
			Task t = app.getTaskList().get(i);
			Processor p = t.legalProcessors.get(index);
			int pIndex = platform.processors.indexOf(p);
			coreAssignments[i] = pIndex;
		}
		for (int i = 0; i < numCores; i++) {
			coreBusy[i] = false;
			coreReserved[i] = false;
		}
		buildReadyList();
		while (tasksLeft()) {
			// fill the schedule at the current timeF
			updateSchedule(chrom);
			// increment time to the next event
			sanityCheck("scheduling");
			updateTime();
			sanityCheck("time");
			// update the readylist
			buildReadyList();
			sanityCheck("ready");
			sanityCheckReady();

		}

		try {
			PrintSchedule.print(numCores, coreAssignments, startTimes, app, "sched");
		} catch (IOException | InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return 10000 - currentTime;
	}
	
	public void sanityCheck(String s){
		for(int i = 0; i < numCores; i++){
			int busyCount = 0;
			ArrayList<Integer> cores = new ArrayList<Integer>();
			if(coreBusy[i]){
				busyCount = 0;
				//Check that exactly one task is running and assigned to it
				for(int j = 0; j < numTasks; j++){
					if(scheduledTasks[j] && !completedTasks[j] && coreAssignments[j] == i){
						cores.add(j);
						busyCount++;
					}
				}
			}
			if(busyCount > 1){
				String err = "";
				for(Integer intg : cores){
					err += app.getTaskList().get(intg).label + ";";
				}
				System.out.println(checkLegal(chromosome));
				throw new RuntimeException(s + " stage: " + "Core " + i + " is running tasks:" + err);
			}
		}
	}
	
	public void sanityCheckReady(){
		for(int i = 0; i < numTasks; i++){
			if(readyList[i] && scheduledTasks[i])
				throw new RuntimeException("task " + i + "is ready and scheduled at the same time");
		}
	}

	private void updateTime() {
		// Look at the cores that are busy
		// find the one with the smallest (startTime + runTime)
		// update the currentTime to startTime + runTime
		int nextTime = Integer.MAX_VALUE;

		// for each task that has been scheduled but not completed
		ArrayList<Integer> over = new ArrayList<Integer>();
		for (int i = 0; i < numTasks; i++) {
			if (scheduledTasks[i] && !completedTasks[i]) {
				int startTime = startTimes[i];
				Task t = app.getTaskList().get(i);
				int runTime = t.executionTime;
				int time = startTime + runTime;
				if (time < nextTime){
					nextTime = time;
					over = new ArrayList<Integer>();
					over .add(i);
				} else if(time == nextTime) {
					over.add(i);
				}
			}
		}
		currentTime = nextTime;
		for(Integer i : over){
			coreBusy[coreAssignments[i]] = false;
			completedTasks[i] = true;
		}
	}

	private boolean tasksLeft() {
		for (int i = 0; i < scheduledTasks.length; i++) {
			if (!scheduledTasks[i])
				return true;
		}
		return false;
	}

	private boolean coreIsAvailable(int tID){
		int core = coreAssignments[tID];
		return !coreBusy[core];
	}
	private boolean coreIsReserved(int tID){
		int core = coreAssignments[tID];
		return coreReserved[core];
	}
	
	private void updateSchedule(IChromosome chrom) {
		//examine each task
		for (int i = 0; i < numTasks; i++) {
			// If the task is ready to run
			if (readyList[i]) {
				
				// If the core is available schedule the next task
				if (coreIsAvailable(i) && !coreIsReserved(i)) {
					// Check if it requires a copy
					Task t = app.getTaskList().get(i);
					if (t.copy != null) {
						int indexCopy = app.getTaskList().indexOf(t.copy);
						// check if the redundant copy's core is busy
						if (coreIsAvailable(indexCopy)){
							scheduleTask(indexCopy, t.copy);
							scheduleTask(i, t);
						} else 
							checkWaitCondition(t.copy,indexCopy);
					} else if(!t.redundant){
						scheduleTask(i, t);
					}
				}
			}
		}

	}
	
	private void checkWaitCondition(Task t, int index){
		
		int core = coreAssignments[index];
		Task runningTask = app.getTaskList().get(coreJob[core]);
		int timeRemaining = runningTask.executionTime + startTimes[coreJob[core]] - currentTime;
		if (timeRemaining < 10)
			coreReserved[core] = true;
		
	}

	private void scheduleTask(int taskIndex, Task t) {
		startTimes[taskIndex] = currentTime;
		readyList[taskIndex] = false;
		scheduledTasks[taskIndex] = true;
		int core = coreAssignments[taskIndex];
		coreBusy[core] = true;
		coreJob[core] = taskIndex;
		coreReserved[core] = false;
	}

	private void buildReadyList() {
		for (int i = 0; i < numTasks; i++) {
			Task task = app.getTaskList().get(i);

			// If the task hasn't been scheduled yet
			if (!scheduledTasks[i]) {
				// If there are no parents
				if (task.parents.isEmpty()) {
					readyList[i] = true;
				} else {
					// If all the parents are done
					boolean ready = true;
					
					for (int j = 0; j < task.parents.size(); j++) {
						Task parent = task.parents.get(j);
						int parentIndex = app.getTaskList().indexOf(parent);
						if (!(completedTasks[parentIndex])) {
							ready = false;
							break;
						}
					}
					readyList[i] = ready;
				}
			} 
		}
	}
}
