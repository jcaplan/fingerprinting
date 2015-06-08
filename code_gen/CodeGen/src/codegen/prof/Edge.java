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
		if(startBlock != null){
			startBlock.succEdges.add(this);
		}
		if(endBlock != null){
			endBlock.predEdges.add(this);
		}
	}
	
	public Edge() {
		// TODO Auto-generated constructor stub
	}

	@Override
	public String toString(){
		String s = "";
		s += "Edge " + index + ": " + startBlock + " -> " + endBlock;
		return s;
	}

	public boolean isSingleBBLoop() {
		return startBlock != null && startBlock.equals(endBlock);
	}
	
}
