package codegen.test;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

import codegen.map.*;
import codegen.util.Logger;
import gnu.getopt.Getopt;
import gnu.getopt.LongOpt;

public class MapTest {

	private static final int MIN_NUM_TASKS = 10;
	private static final double MIN_PERCENT_HI = 0.4;
	private static final double AVERAGE_DEFAULT_UTILIZATION = 0.7;
	private static final double MAX_WCET_FACTOR = 2.0;
	private static Random generator;
	
	private static  int randomSeed = 1;
	private static final int NUM_ODR_CORES = 4;
	private static final int NUM_LOCKSTEP_CORES = 1;
	private static ArrayList<FaultMechanism> ftms;
	static{
		ftms = new ArrayList<>();
		ftms.add(new TMR());
		ftms.add(new DMR());
		ftms.add(new Lockstep());
		ftms.add(new PassiveReplication());
	}
	static Mapper mapper;
	
	public static Application generateRandomApplication(int minNumTasks, double minHiPercent,
			double averageDefaultUtilization, int numOdrCores, int numLockstepCores,
			double maxWcetFactor){

		//create tasks and assign criticality
		ArrayList<Task> taskList = new ArrayList<>();
		int numTasks = minNumTasks + (int)(Math.random()*minNumTasks);
		int numHiTasks = (int) Math.ceil(numTasks * minHiPercent);
		Application app = new Application();
		for(int i = 0; i < numTasks; i++){	
			taskList.add(new Task(0,0,0, (i < numHiTasks),"T" + i));
		}
		
		//shuffle so critical tasks aren't next to each other
		Collections.shuffle(taskList);
		
		//assign execution times:
		for(int i = 0; i < numTasks; i++){
			int rand = (int)(generator.nextInt(9));
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
		
		double util = averageDefaultUtilization*(numOdrCores/2 + numLockstepCores);
		
		//calculate the utilization for each task
		for(int i = 0; i < taskList.size(); i++){
			Task t = taskList.get(i);
			double rand = generator.nextDouble();
			double newUtil = util*Math.pow(rand, 1.0/((double)taskList.size() - (i + 1)));
			t.setUtilization(util-newUtil);
			if(t.getUtilization() > 0.49){
				t.setUtilization(0.49);
				newUtil = util - 0.49;
			}
			
			if(t.isCritical()){
				double cRatio = 1 + generator.nextDouble()*(maxWcetFactor-1);
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
	
	

	public static void main(String[] args) throws Exception{
		
		int minNumTasks = MIN_NUM_TASKS; 
		double minHiPercent = MIN_PERCENT_HI;
		double avgDefaultUtil = AVERAGE_DEFAULT_UTILIZATION; 
		int numOdrCores = NUM_ODR_CORES; 
		int numLockstepCores = NUM_LOCKSTEP_CORES;
		double maxWcetFactor = MAX_WCET_FACTOR;
		
		int c;
		LongOpt[] longopts = new LongOpt[9];
		longopts[0] = new LongOpt("minhinum", LongOpt.REQUIRED_ARGUMENT,
				null, 0);
		longopts[1] = new LongOpt("minhiperc", LongOpt.REQUIRED_ARGUMENT, null, 1);
		longopts[2] = new LongOpt("avgutil", LongOpt.REQUIRED_ARGUMENT,
				null, 2);
		longopts[3] = new LongOpt("numodr", LongOpt.REQUIRED_ARGUMENT, null, 3);
		longopts[4] = new LongOpt("numls", LongOpt.REQUIRED_ARGUMENT, null, 4);
		longopts[5] = new LongOpt("maxwcetf", LongOpt.REQUIRED_ARGUMENT, null, 5);
		longopts[6] = new LongOpt("numthreads", LongOpt.REQUIRED_ARGUMENT, null, 6);
		longopts[7] = new LongOpt("seed", LongOpt.REQUIRED_ARGUMENT, null,7);
		
		
		// One colon: required
		// Two colons: optional
		// Starting colon: differentiate between invalid args and invalid
		// options
		Getopt g = new Getopt("Main", args, "h", longopts);
		g.setOpterr(false);

		while ((c = g.getopt()) != -1) {
			switch (c) {
			case 0:
				minNumTasks = Integer.parseInt(g.getOptarg());
				break;
			case 1:
				minHiPercent = Double.parseDouble(g.getOptarg());
				break;
			case 2:
				avgDefaultUtil = Double.parseDouble(g.getOptarg());
				break;
			case 3:
				numOdrCores = Integer.parseInt(g.getOptarg());
				break;
			case 4:
				numLockstepCores = Integer.parseInt(g.getOptarg());
				break;
			case 5:
				maxWcetFactor = Double.parseDouble(g.getOptarg());
				break;
			case 6:
				MultiThreadGABreeder.NUM_THREADS = Integer.parseInt(g.getOptarg());
				break;
			case 7:
				randomSeed = Integer.parseInt(g.getOptarg());
				break;
			case 'h':
				System.out.println("minhinum=int minhiperc=double avgutil=double \n"
						+ "numodr=int numls=int maxwcetf=double numthreads=int seed=int");
				System.exit(0);
				break;
			case '?':
				throw new Exception("Unrecognized option");
			}
		}
		
		
		Logger.turnLoggingOn(); 
		mapper = new Mapper();

		
		if(numOdrCores < 3){
			ftms.remove(0);
		}
		
		 
		generator = new Random(randomSeed);
		Application app = generateRandomApplication(minNumTasks, minHiPercent, avgDefaultUtil,
				numOdrCores, numLockstepCores, maxWcetFactor);
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
