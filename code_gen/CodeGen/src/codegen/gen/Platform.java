package codegen.gen;

/**
 * Platform holds basic info about default platform. Holds list of cores,
 * @author jonah
 *
 */
public class Platform {
	Configuration config;
	Core[] coreList;
	
	int[] startAddresses = {
			0x64020,
			0x32020,
			0x96020,
			0xc8020
	};
	int memSize = 0x31fe0; //auto-include 0x1000 for sp bin or global data bin
	int numProcessingCores = 0;
	int mainMemoryBase = 0x400000;
	/**
	 * Build platform from configuration
	 * @param config
	 */
	public Platform(Configuration config,int numProcessingCores){
		this.config = config;
		this.numProcessingCores = numProcessingCores;
		addCores(numProcessingCores);
	}
	
	

	private void addCores(int numProcessingCores) {
		coreList = new Core[numProcessingCores + 1];
		for(int i = 0; i < numProcessingCores; i++){
			coreList[i] = new Core("cpu" + i, false, startAddresses[i],memSize,i);
		}
		//global data section consumes 4kB on monitor
		coreList[numProcessingCores] = new Core("cpuM",true,0x20,memSize - 0x1000,numProcessingCores);
	}

	/**
	 * Builds list of functions for each core
	 */
	void addFunctionsToCores() {
		for(Core c : coreList){
			for(Function f : config.funcList){
				if(f.cores.contains(c)){
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
	
	public Core getCore(int index){
		return coreList[index];
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

	public int getNumProcessingCores() {
		return numProcessingCores;
	}
	
}
