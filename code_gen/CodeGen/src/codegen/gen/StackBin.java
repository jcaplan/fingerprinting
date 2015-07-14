package codegen.gen;


public class StackBin {

	
	Function[] bin = new Function[2];
	int[] startAddress = new int[2];
	public String name;
	public static final int size = 0x1000;
	
	public StackBin(){
		
	}
	
	Function[] getBin(){
		return bin;
	}
	
}
