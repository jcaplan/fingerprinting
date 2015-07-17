package codegen.gen;
import java.io.*;
import java.util.regex.*;

public class StackBin {

	
	Function[] bin = new Function[2];
	int[] startAddress = new int[2];
	public String name;
	public static final int size = 0x1000;
	public static final int STACKSIZE_MINOFFSET = 1264;
	public static final int STACKSIZE_MARGINERROR = 600;
	
	public StackBin(){
		
	}
	
	Function[] getBin(){
		return bin;
	}

	public int getStackEnd(Function f, int coreID) {
		int stackEnd = f.stackSize + STACKSIZE_MARGINERROR + STACKSIZE_MINOFFSET-4;
		int funcIndex = getFuncIndex(f);
		if(funcIndex == 1){
			stackEnd += getStackEnd(bin[0],coreID)+4;
		}
		
		return stackEnd - 8;
	}
	

	public int getStackStart(Function f, int coreID) {
		int stackStart = 0;
		int funcIndex = getFuncIndex(f);
		if(funcIndex == 1){
			stackStart = getStackEnd(bin[0],coreID)+4;
		} else if (funcIndex == 0){ 
			stackStart = 0;
		} else {
			stackStart = -1;
		}
		return stackStart;
		
	}

	private int getFuncIndex(Function f) {
		for(int i = 0 ; i < 2; i++){
		if (bin[i].equals(f)){
			return i;
		}
	}
		return -1;
	}

	public boolean existsInSettings(String bspSettings) throws IOException {
		File settings = new File(bspSettings);
		FileReader fr = new FileReader(settings);
		BufferedReader reader = new BufferedReader(fr);

		// initialize the reader and cfg
		reader = new BufferedReader(fr);
		String line = "";
		while ((line = reader.readLine()) != null) {

			if(line.contains(name)){
				reader.close();
				return true;
			}
		}
		
		reader.close();
		return false;
	}

	public boolean matchesSettings(String bspSettings, int core) throws IOException {
		File settings = new File(bspSettings);
		FileReader fr = new FileReader(settings);
		BufferedReader reader = new BufferedReader(fr);

		// initialize the reader and cfg
		reader = new BufferedReader(fr);
		String line = "";
		boolean foundName = false;
		while ((line = reader.readLine()) != null) {
			if (foundName) {
				if (line.contains("offset")) {
					Pattern p = Pattern.compile("-?\\d+");
					Matcher m = p.matcher(line);
					if (m.find()) {
						if(Integer.parseInt(m.group()) == startAddress[core]){
							reader.close();
							return true;
						}
					}
				} 

			}else if (line.contains(name)) {
				foundName = true;
			}
		}

		reader.close();
		return false;
	}

}
