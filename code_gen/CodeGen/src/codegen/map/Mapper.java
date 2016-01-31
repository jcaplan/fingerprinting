package codegen.map;

import java.util.*;

import codegen.util.Logger;

public abstract class Mapper {
	
	protected Schedule bestSchedule;
	protected double fitness;
	protected Map<Task, FaultMechanism> bestTechniqueMap;
	List<FaultMechanism> ftms;
	protected Application app;
	protected ArrayList<Processor> procList;
	
	
	public abstract void findSchedule();
	
	public Mapper(){
		app = new Application();
		procList = new ArrayList<>();
		ftms = new ArrayList<FaultMechanism>();
		initDefaultMechs();
	}
	
	
	public Mapper(Application app, ArrayList<Processor> procList){
		this.app = app;
		this.procList = procList;
	}
	
	public void setApplication(Application app){
		this.app = app;
	}

	public void setProcList(ArrayList<Processor> pList){
		this.procList = pList;
	}
	
	public void printApp() {
		Logger.logMessage(app);
		
	}

	public void printProcessors() {
		for(Processor p : procList){
			Logger.logMessage(p);
		} 
		
	}
	
	protected void initDefaultMechs() {
		ftms.add(new DMR());
		ftms.add(new TMR());
		ftms.add(new PassiveReplication());
		ftms.add(new Lockstep());
	}

	public Application getApplication(){
		return app;
	}
	
	public ArrayList<Processor> getProcList(){
		return procList;
	}
	
	public Schedule getBestSchedule(){
		return bestSchedule;
	}
	
	public double getBestScheduleFitness(){
		return fitness;
	}
	
	public Map<Task, FaultMechanism> getBestFTMs(){
		return bestTechniqueMap;
	}

	public void clearFTMs() {
		ftms.clear();
	}

	public void addFTM(FaultMechanism mech) {
		if(!ftms.contains(mech)){
			ftms.add(mech);
		}
	}

	public List<FaultMechanism> getFtmList() {
		return ftms;
	}
	

	public void setFTMS(ArrayList<FaultMechanism> ftms) {
		this.ftms = ftms;
	}
	

}