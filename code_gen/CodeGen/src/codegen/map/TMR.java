package codegen.map;

import java.util.ArrayList;
import java.util.Map;

public class TMR extends FaultMechanism{

	int[] reexecutionProfile = {1,1,1,1};
	
	@Override
	public String getCommandName() {
		return "TMR";
	}

	@Override
	public void updateTaskList(ArrayList<Task> taskList, int taskIndex,
			ArrayList<MapConstraint> constraints, Map<Task, FaultMechanism> techniqueMap) {
		Task t = taskList.get(taskIndex);
		
		Task replica1 = new Task(t,Task.replica);
		taskList.add(replica1);
		constraints.add(new MapConstraint(t,replica1));
		
		Task replica2 = new Task(t,Task.replica);
		taskList.add(replica2);
		constraints.add(new MapConstraint(t,replica2));
		
		t.setMaxNumReexecutions(reexecutionProfile);
		replica1.setMaxNumReexecutions(reexecutionProfile);
		replica2.setMaxNumReexecutions(reexecutionProfile);
		techniqueMap.put(t, this);
		techniqueMap.put(replica1, this);
		techniqueMap.put(replica2, this);
	}


}
