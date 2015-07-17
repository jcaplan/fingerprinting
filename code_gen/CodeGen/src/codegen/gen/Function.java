package codegen.gen;

import java.util.ArrayList;
import java.util.Comparator;

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
	static Comparator<Function> stackCompareDecreasing = new Comparator<Function>() {
         @Override
         public int compare(Function f1, Function f2) {
           return f2.stackSize - f1.stackSize;
         }
     };
     
	
	public Function(){
		critical = false;
		dataflowList = new ArrayList<>();
		cores = new ArrayList<>();
		includeFiles = new ArrayList<>();
		varDeclarations = new ArrayList<>();
		initialization = new ArrayList<>();
	}


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
	
	@Override
	public boolean equals(Object o){
		if(! (o instanceof Function)){
			return false;
		}
		Function f = (Function) o;
		return f.name.equals(this.name);
		
	}


	public String getVarDeclarationString() {
		String s = "";
		for(String dec : varDeclarations){
			s += dec + "\n";
		}
		return s;
		
	}


	public String printInitialization(int tab) {
		String init = "";
		for(String s : initialization){
			for(int i = 0; i < tab; i++){
				init += "\t";
			}
			init += s + "\n";
		}
		return init;
		
	}


	public String printInitializationWithStruct(int tab) {
		String init = "";
		init += "	RT_MODEL_" + this + "_T *" + this + "_M =\n"+
				"			&" + this + "PackageStruct." + this + "_STRUCT." + this + "_M;\n";
		init += "	ExtU_" + this + "_T *" + this + "_U =\n"+
				"			&" + this + "PackageStruct." + this + "_STRUCT." + this + "_U;\n";
		init += "	ExtY_" + this + "_T *" + this + "_Y =\n"+
				"			&" + this + "PackageStruct." + this + "_STRUCT." + this + "_Y;\n";
		init += "	P_" + this + "_T *" + this + "_P =\n"+
				"			&" +  this + "PackageStruct." + this + "_P;\n";
		for (String s : initialization){
			if(s.contains("DW")){
				init += "	DW_" + this + "_T *" + this + "_DW =\n"+
						"			&" + this + "PackageStruct." + this + "_DW;\n";
				break;
			}
		}
		for(String s : initialization){
			
			
			s = s.replaceAll("&", "");
			
			
			for(int i = 0; i < tab; i++){
				init += "\t";
			}
			init += s + "\n";
		}
		init += "\n";
		return init;
	}
	
	public int getStackEnd(int core) {
		return stackBin.getStackEnd(this,core);
	}


	public int getStackStart(int core) {
		return stackBin.getStackStart(this,core);
	}


}
