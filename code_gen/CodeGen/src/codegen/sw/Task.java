package codegen.sw;

import java.util.ArrayList;

import codegen.hw.*;
import codegen.map.RAGene;
public class Task implements Comparable<Task>{
	
	class MutableInteger {
	    public int value;
	}

	
	public int executionTime;
	public int startTime;
	boolean critical;
	boolean eventTriggered;
	boolean timeTriggered;
	public boolean redundant = false;
	String computationFile;
	public String label;
	ArrayList<Signal> startConditions;
	public ArrayList<Task> parents;
	public ArrayList<Task> children;
	MutableInteger numCriticalChildren;
	public int taskID;
	public Task copy;
	public int rank; //For pretty printing
	public ArrayList<Processor> legalProcessors;
	public boolean scheduled;
	int fdTechnique;
	static int taskCount = 0;
	
	
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
	
	public void changeRank(int rank){
		this.rank = rank;
	}
	
	public void cleanList(ArrayList<Task> list){
		for(Task t : list){
			if(children.contains(t)){
				children.remove(t);
				numCriticalChildren.value--;
			}
			if(parents.contains(t))
				parents.remove(t);
		}
		copy = null;
	}
	
	public Task(){
		taskID = taskCount;
		label = "T" +Integer.toString(taskCount++);
		initLists();
	}
	public Task(String label){
		this.label = label;
		taskID = taskCount++;
		initLists();
	}
	
	public Task(boolean critical){
		taskID = taskCount;
		label = "T" +Integer.toString(taskCount++);
		this.critical = critical;
		initLists();
	}
	
	public Task(int id){
		taskID = id;
		initLists();
	}
	
	public Task(Task t, boolean redundant){
		executionTime = t.executionTime;
		startTime = t.startTime;
		critical = t.critical;
		eventTriggered = t.eventTriggered;
		timeTriggered = t.timeTriggered;
		taskID = t.taskID;
		numCriticalChildren = t.numCriticalChildren;
		this.redundant = redundant;
		computationFile = t.computationFile;
		if(redundant){
			label = t.label + "p";
			startConditions = t.startConditions;
			parents = t.parents;
			children = t.children;
			rank = t.rank;
			legalProcessors = t.legalProcessors;
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
		legalProcessors = new ArrayList<Processor>();
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
		
		//If they have the same taskID then put the original first
		else if(this.taskID == t.taskID){
			if(this.redundant)
				return 1;
			else
				return -1;
		}
		
		return 0;
	}
	
	public boolean equals(int id){
		return this.taskID == id;
	}
	
	public void resetLegalProcessors(){
		legalProcessors = new ArrayList<Processor>();
	}
	
	public void setExecutionTime(int time){
		executionTime = time;
	}
	
	public void addLegalProcessor(Processor p){
		legalProcessors.add(p);
		if(p.hasFprint)
			fdTechnique = RAGene.FD_FPRINT;
		else if(p.faultTolerant)
			fdTechnique = RAGene.FD_DMR;
	}
	
	public boolean isCritical(){
		return critical;
	}
	
	public String toString(){
		return label;
	}
}
