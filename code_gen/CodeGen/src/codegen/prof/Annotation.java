package codegen.prof;

public class Annotation {

	int address;
	int value;
	final int MAX_ITER_TYPE = 1;
	int type;
	
	public Annotation(int address, int value){
		this.address = address;
		this.value = value;
		this.type = MAX_ITER_TYPE;
	}
	
	public Annotation(int address, int value, int type){
		this.address = address;
		this.value = value;
		this.type = type;
	}
	
}
