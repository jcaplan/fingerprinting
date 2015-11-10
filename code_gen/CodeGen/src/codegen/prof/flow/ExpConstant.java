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

}
