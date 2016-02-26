package codegen.test;

import java.io.*;
import java.text.DecimalFormat;
import java.util.ArrayList;

import codegen.map.*;

public class TechniqueTest {

	private static final int NUM_ITERATIONS = 10;

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
		if(args.length > 0 && args[0].equals("-gen")){	
			(new GenerateOdrApps(1.5,iter)).generate();
		}
		
		long time = System.currentTimeMillis();
		
		MultiThreadGABreeder.NUM_THREADS = 20;

		//Set up platforms
		ArrayList<Processor> odrList = addProcessors(4,0);
		ArrayList<Processor> dmrList = addProcessors(4,0);
		ArrayList<FaultMechanism> odr = new ArrayList<>();
		odr.add(new DMR());
		odr.add(new TMR());
		odr.add(new PassiveReplication());
		ArrayList<FaultMechanism> dmr = new ArrayList<>();
		dmr.add(new DMR());
		

		double[][] sched = new double[2][10];
		double[][][] qosAvg = new double [2][10][4];
		int DMR = 0;
		int ODR = 1;
		
		int count = 0;
		
		PrintStream schedPS = new PrintStream("util_sched.csv");
		PrintStream utilPS = new PrintStream("util_avg.csv");
		schedPS.println("util,dmr,odr");
		
		for(double util = 0.5; util < 0.85; util+=0.05){
			//Load the apps from the file
			String utilString = (new DecimalFormat("0.00").format(util));
			String fileName = "tasksets/util_" + utilString + ".b";
			Application[] appList = null;
			ObjectInputStream is = new ObjectInputStream(new FileInputStream(fileName));
			appList = (Application[]) is.readObject();
			is.close();
			File odrResults = new File("util_odr_" + utilString + ".csv");
			File dmrResults = new File("util_dmr_" + utilString + ".csv");
			PrintStream odrPS = new PrintStream(odrResults);
			PrintStream dmrPS = new PrintStream(dmrResults);

			int successfulIterations = 0;
			//Start test on each app
			for(int i = 0; i < iter; i++){
				Application app = appList[i];
				
				mapper = new GAMapper();
				mapper.setApplication(app);
				mapper.setProcList(dmrList);
				mapper.setFTMS(dmr);
				mapper.findSchedule();
				Schedule dmrSched = mapper.getBestSchedule();
				if(dmrSched != null){
					sched[DMR][count]++;
					System.out.println("dmr scheduled");
				} else {
					System.out.println("dmr failed");
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
				}
				
				
				double[] qos;
				boolean failed = (odrSched == null || dmrSched == null);
				if(!failed){

					successfulIterations++;
					qos = odrSched.getQosPerMode();
					System.out.println(mapper.getBestFTMs());
					System.out.println("odr qos [LO,TF,OV,HI]: ");
					for(int j = 0; j < qos.length; j++){
						System.out.println(qos[j] + ", ");
						odrPS.print(qos[j] + ",");
						qosAvg[ODR][count][j] += qos[j];
					}
					odrPS.println("");
	
					qos = dmrSched.getQosPerMode();
					System.out.println("dmr qos [LO,TF,OV,HI]: ");
					for(int j = 0; j < qos.length; j++){
						System.out.println(qos[j] + ", ");
						dmrPS.print(qos[j] + ",");
						qosAvg[DMR][count][j] += qos[j];
					}
					dmrPS.println("");				
				}
			}

			
			sched[ODR][count] /= iter;
			sched[DMR][count] /= iter;
			
			dmrPS.println("schedulability: " + sched[DMR][count]);
			odrPS.println("schedulability: " + sched[ODR][count]);
			
			dmrPS.close();
			odrPS.close();
			
			System.out.println("schedulability DMR: " + sched[DMR][count]);
			System.out.println("schedulability ODR: " + sched[ODR][count]);
			
			schedPS.println(String.format("%s,%f,%f",utilString,sched[DMR][count],sched[ODR][count]));
			String s = utilString + ",";
			for(int i = 0; i < 2; i++){
				for(int j = 0; j < 4; j++){
					s += qosAvg[i][count][j] / successfulIterations + ",";
				}
			}
			utilPS.println(s);
			count++;
		}
		
		schedPS.close();
		utilPS.close();
		
		time = System.currentTimeMillis() - time;
		long seconds = time/1000;
		System.out.println("runtime: " + seconds + "seconds (" + seconds/60.0 + ") minutes");
	}
}
