package codegen.map;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

import codegen.util.Logger;

public class Application implements Serializable {

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
	public String toString() {
		String s = "";
		int count = 0;
		for (Task t : taskList) {
			Logger.logMessage("Task " + count + ": " + t);
			count++;
		}
		return s;
	}

	public int getNumTasks() {
		return taskList.size();
	}

	public int getNumHiTasks() {
		int numHiTasks = 0;
		for (Task t : taskList) {
			if (t.criticality == Task.HI) {
				numHiTasks++;
			}
		}
		return numHiTasks;
	}

	public Task getTask(String name) {
		for (Task t : taskList) {
			if (t.name.equals(name)) {
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
	
	public static Application generateRandomApplication(int minNumTasks,
			double minHiPercent, double averageDefaultUtilization,
			int numOdrCores, int numLockstepCores, double maxWcetFactor){
		return generateRandomApplication(minNumTasks, minHiPercent, averageDefaultUtilization, 
				numOdrCores, numLockstepCores, maxWcetFactor,new Random());
	}

	public static Application generateRandomApplication(int minNumTasks,
			double minHiPercent, double averageDefaultUtilization,
			int numOdrCores, int numLockstepCores, double maxWcetFactor,
			Random random) {

		Application app = new Application();
		
		// create tasks and assign criticality
		ArrayList<Task> taskList = generateTaskList(random,minHiPercent,minNumTasks);

		generatePeriods(random, taskList);

		double util = averageDefaultUtilization
				* (numOdrCores / 2 + numLockstepCores);

		// calculate the utilization for each task
		for (int i = 0; i < taskList.size() - 1; i++) {
			Task t = taskList.get(i);
			double ftemp1 = random.nextDouble();
			double nextUtil = util
					* Math.pow(ftemp1,
							1.0 / ((double) taskList.size() - i));
			t.setUtilization(util - nextUtil);
			if (t.getUtilization() > 0.45) {
				t.setUtilization(0.45);
				nextUtil = util - 0.45;
			}

			if (t.isCritical()) {
				double cRatio = 1 + random.nextDouble() * (maxWcetFactor - 1);
				if (cRatio * t.getUtilization() > 0.45) {
					t.setUtilization(0.45 / cRatio);
				}
				t.setExecutionTimes(cRatio);
				nextUtil = util - t.getUtilization();
			} else {
				t.setExecutionTimes(1);
			}
			util = nextUtil;
		}

		Task t = taskList.get(taskList.size() - 1);
		t.setUtilization(util);

		double cRatio = 1 + random.nextDouble() * (maxWcetFactor - 1);

		t.setExecutionTimes(cRatio);

		app.setTasks(taskList);

		return app;
	}

	private static ArrayList<Task> generateTaskList(Random random, double minHiPercent, int minNumTasks) {
		ArrayList<Task> tasks = new ArrayList<>();
		int numTasks = minNumTasks + (int) (Math.random() * minNumTasks);
		int numHiTasks = (int) Math.ceil(numTasks * minHiPercent);
		for (int i = 0; i < numTasks; i++) {
			tasks.add(new Task(0, 0, 0, (i < numHiTasks), "T" + i));
		}
		// shuffle so critical tasks aren't next to each other
		Collections.shuffle(tasks);
		return tasks;
	}

	private static void generatePeriods(Random random,
			ArrayList<Task> taskList) {
		// assign execution times:
		for (int i = 0; i < taskList.size(); i++) {
			int rand = (int) (random.nextInt(9));
			Task t = taskList.get(i);
			switch (rand) {
			case 0:
				t.setPeriod(10);
				break;
			case 1:
				t.setPeriod(20);
				break;
			case 2:
				t.setPeriod(40);
				break;
			case 3:
				t.setPeriod(50);
				break;
			case 4:
				t.setPeriod(100);
				break;
			case 5:
				t.setPeriod(200);
				break;
			case 6:
				t.setPeriod(400);
				break;
			case 7:
				t.setPeriod(500);
				break;
			case 8:
				t.setPeriod(1000);
			}
		}
	}
}
