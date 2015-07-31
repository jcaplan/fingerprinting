package codegen.gen;

import java.util.ArrayList;
import java.util.Comparator;

/**
 * Function contains all the necessary information about each function in the system.
 * @author jonah
 *
 */
public class Function implements Comparable<Function>{

	String name;
	String codeDirectory;
	int period;
	boolean critical;
	ArrayList<Dataflow> dataflowList;
	ArrayList<String> cores;
	enum Type{periodic,eventDriven};
	Type type;
	int priority;
	ArrayList<String> includeFiles;
	ArrayList<String> varDeclarations;
	ArrayList<String> initialization;
	public int stackSize;
	StackBin stackBin;
	
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
		
		init += "	" + name + "_initialize(" + name + "_M, " + name + "_U, " + name + "_Y);\n";

		init += "	" + name + "UpdatePointers((INT32U)&" + name + "PackageStruct" + " & 0x3FFFFF, " + name + "_M);\n";
		
		init += "\n";
		return init;
	}
	
	/**
	 * @param core	Which core for the given function (in case there are two)
	 * @return	the end address of the stack for this function on this core
	 */
	public int getStackEnd(int core) {
		return stackBin.getStackEnd(this,core);
	}


	/**
	 * @param core	Which core for the given function (in case there are two)
	 * @return	the start address of the stack for this function on this core
	 */
	public int getStackStart(int core) {
		return stackBin.getStackStart(this,core);
	}




}
