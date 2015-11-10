package codegen.prof.flow;

import codegen.prof.*;
import codegen.prof.flow.ExpCompareOp.Type;

public class ExpBinOp extends Expression {

	enum Type{
		PLUS,MINUS,
	};
	
	Type type;
	Expression lhs,rhs;

	public ExpBinOp(String instr) {
		setType(instr);
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
		case PLUS:
			s = " + ";
			break;
		case MINUS:
			s = " - ";
			break;
		default:
			break;
		
		}
		return s;
	}
	

		
	
	
	
}
