package codegen.gen;

import java.util.*;

public class Core {
	String name;
	boolean isMonitor;
	ArrayList<Function> funcList;
	ArrayList<String> includes;
	public String bspDir;
	int mainMemStartAddressOffset;
	int mainMemSize;
	int index;
	
	public Core(String name, boolean isMonitor, int memStartAddress, int memSize, int index){
		this(name,isMonitor,memStartAddress,memSize);
		this.index = index;
	}
	
	public Core(String name, boolean isMonitor, int memStartAddress, int memSize){
		this.name = name;
		this.isMonitor = isMonitor;
		this.mainMemStartAddressOffset = memStartAddress;
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
			s += "/* " + f + "*/\n";
			if(!f.critical){
				s += f.getVarDeclarationString();
			} else if(isMonitor) {
				s += f.getVarDeclarationString();
			}
			s += "\n";
		}
		return s;
		
	}
	
	@Override 
	public String toString(){
		return name;
	}

	public String printInitializations(int tab) {
		String s = "";
		for (Function f : funcList){
			if(!f.critical){ 
				s += f.printInitialization(tab);
				s += "\n";
			} else if(isMonitor) {
				s += f.printInitialization(tab);
				s += "\n";
			}
		}
		return s;
		
	}
	
}
