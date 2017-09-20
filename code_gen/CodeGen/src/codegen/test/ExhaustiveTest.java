package codegen.test;

import java.io.*;
import java.util.*;

import com.sun.corba.se.spi.extension.ZeroPortPolicy;

import codegen.map.*;

public class ExhaustiveTest {

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
	

	private static final int MIN_NUM_TASKS = 10;
	private static final double MIN_PERCENT_HI = 0.5;
	private static final double AVERAGE_DEFAULT_UTILIZATION = 0.6;
	private static final double MAX_WCET_FACTOR = 2.0;

	private static int zeroCount = 0;

	public static void main(String[] args) throws FileNotFoundException, IOException, ClassNotFoundException{
		double sum = 0;
		int count = 0;
		for(int i = 0; i < 100; i++) {

			double result = gaMatchesExhaustive();
			if (result > 0) {
				sum += result;
				count++;
			}

		System.out.println("zero count: " + zeroCount);
		System.out.println("average deviation: " + sum / count);
		System.out.println("matching: " + (i + 1 - count - zeroCount));

		}
	}

	public static double gaMatchesExhaustive() {
		Random random = new Random();

		Application app = Application.generateRandomApplication(MIN_NUM_TASKS, MIN_PERCENT_HI, AVERAGE_DEFAULT_UTILIZATION,
				2, 1, MAX_WCET_FACTOR,random);;
		

		ArrayList<Processor> fpList = addProcessors(2,1);
		ArrayList<FaultMechanism> fp = new ArrayList<>();
		fp.add(new DMR());
		fp.add(new Lockstep());

		ArrayList<Task> tList = app.getTaskList();
		// how many tasks are lockstep/dmr

		//build a list of HI tasks
		ArrayList<Task> hiList = new ArrayList<>();

		ArrayList<Task> loList = new ArrayList<>();
		int[] loProfile = {1,1,1,1};
		for(Task t : tList){
			if(t.isCritical()){
				hiList.add(t);
			} else {
				loList.add(t);
			}
		}

		int numRows = (int)Math.pow(2, hiList.size());
		int numCols = (int)Math.pow(3, loList.size());
		int numIter = numRows*numCols;
		
		double[][] results = new double[numRows][numCols];
		
		double max = 0;

		int count = 0;
		int percent = 0;
		
		for(int i = 0; i < Math.pow(2, hiList.size()); i++){
			Map<Task, int[]> executionProfiles = new HashMap<>();

			Schedule sched = new Schedule();
			
			// binary number -> each task is either dmr (0) or lockstep (1)
			// 2 ^ hi task allocations
			HashMap<Task,Task[]> replicas = new HashMap<>();
			int[] lsProfile = {1,2,1,2};
			for(int j = 0; j < hiList.size(); j++){
				Task t = hiList.get(j);
				if((i & (1 << j)) != 0){
					//DMR
					DMR.makeReplica(t, executionProfiles, replicas);
					//Allocate one to each processing core
					sched.allocate(t, fpList.get(0));
					sched.allocate(replicas.get(t)[0], fpList.get(1));
				} else {
					//Lockstep
					executionProfiles.put(t, lsProfile);
					sched.allocate(t, fpList.get(2));
				}
			}
			
			ArrayList<Task> fullTaskList = new ArrayList<>(tList);
			for(Task[] r : replicas.values()){
				fullTaskList.add(r[0]);
			}
			
			// Now need to cycle through all LO task allocations
			
			// Each lo task can be assigned to one of three processors
			// 3 ^ numLo 
			for(int j = 0; j < Math.pow(3,loList.size()); j++){
				for(int k = 0; k < loList.size(); k++){
					executionProfiles.put(loList.get(k), loProfile);
					sched.allocate(loList.get(k), fpList.get(((int)(j / Math.pow(3, k)) % 3)));
				}
				SchedAnalysis sa = new SchedAnalysis(fullTaskList, sched, fpList, executionProfiles);
				if(sa.schedAnalysis()){
					results[i][j] = sa.qosAnalysis();
					if (results[i][j] > max) {
						max = results[i][j];
						System.out.println("***********");
						System.out.println("i: " + i + "j: " + j);
						System.out.println(sched);
						System.out.println(max +", " + Math.abs(max - 1.0));
						if (Math.abs(max - 1.0) < 0.005) {
							break;
						}
					}
					
				} else {
					results[i][j] = 0;
				}
				
				//deallocate lo tasks
				for(Task t : loList){
					sched.deallocate(t);
				}
				count++;
				if(count == numIter / 100){
					percent++;
					count = 0;
					System.out.println(percent + "%");
				}
			}
			if (Math.abs(max - 1.0) < 0.005) break;
		}

		if (max == 0) {
			zeroCount++;
		}
		mapper = new GAMapper();
		mapper.setApplication(app);
		mapper.setProcList(fpList);
		mapper.setFTMS(fp);
		mapper.findSchedule();
		Schedule fpSched = mapper.getBestSchedule();
		double fp_max = 0;
		if(fpSched != null) {
			SchedAnalysis sa = new SchedAnalysis(fpSched.getTasks(), fpSched, fpList, fpSched.executionProfiles);
			sa.schedAnalysis();
			fp_max = sa.qosAnalysis();
		}
		System.out.println("ga: " + fp_max);
		System.out.println("exhaustive: " + max);

		return Math.abs(max - fp_max);
	}

}
