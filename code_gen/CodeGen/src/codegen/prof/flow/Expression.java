package codegen.prof.flow;

public class Expression implements Comparable<Expression>{

	
	public static final int TOP = 0;
	public static final int BOTTOM = 1;
	public static final int WELL_DEFINED = 2;
	int status;
	protected Expression[] children;
	
	public Expression() {
		status = WELL_DEFINED;
		initChildren();
	}
	

	protected void initChildren() {
		//No children for expression
	}


	public void setChild(int index, Expression child){
		children[index] = child;
	}
	
	public Expression getChild(int index){
		return children[index];
	}


	public void setStatus (int value){
		this.status = value;
	}
	
	public int getStatus (){
		return status;
	}
	
	@Override
	public String toString(){
		String s = "";
		switch(status){
		case TOP:
			s = "TOP";
			break;
		case BOTTOM:
			s = "BOTTOM";
			break;
		case WELL_DEFINED:
			s = "WELL_DEFINED";
		}
		return s;
	}
	
	@Override
	public boolean equals(Object o){
		if(!(o instanceof Expression)){
			return false;
		}
		
		return this.status == ((Expression) o).status;
		
		
	}





	public static Expression sum(ExpConstant c1, ExpConstant c2) {
		return new ExpConstant(c1.value + c2.value);
	}
	
	public static Expression getNewTop(){
		Expression exp = new Expression();
		exp.status = TOP;
		return exp;
	}


	public Expression[] getChildren() {
		return children;
	}


	public int numChildren() {
		return children.length;
	}


	public boolean isTop() {
		return status == TOP;
	}


	@Override
	public int compareTo(Expression exp) {
		return this.toString().compareTo(exp.toString());
	}
	
}
