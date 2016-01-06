package codegen.gen;

/**
 * Platform holds basic info about default platform. Holds list of cores,
 * @author jonah
 *
 */
public class Platform {
	Configuration config;
	Core[] coreList;
	
	int mainMemoryBase = 0x400000;
	/**
	 * Build platform from configuration
	 * @param config
	 */
	public Platform(Configuration config){
		this.config = config;
		coreList = new Core[3];
		coreList[0] = new Core("cpu0",false,0x64020,0x31fe0,0);
		coreList[1] = new Core("cpu1",false,0x32020,0x31fe0,1);
		coreList[2] = new Core("cpuM",true,0x20,0x30fe0,2);
	}
	
	/**
	 * Builds list of functions for each core
	 */
	void addFunctionsToCores() {
		for(Core c : coreList){
			for(Function f : config.funcList){
				if(f.cores.contains(c.name)){
					c.funcList.add(f);
				}
			}
		}
		
	}
	
	/**
	 * Searches for a core based on its name
	 * @param name	Name of core
	 * @return	Core object with matching name
	 */
	public Core getCore(String name) {
		for(Core c : coreList){
			if(c.name.equals(name)){
				return c;
			}
		}
		return null;
	}
	
	/**
	 * Assigns the defaultBSP location for each core.
	 */
	public void useDefaultBSPs() {
		for(int i = 0; i < coreList.length-1; i++){
			coreList[i].bspDir = config.outputDir + "/" + coreList[i].name + "_bsp";
		}
		coreList[coreList.length-1].bspDir = config.outputDir + "/cpuM_bsp";
	}
	
}
