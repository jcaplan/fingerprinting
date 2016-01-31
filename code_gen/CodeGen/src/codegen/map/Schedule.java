package codegen.map;

import java.io.Serializable;
import java.util.*;

public class Schedule implements Serializable{

	Map<Task,Binding> bindings = new HashMap<>();
	
	public void allocate(Task t, Processor p) {
		bindings.put(t,new Binding(p,t.isCritical()));		
	}


	public void deallocate(Task t, Processor p) {
		bindings.get(t).removeBinding(p);
	}


	
	public Processor getLoModeProcessor(Task t){
		Binding b = bindings.get(t);
		if(b == null){
			return null;
		}
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
		bindings.get(t).removeBinding(mode);
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
	
	private class Binding implements Serializable{
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

		public void removeBinding(int mode) {
			processor[mode] = null;
			responseTime[mode] = 0;
		}
		
		public void removeBinding(Processor p){
			for(int i = 0; i < SchedAnalysis.numModes; i++){
				if(processor[i] == p){
					processor[i] = null;
				}
			}
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
	
	public double[] getQosPerMode(){
		double[] results = new double[SchedAnalysis.numModes];
		results[SchedAnalysis.modeLO] = 1;
		int numLoTasks = 0;
		//each mode requires a QoS number
		int[] modes = {SchedAnalysis.modeOV,SchedAnalysis.modeTF,SchedAnalysis.modeHI};
		for(Task t : bindings.keySet()){
			if(t.isCritical()){
				continue;
			}
			
			numLoTasks++;
			
			for(int mode : modes){
				if(isScheduled(t, mode)){
					results[mode]++;
				}
			}
		}
		
		for(int mode : modes){
			results[mode] /= (double)numLoTasks;
		}
//		System.out.println(String.format("OV: %f, TF: %f, HI: %f",results[modeOV],results[modeTF],
//				results[modeHI]));
		
		return results;
	}


}
