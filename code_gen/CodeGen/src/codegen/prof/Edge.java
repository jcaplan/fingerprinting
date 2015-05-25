package codegen.prof;

public class Edge {

	BasicBlock startBlock;
	BasicBlock endBlock;
	int index;
	static int EdgeCount = 1;
	
	public Edge(BasicBlock start, BasicBlock end){
		startBlock = start;
		endBlock = end;
		index = EdgeCount++;
	}
	
	@Override
	public String toString(){
		String s = "";
		s += "Edge " + index + ": " + startBlock + " -> " + endBlock;
		return s;
	}
	
}
