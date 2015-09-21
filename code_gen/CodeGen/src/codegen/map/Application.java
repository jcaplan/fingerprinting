package codegen.map;

import java.util.ArrayList;

public class Application {
	
	ArrayList<Task> taskList;
	ArrayList<Task> redundantList;
	
	public ArrayList<Task> getTaskList() {
		return taskList;
	}

	public ArrayList<Task> getRedundantList() {
		return redundantList;
	}
	
	public Application() {
		taskList = new ArrayList<Task>();
		redundantList = new ArrayList<Task>();
	}

	public void addTask(Task t) {
		taskList.add(t);
	}
	
	@Override
	public String toString(){
		String s = "";
		int count = 0;
		for(Task t : taskList){
			System.out.println("Task " + count + ": " + t);
			count++;
		}
		return s;
	}
	
	public int getNumTasks(){
		return taskList.size();
	}

	public int getNumHiTasks() {
		int numHiTasks = 0;
		for(Task t : taskList){
			if(t.criticality == Task.HI){
				numHiTasks++;
			}
		}
		return numHiTasks;
	}
}
