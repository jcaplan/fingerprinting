package codegen.sw;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.ConcurrentModificationException;
import java.util.Random;

public class Application {
	ArrayList<Task> taskList;
	int rankIndex[];

	public Application() {
		taskList = new ArrayList<Task>();
	}

	void addTask(Task t) {
		taskList.add(t);
	}

	void setTaskConnection(Task parent, Task child) {
		parent.children.add(child);
		child.parents.add(parent);
	}

	public String toString() {
		String s = "digraph d {\n";
		for (Task t : taskList) {
			for (Task adj : t.children) {
				s += "\t" + t + "->" + adj + ";\n";
			}
			if (t.redundant) {
				s += "\t" + t + " [style=filled];\n";
			}
			if (t.critical && !t.redundant) {
				s += "\t" + t + " [peripheries=2];\n";
			}
		}
		int count = 0;
		for (int i = 0; i < rankIndex.length; i++) {
			s += "\t{ rank = same; ";
			for (int j = count; j < rankIndex[i]; j++) {
				Task t = taskList.get(j);
				s += t + "; ";
				if (t.critical && !t.redundant && t.copy != null)
					s += t.copy + "; ";
			}
			s += "}\n";
		}
		s += "}";
		return s;
	}

	public void applyTaskTransformation() {
		ArrayList<Task> newTasks = new ArrayList<Task>();
		for (Task t : taskList) {
			if (t.critical) {
				Task redundant = new Task(t, true);
				newTasks.add(redundant);
				t.copy = redundant;
			}
		}
		taskList.addAll(newTasks);
		for (Task t : newTasks) {
			for (Task p : t.parents){
				p.children.add(t);
			}
		}
	}

	void printDotFile(String fileName) throws FileNotFoundException,
			UnsupportedEncodingException {
		PrintWriter writer = new PrintWriter(fileName + ".dot", "UTF-8");
		writer.println(this);
		writer.close();
	}

	void openGraphPDF(String fileName) throws IOException, InterruptedException {
		String cmd = "dot -Tpdf " + fileName + ".dot -o " + fileName + ".pdf";
		Process p = Runtime.getRuntime().exec(cmd);
		p.waitFor();
		if (System.getProperty("os.name").equals("Linux")) {
			cmd = "gnome-open " + fileName + ".pdf";
			p = Runtime.getRuntime().exec(cmd);
		} else if (System.getProperty("os.name").equals("Mac OS X")) {
			cmd = "open " + fileName + ".pdf";
			p = Runtime.getRuntime().exec(cmd);
		} else {
			cmd = "rundll32 url.dll,FileProtocolHandler " + fileName + ".pdf";
			Runtime.getRuntime().exec(cmd);
		}
	}

	void cleanTempFiles(String fileName) throws IOException,
			InterruptedException {
		String cmd = "rm " + fileName + ".dot";
		Process p = Runtime.getRuntime().exec(cmd);
		p.waitFor();
		cmd = "rm " + fileName + ".pdf";
		p = Runtime.getRuntime().exec(cmd);
	}

	public static void main(String[] args) throws IOException,
			InterruptedException {
		Application a = new Application();
		// Task t1 = new Task("firstTask");
		// Task t2 = new Task();
		// Task t3 = new Task("print");
		// Task t4 = new Task();
		// Task t5 = new Task(true);
		// Task t6 = new Task(true);
		// t2.critical = true;
		// a.addTask(t1);
		// a.addTask(t2);
		// a.addTask(t3);
		// a.addTask(t4);
		// a.addTask(t5);
		// a.addTask(t6);
		// a.setTaskConnection(t1, t2);
		// a.setTaskConnection(t2, t1);
		// a.setTaskConnection(t2, t3);
		// a.setTaskConnection(t3, t4);
		// a.setTaskConnection(t3, t5);
		// a.setTaskConnection(t3, t6);
		// a.setTaskConnection(t5, t4);
		// a.setTaskConnection(t6, t4);

		// Digraph g = DigraphGenerator.simple(12,20);
		// Task t[] = new Task[12];
		// for(int i = 0; i < 12; i++){
		// t[i] = new Task();
		// if(i%3 == 0)
		// t[i].critical = true;
		// a.addTask(t[i]);
		// }
		//
		// for(int i = 0; i < 12; i++){
		// for(int j : g.adj(i)) {
		// a.setTaskConnection(t[i], t[j]);
		// }
		// }

		DigraphGen g = new DigraphGen(2, 4, 6, 0.3, 0.25);
		g.buildGraph(a);
		a.rankIndex = g.rankIndex;
		a.printDotFile("app");
		a.openGraphPDF("app");
		a.applyTaskTransformation();
		a.printDotFile("appT");
		a.openGraphPDF("appT");

		// Thread.sleep(750);
		// a.cleanTempFiles("app");

	}
}
