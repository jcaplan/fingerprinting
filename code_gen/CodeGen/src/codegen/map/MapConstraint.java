package codegen.map;

public class MapConstraint {

	Task t1;
	Task t2;
	
	public MapConstraint(Task t1, Task t2){
		this.t1 = t1;
		this.t2 = t2;
	}

	public static boolean check(Processor p1, Processor p2) {
		return !p1.equals(p2);
	}
	
}
