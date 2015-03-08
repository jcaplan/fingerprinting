package codegen.hw;

public class Bus {
	Port slavePort;
	Port masterPort;
	
	
	Bus(Port slavePort, Port masterPort){
		this.slavePort = slavePort;
		this.masterPort = masterPort;
	}
}
