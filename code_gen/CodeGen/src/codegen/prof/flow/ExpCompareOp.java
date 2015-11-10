package codegen.prof.flow;

public class ExpCompareOp extends Expression{

	enum Type {
		NE,EQ,LT,GT,LE,GE
	}
	
	Type type;
	Expression lhs,rhs;
	
	public ExpCompareOp(String instr) {
		setType(instr);
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
		lhs = exp;
	}

	public void setRHS(Expression exp) {
		rhs = exp;
	}
	
	
	@Override
	public String toString(){
		return "(" + lhs + ")" + getOpString() + "(" + rhs + ")";
		
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
	
}
