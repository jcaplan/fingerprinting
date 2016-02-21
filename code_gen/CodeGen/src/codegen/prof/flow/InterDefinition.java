package codegen.prof.flow;

public class InterDefinition {
	String line;
	boolean merged;
	
	public InterDefinition(String context, int line){
		this.line = context + ":" + Integer.toHexString(line);
	}
	
	public InterDefinition(String line){
		this.line = line;
	}



	public String getLine(){
		return line;
	}
	
	@Override
	public boolean equals(Object o){
		if(!(o instanceof Definition)){
			return false;
		}
		Definition def = (Definition) o;
		return this.line.equals(def.line);
	}
	
	@Override
	public String toString(){
		return line;
	}

	public void setLine(String line) {
		this.line = line;
	}

	public InterDefinition copy() {
		InterDefinition def = new InterDefinition(this.line);
		def.merged = this.merged;
		return def;
	}
	
	public void setMerged(boolean merge){
		merged = merge;
	}
}
