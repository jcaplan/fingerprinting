package codegen.sw;

import java.util.ArrayList;

public class Task {
	int periodMS;
	int startTime;
	boolean critical;
	boolean eventTriggered;
	boolean timeTriggered;
	String computationFile;
	String label;
	ArrayList<Signals> startConditions;
	ArrayList<Task> parents;
	ArrayList<Task> children;
	
}
