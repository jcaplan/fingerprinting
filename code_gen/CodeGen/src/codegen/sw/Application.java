package codegen.sw;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

import codegen.map.RAGene;
import codegen.util.Qsort;

public class Application {
	
	/**
	 * The application is a collection of tasks.
	 * The task graph is transformed in the application class.
	 * Also the printing to .dot files.
	 * 
	 * 
	 */
	ArrayList<Task> taskList;
	ArrayList<Task> redundantList;
	int maxRank = 5;

	public ArrayList<Task> getTaskList() {
		return taskList;
	}

	public ArrayList<Task> getRedundantList() {
		return redundantList;
	}

	public Application() {
		Task.taskCount = 0;
		taskList = new ArrayList<Task>();
		redundantList = new ArrayList<Task>();
	}

	void addTask(Task t) {
		taskList.add(t);
	}

	void setTaskConnection(Task parent, Task child) {
		parent.addChild(child);
		child.addParent(parent);
		if (child.rank <= parent.rank) {
			child.rank = parent.rank + 1;
		}
	}

	private String printRanks() {
		String s = new String();
		// Print the header
		s += "node [shape=plaintext, fontsize=1, fontcolor=white, fixedsize=1];\n";
		for (int i = 1; i <= maxRank; i++) {
			s += i + "->" + (i + 1) + "[color=white];\n";
		}
		for (int i = 0; i <= maxRank; i++) {
			s += "\t{ rank = same; " + (i + 1) + "; ";
			for (Task t : taskList) {
				if (t.rank == i) {
					s += t + "; ";
				}
			}

			s += "}\n";
		}
		return s;
	}

	private String printConnections(Task t) {
		String s = new String();
		for (Task adj : t.children) {
			s += "\t" + t + "->" + adj + ";\n";
		}
		return s;
	}

	private String printParameters(Task t) {
		String s = new String();
		if (t.redundant) {
			s += "\t" + t + " [style=filled];\n";
		}
		if (t.critical && !t.redundant) {
			s += "\t" + t + " [style=filled,fillcolor=crimson];\n";
		}
		if (t.parents.size() == 0 || t.children.size() == 0) {
			s += "\t" + t + " [peripheries=2];\n";
		}
		return s;
	}

	public String toString() {
		String s = "digraph d {\n";
		s += "size=\"8.5,7\";\n";
		s += "splines=ortho\n;";
		for (Task t : taskList) {
			s += printConnections(t);
			s += printParameters(t);
		}
		s += printRanks();

		s += "}";
		return s;
	}

	public void transformationAllTasks() {
		ArrayList<Task> newTasks = new ArrayList<Task>();
		for (Task t : taskList) {
			if (t.critical && !t.redundant && t.copy == null) {
				Task redundant = new Task(t, true);
				newTasks.add(redundant);
				t.copy = redundant;
				for (Task c : redundant.children) {
					c.addParent(redundant);
				}
				for (Task p : redundant.parents) {
					p.addChild(redundant);
				}
			}
		}
		taskList.addAll(newTasks);
		Qsort.sort(taskList);
		redundantList = newTasks;
	}
	
	public void transformTasks() {
		ArrayList<Task> newTasks = new ArrayList<Task>();
		for (Task t : taskList) {
			if (t.critical && !t.redundant && t.copy == null
					&& t.fdTechnique == RAGene.FD_FPRINT) {
				Task redundant = new Task(t, true);
				newTasks.add(redundant);
				t.copy = redundant;
				for (Task c : redundant.children) {
					c.addParent(redundant);
				}
				for (Task p : redundant.parents) {
					p.addChild(redundant);
				}
			}
		}
		taskList.addAll(newTasks);
		Qsort.sort(taskList);
		redundantList = newTasks;	
	}

	public void cleanTransformation() {
		if(redundantList == null){
			return;
		}
		taskList.removeAll(redundantList);
		for (Task t : taskList) {
			t.cleanList(redundantList);
		}
		redundantList = new ArrayList<Task>();
	}
	
	public void build(){
		for(Task t : taskList){
			if(t.parents.isEmpty())
				t.changeRank(0);
			else
				t.changeRank(1);
		}	
		
		for(Task t : taskList){
			if(t.rank == 0){
				setRanks(t, 1);
			}
		}
		Qsort.sort(taskList);
	}
	
	private void setRanks(Task t, int rank){
		for(Task child : t.children){
			setRanks(child, rank + 1);
			if(rank > child.rank)
				child.changeRank(rank);
		}
	}
	

	public void printDotFile(String fileName) throws FileNotFoundException,
			UnsupportedEncodingException {
		PrintWriter writer = new PrintWriter(fileName + ".dot", "UTF-8");
		writer.println(this);
		writer.close();
	}

	void generatePDF(String fileName) throws IOException, InterruptedException {
		String cmd = "dot -Tpdf " + fileName + ".dot -o " + fileName + ".pdf";
		Process p = Runtime.getRuntime().exec(cmd);
		p.waitFor();
	}

	void openGraphPDF(String fileName) throws IOException, InterruptedException {
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

	public void getGraphs(String fileName) throws IOException,
			InterruptedException {
		printDotFile(fileName);
		generatePDF(fileName);
		openGraphPDF(fileName);

	}

	void getGraphs() throws IOException, InterruptedException {
		getGraphs("app");
	}

	void cleanTempFiles(String fileName) throws IOException,
			InterruptedException {
		String cmd = "rm " + fileName + ".dot";
		Process p = Runtime.getRuntime().exec(cmd);
		p.waitFor();
		cmd = "rm " + fileName + ".pdf";
		p = Runtime.getRuntime().exec(cmd);
	}

}
