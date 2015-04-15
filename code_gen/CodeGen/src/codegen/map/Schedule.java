package codegen.map;

import java.io.FileNotFoundException;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.*;

import codegen.sw.*;

/**
 * The schedule class is a static collection that is used to keep track of the best
 * schedule produced by the GA and also printing the schedule along with the PrintSchedule class.
 * @author jonah
 *
 */
public class Schedule {
	static int numCores;
	static int[] coreAssignment;
	static int[] startTimes; 
	static int[] executionTimes;
	static String[] taskLabels;
	static Application app;
	static int fitnessValue;
	public static String fileName;
	
	static boolean submitSchedule(int numCores,int[] coreAssignment,int[] startTimes,Application app,int fitnessValue){
		if(fitnessValue > Schedule.fitnessValue){
			Schedule.fitnessValue = fitnessValue;
//			System.out.println("Time" + System.currentTimeMillis()/1000 + ": " + fitnessValue);
			Schedule.numCores = numCores;
			int numTasks = coreAssignment.length;
			Schedule.coreAssignment = new int[numTasks];
			Schedule.startTimes = new int[numTasks];
			Schedule.app = app;
			executionTimes = new int[numTasks];
			taskLabels = new String[numTasks];
			for(int i = 0; i < numTasks; i++){
				Schedule.coreAssignment[i] = coreAssignment[i];
				Schedule.startTimes[i] = startTimes[i];
				executionTimes[i] = app.getTaskList().get(i).executionTime;
				taskLabels[i] = app.getTaskList().get(i).label;
			}
			try {
				app.printDotFile(fileName + "T");
			} catch (FileNotFoundException | UnsupportedEncodingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			printSchedule();
			return true;
		}
		return false;
	}
	
	static void printSchedule(){
		PrintSchedule p = new PrintSchedule();
		p.print(numCores, coreAssignment, startTimes, app, executionTimes, taskLabels, fileName);
		

	}
	
	static void initSchedule(){
		fitnessValue = 0;
	}
}
