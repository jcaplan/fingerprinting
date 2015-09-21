package codegen.map;

import java.util.ArrayList;
import java.util.Map;

public class PassiveReplication extends FaultMechanism{
	
	int[] reexecutionProfileOriginal = {1,2,1,2};
	int[] reexecutionProfileReplica = {1,1,1,1};
	
	@Override
	public String getCommandName() {
		return "PassiveReplication";
	}

	@Override
	public void updateTaskList(ArrayList<Task> taskList, int taskIndex,
			ArrayList<MapConstraint> constraints, Map<Task, FaultMechanism> techniqueMap) {
		Task t = taskList.get(taskIndex);
		Task replica = new Task(t,Task.replica);
		taskList.add(replica);
		constraints.add(new MapConstraint(t,replica));
		t.setMaxNumReexecutions(reexecutionProfileOriginal);
		replica.setMaxNumReexecutions(reexecutionProfileReplica);
		techniqueMap.put(t, this);
		techniqueMap.put(replica, this);
	}

}
