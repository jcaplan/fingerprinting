package codegen.map;

import java.util.Comparator;

import codegen.gen.Function;

public class Task implements Comparable<Task>{

	double wcetLowerBound;
	double wcetUpperBound;
	double period;
	boolean criticality;
	public static final boolean HI = true;
	public static final boolean LO = false;
	boolean type;
	public static final boolean replica = true;
	public static final boolean original = false;
	
	Processor proc;
	
	int[] maxNumReexecutions = new int[SchedAnalysis.numModes];
	
	
	
	public Task(double clo, double chi, double period, boolean criticality) {
		wcetLowerBound = clo;
		wcetUpperBound = chi;
		this.period = period;
		this.criticality = criticality;
	}

	public Task(Task t, boolean isReplica) {
		wcetLowerBound = t.wcetLowerBound;
		wcetUpperBound = t.wcetUpperBound;
		this.period = t.period;
		this.criticality = t.criticality;
		this.type = Task.replica;
	}

	@Override
	public String toString(){
		String s = "Clo: " + wcetLowerBound + ", Chi: " + wcetUpperBound +
				", period: " + period + ", criticality: ";
		if(criticality == Task.HI){
			s += "HI";
		} else {
			s += "LO";
		}
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
			result = 1;
		} else if(this.period == task.period){
			if(System.currentTimeMillis()%2 == 0){
				result = 1;
			} else {
				result = -1;
			}
		} else {
			result = -1;
		}
		return result;
	}
	
}
