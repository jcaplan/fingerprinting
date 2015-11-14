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
	
	@Override
	public boolean equals(Object o){
		if(!(o instanceof ExpIdentifier)){
			return false;
		}
		
		ExpIdentifier other = (ExpIdentifier) o;
		
		return id.equals(other.id);
	}
}
