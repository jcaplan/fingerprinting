package codegen.prof.flow;

public class ExpIdentifier extends Expression {

	String id;
	
	public ExpIdentifier(String id){
		this.id = id;
	}
	
	@Override
	public Expression copy(){
		Expression exp = new ExpIdentifier(id);
		exp.status = this.status;
		if(children != null){
			for(int i = 0; i < children.length; i++){
				exp.setChild(i, children[i]);
			}
		}
		exp.line = line;
		return exp;
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
