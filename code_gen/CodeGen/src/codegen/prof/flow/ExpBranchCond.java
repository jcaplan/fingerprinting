package codegen.prof.flow;


public class ExpBranchCond extends ExpCompareOp{
	
	public ExpBranchCond(String instr) {
		super(instr);
		
	}

	@Override
	protected void setType(String instr) { 
		switch(instr){
		case "beq":
			type = Type.EQ;
			break;
		case "bge":
		case "bgeu":	
			type = Type.GE;
			break;
		case "bgt":
		case "bgtu":
			type = Type.GT;
			break;
		case "ble":
		case "bleu":
			type = Type.LE;
			break;
		case "blt":
		case "bltu":
			type = Type.LT;
			break;
		case "bne":
			type = Type.NE;
			break;
		default:
			break;
		}
		
	}
}
