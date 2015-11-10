package codegen.prof.flow;

public class ExpIdentifier extends Expression {

	String id;
	
	public ExpIdentifier(String id){
		this.id = id;
	}
	
	
	@Override
	public String toString(){
		return id;
	}
}
