package codegen.prof.flow;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import codegen.prof.BasicBlock;
import codegen.prof.Code;
import codegen.prof.Function;


/*
 * Each Basic block stores its IN-set
 * Compute along edges
 * 
 * Algorithm
 * ---------
 * 
 * Worklist = all blocks (reverse post order..)
 * 
 * initialize each BB.inSet
 * 
 * 	while(!worklist.isEmpty()
 * 		x = choose first block from worklist and remove
 * 		in_x = in[x]
 * 		for each y in x.successors() do
 * 			out_x = flow[x,y](in_x)
 * 			new_in_y = out_x merge in[y]
 * 			if (new_in_y != in[y] 
 * 				in[y] = new_in_y
 * 				add y to worklist if not there
 * 		until no nodes left in worklist
 * 	
 * 
 * 
 * 
 * 
 * Need to keep track of in and out for each line of code
 * also need to keep track of in and out for each basic block
 * 
 * but each basic block also needs
 * 
 */


public abstract class ForwardAnalysis<A> {
	
	Function func;
	List<BasicBlock> bbList;
	List<BasicBlock> workList;
	
	Map<Code,A> codeInSet;
	Map<Code,A> codeOutSet;
	Map<BasicBlock, A>  bbInSet;
	
	public ForwardAnalysis(Function root) {
		func = root;
		setBBList(new ArrayList<>(func.getBasicBlockList()));
		
	}	
	
	public ForwardAnalysis(List<BasicBlock> bbList) {
		setBBList(bbList);
	}	
	
	public void setBBList(List<BasicBlock> bbList) {
		this.bbList = bbList;
		resetAnalysis();
	}

	public void analyze(){
		
		
		while(!workList.isEmpty()){
			BasicBlock bb = workList.get(0);
			workList.remove(0);
			
			A bb_in = bbInSet.get(bb);
			
			if(bb.getSuccesors().isEmpty()){
				analyzeBasicBlock(bb, null, bb_in);
			}
			
			for(BasicBlock succ : bb.getSuccesors()){
				if(bbList.contains(succ)){								//if doing entire function then this should always be true
					A bb_out = analyzeBasicBlock(bb,succ,bb_in);
					
					A succ_in = merge(bb_out, bbInSet.get(succ));
					
					
					if(!succ_in.equals(bbInSet.get(succ))){
						bbInSet.put(succ, succ_in);
						if(!workList.contains(succ)){
							workList.add(succ);
						}		
					}
				} else {													// sometimes if analyzing only a subset of basic blocks 
																			// the successor (e.g. head of a loop) was deliberately omitted 
																			// from worklist.
																			// This ensures that the analysis is still done.
																			// Should not be more than one successor but break just in case.
					analyzeBasicBlock(bb,null, bb_in);
					break;
				}
			}	
		}
	}

	public void resetAnalysis() {
		codeInSet = new HashMap<Code, A>();
		codeOutSet = new HashMap<Code, A>();
		bbInSet = new HashMap<BasicBlock, A>();
		workList = new ArrayList<>(bbList);
		initAllSets();
		
	}

	public A analyzeBasicBlock(BasicBlock bb, BasicBlock succ, A inSet) {
		ArrayList<Code> codeList = bb.getCode();
		
		A c_in = inSet;
		codeInSet.put(codeList.get(0), inSet);
		Code next = codeList.get(0); 				//if only one line of code 
														//skip for loop
														//next points to first line of code
		
		for(int i = 0; i < codeList.size() - 1; i++){
			Code c = codeList.get(i);
			A c_out = getCodeOut(c,c_in,null);
			codeOutSet.put(c, c_out);
			next = codeList.get(i + 1);
			codeInSet.put(next, c_out);
			c_in = c_out;
		}
		
		//The last line uses the successor
		
		A c_out = getCodeOut(next,c_in,succ);
		codeOutSet.put(next,c_out);
		
		return  c_out;
	}

	private void initAllSets() {
		for(BasicBlock bb : workList){
			bbInSet.put(bb, initSet());
			for(Code c : bb.getCode()){
				codeInSet.put(c,initSet());
			}
		}
	}
	

	protected abstract A initSet();

	protected A getCodeOut(Code c, A c_in, BasicBlock succ){
		A result = null;
		switch(c.getType()){
		case CALL:
			result = caseCall(c,c_in,succ);
			break;
		case COND_BRANCH:
			result = caseCondBranch(c,c_in,succ);
			break;
		case CUSTOM:
			result = caseCustom(c,c_in,succ);
			break;
		case INDIRECTCALL:
			result = caseIndirectCall(c,c_in,succ);
			break;
		case JUMP:
			result = caseJump(c,c_in,succ);
			break;
		case LOAD:
			result = caseLoad(c,c_in,succ);
			break;
		case OTHER:
			result = caseOther(c,c_in,succ);
			break;
		case RETURN:
			result = caseReturn(c,c_in,succ);
			break;
		case STORE:
			result = caseStore(c,c_in,succ);
			break;
		case UNCOND_BRANCH:
			result = caseUncondBranch(c,c_in,succ);
			break;
		case BINOP:
			result = caseBinOp(c, c_in, succ);
			break;
		case COMPARE:
			result = caseCompareOp(c, c_in, succ);
			break;
		case MOVE:
			result = caseMoveOp(c, c_in, succ);
			break;
		default:
			result = caseDefault(c, c_in, succ);
			break;
		
		}
		
		
		return result;
	}

	




	protected abstract A merge(A bbOutSet2, A a);
	
	protected abstract A copy(A original);
	
	private A caseDefault(Code c, A c_in, BasicBlock succ) {
		return copy(c_in);
	}
	
	protected A caseUncondBranch(Code c, A c_in, BasicBlock succ) {
		return caseDefault(c,c_in, succ);
	}

	protected A caseStore(Code c, A c_in, BasicBlock succ) {		
		return caseDefault(c,c_in, succ);
	}

	protected A caseReturn(Code c, A c_in, BasicBlock succ) {
		return caseDefault(c,c_in, succ);
	}

	protected A caseOther(Code c, A c_in, BasicBlock succ) {
		return caseDefault(c,c_in, succ);
	}

	protected A caseLoad(Code c, A c_in, BasicBlock succ) {
		return caseDefault(c,c_in, succ);
	}

	protected A caseJump(Code c, A c_in, BasicBlock succ) {
		return caseDefault(c,c_in, succ);
	}

	protected A caseIndirectCall(Code c, A c_in, BasicBlock succ) {
		return caseDefault(c,c_in, succ);
	}

	protected A caseCustom(Code c, A c_in, BasicBlock succ) {
		return caseDefault(c,c_in, succ);
	}

	protected A caseCondBranch(Code c, A c_in, BasicBlock succ) {
		return caseDefault(c,c_in, succ);
	}

	protected A caseCall(Code c, A c_in, BasicBlock succ) {
		return caseDefault(c,c_in, succ);
	}
	
	protected A caseBinOp(Code c, A c_in,
			BasicBlock succ) {
		return caseDefault(c,c_in, succ);
	}
	
	protected A caseCompareOp(Code c, A c_in, BasicBlock succ) {
		return caseDefault(c,c_in, succ);
	}
	
	protected A caseMoveOp(Code c, A c_in, BasicBlock succ) {
		return caseDefault(c,c_in, succ);
	}
	
	
	public void prettyPrint(){
		for(BasicBlock bb : bbList){
			prettyPrint(bb);
			
		}
	}
	

	public A getInSet(BasicBlock bb){
		return (A) bbInSet.get(bb);
	}

	public A getOutSet(BasicBlock bb) {
		return (A) codeOutSet.get(bb.getLastCode());
	}

	public A getMergedOutSet(ArrayList<BasicBlock> entryList) {
		A result = initSet();
		for(BasicBlock bb : entryList){
			result = merge(result, getOutSet(bb));
		}
		return result;
	}

	public void prettyPrint(BasicBlock bb) {
		System.out.println("****************************************************");
		System.out.println("Basic block start address: " + bb.getStartAddressHex());
		for(Code c: bb.getCode()){
			System.out.println(c);
			System.out.println("in: " + codeInSet.get(c));
			System.out.println("out: " + codeOutSet.get(c));
			System.out.println("-------------------------------");	
		}			
		
	}
}
