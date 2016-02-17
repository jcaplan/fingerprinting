package codegen.test;

import gnu.getopt.Getopt;
import gnu.getopt.LongOpt;

import java.io.File;
import java.io.FileInputStream;
import java.io.ObjectInputStream;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Random;

import codegen.map.Application;
import codegen.map.DMR;
import codegen.map.FaultMechanism;
import codegen.map.GAMapper;
import codegen.map.Lockstep;
import codegen.map.Mapper;
import codegen.map.MultiThreadGABreeder;
import codegen.map.Processor;
import codegen.map.Schedule;
import codegen.util.Logger;

public class PlatformTest {

	private static final int MIN_NUM_TASKS = 10;
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
		String appFile = null;
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
		longopts[8] = new LongOpt("appfile", LongOpt.REQUIRED_ARGUMENT, null, 8);
		
		
		
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
			case 8:
				appFile = g.getOptarg();
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
		ArrayList<Processor> odrList = addProcessors(4,0);
		ArrayList<Processor> fpList = addProcessors(2,1);
		ArrayList<FaultMechanism> ls = new ArrayList<>();
		ls.add(new Lockstep());
		ArrayList<FaultMechanism> odr = new ArrayList<>();
		odr.add(new DMR());
		ArrayList<FaultMechanism> fp = new ArrayList<>();
		fp.add(new DMR());
		fp.add(new Lockstep());
		
		
		double[] sched = new double[3];
		int LS = 0;
		int ODR = 1;
		int FP = 2;
		
		// If an application binary is provided then use those applications
		Application[] appList = null;
		if(appFile != null){
			ObjectInputStream is = new ObjectInputStream(new FileInputStream(appFile));
			appList = (Application[]) is.readObject();
			is.close();
			if(iter > appList.length){
				throw new RuntimeException("Iterations set to " + iter + " but application array provided "
						+ " in file " + appFile + " only has length " + appList.length);
			}
		}
				
		
		File lsResults = new File("lsResults.csv");
		File odrResults = new File("odrResults.csv");
		File fpResults = new File("fpResults.csv");
		PrintStream lsPS = new PrintStream(lsResults);
		PrintStream odrPS = new PrintStream(odrResults);
		PrintStream fpPS = new PrintStream(fpResults);
		
		
		for(int i = 0; i < iter; i++){
			Application app;
			if(appList != null){
				app = appList[iter];
			} else {
				app = Application.generateRandomApplication(minNumTasks, minHiPercent, avgDefaultUtil,
					2, 1, maxWcetFactor,random);
			}

			mapper = new GAMapper();
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
			
			mapper = new GAMapper();
			mapper.setApplication(app);
			mapper.setProcList(odrList);
			mapper.setFTMS(odr);
			mapper.findSchedule();
			Schedule odrSched = mapper.getBestSchedule();
			if(odrSched != null){
				sched[ODR]++;
				System.out.println("odr scheduled");
			} else {
				System.out.println("odr failed");
			}
			
			mapper = new GAMapper();
			mapper.setApplication(app);
			mapper.setProcList(fpList);
			mapper.setFTMS(fp);
			mapper.findSchedule();
			Schedule fpSched = mapper.getBestSchedule();
			if(fpSched != null){
				sched[FP]++;
				System.out.println("fp scheduled");
			} else {
				System.out.println("fp failed");
			}
			
			boolean success = (lsSched != null) || (odrSched != null) || (fpSched != null);
			if(lsSched != null){
				double[] qos = lsSched.getQosPerMode();
				System.out.println("ls qos [LO,TF,OV,HI]: ");
				for(int j = 0; j < qos.length; j++){
					System.out.println(qos[j] + ", ");
					lsPS.print(qos[j] + ",");
				}
				lsPS.println("");
				System.out.println(lsSched);
			} else if(success){
				lsPS.println("0,0,0,0");
			}
			if(odrSched != null){
				double[] qos = odrSched.getQosPerMode();
				System.out.println("odr qos [LO,TF,OV,HI]: ");
				for(int j = 0; j < qos.length; j++){
					System.out.println(qos[j] + ", ");
					odrPS.print(qos[j] + ",");
				}
				odrPS.println("");
				System.out.println(odrSched);
			}  else if (success) {
				odrPS.println("0,0,0,0");
			}
			if(fpSched != null){
				double[] qos = fpSched.getQosPerMode();
				System.out.println("fp qos [LO,TF,OV,HI]: ");
				for(int j = 0; j < qos.length; j++){
					System.out.println(qos[j] + ", ");
					fpPS.print(qos[j] + ",");
				}
				fpPS.println("");
				System.out.println(fpSched);
			} else if(success){
				fpPS.println("0,0,0,0");
			}
			
		}
		
		sched[LS] /= iter;
		sched[ODR] /= iter;
		sched[FP] /= iter;
		
		fpPS.println("schedulability: " + sched[FP]);
		odrPS.println("schedulability: " + sched[ODR]);
		lsPS.println("schedulability: " + sched[LS]);
		
		fpPS.close();
		odrPS.close();
		lsPS.close();
		
		System.out.println("schedulability LS: " + sched[LS]);
		System.out.println("schedulability FP: " + sched[FP]);
		System.out.println("schedulability ODR: " + sched[ODR]);
	}



	
}
