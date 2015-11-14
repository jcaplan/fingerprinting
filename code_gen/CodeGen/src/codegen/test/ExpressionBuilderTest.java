package codegen.test;

import java.io.FileNotFoundException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

import javax.management.RuntimeErrorException;

import codegen.prof.BasicBlock;
import codegen.prof.Loop;
import codegen.prof.Profiler;
import codegen.prof.flow.ExpressionBuilder;

public class ExpressionBuilderTest {
	
	public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException{
		
		if(args.length != 3){
			throw new RuntimeErrorException(new Error("Usage java -jar Codegen.jar directory rootName entryFunction"));
			
		}
		
		
		
		/*
		 * Three arguments are required:
		 * 1. The directory where the files to be parsed are located
		 * 2. The file root name. The two files should be called rootName.objdump
		 *    and rootName.annot
		 * 3. The entry function for the analysis.
		 * 
		 * 
		 */
		String fileDir=args[0];
		String rootName = args[1];
		String entryPoint = args[2];
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		prof.getCfg().printDotCFG(entryPoint);
		
		ExpressionBuilder builder;
		Loop l = prof.getCfg().getFunction(entryPoint).getLoopList().get(0);
		
		
//		ExpressionBuilder builder = new ExpressionBuilder(l.getBody());
//		builder.analyzeBasicBlock(l.getHead(), null, new HashMap<String,Expression>());
//		builder.prettyPrint(l.getHead());
		
//		ExpressionBuilder builder = new ExpressionBuilder(l.getBody());
//		BasicBlock bb = l.getBody().get(1);
//		builder.analyzeBasicBlock(bb, null, new HashMap<String,Expression>());
//		builder.prettyPrint(bb);

		
		ArrayList<BasicBlock> bbList = new ArrayList<>(l.getBody());
		bbList.remove(l.getHead());
//		bbList.remove(l.getTail());
		
		builder = new ExpressionBuilder(bbList);
		builder.analyze();
		builder.prettyPrint();
//		
//		bbList.clear();;
//		bbList.add(l.getHead());
//		
//		builder = new ExpressionBuilder(bbList);
//		builder.analyze();
//		builder.prettyPrint();
		
		
//		builder = new ExpressionBuilder(prof.getCfg().getFunction(entryPoint));
//		builder.analyze();
//		builder.prettyPrint(l.getHead());
	}
}
