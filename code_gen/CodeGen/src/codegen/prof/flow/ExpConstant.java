package codegen.prof.flow;

public class ExpConstant extends Expression {

	
	int value;
	
	public ExpConstant(int val) {
		value = val;
	}
	
	@Override
	public Expression copy(){
		Expression exp = new ExpConstant(value);
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
		return Integer.toString(value);
	}
	

	@Override
	public boolean equals(Object o){
		if(!(o instanceof ExpConstant)){
			return false;
		}
		
		ExpConstant other = (ExpConstant) o;
		return this.value == other.value;  
	}	
	

}
