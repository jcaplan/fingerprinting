package codegen.gen;

import java.util.*;

public class Core {
	String name;
	boolean isMonitor;
	ArrayList<Function> funcList;
	ArrayList<String> includes;
	public String bspDir;
	int mainMemStartAddress;
	int mainMemSize;
	
	public Core(String name, boolean isMonitor, int memStartAddress, int memSize){
		this.name = name;
		this.isMonitor = isMonitor;
		this.mainMemStartAddress = memStartAddress;
		this.mainMemSize = memSize;
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

	public String printIncludes() {
		String s = "";
		for (String include : includes) {
			s += "#include " + include + "\n";
		}
		return s;
	}

	public String printVarDeclarations() {
		String s = "";
		for (Function f : funcList){
			if(!f.critical){
				s += f.getVarDeclarationString();
			} else if(isMonitor) {
				s += f.getVarDeclarationString();
			}
		}
		return s;
		
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
