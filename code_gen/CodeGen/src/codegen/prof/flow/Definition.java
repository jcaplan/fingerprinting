package codegen.prof.flow;

public class Definition {

	int line;
	boolean merged;
	
	public Definition(int line){
		this.line = line;
	}
	



	public int getLine(){
		return line;
	}
	
	@Override
	public boolean equals(Object o){
		if(!(o instanceof Definition)){
			return false;
		}
		Definition def = (Definition) o;
		return this.line == def.line;
	}
	
	@Override
	public String toString(){
		return Integer.toHexString(line) + "";
	}

	public void setLine(int line) {
		this.line = line;
	}

	public Definition copy() {
		Definition def = new Definition(this.line);
		def.merged = this.merged;
		return def;
	}
	
	public void setMerged(boolean merge){
		merged = merge;
	}
}
