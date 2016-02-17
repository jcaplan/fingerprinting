package codegen.test;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.util.ArrayList;

import codegen.map.Application;
import codegen.map.FaultMechanism;
import codegen.map.GAMapper;
import codegen.map.Lockstep;
import codegen.map.Processor;
import codegen.map.*;

public class GenerateApps {

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
		Application app = null;
		
		Application[] appList = new Application[1000];
		int count = 0;

		
		ArrayList<Processor> procList = addProcessors(0,2);
		ArrayList<FaultMechanism> mecList = new ArrayList<>();
		mecList.add(new Lockstep());
		
		while(count < 1000){
			app = Application.generateRandomApplication(10, 0.5, 0.8, 2, 1, 2);
		
			
			
			
			Mapper mapper = new GAMapper();
			mapper.setApplication(app);
			mapper.setProcList(procList);
			mapper.setFTMS(mecList);
			mapper.findSchedule();
			if(mapper.getBestSchedule() != null){
				appList[count] = app;
				count++;
				System.out.println(count);
			}
		}
		ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("app.b"));
		
//		System.out.println(app);
		oos.writeObject(appList);
		
		
		oos.close();
	
	
	}
}
