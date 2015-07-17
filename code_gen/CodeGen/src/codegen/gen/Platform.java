package codegen.gen;

public class Platform {
	Configuration config;
	Core[] coreList;
	
	int mainMemoryBase = 0x400000;
	public Platform(Configuration config){
		this.config = config;
		coreList = new Core[3];
		coreList[0] = new Core("cpu0",false,0x64020,0x30fe0,0);
		coreList[1] = new Core("cpu1",false,0x32020,0x30fe0,1);
		coreList[2] = new Core("cpum",true,0x20,0x30fe0);
	}
	void addFunctionsToCores() {
		for(Core c : coreList){
			for(Function f : config.funcList){
				if(f.cores.contains(c.name)){
					c.funcList.add(f);
				}
			}
		}
		
	}
	
	public Core getCore(String name) {
		for(Core c : coreList){
			if(c.name.equals(name)){
				return c;
			}
		}
		return null;
	}
	public void useDefaultBSPs() {
		for(int i = 0; i < coreList.length-1; i++){
			coreList[i].bspDir = config.outputDir + "/" + coreList[i].name + "_bsp";
		}
		coreList[coreList.length-1].bspDir = config.outputDir + "/cpuM_bsp";
	}
	
}
