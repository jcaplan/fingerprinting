package codegen.sw;

import java.util.ArrayList;

public class Task {
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
	int taskID;
	Task copy;
	static int taskCount = 1;
	
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
		this.redundant = redundant;
		computationFile = t.computationFile;
		if(redundant){
			label = t.label + "_copy";
			startConditions = t.startConditions;
			parents = t.parents;
			children = t.children;
		}else{
			label = t.label;
		}
	}
	
	ArrayList<Task> getParents(){
		return parents;
	}
	void initLists(){
		startConditions = new ArrayList<Signal>();
		parents = new ArrayList<Task>();
		children = new ArrayList<Task>();
	}
	@Override
	public boolean equals(Object obj){
		Task t = (Task)obj;
		return this.taskID == t.taskID;
	}
	
	public boolean equals(int id){
		return this.taskID == id;
	}
	
	
	public String toString(){
		return label;
	}
}
