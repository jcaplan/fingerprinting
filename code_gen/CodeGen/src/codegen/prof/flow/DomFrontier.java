package codegen.prof.flow;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import codegen.prof.BasicBlock;
import codegen.prof.Function;

public class DomFrontier extends IntraForwardAnalysis<List<BasicBlock>>{

	
	Map<BasicBlock,List<BasicBlock>>  bbOutSet;
	
	public DomFrontier(Function root) {
		super(root);
		bbOutSet = new HashMap<BasicBlock, List<BasicBlock>>();
	}
	
	@Override
	protected void analysisPreprocessing() {
		for(BasicBlock bb : bbList){
			bbInSet.put(bb, new ArrayList<>(bbList));
		}
		BasicBlock start = func.getBasicBlockList().get(0);
		ArrayList<BasicBlock> startList = new ArrayList<>();
		startList.add(start);
		bbInSet.put(start, startList);
	}
	

	@Override
	protected List<BasicBlock> initSet() {
		return new ArrayList<>();
	}

	@Override
	protected List<BasicBlock> merge(List<BasicBlock> set1,
			List<BasicBlock> set2) {
		//intersection
		
		if(set2.isEmpty()){
			return set1;
		} else if(set1.isEmpty()){
			return set2;
		}
	
		List<BasicBlock> result = new ArrayList<>();
		
		for(BasicBlock bb : set1){
			if(set2.contains(bb)){
				result.add(bb);
			}
		}
		return result;
	}

	
	@Override 
	public void analyze(){
		super.analyze();
		
		//Skip the root 
		for(int i = 1; i < bbList.size(); i++){
			BasicBlock bb = bbList.get(i);
			// Copy  reference to the dominator set into each BasicBlock
			bb.setDominators(bbOutSet.get(bb));
		
			//find the immediate dominator
			BasicBlock idom = null;
			int highestRank = -1;
			for(BasicBlock dom : bb.getDominators()){
				if(!dom.equals(bb) && dom.getAge() > highestRank){
					idom = dom;
					highestRank = dom.getAge();
				}
			}
			bb.setImmDominator(idom);
			idom.getDomTreeChildren().add(bb);
			
		}	
		//Find the dominance frontier
		computeDF(func.getBasicBlockList().get(0));		
	}
	
	private void computeDF(BasicBlock bb) {
		
		//Find the local dominance frontier
		for(BasicBlock succ : bb.getSuccesors()){
			if(!succ.getImmDominator().equals(bb)){
				bb.addToDomFrontier(succ);
			}
		}
		
		//Propagate upwards
		for(BasicBlock child : bb.getDomTreeChildren()){
			computeDF(child);
			for(BasicBlock grandchild : child.getDomFrontier()){
				if(!grandchild.getDominators().contains(bb) || bb.equals(grandchild)){
					bb.addToDomFrontier(grandchild);
				}
			}
		}
	}

	@Override
	protected List<BasicBlock> copy(List<BasicBlock> original) {
		return new ArrayList<>(original);
	}

	@Override
	public List<BasicBlock> analyzeBasicBlock(BasicBlock bb, BasicBlock succ, List<BasicBlock> inSet) {
		
		List<BasicBlock> c_out = new ArrayList<>(inSet);
		if(!c_out.contains(bb)){
			c_out.add(bb);
		}
		bbOutSet.put(bb, c_out);
		return  c_out;
	}
	
	@Override
	public void prettyPrintBlockList(){
		for(BasicBlock bb : bbList){
			System.out.println(bb + ": " + bbOutSet.get(bb));			
		}
	}
	
	public void printDominanceFrontiers(){
		for(BasicBlock bb : bbList){
			System.out.println(bb + ": " + bb.getDomFrontier());
		}
	}
}