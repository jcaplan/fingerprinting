package codegen.test;

import static org.junit.Assert.fail;

import java.io.FileNotFoundException;
import java.io.UnsupportedEncodingException;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import codegen.prof.Profiler;
import codegen.prof.flow.InterReachingDef;
import codegen.prof.flow.ReachingExp;

public class InterReachingDefTest {
	
	String fileDir = "/home/jonah/fingerprinting/code_gen/CodeGen/test/interTest";
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
	public void testMain(){
		entryPoint = "main";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		
		InterReachingDef rd = new InterReachingDef(prof.getCfg().getFunction(entryPoint));
		rd.analyze();
		rd.prettyPrint();
	}

}
