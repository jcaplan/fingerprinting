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
	public void updateTaskList(ArrayList<Task> taskList, int taskIndex,
			ArrayList<MapConstraint> constraints, Map<Task, FaultMechanism> techniqueMap) {
		Task t = taskList.get(taskIndex);
		t.setMaxNumReexecutions(reexecutionProfile);
		techniqueMap.put(t, this);
	}

}
