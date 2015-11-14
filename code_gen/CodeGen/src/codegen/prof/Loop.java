package codegen.prof;

import java.util.ArrayList;

import lpsolve.BbListener;

public class Loop {

	BasicBlock head;
	BasicBlock tail;
	ArrayList<BasicBlock> body;
	int maxIterations = 0;
	public static final int defaultMaxIterations = 10;
	
	public Loop(BasicBlock head, BasicBlock tail) {
		this.head = head;
		this.tail = tail;
		buildBody();
	}

	public String getDotGraphString() {
		String s = "";
		s += "subgraph cluster_loop_" + head.getStartAddressHex() + " {\n";
		s += "\tgraph [label = \"loop\"];\n";
		s += "\tnode_" + head.getStartAddressHex();
		s += " [color = green]\n";
		for (BasicBlock bb : body) {
			s += bb.getDotNodeString();
		}

		s += "}\n";

		return s;
	}

	private void buildBody() {

		// The tail is not a direct descendant
		body = new ArrayList<>();
		body.add(head);
		// If single block loop then there are no other bb in body
		// -------------------------------------------------------
		if (head.equals(tail)) {
			return;
		}

		findAncestors(tail);

	}

	private void findAncestors(BasicBlock end) {
		if (!body.contains(end)) {
			body.add(end);
			// Important to go here in case there are loops inside of other
			// loops
			// ------------------------------------------------------------
			for (BasicBlock bb : end.predecessors) {
				if (!bb.equals(head)) {
					findAncestors(bb);
				}
			}
		}
	}

	@Override
	public String toString() {
		String s = "";
		s += "loop head: " + head.getStartAddressHex() + "; tail: "
				+ tail.getStartAddressHex() + "; body: ";
		if (body != null) {
			for (BasicBlock bb : body) {
				s += bb.getStartAddressHex() + ", ";
			}
			s = s.substring(0, s.length() - 1);
		}
		return s;
	}

	public boolean containsAddress(int address) {
		for(BasicBlock bb : body){
			if(bb.containsCode(address)){
				return true;
			}
		}
		return false;
	}
	
	public BasicBlock getHead(){
		return head;
	}
	
	public BasicBlock getTail(){
		return tail;
	}
	
	public ArrayList<BasicBlock> getBody(){
		return body;
	}

	public BasicBlock getSingleEntry() {
		
		boolean foundSingle = false;
		BasicBlock entry = null;
		for(BasicBlock bb : head.predecessors){
			if(!body.contains(bb)){
				if(!foundSingle){
					entry = bb;
					foundSingle = true;
				} else {
					entry = null;
					break;
				}
			}
		}
		
		return entry;
	}
	

}
