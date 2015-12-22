package codegen.map;

import java.util.ArrayList;
import java.util.Map;

public class TMR extends FaultMechanism {

	int[] reexecutionProfile = {1,1,1,1};
	
	@Override
	public String getCommandName() {
		return "TMR";
	}

	@Override
	public synchronized void updateTaskList(ArrayList<Task> taskList, int taskIndex,
			Map<Task,Task[]> replicas, Map<Task, FaultMechanism> techniqueMap) {
		Task t = taskList.get(taskIndex);
		
		Task replica1 = new Task(t,Task.replica);
		taskList.add(replica1);
		
		Task replica2 = new Task(t,Task.replica);
		taskList.add(replica2);
		replicas.put(t,new Task[] {replica1,replica2});
		
		t.setMaxNumReexecutions(reexecutionProfile);
		replica1.setMaxNumReexecutions(reexecutionProfile);
		replica2.setMaxNumReexecutions(reexecutionProfile);
		techniqueMap.put(t, this);
//		techniqueMap.put(replica1, this);
//		techniqueMap.put(replica2, this);
	}

	@Override
	public String toString(){
		return "TMR";
	}

	@Override
	public int getPermutationSize(int numLegalProcessors) {
		return numLegalProcessors * (numLegalProcessors - 1) 
				* (numLegalProcessors - 2);
	}

	@Override
	public void allocateProcessors(Schedule schedule, int permutation,
			Map<Task, ArrayList<Processor>> legalMappings, Task t,
			Map<Task, Task[]> replicas) {
		ArrayList<Processor> pList = new ArrayList<>(legalMappings.get(t));
		int numCores = pList.size();
		int tIndex = permutation / ((numCores - 1)*(numCores - 2));
		int rep0Index = permutation % (((numCores-1)*(numCores-2))/(numCores-2));
		int rep1Index = permutation % (numCores - 2);
		Processor proc = pList.get(tIndex);
		schedule.allocate(t,proc);
		pList.remove(proc);
		
		Task replica = replicas.get(t)[0];
		proc = pList.get(rep0Index);
		schedule.allocate(replica, proc);
		pList.remove(proc);
		
		replica = replicas.get(t)[1];
		proc = pList.get(rep1Index);
		schedule.allocate(t,proc);
	}

}
