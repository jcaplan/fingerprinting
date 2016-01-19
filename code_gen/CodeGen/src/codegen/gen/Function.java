package codegen.gen;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;

import codegen.prof.IpetAnalysis;

/**
 * Function contains all the necessary information about each function in the system.
 * @author jonah
 *
 */
public class Function implements Comparable<Function>{

	String name;
	String codeDirectory;
	int period;
	int deadline;
	boolean critical;
	ArrayList<Dataflow> dataflowList;
	ArrayList<Core> cores;
	enum Type{periodic,eventDriven};
	Type type;
	int priority;
	ArrayList<String> includeFiles;
	ArrayList<String> varDeclarations;
	ArrayList<String> initialization;
	public int stackSize;
	HashMap<Integer,StackBin> stackBin; //coreId,stackBin
	int funcID;
	
	boolean printRuntimes;
	boolean hasState;
	boolean hasDefaultParameters;
	String preambleFileName;
	public int wcetLowerBound;
	public int wcetUpperBound;
	public boolean tmr = false;
	
	/**
	 * When doing work on stack analysis, it is convenient to arrange functions in order of maximum stack size.
	 */
	static Comparator<Function> stackCompareDecreasing = new Comparator<Function>() {
         @Override
         public int compare(Function f1, Function f2) {
           return f2.stackSize - f1.stackSize;
         }
     };
     
	
     /**
      * All function properties are entered manually from higher level.
      * Only empty function constructor available.
      */
	public Function(){
		critical = false;
		dataflowList = new ArrayList<>();
		cores = new ArrayList<>();
		includeFiles = new ArrayList<>();
		varDeclarations = new ArrayList<>();
		initialization = new ArrayList<>();
		stackBin = new HashMap<>();
		hasState = false;
		hasDefaultParameters = false;
		printRuntimes = false;
		
	}

	/**
	 * Not currently implemented
	 * @param dest
	 * @param sourceVariable
	 * @param destVariable
	 */
	public void addDataflow(Function dest, String sourceVariable,
			String destVariable) {
		//Type check the dataflow...
		Dataflow df = new Dataflow(this,dest,sourceVariable,destVariable);
		dataflowList.add(df);
		
	}
	
	@Override
	public String toString(){
		return name;
	}


	/**
	 * Compare functions by default using priority. (0 is highest priority)
	 */
	@Override
	public int compareTo(Function arg0) {
		if(this.priority < arg0.priority){
			return -1;
		} else if(this.priority > arg0.priority){
			return 1;
		} else {
			return 0;
		}
	}
	
	/**
	 * Equality is simply if the names are equal.
	 * Should not have two function objects with same name in system.
	 */
	@Override
	public boolean equals(Object o){
		if(! (o instanceof Function)){
			return false;
		}
		Function name = (Function) o;
		return name.name.equals(this.name);
		
	}

	/**
	 * @return	a formatted string of the static variable declarations parsed from
	 * the source files.
	 */
	public String getVarDeclarationString() {
		String s = "";
		for(String dec : varDeclarations){
			s += dec + "\n";
		}
		return s;
		
	}

	/**
	 * 
	 * @param tab	Number of tabs for formatting output
	 * @return	String for initializing variables parsed from source file (ert_main.c)
	 */
	public String printInitialization(int tab) {
		String init = "";
		for(String s : initialization){
			for(int i = 0; i < tab; i++){
				init += "\t";
			}
			init += s + "\n";
		}
		init += "\n";
		return init;
		
	}

	/**
	 * Fingerprinted functions require a bit more work. The variables are located in structs so new
	 * pointers are required.
	 * @param tab	number of tabs for proper formatting
	 * @return	Initialization code for fingerprinted tasks.
	 */
	public String printInitializationWithStruct(int tab) {
		String init = "";
		init += "	RT_MODEL_" + this + "_T *" + this + "_M =\n"+
				"			&" + this + "PackageStruct." + this + "_STRUCT." + this + "_M;\n";
		init += "	ExtU_" + this + "_T *" + this + "_U =\n"+
				"			&" + this + "PackageStruct." + this + "_STRUCT." + this + "_U;\n";
		init += "	ExtY_" + this + "_T *" + this + "_Y =\n"+
				"			&" + this + "PackageStruct." + this + "_STRUCT." + this + "_Y;\n";

		init += "	" + name + "UpdatePointers((INT32U)&" + name + "PackageStruct" + ", " + name + "_M);\n";
		
//		init += "	" + name + "_initialize(" + name + "_M, " + name + "_U, " + name + "_Y);\n";
		init += "	" + initialization.get(initialization.size()-1);
		if(!init.endsWith("\n")){
			init += "\n";
		}

		init += "	" + name + "UpdatePointers((INT32U)&" + name + "PackageStruct" + " & 0x3FFFFF, " + name + "_M);\n";
		
		init += "\n";
		return init;
	}
	
	/**
	 * @param core	Which core for the given function (in case there are two)
	 * @return	the end address of the stack for this function on this core
	 */
	public int getStackEnd(int core) {
		return stackBin.get(core).getStackEnd(this);
	}


	/**
	 * @param core	Which core for the given function (in case there are two)
	 * @return	the start address of the stack for this function on this core
	 */
	public int getStackStart(int core) {
		return stackBin.get(core).getStackStart(this);
	}

	public String getPreambleString() {
		FileReader fr;
		String s = "";
		try {
			fr = new FileReader(codeDirectory + "/" + preambleFileName);
			BufferedReader reader = new BufferedReader(fr);
			String line = "";
			while ((line = reader.readLine()) != null) {
				s += line;
			}
			fr.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return s;
	}
	
	public String getUpperCaseName(){
		return name.toUpperCase();
	}


	public String getPeriodString(){
		return getUpperCaseName() + "_PERIOD";
	}

	public String getDeadlineString(){
		return getUpperCaseName() + "_DEADLINE";
	}
	public String getRuntimeIndexString(){
		return getUpperCaseName() + "_RT_PRIO";
	}

	public String getPriorityString() {
		return getUpperCaseName() + "_PRIORITY";
	}

	public String getStackSizeString() {
		return getUpperCaseName() + "_STACKSIZE";
	}
	
	public String getTableIndexString() {
		return getUpperCaseName() + "_TABLE_INDEX";
	}
	
	public String getWcetLowerBoundString() {
		return getUpperCaseName() + "_WCET_LOWERBOUND";
	}

	public void setWCET(int wcetLowerBound) {
		this.wcetLowerBound = wcetLowerBound;
		if(critical){
			wcetUpperBound = (int)(IpetAnalysis.WCET_RATIO * wcetLowerBound);
		}
	}

	public double getChiMS() {
		return wcetUpperBound * 1000 / Configuration.CLOCK_FREQUENCY;
	}

	public double getCloMS() {
		return wcetLowerBound * 1000 / Configuration.CLOCK_FREQUENCY;
	}

	public int getVirtualStackStart() {
		return 0x32000 + (StackBin.SIZE * funcID);
	}

	public int getVirtualStackEnd() {
	
		return getVirtualStackStart() + stackSize + StackBin.STACKSIZE_MARGINERROR 
				+ StackBin.STACKSIZE_MINOFFSET;
	}

	public int getLogicalID(Core core) {
		int id = -1;
		for(int i = 0; i < cores.size(); i++){
			if(cores.get(i).equals(core)){
				id = i;
				break;
			}
		}
		return id;
	}
	
}
