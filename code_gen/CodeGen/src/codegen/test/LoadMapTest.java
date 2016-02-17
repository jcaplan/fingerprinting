package codegen.test;

import java.io.*;
import java.util.*;

import codegen.map.*;

public class LoadMapTest {

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

		ArrayList<FaultMechanism> fp = new ArrayList<>();
		fp.add(new DMR());
		fp.add(new Lockstep());
		
		
		Heur2Mapper mapper = new Heur2Mapper();
		mapper.setApplication(app);
		mapper.setProcList(fpList);
		mapper.setFTMS(fp);
		mapper.findSchedule();
		
		PrintStream os = new PrintStream("heurResults.txt");
		double[] results = mapper.getResults();
		for(int i = 0; i < results.length; i++){
			os.println(results[i]);
		}
		
	}
}
