package codegen.test;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import codegen.prof.Loop;
import codegen.prof.Profiler;
import codegen.prof.flow.LoopAnalysis;

public class LoopAnalysisTest {
	
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
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(99,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());
	}
	
	@Test
	public void testG1(){
		entryPoint = "g1";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(99,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());

	}
	
	@Test
	public void testG2(){
		entryPoint = "g2";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(50,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());

	}
	
	@Test
	public void testG3(){
		entryPoint = "g3";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(399,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());

	}
	
	@Test
	public void testG4(){
		entryPoint = "g4";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);		
		assertEquals(400,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());

	}
	
	@Test
	public void testG5(){
		entryPoint = "g5";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(10,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());

	}
	
	@Test
	public void testG6(){
		entryPoint = "g6";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(100,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());

	}
	
	@Test
	public void testG7(){
		entryPoint = "g7";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
	}
	
	@Test
	public void testG8(){
		entryPoint = "g8";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(105,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());

	}
	
	@Test
	public void testG9(){
		entryPoint = "g9";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(105,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());

	}
	
	@Test
	public void testG10(){
		entryPoint = "g10";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(301,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());

	}
	
	@Test
	public void testG11(){
		entryPoint = "g11";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(101,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());
	}
	
	@Test
	public void testG12(){
		entryPoint = "g12";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertFalse(result);
	}
	
	@Test
	public void testG13(){
		entryPoint = "g13";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(200,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());
	}
	
	@Test
	public void testG14(){
		entryPoint = "g14";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(100,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());
	}
	
	@Test
	public void testG15(){
		entryPoint = "g15";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(50,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());
	}
	
	@Test
	public void testG16(){
		entryPoint = "g16";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		la.analyze();
		
	}
	
	@Test
	public void testG17(){
		entryPoint = "g17";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		la.analyze();
	}
	
	@Test
	public void testG18(){
		entryPoint = "g18";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(100,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());
	}
	
	@Test
	public void testG19(){
		entryPoint = "g19";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		assertEquals(100,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());
	}
	
	@Test
	public void testG20(){
		entryPoint = "g20";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertFalse(prof.getCfg().getFunction(entryPoint).getLoopList().size() > 0 && result);
	}
	
	@Test
	public void testG21(){
		entryPoint = "g21";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);	
		assertEquals(100,prof.getCfg().getFunction(entryPoint).getLoopList().get(0).getMaxIterations());
		
	}
	
	@Test
	public void testG22(){
		entryPoint = "g22";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertFalse(result);	
	}
	
	@Test
	public void testG23(){
		entryPoint = "g23";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);	
	}
	
	@Test
	public void testG24(){
		entryPoint = "g24";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		
	}
	
	@Test
	public void testG25(){
		entryPoint = "g25";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		
	}
	
	
	@Test
	public void testF(){
		entryPoint = "f";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		
		int count = 0;
		for(Loop l : prof.getCfg().getFunction(entryPoint).getLoopList()){
			count += l.getMaxIterations();
		}
		
		assertEquals(45+100+200+3,count);		
	}
	
	@Test
	public void matmulTest(){
		entryPoint = "matmul";
		Profiler prof = new Profiler(fileDir,rootName);
		prof.parseFile(entryPoint);
		LoopAnalysis la = new LoopAnalysis(prof.getCfg().getFunction(entryPoint));
		boolean result = la.analyze();
		assertTrue(result);
		
		int count = 0;
		for(Loop l : prof.getCfg().getFunction(entryPoint).getLoopList()){
			count += l.getMaxIterations();
		}
		
		assertEquals(384,count);		
	}
	
}
