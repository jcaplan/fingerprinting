package codegen.gen;

import java.util.ArrayList;

public class Function {

	String name;
	String codeDirectory;
	int period;
	boolean critical;
	ArrayList<Dataflow> dataflowList;
	ArrayList<String> cores;
	enum Type{periodic,eventDriven};
	Type type;
	
	public Function(){
		critical = false;
		dataflowList = new ArrayList<>();
		cores = new ArrayList<>();
	}


	public void addDataflow(Function dest, String sourceVariable,
			String destVariable) {
		//Type check the dataflow...
		Dataflow df = new Dataflow(this,dest,sourceVariable,destVariable);
		dataflowList.add(df);
		
	}
	
	@Override
	public String toString(){
		return name;
	}
}
