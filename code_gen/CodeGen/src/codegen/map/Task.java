package codegen.map;

import java.io.Serializable;



public class Task implements Comparable<Task>, Serializable{

	double wcetLowerBound;
	double wcetUpperBound;
	double period;
	boolean criticality;
	public static final boolean HI = true;
	public static final boolean LO = false;
	boolean type;
	public static final boolean replica = true;
	public static final boolean original = false;
	double utilization;
	public String name;
	Processor proc;
	static int tID;
	static int numOriginalTasks;
	int id;
	int[] maxNumReexecutions = {1,1,1,1};
	
	
	
	public Task(double clo, double chi, double period, boolean criticality, String name) {
		wcetLowerBound = clo;
		wcetUpperBound = chi;
		this.period = period;
		this.criticality = criticality;
		this.name = name;
		id = tID++;
		numOriginalTasks++;
	}

	public Task(Task t, boolean isReplica) {
		wcetLowerBound = t.wcetLowerBound;
		wcetUpperBound = t.wcetUpperBound;
		this.period = t.period;
		this.criticality = t.criticality;
		this.type = Task.replica;
		id = tID++;
		this.maxNumReexecutions = t.maxNumReexecutions;
		if(replica){
			name = t.name + "R";
		} else {
			name = t.name;
		}
	}


	public String toFullString(){
		String s = name + ": ";
		if(criticality == Task.HI){
			s += "HI ";
		} else {
			s += "LO ";
		}
		s += "Clo: " + wcetLowerBound + ", Chi: " + wcetUpperBound +
			", period: " + period + ", criticality: ";
		s +=  ", replica: " + type;
		return s;
	}

	public Object getProcessor() {
		return proc;
	}
	
	public void setMaxNumReexecutions(int[] n){
		for(int i = 0; i < SchedAnalysis.numModes; i++){
			maxNumReexecutions[i] = n[i];
		}
	}
	
	//TODO: is this an OK sorting algorithm??

	@Override
	public int compareTo(Task task) {
		int result;
		if(this.period > task.period){
			result = -1;
		} else if(this.period == task.period){
			result = this.id > task.id ? -1 : 1;
		} else {
			result = 1;
		}
		return result;
	}

	public boolean isCritical() {
		return criticality == true;
	}
	
	@Override
	public String toString(){
		return name;
	}

	public void setPeriod(int period) {
		this.period = period;	
	}

	public void setUtilization(double utilization) {
		this.utilization = utilization;
	}

	public double getUtilization() {
		return utilization;
	}

	public void setExecutionTimes(double cRatio) {
		wcetLowerBound = period * utilization;
		wcetUpperBound = wcetLowerBound * cRatio;
	}
	
	public static int getNumOriginalTasks(){
		return numOriginalTasks;
	}
	
	public boolean isReplica(){
		return type = replica;
	}
}
