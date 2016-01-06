package codegen.gen;

import java.util.*;

/**
 * Contains all necessary info for generation on each core
 * @author jonah
 *
 */
public class Core {
	String name;
	boolean isMonitor;
	ArrayList<Function> funcList;
	ArrayList<String> includes;
	public String bspDir;
	int mainMemStartAddressOffset;
	int mainMemSize;
	int totalMemSize;
	int index;
	boolean requiresCriticalHeader = false;
	
	/**
	 * Constructor
	 * @param name	Each core requires a unique string name
	 * @param isMonitor	The core may be identified as the monitor core (should only be one)
	 * @param memStartAddress	The start address for this core in memory
	 * @param memSize	The size allocated to this core in main memory
	 * @param index	The index of this core in the core Table
	 */
	public Core(String name, boolean isMonitor, int memStartAddress, int memSize, int index){
		this(name,isMonitor,memStartAddress,memSize);
		this.index = index;
	}
	
	/**
	 * No longer used publicly. Does not include index parameter.
	 * @param name
	 * @param isMonitor
	 * @param memStartAddress
	 * @param memSize
	 */
	
	private Core(String name, boolean isMonitor, int memStartAddress, int memSize){
		this.name = name;
		this.isMonitor = isMonitor;
		this.mainMemStartAddressOffset = memStartAddress;
		this.mainMemSize = memSize;
		this.totalMemSize = memSize;
		funcList = new ArrayList<>();
		includes = new ArrayList<>();
	}
	
	
	/**
	 * Adds header to list of includes.
	 * lib headers should be provided with format <lib.h>
	 * The name of the file should be provided in other cases.
	 * The function automatically inserts the necessary quotes
	 * @param header
	 */
	public void addHeader(String header){
		if(header.startsWith("<")){
			includes.add(header);
		}else{
			includes.add("\"" + header + "\"");	
		}
	}

	/**
	 * 
	 * @return String list of includes
	 */
	public String getIncludesString() {
		String s = "";
		for (String include : includes) {
			s += "#include " + include + "\n";
		}
		return s;
	}

	/**
	 * 
	 * @return A string of the static variable declarations parsed from ert_main.c for 
	 * all functions running on this core.
	 */
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
	
	/**
	 * Default string behaviour prints name
	 */
	@Override 
	public String toString(){
		return name;
	}

	/**
	 * @param tab	for proper formatting
	 * @return	String of function initializations for all functions on this core
	 */
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
	
	public int getMemSizeWithStackBins(){
		return totalMemSize;
	}
	
}
