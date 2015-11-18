package codegen.prof.flow;

public class ExpCompareOp extends Expression{

	enum Type {
		NE,EQ,LT,GT,LE,GE
	}
	
	Type type;
	

	public static final int LHS = 0;
	public static final int RHS = 1;
	
	
	
	@Override 
	protected void initChildren(){
		children = new Expression[2];
	}
	
	public ExpCompareOp(String instr) {
		setType(instr);
		initChildren();
	}
	
	public ExpCompareOp(Type type){
		this.type = type;
		initChildren();
	}

	public void setType(Type type){
		this.type = type;
	}
	protected void setType(String instr) {
		switch(instr){
		case "cmpeq":
		case "compeqi":
			type = Type.EQ;
			break;	
		case "cmpge":
		case "cmpgei":
		case "cmpgeu":
		case "cmpgeui":
			type = Type.GE;
			break;
		case "cmpgt":
		case "cmpgti":
		case "cmpgtu":
		case "cmpgtui":
			type = Type.GT;
			break;
		case "cmple":
		case "cmplei":
		case "cmpleu":
		case "cmpleui":
			type = Type.LE;
			break;
		case "cmplt":
		case "cmplti":
		case "cmpltu":
		case "cmpltui":
			type = Type.LT;
			break;
		case "cmpne":
		case "cmpnei":
			type = Type.NE;
			break;
		default:
			break;
		}
		
	}

	public void setLHS(Expression exp) {
		setChild(LHS, exp); 
	}

	public void setRHS(Expression exp) {
		setChild(RHS, exp);
	}
	
	public Expression getLHS(){
		return getChild(LHS);
	}
	
	public Expression getRHS(){
		return getChild(RHS);
	}
	
	
	
	@Override
	public String toString(){
		return "(" + getLHS() + ")" + getOpString() + "(" + getRHS() + ")";
		
	}

	private String getOpString() {
		String s = "";
		switch(type){
		case EQ:
			s = " = ";
			break;
		case GE:
			s = " >= ";
			break;
		case GT:
			s = " > ";
			break;
		case LE:
			s = " <= ";
			break;
		case LT:
			s = " < ";
			break;
		case NE:
			s = " != ";
			break;
		default:
			break;
		
		}
		return s;
	}
	

	@Override
	public boolean equals(Object o){
		if(!(o instanceof ExpBinOp)){
			return false;
		}
		 
		ExpBinOp other = (ExpBinOp) o;
		return getLHS().equals(other.getLHS()) && getRHS().equals(other.getRHS()) && type.equals(other.type);  
	}	
	
	public static Type getInvertedType(Type type) {
		Type newType = null;
		
		switch(type){
		case EQ:
			type = Type.NE;
			break;
		case GE:
			type = Type.LT;
			break;
		case GT:
			type = Type.LE;
			break;
		case LE:
			type = Type.GT;
			break;
		case LT:
			type = Type.GE;
			break;
		case NE:
			type = Type.EQ;
			break;
		default:
			break;
		
		}
		return newType;
	}
	

	public void changeDirection() {
		switch(type){
		case EQ:
			type = Type.EQ;
			break;
		case GE:
			type = Type.LE;
			break;
		case GT:
			type = Type.LT;
			break;
		case LE:
			type = Type.GE;
			break;
		case LT:
			type = Type.GT;
			break;
		case NE:
			type = Type.NE;
			break;
		default:
			break;
		
		}
	}
	public Type getType() {
		return type;
	}
	
	public void invertType() {
		type = getInvertedType(type);
	}
}
