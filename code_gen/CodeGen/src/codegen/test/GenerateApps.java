package codegen.test;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;

import codegen.map.Application;
import codegen.map.DMR;
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
		
		
		boolean success = false;
		while(!success){
			app = Application.generateRandomApplication(10, 0.5, 0.8, 2, 1, 2);
		
			
			
			ArrayList<Processor> fpList = addProcessors(2,1);
			ArrayList<FaultMechanism> fp = new ArrayList<>();
			fp.add(new DMR());
			fp.add(new Lockstep());
			
			
			Mapper mapper = new HeurMapper();
			mapper.setApplication(app);
			mapper.setProcList(fpList);
			mapper.setFTMS(fp);
			mapper.findSchedule();
			if(mapper.getBestSchedule() != null){
				success = true;
			}
		}
		ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("app.b"));
		
		System.out.println(app);
		oos.writeObject(app);
		
		
		oos.close();
	
	
	}
}
