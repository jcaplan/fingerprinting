package codegen.map;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;

import codegen.util.Logger;

public class Application implements Serializable{
	
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
			Logger.logMessage("Task " + count + ": " + t);
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

	public Task getTask(String name) {
		for(Task t : taskList){
			if(t.name.equals(name)){
				return t;
			}
		}
		return null;
	}

	public void shuffleTasks() {
		Collections.shuffle(taskList);
	}

	public void setTasks(ArrayList<Task> taskList) {
		this.taskList = taskList;
	}
}
