package codegen.gen;
import java.io.*;
import java.util.regex.*;

/**
 * The StackBin object is used to store information about the stacks. Each bin can contain 0 or 1 functions.
 * @author jonah
 *
 */
public class StackBin {

	
	Function[] bin = new Function[2];
	int[] startAddress = new int[2];
	public String name;
	/**
	 * The size of a stack bin. Currently set to 4kB.
	 */
	public static final int size = 0x1000;
	/**
	 * The minimum offset for a stack (account for exception offsets
	 */
	public static final int STACKSIZE_MINOFFSET = 1264;
	/**
	 * Any buffer beyond the empirically measured minimum
	 */
	public static final int STACKSIZE_MARGINERROR = 600;
	
	public StackBin(){
		
	}
	
	/**
	 * 
	 * @return The array of functions stored in the bin
	 */
	Function[] getBin(){
		return bin;
	}

	/**
	 * @param f	The function. Assumed that check for function in stack has already been done.
	 * @param coreID	The desired core.
	 * @return	The end address for the stack of a function on a given core
	 */
	public int getStackEnd(Function f, int coreID) {
		int stackEnd = f.stackSize + STACKSIZE_MARGINERROR + STACKSIZE_MINOFFSET;
		int funcIndex = getFuncIndex(f);
		if(funcIndex == 1){
			stackEnd += getStackEnd(bin[0],coreID)+4;
		}
		
		return stackEnd - 4;
	}
	

	/**
	 * @param f	The function. Assumed that check for function in stack has already been done.
	 * @param coreID	The desired core.
	 * @return	The start address for the stack of a function on a given core
	 */
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

	/**
	 * Checks if function is first or second in the bin
	 * @param f
	 * @return -1 if fail
	 */
	private int getFuncIndex(Function f) {
		for(int i = 0 ; i < 2; i++){
		if (bin[i].equals(f)){
			return i;
		}
	}
		return -1;
	}

	/**
	 * Checks that stack bin name is in BSP settings
	 * @param bspSettings	BSP settings file location
	 * @return	True if bspSettings contains stack bin name
	 * @throws IOException
	 */
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

	
	/**
	 * Checks that the settings for the stack bin are up to date
	 * @param bspSettings	BSP settings file location
	 * @param core	Core specific
	 * @return	true if the settings are up to date for the given core
	 * @throws IOException
	 */
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
