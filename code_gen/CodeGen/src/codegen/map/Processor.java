package codegen.map;

public class Processor {

	boolean faultTolerant;
	String name;
	
	public static final boolean lockstepType = true;
	public static final boolean OdrType = false;
	
	public Processor(String name, boolean faultTolerant) {
		this.name = name;
		this.faultTolerant = faultTolerant;
	}

	@Override
	public String toString(){
		String s = name + ": ";
		if(faultTolerant){
			s += "lockstep";
		} else {
			s +=  "processing core";
		}
		return s;
	}
}
