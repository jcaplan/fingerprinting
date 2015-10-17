package codegen.map;

import java.util.ArrayList;
import java.util.Map;

public class PassiveReplication extends FaultMechanism{
	
	int[] reexecutionProfileOriginal = {1,1,1,1};
	int[] reexecutionProfileFault = {0,1,0,1};
	
	@Override
	public String getCommandName() {
		return "PassiveReplication";
	}

	@Override
	public void updateTaskList(ArrayList<Task> taskList, int taskIndex,
			ArrayList<MapConstraint> constraints, Map<Task, FaultMechanism> techniqueMap) {
		Task t = taskList.get(taskIndex);
		Task replica1 = new Task(t,Task.replica);
		taskList.add(replica1);
		Task replica2 = new Task(t,Task.replica);
		constraints.add(new MapConstraint(t,replica1));
		t.setMaxNumReexecutions(reexecutionProfileOriginal);
		replica1.setMaxNumReexecutions(reexecutionProfileOriginal);
		replica2.setMaxNumReexecutions(reexecutionProfileFault);
		techniqueMap.put(t, this);
		techniqueMap.put(replica1, this);
		techniqueMap.put(replica2,this);
	}

}
