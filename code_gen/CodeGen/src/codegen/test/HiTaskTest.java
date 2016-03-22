package codegen.test;

import java.io.*;
import java.text.DecimalFormat;
import java.util.ArrayList;

import codegen.map.*;

public class HiTaskTest {

	private static final int MIN_NUM_TASKS = 10;
	private static final double MIN_PERCENT_HI = 0.5;
	private static final double AVERAGE_DEFAULT_hitaskIZATION = 0.8;
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

		
		
		MultiThreadGABreeder.NUM_THREADS = 20;

		//Set up platforms
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
		

		double[][] sched = new double[3][10];
		double[][][] qosAvg = new double [3][10][4];
		int LS = 0;
		int ODR = 1;
		int FP = 2;
		
		int count = 0;
		
		PrintStream schedPS = new PrintStream("hitask_sched.csv");
		PrintStream hitaskPS = new PrintStream("hitask_avg.csv");
		schedPS.println("hitask,ls,fp,odr");
		
		for(double hitask = 0.2; hitask < 0.9; hitask+=0.1){
			String hitaskString = (new DecimalFormat("0.00").format(hitask));
			File lsResults = new File("hitask_ls_" + hitaskString + ".csv");
			File odrResults = new File("hitask_odr_" + hitaskString + ".csv");
			File fpResults = new File("hitask_fp_" + hitaskString + ".csv");
			PrintStream lsPS = new PrintStream(lsResults);
			PrintStream odrPS = new PrintStream(odrResults);
			PrintStream fpPS = new PrintStream(fpResults);

			int successfulIterations = 0;
			//Start test on each app
			for(int i = 0; i < iter; i++){
				Application app = Application.generateRandomApplication2(20, hitask, 0.7, 2,
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
			
			schedPS.println(String.format("%s,%f,%f,%f",hitaskString,sched[LS][count],sched[FP][count],sched[ODR][count]));
			String s = hitaskString + ",";
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 4; j++){
					s += qosAvg[i][count][j] / successfulIterations + ",";
				}
			}
			hitaskPS.println(s);
			count++;
		}
		
		schedPS.close();
		hitaskPS.close();
		
	}
}
