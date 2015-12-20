package codegen.test;

import static org.junit.Assert.*;
import org.junit.*;
import codegen.map.*;
import java.util.*;

public class SchedTest {
	
	
	@Test
	public void schedTest(){
		int[] period = {20,20,50,200,20,10,40,200,10,200};
		double[] clo = {1.06629,1.31022,11.9918,3.45817,0.388576,0.996887,4.48258,2.09066,0.173396,33.0145};
		double[] chi = {1.86754,1.32027,11.9918,3.45817,0.388576,0.996887,6.1738,3.54093,0.277926,33.0145,};
		int[] crit = {1,1,0,0,0,0,1,1,1,0};

		Schedule schedule = new Schedule();
		Processor p = new Processor("p0", true);
		
		ArrayList<Task> taskList = new ArrayList<>();
		for(int i = 0; i < period.length; i++){	
			boolean criticality = crit[i] == 1;
			Task t = new Task(clo[i], chi[i], period[i], criticality, "t" + i);
			int[] n;
			if(criticality){
				int[] def = {1,2,1,2};
				n = def;
			} else {
				int[] def = {1,1,1,1};
				n = def;
			}
			t.setMaxNumReexecutions(n);
			taskList.add(t);
			schedule.allocate(t, p);
		}
		ArrayList<Processor> procList = new ArrayList<>();
		procList.add(p);
		
		SchedAnalysis schedAnalysis = new SchedAnalysis(taskList, schedule, procList);
		System.out.println("system schedulable: " + schedAnalysis.schedAnalysis());
		System.out.println(schedule);
		for(Task t : taskList){
			System.out.print(t.name + ": ");
			for(int i = 0; i < 4; i++){
				System.out.print(schedule.getResponseTime(t, i) +", ");	
			}
			System.out.println("");
		}
		
		double[] qos = schedule.getQosPerMode();
		double[] expected = {1,0.8,1,0.6};
		for(int i = 0; i < 4; i++){
			assertTrue(Math.abs(expected[i] - qos[i]) < 0.01);
		}
		
		for(int i = 0; i < qos.length; i++){
			System.out.print(qos[i] + ",");
		}
		System.out.println("");
		
		
		double[][] cResults = {{2.23657084,3.47625518,3.14235497,5.2878232},
				{3.54679537,6.09670448,4.46262741,7.92836714},
				{25.5154209,-1,29.1428432,-1},
				{28.9735889,56.8671684,33.8758202,79.6239319},
				{3.9353714,6.48528004,4.85120296,8.31694317},
				{0.99688679,0.99688679,0.99688679,0.99688679},
				{8.41794682,16.7941113,12.2998199,32.0869713},
				{32.2345314,67.5337677,37.4167557,118.792778},
				{1.17028332,1.34367967,1.27481306,1.55273938},
				{118.620407,199.744766,148.417572,-1}
				};
		String[] modes = {"LO","TF","OV","HI"};
		for(int i = 0; i < taskList.size(); i++){
			for(int j = 0; j < 4; j++){
				double rt = schedule.getResponseTime(taskList.get(i), j);
				assertTrue("testing task " + i + " mode " + modes[j], 
						Math.abs(rt - cResults[i][j]) < 0.01 || 
						(rt == 0 && cResults[i][j] == -1));
			}
		}
	}

}
