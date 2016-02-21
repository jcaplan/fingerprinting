package codegen.prof.flow;


import java.util.ArrayList;

import codegen.prof.BasicBlock;
import codegen.prof.Code;



public interface ForwardAnalysisInterface<A> {
	
	
	
	public void analyze();
	
	public void resetAnalysis();
	
	public void prettyPrint();
	
	public void prettyPrint(BasicBlock bb);
	
	public A getInSet(BasicBlock bb);
	
	public A getInSet(Code c);
	
	public A getOutSet(BasicBlock bb);
	
	public A getMergedOutSet(ArrayList<BasicBlock> entryList);
	
	
	
}