package codegen.gen;

import java.util.*;

public class Core {
	String name;
	boolean isMonitor;
	ArrayList<Function> funcList;
	ArrayList<String> includes;
	
	public Core(String name, boolean isMonitor){
		this.name = name;
		this.isMonitor = isMonitor;
		funcList = new ArrayList<>();
		includes = new ArrayList<>();
	}
	
	public void addHeader(String header){
		if(header.startsWith("<")){
			includes.add(header);
		}else{
			includes.add("\"" + header + "\"");	
		}
	}

	public void printIncludes() {
		for (String include : includes) {
			System.out.println("#include " + include);
		}
	}

	public void printVarDeclarations() {
		for (Function f : funcList){
			if(!f.critical){
				f.printVarDeclarations();
			} else if(isMonitor) {
				f.printVarDeclarations();
			}
		}
		
	}
	
	@Override 
	public String toString(){
		return name;
	}

	public void printInitializations() {
		for (Function f : funcList){
			if(!f.critical){ 
				f.printInitialization();
			} else if(isMonitor) {
				f.printInitialization();
			}
		}
		
	}
	
}
