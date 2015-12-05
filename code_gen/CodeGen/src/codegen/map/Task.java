package codegen.map;



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
	public String name;
	Processor proc;
	
	int[] maxNumReexecutions = {1,1,1,1};
	
	
	
	public Task(double clo, double chi, double period, boolean criticality, String name) {
		wcetLowerBound = clo;
		wcetUpperBound = chi;
		this.period = period;
		this.criticality = criticality;
		this.name = name;
	}

	public Task(Task t, boolean isReplica) {
		wcetLowerBound = t.wcetLowerBound;
		wcetUpperBound = t.wcetUpperBound;
		this.period = t.period;
		this.criticality = t.criticality;
		this.type = Task.replica;
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
			if(System.currentTimeMillis()%2 == 0){
				result = -1;
			} else {
				result = 1;
			}
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
	
}
