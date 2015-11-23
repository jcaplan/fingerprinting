package codegen.test;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.FileNotFoundException;
import java.io.UnsupportedEncodingException;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import codegen.prof.Profiler;
import codegen.prof.flow.ReachingDef;

public class ReachingDefTest {
	
	String fileDir = "/home/jonah/fingerprinting/code_gen/CodeGen/test/ipetTest";
	String rootName = "ipet";
	String entryPoint;
	
	@Before
	public void beforeTest(){
		System.out.println("******************************");
	}
	
	@After
	public void afterTest(){
		System.out.println(entryPoint);
	}
	
	@Test
	public void testG(){
		entryPoint = "g";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		try {
			prof.getCfg().printDotCFG(entryPoint);
		} catch (FileNotFoundException | UnsupportedEncodingException e) {
			e.printStackTrace();
			fail(e.getMessage());
		}
		
		ReachingDef rd = new ReachingDef(prof.getCfg().getFunction(entryPoint));
		rd.analyze();
		rd.prettyPrint();
		
	}

	@Test
	public void testG2(){
		entryPoint = "g2";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		try {
			prof.getCfg().printDotCFG(entryPoint);
		} catch (FileNotFoundException | UnsupportedEncodingException e) {
			e.printStackTrace();
			fail(e.getMessage());
		}
		
		ReachingDef rd = new ReachingDef(prof.getCfg().getFunction(entryPoint));
		rd.analyze();
		rd.prettyPrint();
		
	}

}
