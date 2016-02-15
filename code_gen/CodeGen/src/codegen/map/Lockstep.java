package codegen.map;

import java.util.ArrayList;
import java.util.Map;

public class Lockstep extends FaultMechanism{
	
	int[] reexecutionProfile = {1,2,1,2};
	
	@Override
	public String getCommandName() {
		return "Lockstep";
	}

	@Override
	public synchronized void updateTaskList(ArrayList<Task> taskList, int taskIndex,
			Map<Task,Task[]> constraints, Map<Task, FaultMechanism> techniqueMap,
			Map<Task, int[]> executionProfiles) {
		Task t = taskList.get(taskIndex);
		executionProfiles.put(t,reexecutionProfile);
		techniqueMap.put(t, this);
	}

	@Override
	public String toString(){
		return "LS";
	}

	@Override
	public int getPermutationSize(int numLegalProcessors) {
		return numLegalProcessors;
	}

	@Override
	public void allocateProcessors(Schedule schedule, int permutation,
			Map<Task, ArrayList<Processor>> legalMappings, Task t,
			Map<Task, Task[]> replicas) {
		schedule.allocate(t,legalMappings.get(t).get(permutation));
	}
}
