package codegen.map;

import java.util.*;

public class Schedule {

	private class Binding {
		Processor processor;
		int priority;
		
		
		public Binding(Processor processor){

			this.processor = processor;
			this.priority = Integer.MAX_VALUE;
		}
		
		public void setPriority(int prio){
			priority = prio;
		}
	}
	
	Map<Task,Binding> bindings = new HashMap<>();
	
	
	public void allocate(Task t, Processor p) {
		bindings.put(t,new Binding(p));		
	}


	public boolean checkConstraints(ArrayList<MapConstraint> constraints) {
		for(MapConstraint c : constraints){
			Processor p1 = bindings.get(c.t1).processor;
			Processor p2 = bindings.get(c.t2).processor;
			if(p1 == p2){
				return false;
			}
		}
		return true;
	}
	
	public Processor getProcessor(Task t){
		return bindings.get(t).processor;
	}


	public void setPriority(Task t, int i) {
		bindings.get(t).setPriority(i);
		
	}

}
