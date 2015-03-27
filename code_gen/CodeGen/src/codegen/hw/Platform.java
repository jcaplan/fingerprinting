package codegen.hw;

import java.util.ArrayList;

public class Platform {

	public ArrayList<Processor> processors;
	
	public Platform(){
		processors = new ArrayList<Processor>();
	}
	
	public void addProcessor(boolean faultTolerant, boolean hasFprint, String label){
		Processor p = new Processor(faultTolerant, hasFprint,label);
		processors.add(p);
	}
	
	
}
