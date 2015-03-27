package codegen.hw;

import java.util.*;
import codegen.sw.*;

public class Processor {
	public boolean faultTolerant;
	int scratchpadSize; //bytes
	int timePeriodMS; //Milliseconds
	public boolean hasFprint;
	boolean hasICache;
	boolean hasDCache;
	boolean hasDMA;
	int sharedMemorySize;
	String label;
	Bus ICache;
	Bus DCache;
	Wire Interrupts[] = new Wire[32];
	public ArrayList<Task> readyList;
	
	
	public Processor(boolean faultTolerant, boolean hasFprint, String label){
		this.faultTolerant = faultTolerant;
		this.hasFprint = hasFprint;
		this.label = label;
		
	}
	
}
