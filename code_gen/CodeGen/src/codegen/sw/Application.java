package codegen.sw;

import java.awt.Desktop;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.ConcurrentModificationException;

public class Application {
	ArrayList<Task> taskList;

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
				
				try {
					for (Task p : redundant.parents) {
						setTaskConnection(p, redundant);
					}
				} catch (ConcurrentModificationException e) {
				}
				;
				
			}
		}
		taskList.addAll(newTasks);
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
		} else {
			cmd = "open " + fileName + ".pdf";
			p = Runtime.getRuntime().exec(cmd);
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
		Task t1 = new Task("firstTask");
		Task t2 = new Task();
		Task t3 = new Task("print");
		Task t4 = new Task();
		a.addTask(t1);
		a.addTask(t2);
		t2.critical = true;
		a.addTask(t3);
		a.addTask(t4);
		a.setTaskConnection(t1, t2);
		a.setTaskConnection(t2, t1);
		a.setTaskConnection(t2, t3);
		a.setTaskConnection(t3, t4);
		Task t5 = new Task(true);
		Task t6 = new Task(true);
		a.addTask(t5);
		a.addTask(t6);
		a.setTaskConnection(t3, t5);
		a.setTaskConnection(t3, t6);
		a.setTaskConnection(t5, t4);
		a.setTaskConnection(t6, t4);
		a.printDotFile("app");
		a.openGraphPDF("app");
		 
		a.applyTaskTransformation();
		a.printDotFile("appT");
		a.openGraphPDF("appT");

		// Thread.sleep(750);
		// a.cleanTempFiles("app");

	}
}
