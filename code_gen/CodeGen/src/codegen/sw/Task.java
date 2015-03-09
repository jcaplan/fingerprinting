package codegen.sw;

import java.util.ArrayList;

public class Task implements Comparable<Task>{
	
	class MutableInteger {
	    public int value;
	}

	
	int periodMS;
	int startTime;
	boolean critical;
	boolean eventTriggered;
	boolean timeTriggered;
	boolean redundant = false;
	String computationFile;
	String label;
	ArrayList<Signal> startConditions;
	ArrayList<Task> parents;
	ArrayList<Task> children;
	MutableInteger numCriticalChildren;
	int taskID;
	Task copy;
	int rank; //For pretty printing
	static int taskCount = 1;
	
	public void printTaskStats(){
		System.out.println(label);
		System.out.println("\tRank: " + rank);
		System.out.println("\tCritical: " + critical);
		System.out.println("\tNum Critical Children: " + numCriticalChildren.value);
		System.out.println("\tnumChildren: " + children.size());
		
	}
	
	public void addChild(Task t){
		//have to watch out for shallow copies
		//only add to original
		if(redundant)
			return;
		children.add(t);
		if(t.critical){
			numCriticalChildren.value++;
		}
	}
	
	public void addParent(Task t){
		if(parents.contains(t))
			return;
		parents.add(t);
	}
	
	public void cleanList(ArrayList<Task> list){
		children.removeAll(list);
		parents.removeAll(list);
		numCriticalChildren.value -= list.size();
	}
	
	public Task(){
		taskID = taskCount;
		label = "Task_" +Integer.toString(taskCount++);
		initLists();
	}
	public Task(String label){
		this.label = label;
		taskID = taskCount++;
		initLists();
	}
	
	public Task(boolean critical){
		taskID = taskCount;
		label = "Task_" +Integer.toString(taskCount++);
		this.critical = critical;
		initLists();
	}
	
	public Task(int id){
		taskID = id;
		initLists();
	}
	
	public Task(Task t, boolean redundant){
		periodMS = t.periodMS;
		startTime = t.startTime;
		critical = t.critical;
		eventTriggered = t.eventTriggered;
		timeTriggered = t.timeTriggered;
		taskID = t.taskID;
		numCriticalChildren = t.numCriticalChildren;
		this.redundant = redundant;
		computationFile = t.computationFile;
		if(redundant){
			label = t.label + "_copy";
			startConditions = t.startConditions;
			parents = t.parents;
			children = t.children;
			rank = t.rank;
		}else{
			label = t.label;
		}
	}
	
	ArrayList<Task> getParents(){
		return parents;
	}
	void initLists(){
		numCriticalChildren = new MutableInteger();
		startConditions = new ArrayList<Signal>();
		parents = new ArrayList<Task>();
		children = new ArrayList<Task>();
	}
	@Override
	public boolean equals(Object obj){
		Task t = (Task)obj;
		return this.taskID == t.taskID &&
				this.redundant == t.redundant;
	}
	
	@Override
	public int compareTo(Task t) {
		//1. check if one has lower rank
		if(this.rank < t.rank)
			return -1;
		else if(this.rank > t.rank)
			return 1;
		//2. check if only one is critical
		if(this.critical && !t.critical){
			return -1;
		} else if(!this.critical && t.critical){
			return 1;
		}
		//3. See if one has more critical children

		if(numCriticalChildren.value > t.numCriticalChildren.value)
			return -1;
		else if(numCriticalChildren.value < t.numCriticalChildren.value)
			return 1;
		//4. See if one has more children
		if(this.children.size() > t.children.size())
			return -1;
		else if(t.children.size() < this.children.size())
			return 1;
		
		return 0;
	}
	
	public boolean equals(int id){
		return this.taskID == id;
	}
	
	
	public String toString(){
		return label;
	}
}
