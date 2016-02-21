package codegen.prof.flow;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Stack;

import codegen.prof.BasicBlock;
import codegen.prof.Code;
import codegen.prof.Function;

public abstract class InterForwardAnalysis<A> implements ForwardAnalysisInterface<A> {

	/*
	 * Interprocedural analysis requires a context for each basic function (and containing blocks/code)
	 * The context must also be easy to identify from the IPET analysis to identify each edge for different cost.
	 * 
	 * Context represented by a string or stack of function call sites.
	 * Instead of one codeInSet/Outset, search based on context.
	 * 
	 * Make a context a string of function names separated by semicolons
	 * f calls g calls h -> "f;g;h"
	 * 
	 * Then define push and pop by tokenizing strings and adding/removing tokens
	 * and building a new string
	 * 
	 * 
	 * 
	 */
	
	
	
	Stack<Function> callStack = new Stack<>();
	Function func;
	
	//Context 
	// key is the context
	// each context has a codeInSet, codeOutSet, and bbInSet
	// need to supply a context when requesting codeInSet and codeOutSet

	Map<String,List<BasicBlock>> bbListC;
	Map<String,List<BasicBlock>> workListC;
	Map<String,Map<Code,A>> codeInSetC;
	Map<String,Map<Code,A>> codeOutSetC;
	Map<String,Map<BasicBlock, A>>  bbInSetC;
	
	Set<String> seenContexts;
	
	//reference to current context variables
	List<BasicBlock> bbList;
	List<BasicBlock> workList;
	Map<Code,A> codeInSet;
	Map<Code,A> codeOutSet;
	Map<BasicBlock, A>  bbInSet;
	
	String context;
	
	public InterForwardAnalysis(Function root) {
		func = root;
		context = func.getStartAddressHex();
		bbListC = new HashMap<>();
		workListC = new HashMap<>();
		codeInSetC = new HashMap<>();
		codeOutSetC = new HashMap<>();
		bbInSetC = new HashMap<>();
		seenContexts = new HashSet<>();
		
		setBBList(new ArrayList<>(func.getBasicBlockList()));
	}	
	
	public InterForwardAnalysis(List<BasicBlock> bbList) {
		setBBList(bbList);
	}	
	
	public void setBBList(List<BasicBlock> bbList) {
		this.bbList = bbList;
		resetAnalysis();
	}

	public void analyze(){
		
		analysisPreprocessing();
		
		seenContexts.add(context);		
		
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

	protected void analysisPreprocessing() {
		
	}

	public void resetAnalysis() {
		//Initialize the data structures
		codeInSet = new HashMap<Code, A>();
		codeOutSet = new HashMap<Code, A>();
		bbInSet = new HashMap<BasicBlock, A>();
		workList = new ArrayList<>(bbList);
		
		//Store them by context
		codeInSetC.put(context,codeInSet);
		codeOutSetC.put(context,codeOutSet);
		bbInSetC.put(context, bbInSet);
		workListC.put(context, workList);
		bbListC.put(context, bbList);
		
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
		case PHI:
			result = casePhiOp(c, c_in, succ);
			break;
		default:
			result = caseDefault(c, c_in, succ);
			break;
		
		}
		
		
		return result;
	}

	




	protected abstract A merge(A set1, A set2);
	
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
		String callee = c.getOperands()[1];
		Function calledF = func.getCalledFunction(callee);
		pushContext(c,calledF);
		bbInSet.put(bbList.get(0), c_in);
		
		analyze();
		
		A c_out = codeOutSet.get(calledF.getReturnBlock().getLastCode());
		
		popContext();
		
		return c_out;
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
	
	protected A casePhiOp(Code c, A c_in, BasicBlock succ) {
		return caseDefault(c,c_in, succ);
	}

	
	public void prettyPrint(){
		for(String context : seenContexts){
			System.out.println("**********CONTEXT:" + context + "******************");
			loadContext(context);
			for(BasicBlock bb : bbList){
				prettyPrint(bb);
				
			}
		}
		
	}
	
	public void prettyPrintBlockList(){
		for(BasicBlock bb : bbList){
			System.out.println(bb + ": " + bbInSet.get(bb));
			
		}
	}
	

	public A getInSet(BasicBlock bb){
		return (A) bbInSet.get(bb);
	}
	
	public A getInSet(Code c){
		return (A) codeInSet.get(c);
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
	
	
	public void pushContext(Code c, Function callee){
		context += ";" + c.getAddressHex();
		//Check if first occurrence of context
		callStack.push(func);
		func = callee;
		if(!seenContexts.contains(context)){
			setBBList(new ArrayList<>(callee.getBasicBlockList()));
		} else {
			loadContext(context);
			//Add the root back (list should always be empty here but check anyway)
			if(!workList.contains(bbList.get(0))){
				workList.add(0,bbList.get(0));
			}
		}
	}
	
	public void popContext(){
		context = context.substring(0, context.lastIndexOf(";"));
		func = callStack.pop();
		loadContext(context);
		
	}

	private void loadContext(String cont) {
		codeInSet = codeInSetC.get(cont);
		codeOutSet = codeOutSetC.get(cont);
		bbInSet = bbInSetC.get(cont);
		workList = workListC.get(cont);
		bbList = bbListC.get(cont);
		
	}
}
