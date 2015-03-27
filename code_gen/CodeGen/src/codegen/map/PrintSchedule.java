package codegen.map;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;

import codegen.sw.Application;
import codegen.sw.Task;

public class PrintSchedule {

	static final int SCALE = 15;
	public static void print(int numCores, int[] coreAssignments,
			int[] startTimes, Application app, String fileName) throws IOException, InterruptedException {
		String s = "";
		s += printTopMatter();
		//What is the execution time?
		//last start time + 100
		int numTasks = startTimes.length;
		int lastStart = 0;
		for(int i = 0; i < numTasks; i++){
			if(startTimes[i] > lastStart)
				lastStart = startTimes[i];
		}
		lastStart += 100;
		lastStart /= SCALE;
		s += "\\begin{RTGrid}[width=\\linewidth,nosymbols=1]{" + numCores + "}{" + lastStart + "}\n";
		for(int i = 0; i < numCores; i++){
			s += "\\RowLabel{" + (i + 1) + "}{CPU" + i + "}\n";
			for(int j = 0; j < numTasks; j++){
				Task task = app.getTaskList().get(j);
				if(coreAssignments[j] == i){
					s += "\\TaskExecution[execlabel=" + task.label;
					if(task.isCritical())
						s += ",color=red";
					s += "]";
					s += "{" + (i + 1) + "}{" + startTimes[j]/SCALE + 
								"}{" + (startTimes[j] + task.executionTime)/SCALE + "}\n";
				}
			}
		}
		s += "\\end{RTGrid}\n";
		s += "\n\\end{document}\n";
		
		PrintWriter writer = new PrintWriter(fileName + ".tex", "UTF-8");
		writer.println(s);
		writer.close();
		
		generatePDF(fileName);
		openGraphPDF(fileName);
	}

	static void openGraphPDF(String fileName) throws IOException, InterruptedException {
		String cmd;
		Process p;
		if (System.getProperty("os.name").equals("Linux")) {
			cmd = "gnome-open " + fileName + ".pdf";
			p = Runtime.getRuntime().exec(cmd);
		} else if (System.getProperty("os.name").equals("Mac OS X")) {
			cmd = "open " + fileName + ".pdf";
			p = Runtime.getRuntime().exec(cmd);
		} else {
			cmd = "rundll32 url.dll,FileProtocolHandler " + fileName + ".pdf";
			p = Runtime.getRuntime().exec(cmd);
		}
		p.waitFor();
	}
	
	static void generatePDF(String fileName) throws IOException, InterruptedException {
		String cmd = "latex " + fileName + ".tex";
		Process p = Runtime.getRuntime().exec(cmd);
		p.waitFor();
		cmd = "dvips " + fileName + ".dvi";
		p = Runtime.getRuntime().exec(cmd);
		p.waitFor();
		cmd = "ps2pdf " + fileName + ".ps";
		p = Runtime.getRuntime().exec(cmd);
		p.waitFor();
	}
	
	
	private static String printTopMatter() {
		String s = "";
		s += "%----------------------------------------------------------------------------------------\n";
		s += "%	PACKAGES AND OTHER DOCUMENT CONFIGURATIONS\n";
		s += "%----------------------------------------------------------------------------------------\n";
		s += "\n";
		s += "\\documentclass{article}\n";
		s += "\\usepackage[landscape]{geometry}\n";
		s += "\\usepackage[usenames,dvipsnames]{color} % Required for custom colors\n";
		s += "\\usepackage{rtsched/rtsched}\n";
		s += "\n";
		s += "\\begin{document}\n";
		return s;
	}
	
	

}
