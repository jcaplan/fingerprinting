package codegen.map;

import java.util.*;
import codegen.sw.*;
public class Schedule {
	ArrayList<Task> taskList;
	ArrayList<Integer> startTimes;
	boolean busy;
	public Schedule(){
		taskList = new ArrayList<Task>();
		startTimes = new ArrayList<Integer>();
		busy = false;
	}
}
