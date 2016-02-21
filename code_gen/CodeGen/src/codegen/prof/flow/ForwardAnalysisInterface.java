package codegen.prof.flow;


public interface ForwardAnalysisInterface<A> {
	
	public void analyze();
	
	public void resetAnalysis();
	
	public void prettyPrint();
	
}