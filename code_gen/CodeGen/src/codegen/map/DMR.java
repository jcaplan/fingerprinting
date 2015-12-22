package codegen.map;

import java.util.ArrayList;
import java.util.Map;

import org.jgap.Gene;

public class DMR extends FaultMechanism {
	
	private final int[] reexecutionProfile = {1,2,1,2};
	
	
	@Override
	public String getCommandName() {
		return "DMR";
	}

	@Override
	public synchronized void updateTaskList(ArrayList<Task> taskList, int taskIndex,
			Map<Task,Task[]> replicas, Map<Task, FaultMechanism> techniqueMap) {
		Task t = taskList.get(taskIndex);
		Task replica = new Task(t,Task.replica);
		taskList.add(replica);
		replicas.put(t, new Task[] {replica});
		t.setMaxNumReexecutions(reexecutionProfile);
		replica.setMaxNumReexecutions(reexecutionProfile);
		techniqueMap.put(t, this);
//		techniqueMap.put(replica, this);
	}

	@Override
	public String toString(){
		return "DMR";
	}

	@Override
	public int getPermutationSize(int numLegalProcessors) {
		return numLegalProcessors * (numLegalProcessors - 1);
	}

	@Override
	public void allocateProcessors(Schedule schedule, int permutation,
			Map<Task, ArrayList<Processor>> legalMappings, Task t,
			Map<Task,Task[]> replicas) {
		ArrayList<Processor> pList = new ArrayList<>(legalMappings.get(t));		
		int tIndex = permutation / (pList.size() - 1);
		int repIndex = permutation % (pList.size() - 1);
		Processor tProc = pList.get(tIndex);
		schedule.allocate(t,tProc);
		pList.remove(tProc);
		Task replica = replicas.get(t)[0];
		schedule.allocate(replica, pList.get(repIndex));
	}
	
}
