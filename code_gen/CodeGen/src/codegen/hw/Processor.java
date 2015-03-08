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
	
	Bus ICache;
	Bus DCache;
	Wire Interrupts[] = new Wire[32];
	
	
	
}
