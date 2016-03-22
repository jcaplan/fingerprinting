package codegen.test;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.text.DecimalFormat;
import java.util.ArrayList;

import org.apache.commons.io.FileUtils;

import codegen.map.*;

public class GenerateOdrApps {
	int pcount = 0;
	double odrFactor;
	ArrayList<Processor> fpList;
	ArrayList<FaultMechanism> mecList;
	int numApps;
	private int numOdrProcessors;
	int numTasks;
	double utilUpperBound;
	
	public GenerateOdrApps(){
		odrFactor = 1.5;
		numApps = 1000;
		numOdrProcessors = 4;
		numTasks = 20;
		utilUpperBound = 0.95;
	}
	
	public GenerateOdrApps(double odrFactor, int numApps, int numOdrProcessors, int numTasks,
			double utilUpperBound){
		this.odrFactor = odrFactor;
		this.numApps = numApps;
		this.numOdrProcessors = numOdrProcessors;
		this.numTasks = numTasks;
		this.utilUpperBound = utilUpperBound;
	}
	
	public void generate(){

		File outputDir = new File("tasksets");
		try {
			FileUtils.forceMkdir(outputDir);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		
		fpList = addProcessors(numOdrProcessors, 0);
		mecList = new ArrayList<>();
		mecList.add(new DMR());
		GenThread[] threads = new GenThread[10];
		int count = 0;
		for (double i = 0.5; i <= utilUpperBound + 0.009; i += 0.05) {
			System.out.println("Starting gen thread " + i);
			threads[count] = new GenThread(i);
			threads[count].start();
			count++;
		}
		for (int i = 0; i < (int)((utilUpperBound - 0.5) / 0.05); i++){
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	private ArrayList<Processor> addProcessors(int numOdrCore,
			int numLockstepCore) {
		ArrayList<Processor> pList = new ArrayList<>();

		for (int i = 0; i < numOdrCore; i++) {
			pList.add(new Processor("cpu" + pcount++, Processor.OdrType));
		}
		for (int i = 0; i < numLockstepCore; i++) {
			pList.add(new Processor("cpuM" + pcount++, Processor.lockstepType));
		}

		return pList;
	}


	
	private class GenThread extends Thread{ 
		
		double util;
		
		public GenThread(double util){
			this.util = util;
		}
		
		public void run(){
			Application[] appList = new Application[1000];
			int count = 0;
			Application app;
			String utilString = (new DecimalFormat("0.00").format(util));
			while (count < numApps) {
				app = Application.generateRandomApplication(numTasks, 0.5, util, numOdrProcessors, 0,
						2,odrFactor);

				Mapper mapper = new HeurMapper();
				mapper.setApplication(app);
				mapper.setProcList(fpList);
				mapper.setFTMS(mecList);
				mapper.findSchedule();
				if (mapper.getBestSchedule() != null) {
					appList[count] = app;
					count++;
					if(count % 10 == 0){
						System.out.println("u" + utilString + ": " + count/10  + "%");						
					}
				}
			}
			ObjectOutputStream oos;
			try {
				oos = new ObjectOutputStream(
						new FileOutputStream("tasksets/util_" + utilString + ".b"));
				oos.writeObject(appList);

				oos.close();
			} catch (IOException e) {
				e.printStackTrace();
			}

			
		}
		
		
	}
	
	
	public static void main(String[] args) throws FileNotFoundException,
			IOException, ClassNotFoundException {
		GenerateOdrApps gen = new GenerateOdrApps(1.5,10,4,20,0.80);
		gen.generate();
	}
}
