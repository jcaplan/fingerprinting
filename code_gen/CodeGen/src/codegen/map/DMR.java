package codegen.map;

import java.util.ArrayList;
import java.util.Map;

public class DMR extends FaultMechanism {
	
	private final int[] reexecutionProfile = {1,2,1,2};
	
	
	@Override
	public String getCommandName() {
		return "DMR";
	}

	@Override
	public synchronized void updateTaskList(ArrayList<Task> taskList, int taskIndex,
			ArrayList<MapConstraint> constraints, Map<Task, FaultMechanism> techniqueMap) {
		Task t = taskList.get(taskIndex);
		Task replica = new Task(t,Task.replica);
		taskList.add(replica);
		constraints.add(new MapConstraint(t,replica));
		t.setMaxNumReexecutions(reexecutionProfile);
		replica.setMaxNumReexecutions(reexecutionProfile);
		techniqueMap.put(t, this);
		techniqueMap.put(replica, this);
	}

	@Override
	public String toString(){
		return "DMR";
	}
	
}
