package codegen.test;

import java.util.ArrayList;
import java.util.Random;

import codegen.map.*;
import codegen.util.Logger;
import gnu.getopt.Getopt;
import gnu.getopt.LongOpt;

public class MapTest {

	private static final int MIN_NUM_TASKS = 20;
	private static final double MIN_PERCENT_HI = 0.5;
	private static final double AVERAGE_DEFAULT_UTILIZATION = 0.8;
	private static final double MAX_WCET_FACTOR = 2.0;
	private static Random random;
	
	private static  int randomSeed = 1;
	private static final int NUM_ODR_CORES = 2;
	private static final int NUM_LOCKSTEP_CORES = 1;
	private static ArrayList<FaultMechanism> ftms;
	static Mapper mapper;
	


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
		longopts[0] = new LongOpt("mintasknum", LongOpt.REQUIRED_ARGUMENT,
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
		
		ftms = new ArrayList<>();
		
		if(numLockstepCores > 0){
			ftms.add(new Lockstep());
		}
		
		if(numOdrCores >= 2){
			ftms.add(new DMR());
			ftms.add(new PassiveReplication());
		} 
		if(numOdrCores >= 3){
			ftms.add(new TMR());
		}
		
		 
		random = new Random(randomSeed);
		
		
		
		Application app = Application.generateRandomApplication(minNumTasks, minHiPercent, avgDefaultUtil,
				numOdrCores, numLockstepCores, maxWcetFactor,random);
		

		mapper = new Mapper();
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
		if(mapper.getBestSchedule() == null){
			System.out.println("no schedule found");
			System.exit(0);
		}
		System.out.println("**********************************");
		System.out.println(mapper.getBestSchedule());
		System.out.println("**********************************");
		System.out.println(mapper.getBestFTMs());
		System.out.println("**********************************");
		System.out.println(mapper.getBestScheduleFitness());
		System.out.println("**********************************");
		double[] qos = mapper.getBestSchedule().getQosPerMode();
		System.out.println("qos [LO,TF,OV,HI]: ");
		for(int i = 0; i < qos.length; i++){
			System.out.println(qos[i] + ", ");
		}
	}



	
}
