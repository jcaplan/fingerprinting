package codegen.prof.flow;

public class Expression implements Comparable<Expression>{

	
	public static final int TOP = 0;
	public static final int BOTTOM = 1;
	public static final int WELL_DEFINED = 2;
	int status;
	protected Expression[] children;
	int line;
	
	public Expression() {
		status = WELL_DEFINED;
		initChildren();
	}
	

	public Expression copy(){
		Expression exp = new Expression();
		exp.status = this.status;
		if(children != null){
			for(int i = 0; i < children.length; i++){
				exp.setChild(i, children[i]);
			}
		}
		exp.line = line;
		return exp;
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





	public static Expression getNewTop(){
		Expression exp = new Expression();
		exp.status = TOP;
		return exp;
	}
	
	public static Expression getNewBottom(){
		Expression exp = new Expression();
		exp.status = BOTTOM;
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

	public boolean isBottom() {
		return status == BOTTOM;
	}


	@Override
	public int compareTo(Expression exp) {
		return this.toString().compareTo(exp.toString());
	}


	public static Expression sum(ExpConstant c1, ExpConstant c2) {
		return new ExpConstant(c1.value + c2.value);
	}
	
	
	public static Expression difference(ExpConstant c1, ExpConstant c2) {
		return new ExpConstant(c1.value - c2.value);
	}


	public static Expression shiftLeftLogical(ExpConstant c1, ExpConstant c2) {
		return new ExpConstant(c1.value << c2.value);
	}
	
	public static Expression shiftRightLogical(ExpConstant c1, ExpConstant c2) {
		return new ExpConstant(c1.value >>> c2.value);
	}
	
	public static Expression shiftRightArithmetic(ExpConstant c1, ExpConstant c2) {
		return new ExpConstant(c1.value >> c2.value);
	}


	public static Expression multiply(ExpConstant c1, ExpConstant c2) {
		return new ExpConstant(c1.value * c2.value);
	}
	
	public static Expression divide(ExpConstant c1, ExpConstant c2){
		return new ExpConstant(c1.value / c2.value);
	}


	public void setLine(int address) {
		line = address;
		
	}
	
}
