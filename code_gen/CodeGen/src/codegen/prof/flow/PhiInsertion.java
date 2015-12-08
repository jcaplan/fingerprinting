package codegen.prof.flow;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Set;

import codegen.prof.BasicBlock;
import codegen.prof.Code;
import codegen.prof.Function;

public class PhiInsertion {

	HashMap<String, List<BasicBlock>> defsites;
	HashMap<BasicBlock, List<String>> phiList;
	HashMap<BasicBlock, List<String>> varsDefined;

	Function root;
	List<BasicBlock> bbList;
	public PhiInsertion(Function root){
		this.root = root;
		this.bbList = root.getBasicBlockList();
		defsites = new HashMap<>();
		phiList = new HashMap<>();
		varsDefined = new HashMap<>();
	}
	
	
	
	public void analyze(){
		initLists();

		for(String var : defsites.keySet()){
			ArrayList<BasicBlock> worklist = (ArrayList<BasicBlock>) defsites.get(var);
			while(!worklist.isEmpty()){
				BasicBlock node = worklist.get(0);
				worklist.remove(node);
				for(BasicBlock y : node.getDomFrontier()){
					if(!phiList.get(y).contains(var)){
						phiList.get(y).add(var);
						y.addCodeToFront(new PhiCode(var,y.getStartAddress(),y.getPredecessors().size()));
						//insert the statement into the block
						if(!varsDefined.get(y).contains(var)){
							if(!worklist.contains(y)){
								worklist.add(y);
							}
						}
					}
				}
			}
		}
	}


	private void initLists() {
		for (BasicBlock bb: bbList){
			varsDefined.put(bb, new ArrayList<String>());			
			phiList.put(bb, new ArrayList<String>());
			for(Code c : bb.getCode()){
				String[] ops = c.getOperands();
				//add a defsite for each line as appropriate
				switch(c.getType()){
				case CALL:
				case INDIRECTCALL:
					//return registers
					addToStartLists(bb, "r2");
					addToStartLists(bb, "r3");
					break;
				case STORE:
					addToStartLists(bb, ops[1]);
					break;
				case LOAD:
				case CUSTOM:
				case BINOP:
				case COMPARE:
				case MOVE:
					addToStartLists(bb, ops[0]);
					break;
				default:
					break;
				}
				//build varsDefined also
			}
		}
	}


	private void addToStartLists(BasicBlock bb, String var) {
		List<String> vars = varsDefined.get(bb);
		if(!vars.contains(var)){
			varsDefined.get(bb).add(var);
		}
		ArrayList<BasicBlock> list = (ArrayList<BasicBlock>) defsites.get(var);
		if(list == null){
			list = new ArrayList<BasicBlock>();
			list.add(bb);
			defsites.put(var, list);
		} else {
			if(!list.contains(bb)){
				list.add(bb);
			}
		}	
	}
	
	public void prettyPrint(){
		for(BasicBlock bb : bbList){
			System.out.println("*************************");
			System.out.println(bb + ": numlines = " + bb.getNumInstructions());
			for(Code c : bb.getCode()){
				System.out.println(c);
			}
		}
	}



	public Set<String> getVarSet() {
		return defsites.keySet();
	}
	
}
