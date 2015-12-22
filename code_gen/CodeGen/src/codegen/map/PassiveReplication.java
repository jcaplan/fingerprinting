package codegen.map;

import java.util.ArrayList;
import java.util.Map;

public class PassiveReplication extends FaultMechanism {
	
	int[] reexecutionProfileOriginal = {1,1,1,1};
	int[] reexecutionProfileFault = {0,1,0,1};
	
	@Override
	public String getCommandName() {
		return "PassiveReplication";
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
		t.setMaxNumReexecutions(reexecutionProfileOriginal);
		replica1.setMaxNumReexecutions(reexecutionProfileOriginal);
		replica2.setMaxNumReexecutions(reexecutionProfileFault);
		techniqueMap.put(t, this);
//		techniqueMap.put(replica1, this);
//		techniqueMap.put(replica2,this);
	}
	
	@Override
	public String toString(){
		return "PR";
	}

	@Override
	public int getPermutationSize(int numLegalProcessors) {
		return numLegalProcessors * numLegalProcessors 
				* (numLegalProcessors - 1);
	}

	@Override
	public void allocateProcessors(Schedule schedule, int permutation,
			Map<Task, ArrayList<Processor>> legalMappings, Task t,
			Map<Task, Task[]> replicas) {
		ArrayList<Processor> pList = new ArrayList<>(legalMappings.get(t));
		int numCores = pList.size();
		int tIndex = permutation / ((numCores)*(numCores - 1));
		int rep0Index = permutation % (((numCores)*(numCores-1))/(numCores-1));
		int rep1Index = permutation % (numCores - 1);
		Processor proc = pList.get(tIndex);
		schedule.allocate(t,proc);
		
		
		Task replica = replicas.get(t)[0];

		replica = replicas.get(t)[0];
		Processor repProc = pList.get(rep0Index);
		schedule.allocate(replica,repProc);
		
		pList.remove(proc);
		
		replica = replicas.get(t)[1];
		proc = pList.get(rep1Index);
		schedule.allocate(replica, proc);
		
	}

}
