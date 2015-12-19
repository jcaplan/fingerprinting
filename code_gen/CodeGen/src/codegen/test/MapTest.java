package codegen.test;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

import codegen.map.*;
import codegen.util.Logger;

public class MapTest {

	private static final int MIN_NUM_TASKS = 10;
	private static final double MIN_PERCENT_HI = 0.4;
	private static final double AVERAGE_DEFAULT_UTILIZATION = 0.4;
	private static final double MAX_WCET_FACTOR = 2.0;
	
	private static final int randomSeed = 1;
	private static final double percentHI = 0.45;
	private static Random generator = new Random(randomSeed);
	private static final int numOdrCores = 4;
	private static final int numLockstepCores = 1;
	private static ArrayList<FaultMechanism> ftms;
	static{
		ftms = new ArrayList<>();
		ftms.add(new DMR());
		ftms.add(new TMR());
		ftms.add(new Lockstep());
		ftms.add(new PassiveReplication());
	}
	static Mapper mapper;
	
	public static Application generateRandomApplication(){

		//create tasks and assign criticality
		ArrayList<Task> taskList = new ArrayList<>();
		int numTasks = MIN_NUM_TASKS + (int)(Math.random()*MIN_NUM_TASKS);
		int numHiTasks = (int) Math.ceil(numTasks * MIN_PERCENT_HI);
		Application app = new Application();
		for(int i = 0; i < numTasks; i++){	
			taskList.add(new Task(0,0,0, (i < numHiTasks),"T" + i));
		}
		
		//shuffle so critical tasks aren't next to each other
		Collections.shuffle(taskList);
		
		//assign execution times:
		for(int i = 0; i < numTasks; i++){
			int rand = (int)(Math.random()*9);
			Task t = taskList.get(i);
			switch(rand){
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
		
		double util = AVERAGE_DEFAULT_UTILIZATION*(numOdrCores/2 + numLockstepCores);
		
		//calculate the utilization for each task
		for(int i = 0; i < taskList.size(); i++){
			Task t = taskList.get(i);
			double rand = Math.random();
			double newUtil = util*Math.pow(rand, 1.0/((double)taskList.size() - (i + 1)));
			t.setUtilization(util-newUtil);
			if(t.getUtilization() > 0.49){
				t.setUtilization(0.49);
				newUtil = util - 0.49;
			}
			
			if(t.isCritical()){
				double cRatio = 1 + Math.random()*(MAX_WCET_FACTOR-1);
				t.setExecutionTimes(cRatio);
			} else {
				t.setExecutionTimes(1);
			}
		}
		
		
		app.setTasks(taskList);
		
		return app;
	}

	private static void addProcessors(int numOdrCore, int numLockstepCore) {
		ArrayList<Processor> pList = new ArrayList<>();
		
		for(int i = 0; i < numOdrCore; i++){
			pList.add(new Processor("cpu" + i, Processor.OdrType));	
		}
		for(int i = 0; i < numLockstepCore; i++){
			pList.add(new Processor("cpuM" + i,Processor.lockstepType));
		}
		
		mapper.setProcList(pList);
	}
	
	

	public static void main(String[] args){
		
		Logger.turnLoggingOn(); 
		mapper = new Mapper();
		Application app = generateRandomApplication();
		mapper.setApplication(app);
		mapper.printApp();
		
		addProcessors(numOdrCores,numLockstepCores);
		mapper.printProcessors();
		mapper.setFTMS(ftms);

		long time = System.currentTimeMillis();
		mapper.findSchedule();
		System.out.println("time: " + (System.currentTimeMillis() - time));
		System.out.println("**********************************");
		for(Task t : app.getTaskList()){
			System.out.println(t.toFullString());
		}
		System.out.println("**********************************");
		System.out.println(mapper.getBestSchedule());
		System.out.println("**********************************");
		System.out.println(mapper.getBestScheduleFitness());
		System.out.println("**********************************");
		System.out.println(mapper.getBestFTMs());
	}



	
}
