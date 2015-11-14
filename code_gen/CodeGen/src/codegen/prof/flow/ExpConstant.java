package codegen.prof.flow;

public class ExpConstant extends Expression {

	
	int value;
	
	public ExpConstant(int val) {
		value = val;
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
