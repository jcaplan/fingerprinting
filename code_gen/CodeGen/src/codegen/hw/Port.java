package codegen.hw;

public class Port {
	
	Bus bus;
	int startAddress;
	int size;
	int endAddress;
	
	
	Port(Bus b, int startAddress, int size){
		this.bus = b;
		this.size = size;
		this.startAddress = startAddress;

	}

}
