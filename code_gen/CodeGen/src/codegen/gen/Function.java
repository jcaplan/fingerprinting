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


	public void printInitialization() {
		for(String s : initialization){
			System.out.println(s);
		}
		
	}
}
