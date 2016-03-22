package codegen.test;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.text.DecimalFormat;
import java.util.ArrayList;

import org.apache.commons.io.FileUtils;

import codegen.map.Application;
import codegen.map.FaultMechanism;
import codegen.map.HeurMapper;
import codegen.map.Lockstep;
import codegen.map.Mapper;
import codegen.map.Processor;

public class GenerateApps {

	static int pcount = 0;

	static ArrayList<Processor> fpList;
	static ArrayList<FaultMechanism> mecList;
	
	public void generate(){

		File outputDir = new File("tasksets");
		try {
			FileUtils.forceMkdir(outputDir);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		
		fpList = addProcessors(0, 2);
		mecList = new ArrayList<>();
		mecList.add(new Lockstep());
		GenThread[] threads = new GenThread[10];
		int count = 0;
		for (double i = 0.5; i < 1; i += 0.05) {
			threads[count] = new GenThread(i);
			threads[count].start();
			count++;
		}
		for (int i = 0; i < 10; i++){
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	private static ArrayList<Processor> addProcessors(int numOdrCore,
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

	
	private static class GenThread extends Thread{ 
		
		double util;
		
		public GenThread(double util){
			this.util = util;
		}
		
		public void run(){
			Application[] appList = new Application[1000];
			int count = 0;
			Application app;
			String utilString = (new DecimalFormat("0.00").format(util));
			while (count < 1000) {
				app = Application.generateRandomApplication(20, 0.5, util, 2, 1,
						2);

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
		GenerateApps gen = new GenerateApps();
		gen.generate();
	}

	public void generateHiTasks() {

		File outputDir = new File("tasksets");
		try {
			FileUtils.forceMkdir(outputDir);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		
		fpList = addProcessors(0, 2);
		mecList = new ArrayList<>();
		mecList.add(new Lockstep());
		GenHiTaskThread[] threads = new GenHiTaskThread[10];
		int count = 0;
		for (double i = 0.2; i < 0.9; i += 0.1) {
			threads[count] = new GenHiTaskThread(i);
			threads[count].start();
			count++;
		}
		for (int i = 0; i < 7; i++){
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	private static class GenHiTaskThread extends Thread{ 
		
		double hitask;
		
		public GenHiTaskThread(double hitask){
			this.hitask = hitask;
		}
		
		public void run(){
			Application[] appList = new Application[1000];
			int count = 0;
			Application app;
			String hitaskString = (new DecimalFormat("0.00").format(hitask));
			while (count < 1000) {
				app = Application.generateRandomApplication(20, hitask, 0.7, 2, 1,
						2);

				Mapper mapper = new HeurMapper();
				mapper.setApplication(app);
				mapper.setProcList(fpList);
				mapper.setFTMS(mecList);
				mapper.findSchedule();
				if (mapper.getBestSchedule() != null) {
					appList[count] = app;
					count++;
					if(count % 10 == 0){
						System.out.println("hi:" + hitaskString + ": " + count/10  + "%");						
					}
				}
			}
			ObjectOutputStream oos;
			try {
				oos = new ObjectOutputStream(
						new FileOutputStream("tasksets/hitask_" + hitaskString + ".b"));
				oos.writeObject(appList);

				oos.close();
			} catch (IOException e) {
				e.printStackTrace();
			}

			
		}
		
		
	}

}
