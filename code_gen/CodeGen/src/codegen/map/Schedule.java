package codegen.map;

import java.util.*;

public class Schedule {

	private class Binding {
		Processor[] processor = new Processor[SchedAnalysis.numModes];
		int priority;
		double[] responseTime = new double[SchedAnalysis.numModes];
		
		
		public Binding(Processor processor){

			this.processor[SchedAnalysis.modeLO] = processor;
			this.priority = Integer.MAX_VALUE;
		}
		
		public void setPriority(int prio){
			priority = prio;
		}
		
		public int getPriority(){
			return priority;
		}
		
		public Processor getLoModeProcessor(){
			return processor[SchedAnalysis.modeLO];
		}

		public double getResponseTime(int mode) {
			return responseTime[mode];
		}
	}
	
	Map<Task,Binding> bindings = new HashMap<>();
	
	
	public void allocate(Task t, Processor p) {
		bindings.put(t,new Binding(p));		
	}


	public boolean checkConstraints(ArrayList<MapConstraint> constraints) {
		for(MapConstraint c : constraints){
			Processor p1 = bindings.get(c.t1).getLoModeProcessor();
			Processor p2 = bindings.get(c.t2).getLoModeProcessor();
			
			if(!MapConstraint.check(p1,p2)){
				return false;
			}
		}
		return true;
	}
	
	public Processor getLoModeProcessor(Task t){
		return bindings.get(t).getLoModeProcessor();
	}


	public void setPriority(Task t, int prio) {
		bindings.get(t).setPriority(prio);
		
	}
	
	public void setResponseTime(Task t, int mode, double rt){
		bindings.get(t).responseTime[mode] = rt;
	}

	public int getPriority(Task t){
		return bindings.get(t).getPriority();
	}


	public double getResponseTime(Task t, int mode) {
		return bindings.get(t).getResponseTime(mode);
	}
}
