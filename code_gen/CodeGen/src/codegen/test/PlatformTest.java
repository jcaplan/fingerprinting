package codegen.test;

import gnu.getopt.Getopt;
import gnu.getopt.LongOpt;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import org.jgap.Chromosome;
import org.jgap.InvalidConfigurationException;

import codegen.map.Application;
import codegen.map.DMR;
import codegen.map.FaultMechanism;
import codegen.map.GAEngine;
import codegen.map.Lockstep;
import codegen.map.MSFitnessFunction;
import codegen.map.MapConfiguration;
import codegen.map.MapConstraint;
import codegen.map.Mapper;
import codegen.map.MultiThreadGABreeder;
import codegen.map.Processor;
import codegen.map.RAConfiguration;
import codegen.map.RAFitnessFunction;
import codegen.map.Schedule;
import codegen.map.Task;

public class PlatformTest {

	private static final int MIN_NUM_TASKS = 20;
	private static final double MIN_PERCENT_HI = 0.5;
	private static final double AVERAGE_DEFAULT_UTILIZATION = 0.8;
	private static final double MAX_WCET_FACTOR = 2.0;
	private static final int NUM_ITERATIONS = 100;
	private static Random generator;
	
	private static  int randomSeed = 2;
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
		for(int i = 0; i < taskList.size() - 1; i++){
			Task t = taskList.get(i);
			double ftemp1 = generator.nextDouble();
			double ftemp2 = util*Math.pow(ftemp1, 1.0/((double)taskList.size() - (i + 1)));
			t.setUtilization(util-ftemp2);
			if(t.getUtilization() > 0.45){
				t.setUtilization(0.45);
				ftemp2 = util - 0.45;
			}
			
			if(t.isCritical()){
				double cRatio = 1 + generator.nextDouble()*(maxWcetFactor-1);
				if(cRatio * t.getUtilization() > 0.45){
					 t.setUtilization(0.45 / cRatio);
				}
				t.setExecutionTimes(cRatio);
				ftemp2 = util - t.getUtilization();
			} else {
				t.setExecutionTimes(1);
			}
			util = ftemp2;
		}
		
				
		Task t = taskList.get(taskList.size() - 1);
		t.setUtilization(util);
		
		double cRatio = 1 + generator.nextDouble()*(maxWcetFactor-1);
		
		t.setExecutionTimes(cRatio);
		
		app.setTasks(taskList);
		
		return app;
	}

	private static ArrayList<Processor> addProcessors(int numOdrCore, int numLockstepCore) {
		ArrayList<Processor> pList = new ArrayList<>();
		
		for(int i = 0; i < numOdrCore; i++){
			pList.add(new Processor("cpu" + i, Processor.OdrType));	
		}
		for(int i = 0; i < numLockstepCore; i++){
			pList.add(new Processor("cpuM" + i,Processor.lockstepType));
		}
		
		return pList;
	}
	
	

	public static void main(String[] args) throws Exception{
		
		int minNumTasks = MIN_NUM_TASKS; 
		double minHiPercent = MIN_PERCENT_HI;
		double avgDefaultUtil = AVERAGE_DEFAULT_UTILIZATION; 
		double maxWcetFactor = MAX_WCET_FACTOR;
		int iter = NUM_ITERATIONS;
		int c;
		LongOpt[] longopts = new LongOpt[9];
		longopts[0] = new LongOpt("mintasknum", LongOpt.REQUIRED_ARGUMENT,
				null, 0);
		longopts[1] = new LongOpt("minhiperc", LongOpt.REQUIRED_ARGUMENT, null, 1);
		longopts[2] = new LongOpt("avgutil", LongOpt.REQUIRED_ARGUMENT,
				null, 2);
		longopts[3] = new LongOpt("maxwcetf", LongOpt.REQUIRED_ARGUMENT, null, 3);
		longopts[4] = new LongOpt("numthreads", LongOpt.REQUIRED_ARGUMENT, null, 4);
		longopts[5] = new LongOpt("seed", LongOpt.REQUIRED_ARGUMENT, null,5);
		longopts[6] = new LongOpt("iter", LongOpt.REQUIRED_ARGUMENT, null,6);
		
		
		
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
				maxWcetFactor = Double.parseDouble(g.getOptarg());
				break;
			case 4:
				MultiThreadGABreeder.NUM_THREADS = Integer.parseInt(g.getOptarg());
				break;
			case 5:
				randomSeed = Integer.parseInt(g.getOptarg());
				break;
			case 6:
				iter = Integer.parseInt(g.getOptarg());
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
//		Logger.turnLoggingOn();  
		generator = new Random(randomSeed);
		
		ArrayList<Processor> lsList = addProcessors(0,2);
		ArrayList<Processor> fpList = addProcessors(4,0);

		ArrayList<FaultMechanism> ls = new ArrayList<>();
		ls.add(new Lockstep());
		ArrayList<FaultMechanism> odr = new ArrayList<>();
		odr.add(new DMR());
//		odr.add(new Lockstep());
		
		
		double[] sched = new double[2];
		int LS = 0;
		int FP = 1;
		for(int i = 0; i < iter; i++){
			Application app = generateRandomApplication(minNumTasks, minHiPercent, avgDefaultUtil,
					0, 2, maxWcetFactor);
			

			//Skip the first stage, go around mapper
			Map<Task, ArrayList<Processor>> legalMappings = new HashMap<>();
			for(Task t : app.getTaskList()){
				legalMappings.put(t, lsList);
			}

			MapConfiguration.reset();
			MapConfiguration msConfig = new MapConfiguration("msEngine" + RAFitnessFunction.count++);
			Chromosome sampleChromosome = null;
			try {
				sampleChromosome = RAFitnessFunction.createMSChromosome(msConfig,
						app.getTaskList(), legalMappings);
			} catch (InvalidConfigurationException e) {
				e.printStackTrace();
			}
			ArrayList<MapConstraint> contraints = new ArrayList<>();
			MSFitnessFunction msFF = new MSFitnessFunction(app.getTaskList(), contraints,legalMappings,lsList);
			GAEngine msEngine = new GAEngine(msFF, msConfig,sampleChromosome);

			msEngine.findSolution();
			Schedule lsSched = null;
			Schedule fpSched = null;
			if(msFF.getBestSchedule() != null){
				sched[LS]++;
				lsSched = msFF.getBestSchedule();
				System.out.println("ls scheduled");
			} else {
				System.out.println("lockstep failed");
			}
			RAConfiguration.reset();
			mapper = new Mapper();
			mapper.setApplication(app);
			mapper.setProcList(fpList);
			mapper.setFTMS(odr);
			mapper.findSchedule();
			if(mapper.getBestSchedule() != null){
				sched[FP]++;
				fpSched = mapper.getBestSchedule();
				System.out.println("odr scheduled");
			} else {
				System.out.println("odr failed");
			}
			
			if(lsSched != null && fpSched != null){
				double[] qos = lsSched.getQosPerMode();
				System.out.println("ls qos [LO,TF,OV,HI]: ");
				for(int j = 0; j < qos.length; j++){
					System.out.println(qos[j] + ", ");
				}
				qos = fpSched.getQosPerMode();
				System.out.println("fp qos [LO,TF,OV,HI]: ");
				for(int j = 0; j < qos.length; j++){
					System.out.println(qos[j] + ", ");
				}
			}
			
		}
		
		
		sched[LS] /= iter;
		sched[FP] /= iter;
		
		System.out.println("schedulability LS: " + sched[LS] + "FP: " + sched[FP]);
	}



	
}
