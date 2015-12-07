package codegen.prof.flow;


public class ExpBinOp extends Expression {

	enum Type{
		PLUS,MINUS,TIMES,SLL,DIV,SRL,SRA
	};
	
	Type type;
	public static final int LHS = 0;
	public static final int RHS = 1;

	@Override
	public Expression copy(){
		Expression exp = new ExpBinOp(type);
		exp.status = this.status;
		if(children != null){
			for(int i = 0; i < children.length; i++){
				exp.setChild(i, children[i]);
			}
		}
		exp.line = line;
		return exp;
	}
	
	public ExpBinOp(String instr) {
		setType(instr);
		initChildren();
	}

	public ExpBinOp(Type type){
		this.type = type;
		initChildren();
	}
	
	@Override 
	protected void initChildren(){
		children = new Expression[2];
	}
	
	protected void setType(String instr) {
		switch(instr){
		case "add":
		case "addi":
			type = Type.PLUS;
			break;	
		case "sub":
		case "subi":
			type = Type.MINUS;
			break;
		case "mul":
		case "muli":
			type = Type.TIMES;
			break;
		case "sll":
		case "slli":
			type = Type.SLL;
			break;
		case "sra":
		case "srai":
			type = Type.SRA;
			break;
		case "srl":
		case "srli":
			type = Type.SRL;
			break;
		case "div":
		case "divi":
			type = Type.DIV;
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
		return "(" + getChild(LHS) + ")" + getOpString() + "(" + getChild(RHS) + ")";
		
	}

	private String getOpString() {
		String s = "";
		if(type == null){
			System.out.println("problem here");
		}
		switch(type){
		case PLUS:
			s = " + ";
			break;
		case MINUS:
			s = " - ";
			break;
		case SLL:
			s = " << ";
			break;
		case SRA:
			s = " >> ";
			break;
		case SRL:
			s = " >>> ";
			break;
		case TIMES:
			s = " * ";
			break;
		case DIV:
			s = " / ";
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
	
	
	
}
