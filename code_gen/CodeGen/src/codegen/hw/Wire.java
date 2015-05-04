package codegen.hw;

public class Wire {
	Port slavePort;
	Port masterPort;
	int width;
	
	Wire(Port slavePort, Port masterPort, int width){
		this.slavePort = slavePort;
		this.masterPort = masterPort;
		this.width = width;
	}
	
	
}
