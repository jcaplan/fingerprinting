package codegen.test;

import java.util.ArrayList;
import java.util.Random;
import java.util.Set;

import codegen.map.*;
import codegen.util.Logger;

public class MapTest {

	private static final int randomSeed = 1;
	private static final double percentHI = 0.5;
	private static Random generator = new Random(randomSeed);
	private static final int numOdrCores = 4;
	private static final int numLockstepCores = 1;
	
	static Mapper mapper;
	
	public static Application generateRandomApplication(double pHI){

		Application app = new Application();
		for(int i = 0; i < 10; i++){
			
			double Clo = 100*generator.nextDouble();
			double Chi = Clo*1.5;
			double period = Chi*(4 + 2*generator.nextDouble());
			boolean criticality = (generator.nextDouble() > pHI);
			
			
			app.addTask(new Task(Clo,Chi,period,criticality));
		}
		
		return app;
	}

	private static void addProcessors(int numOdrCore, int numLockstepCore) {
		ArrayList<Processor> pList = new ArrayList<>();
		
		for(int i = 0; i < numOdrCore; i++){
			pList.add(new Processor("cpu" + i, Processor.OdrType));	
		}
		for(int i = 0; i < numLockstepCore; i++){
			pList.add(new Processor("cpuM" + i,Processor.lockstepType));
		}
		
		mapper.setProcList(pList);
	}
	
	

	public static void main(String[] args){
		
		Logger.turnLoggingOn(); 
		mapper = new Mapper();
		Application app = generateRandomApplication(percentHI);
		mapper.setApplication(app);
		mapper.printApp();
		
		addProcessors(numOdrCores,numLockstepCores);
		mapper.printProcessors();

		mapper.findSchedule();
	}



	
}
