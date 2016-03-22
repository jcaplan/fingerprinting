package codegen.test;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.PrintStream;
import java.text.DecimalFormat;
import java.util.ArrayList;

import codegen.map.Application;
import codegen.map.DMR;
import codegen.map.FaultMechanism;
import codegen.map.GAMapper;
import codegen.map.Lockstep;
import codegen.map.Mapper;
import codegen.map.MultiThreadGABreeder;
import codegen.map.Processor;
import codegen.map.Schedule;

public class EightCoreTest {
	private static final int MIN_NUM_TASKS = 10;
	private static final double MIN_PERCENT_HI = 0.5;
	private static final double AVERAGE_DEFAULT_UTILIZATION = 0.8;
	private static final double MAX_WCET_FACTOR = 2.0;
	private static final int NUM_ITERATIONS = 1000;

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
	
	public static void main(String[] args) throws FileNotFoundException, IOException, ClassNotFoundException{
		
		int iter = NUM_ITERATIONS;
		double upperBound = 0.85;
//		if(args.length > 0 && args[0].equals("-gen")){	
//			(new GenerateOdrApps(2,10,8,80,0.85)).generate();
//		}
		
		
		MultiThreadGABreeder.NUM_THREADS = 20;

		//Set up platforms
		ArrayList<Processor> lsList = addProcessors(0,4);
		ArrayList<Processor> odrList = addProcessors(8,0);
		ArrayList<Processor> fpList = addProcessors(4,2);
		ArrayList<FaultMechanism> ls = new ArrayList<>();
		ls.add(new Lockstep());
		ArrayList<FaultMechanism> odr = new ArrayList<>();
		odr.add(new DMR());
		ArrayList<FaultMechanism> fp = new ArrayList<>();
		fp.add(new DMR());
		fp.add(new Lockstep());
		

		double[][] sched = new double[3][10];
		double[][][] qosAvg = new double [3][10][4];
		int LS = 0;
		int ODR = 1;
		int FP = 2;
		
		int count = 0;
		
		PrintStream schedPS = new PrintStream("util_sched.csv");
		PrintStream utilPS = new PrintStream("util_avg.csv");
		schedPS.println("util,ls,fp,odr");
		
		for(double util = 0.50; util <= upperBound + 0.009; util+=0.05){
			//Load the apps from the file
			String utilString = (new DecimalFormat("0.00").format(util));
//			String fileName = "tasksets/util_" + utilString + ".b";
//			Application[] appList = null;
//			ObjectInputStream is = new ObjectInputStream(new FileInputStream(fileName));
//			appList = (Application[]) is.readObject();
//			is.close();
			File lsResults = new File("util_ls_" + utilString + ".csv");
			File odrResults = new File("util_odr_" + utilString + ".csv");
			File fpResults = new File("util_fp_" + utilString + ".csv");
			PrintStream lsPS = new PrintStream(lsResults);
			PrintStream odrPS = new PrintStream(odrResults);
			PrintStream fpPS = new PrintStream(fpResults);

			int successfulIterations = 0;
			//Start test on each app
			for(int i = 0; i < iter; i++){
				Application app = Application.generateRandomApplication2(40, 0.5, util, 4,
						2);
				mapper = new GAMapper();
				mapper.setApplication(app);
				mapper.setProcList(lsList);
				mapper.setFTMS(ls);
				mapper.findSchedule();	
				
				
				Schedule lsSched = mapper.getBestSchedule();
				if(lsSched != null){
					sched[LS][count]++;
					System.out.println("ls scheduled");
				} else {
					System.out.println("lockstep failed");
					continue;
				}
				
				mapper = new GAMapper();
				mapper.setApplication(app);
				mapper.setProcList(odrList);
				mapper.setFTMS(odr);
				mapper.findSchedule();
				Schedule odrSched = mapper.getBestSchedule();
				if(odrSched != null){
					sched[ODR][count]++;
					System.out.println("odr scheduled");
				} else {
					System.out.println("odr failed");
					continue;
				}
				
				mapper = new GAMapper();
				mapper.setApplication(app);
				mapper.setProcList(fpList);
				mapper.setFTMS(fp);
				mapper.findSchedule();
				Schedule fpSched = mapper.getBestSchedule();
				if(fpSched != null){
					sched[FP][count]++;
					System.out.println("fp scheduled");
				} else {
					System.out.println("fp failed");
					continue;
				}
				
				boolean fail = (lsSched == null) || (odrSched == null) || (fpSched == null);
				if(!fail){
					successfulIterations++;
					double[] qos = lsSched.getQosPerMode();
					System.out.println("ls qos [LO,TF,OV,HI]: ");
					for(int j = 0; j < qos.length; j++){
						System.out.println(qos[j] + ", ");
						lsPS.print(qos[j] + ",");
						qosAvg[LS][count][j] += qos[j];
					}
					lsPS.println("");

					qos = odrSched.getQosPerMode();
					System.out.println("odr qos [LO,TF,OV,HI]: ");
					for(int j = 0; j < qos.length; j++){
						System.out.println(qos[j] + ", ");
						odrPS.print(qos[j] + ",");
						qosAvg[ODR][count][j] += qos[j];
					}
					odrPS.println("");

					qos = fpSched.getQosPerMode();
					System.out.println("fp qos [LO,TF,OV,HI]: ");
					for(int j = 0; j < qos.length; j++){
						System.out.println(qos[j] + ", ");
						fpPS.print(qos[j] + ",");
						qosAvg[FP][count][j] += qos[j];
					}
					fpPS.println("");
					
				}
			
			}

			
			sched[LS][count] /= iter;
			sched[ODR][count] /= iter;
			sched[FP][count] /= iter;
			
			fpPS.println("schedulability: " + sched[FP][count]);
			odrPS.println("schedulability: " + sched[ODR][count]);
			lsPS.println("schedulability: " + sched[LS][count]);
			
			fpPS.close();
			odrPS.close();
			lsPS.close();

			System.out.println("schedulability LS: " + sched[LS][count]);
			System.out.println("schedulability FP: " + sched[FP][count]);
			System.out.println("schedulability ODR: " + sched[ODR][count]);
			
			schedPS.println(String.format("%s,%f,%f,%f",utilString,sched[LS][count],sched[FP][count],sched[ODR][count]));
			String s = utilString + ",";
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 4; j++){
					s += qosAvg[i][count][j] / successfulIterations + ",";
				}
			}
			utilPS.println(s);
			count++;
		}
		
		schedPS.close();
		utilPS.close();
		
	}
}
