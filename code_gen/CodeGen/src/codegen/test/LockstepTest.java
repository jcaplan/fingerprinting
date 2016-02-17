package codegen.test;

import java.io.*;
import java.util.*;

import codegen.map.*;

public class LockstepTest {

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
		Application[] appList = (Application[]) ois.readObject();
		ois.close();
		

		ArrayList<Processor> fpList = addProcessors(0,2);

		ArrayList<FaultMechanism> fp = new ArrayList<>();
		fp.add(new Lockstep());
		
		for(Application app : appList){
			GAMapper mapper = new GAMapper();
			mapper.setApplication(app);
			mapper.setProcList(fpList);
			mapper.setFTMS(fp);
			mapper.findSchedule();
			Schedule lsSched = mapper.getBestSchedule();
			
			if(lsSched != null){
				double[] qos = lsSched.getQosPerMode();
				System.out.println("ls qos [LO,TF,OV,HI]: ");
				for(int j = 0; j < qos.length; j++){
					System.out.println(qos[j] + ", ");
					
				}
				System.out.println(lsSched);
			} else {
				System.out.println("0,0,0,0");
			}
		}
	}
}
