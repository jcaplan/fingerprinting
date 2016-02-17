package codegen.test;

import java.io.*;
import java.util.*;

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
	
	
	public static void main(String[] args) throws FileNotFoundException, IOException, ClassNotFoundException{
		
		ObjectInputStream ois = new ObjectInputStream(new FileInputStream("app.b"));
		Application app = (Application) ois.readObject();
		ois.close();
		

		ArrayList<Processor> fpList = addProcessors(2,1);
		Map<Task, int[]> executionProfiles = new HashMap<>();
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
				executionProfiles.put(t,loProfile);
				loList.add(t);
			}
		}
		
		int numRows = (int)Math.pow(2, hiList.size());
		int numCols = (int)Math.pow(3, loList.size());
		int numIter = numRows*numCols;
		
		double[][] results = new double[numRows][numCols];
		
		int count = 0;
		int percent = 0;
		
		for(int i = 0; i < Math.pow(2, hiList.size()); i++){
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
					sched.allocate(loList.get(k), fpList.get(((int)(j / Math.pow(3, k)) % 3)));
				}
				SchedAnalysis sa = new SchedAnalysis(fullTaskList, sched, fpList, executionProfiles);
				if(sa.schedAnalysis()){
					results[i][j] = sa.qosAnalysis();
					
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
		}
		

		PrintStream ps = new PrintStream("results.txt");
		for(int i = 0; i < numRows; i++){
			for(int j = 0; j < numCols; j++){
				ps.print(results[i][j] + ",");
			}
			ps.println("\n");
		}
		ps.close();
	}

}
