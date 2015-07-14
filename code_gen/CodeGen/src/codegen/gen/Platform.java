package codegen.gen;

public class Platform {
	Configuration config;
	Core[] coreList;
	public Platform(Configuration config){
		this.config = config;
		coreList = new Core[3];
		coreList[0] = new Core("cpu0",false);
		coreList[1] = new Core("cpu1",false);
		coreList[2] = new Core("cpum",true);
		
		addFunctionsToCores();
	}
	private void addFunctionsToCores() {
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
	
}
