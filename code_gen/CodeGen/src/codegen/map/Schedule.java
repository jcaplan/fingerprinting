package codegen.map;

import java.util.*;

public class Schedule {

	Map<Task,Binding> bindings = new HashMap<>();
	
	public void allocate(Task t, Processor p) {
		bindings.put(t,new Binding(p,t.isCritical()));		
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


	public void setBinding(Task t, Processor p, int mode) {
		bindings.get(t).setProcessor(p, mode);
	}
	
	public void removeBinding(Task t, int mode){
		bindings.get(t).removBinding(mode);
	}


	public boolean isScheduled(Task t, int mode) {
		return bindings.get(t).processor[mode] != null;
	}
	
	@Override
	public String toString(){
		String s = "";
		for(Task t : bindings.keySet()){
			s += t.toString() + bindings.get(t);
		}
		return s;
	}
	
	private class Binding {
		Processor[] processor = new Processor[SchedAnalysis.numModes];
		int priority;
		double[] responseTime = new double[SchedAnalysis.numModes];	
		
		public Binding(Processor processor, boolean allModes){
			this.processor[SchedAnalysis.modeLO] = processor;
			this.priority = Integer.MAX_VALUE;
			if(allModes){
				this.processor[SchedAnalysis.modeOV] = processor;
				this.processor[SchedAnalysis.modeTF] = processor;
				this.processor[SchedAnalysis.modeHI] = processor;
				
			}
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
		
		public void setProcessor(Processor p, int mode){
			processor[mode] = p;
		}

		public void removBinding(int mode) {
			processor[mode] = null;
		}
		
		@Override
		public String toString(){
			return String.format("[%s,%s,%s]\n",processor[1],processor[2],processor[3]);
		}
	}

	public List<Processor> getProcessorsForAllPreplicas(Task t) {
		List<Processor> procList = new ArrayList<>();
		procList.add(bindings.get(t).getLoModeProcessor());
		String name = t.name;
		if(name.endsWith("R")){
			name = name.substring(0, name.length() - 2);
		}
		
		for(Task key : bindings.keySet()){
			if(key.name.startsWith(name)){
				Processor p = bindings.get(key).getLoModeProcessor(); 
				if(!procList.contains(p)){
					procList.add(p);
				}
			}
		}
		return procList;
	}
}
