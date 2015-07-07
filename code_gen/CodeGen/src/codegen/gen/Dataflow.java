package codegen.gen;

public class Dataflow {
	Function source;
	Function destination;
	String sourceVariable;
	String destVariable;
	
	
	public Dataflow(){
		
	}
	
	public Dataflow(Function source, Function destination, String sourceVariable, String destVariable){
		this.source = source;
		this.destination = destination;
		this.sourceVariable = sourceVariable;
		this.destVariable = destVariable;
		
	}
	


	@Override
	public String toString(){
		String s = "";
		s += source + " sends " + sourceVariable + " to " + destination + " at " + destVariable;
		return s;
	}
}
