package codegen.test;

import gnu.getopt.Getopt;
import gnu.getopt.LongOpt;

import java.util.ArrayList;
import java.util.Random;


import codegen.map.Application;
import codegen.map.DMR;
import codegen.map.FaultMechanism;
import codegen.map.Lockstep;
import codegen.map.Mapper;
import codegen.map.MultiThreadGABreeder;
import codegen.map.Processor;
import codegen.map.Schedule;
import codegen.util.Logger;

public class PlatformTest {

	private static final int MIN_NUM_TASKS = 20;
	private static final double MIN_PERCENT_HI = 0.5;
	private static final double AVERAGE_DEFAULT_UTILIZATION = 0.8;
	private static final double MAX_WCET_FACTOR = 2.0;
	private static final int NUM_ITERATIONS = 100;
	private static Random random;
	
	private static  int randomSeed = 2;
	static Mapper mapper;
	
	static int pcount = 0;

	private static ArrayList<Processor> addProcessors(int numOdrCore, int numLockstepCore) {
		ArrayList<Processor> pList = new ArrayList<>();
		
		for(int i = 0; i < numOdrCore; i++){
			pList.add(new Processor("cpu" + pcount++, Processor.OdrType));	
		}
		for(int i = 0; i < numLockstepCore; i++){
			pList.add(new Processor("cpuM" + pcount++,Processor.lockstepType));
		}
		
		return pList;
	}
	
	

	public static void main(String[] args) throws Exception{
		
		int minNumTasks = MIN_NUM_TASKS; 
		double minHiPercent = MIN_PERCENT_HI;
		double avgDefaultUtil = AVERAGE_DEFAULT_UTILIZATION; 
		double maxWcetFactor = MAX_WCET_FACTOR;
		int iter = NUM_ITERATIONS;
		boolean log = false;
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
		longopts[7] = new LongOpt("log", LongOpt.NO_ARGUMENT, null,7);
		
		
		
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
			case 7:
				log = true;
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
		if(log){
			Logger.turnLoggingOn();  
		}
		random = new Random(randomSeed);
		
		ArrayList<Processor> lsList = addProcessors(0,2);
		ArrayList<Processor> fpList = addProcessors(2,1);

		ArrayList<FaultMechanism> ls = new ArrayList<>();
		ls.add(new Lockstep());
		ArrayList<FaultMechanism> odr = new ArrayList<>();
		odr.add(new DMR());
		odr.add(new Lockstep());
		
		
		double[] sched = new double[2];
		int LS = 0;
		int FP = 1;
		for(int i = 0; i < iter; i++){
			Application app = Application.generateRandomApplication(minNumTasks, minHiPercent, avgDefaultUtil,
					2, 1, maxWcetFactor,random);
			

			mapper = new Mapper();
			mapper.setApplication(app);
			mapper.setProcList(lsList);
			mapper.setFTMS(ls);
			mapper.findSchedule();
			
			Schedule lsSched = mapper.getBestSchedule();
			if(lsSched != null){
				sched[LS]++;
				System.out.println("ls scheduled");
			} else {
				System.out.println("lockstep failed");
			}
			
			mapper = new Mapper();
			mapper.setApplication(app);
			mapper.setProcList(fpList);
			mapper.setFTMS(odr);
			mapper.findSchedule();
			Schedule fpSched = mapper.getBestSchedule();
			if(fpSched != null){
				sched[FP]++;
				fpSched = mapper.getBestSchedule();
				System.out.println("odr scheduled");
			} else {
				System.out.println("odr failed");
			}
			
			if(lsSched != null){
				double[] qos = lsSched.getQosPerMode();
				System.out.println("ls qos [LO,TF,OV,HI]: ");
				for(int j = 0; j < qos.length; j++){
					System.out.println(qos[j] + ", ");
				}
				System.out.println(lsSched);
			}
			if(fpSched != null){
				double[] qos = fpSched.getQosPerMode();
				System.out.println("fp qos [LO,TF,OV,HI]: ");
				for(int j = 0; j < qos.length; j++){
					System.out.println(qos[j] + ", ");
				}
				System.out.println(fpSched);
			}	
		}
		sched[LS] /= iter;
		sched[FP] /= iter;
		
		System.out.println("schedulability LS: " + sched[LS] + "FP: " + sched[FP]);
	}



	
}
