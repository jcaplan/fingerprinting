package codegen.gen;
import java.io.*;
import java.util.regex.*;

/**
 * The StackBin object is used to store information about the stacks. Each bin can contain 0 or 1 functions.
 * @author jonah
 *
 */
public class StackBin {

	
	Function function;
	int startAddress;
	public String name;
	int numFunctions = 0;
	Core core;
	/**
	 * The size of a stack bin. Currently set to 4kB.
	 */
	public static final int SIZE = 0x1000;
	/**
	 * The minimum offset for a stack (account for exception offsets
	 */
	public static final int STACKSIZE_MINOFFSET = 1264;
	/**
	 * Any buffer beyond the empirically measured minimum
	 */
	public static final int STACKSIZE_MARGINERROR = 600;
	
	public StackBin(Core c, int index){
		core = c;
		name = "stack_bin_" + index;
	}
	


	/**
	 * @param f	The function. Assumed that check for function in stack has already been done.
	 * @param coreID	The desired core.
	 * @return	The end address for the stack of a function on a given core
	 */
	public int getStackEnd(Function f) {
		return SIZE - 4;
	}
	

	/**
	 * @param f	The function. Assumed that check for function in stack has already been done.
	 * @param coreID	The desired core.
	 * @return	The start address for the stack of a function on a given core
	 */
	public int getStackStart(Function f) {
		return 0;
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
	public boolean matchesSettings(String bspSettings) throws IOException {
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
						if(Integer.parseInt(m.group()) == startAddress){
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
	
	public void setFunction(Function f){
		function = f;
		f.stackBin.put(core.index,this);
	}
	
	
}
