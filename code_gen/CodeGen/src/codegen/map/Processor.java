package codegen.map;

import java.io.Serializable;

public class Processor implements Serializable{

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
	
	@Override
	public boolean equals(Object o){
		Processor p = (Processor) o;
		return name.equals(p.name);
	}

	public String getName() {
		return name;
	}
}
