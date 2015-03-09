package codegen.hw;

public class Processor {
	boolean faultTolerant;
	int scratchpadSize; //bytes
	int timePeriodMS; //Milliseconds
	boolean hasFprint;
	boolean hasICache;
	boolean hasDCache;
	boolean hasDMA;
	int sharedMemorySize;
	String label;
	Bus ICache;
	Bus DCache;
	Wire Interrupts[] = new Wire[32];
	
	public Processor(boolean faultTolerant, boolean hasFprint, String label){
		this.faultTolerant = faultTolerant;
		this.hasFprint = hasFprint;
		this.label = label;
	}
	
}
