package codegen.gen;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;

/**
 * Class for creating stack bin objects and generating scripts that update BSP.
 * "StackBins" are chunks of main memory placed in separate region in order to ensure
 * that stacks, when allocated to those regions, are page aligned with each other.
 * Otherwise the stacks will never match on redundant tasks.
 * @author jonah
 *
 */
public class GenStackBin {


	ArrayList<StackBin> stackBins;
	ArrayList<Function> fprintList;
	Platform platform;
	Configuration config;
	
	/**
	 * Constructor
	 * @param stackBins	An empty array to place the stack bins passed from CodeGen
	 * @param fprintList	The list of fingerprinted functions
	 * @param platform	The platform
	 * @param config	The configuration file
	 */
	public GenStackBin(ArrayList<StackBin> stackBins, ArrayList<Function> fprintList, Platform platform,
			Configuration config){
		this.fprintList = fprintList;
		this.stackBins = stackBins;
		this.platform = platform;
		this.config = config;
	}
	
	/**
	 * This method is called to find the stack bins and create and run the scripts that update the
	 * BSP.
	 */
	public void genStackBins() {
		getStackBins();
		setStackBinAddresses();
		try {
			updateStackBinRegions();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * This method creates the StackBin objects
	 */
	private void getStackBins() {
		// MIN_OFFSET = 1256
		// SAFETY = 80
		// MIN_OFFSET + SAFETY + f.stacksize > 2048
		// i.e. f.stacksize < 712
		// TODO : Need separate stack bins to ensure pages align for more than 2
		// cores

		// how do you organize the calculation/storage of these data structures?

		// first fit bin packing
		// sort functions by stack size
		// then stick them in bins

		// ////////////////////////////////////////////////

		
		@SuppressWarnings("unchecked")
		ArrayList<Function> fprintListCopy = (ArrayList<Function>) fprintList.clone();
		Collections.sort(fprintListCopy, Function.stackCompareDecreasing);
		if (fprintListCopy.get(0).stackSize > 2760) {
			// TODO error
		}
		stackBins = new ArrayList<>();
		int binIndex = 0;
		int currentBinSize = 0;
		int currentBinIndex = 0;
		for (Function f : fprintListCopy) {
			Function[] bin = null;
			if (currentBinIndex == 1 && (f.stackSize + currentBinSize < 1424)) {
				bin = stackBins.get(binIndex).getBin();
				bin[1] = f;
				f.stackBin = stackBins.get(binIndex);
				binIndex++;
				currentBinIndex = 0;
			} else {
				stackBins.add(new StackBin());
				if(currentBinIndex == 1){
					binIndex++;
				}
				bin = stackBins.get(binIndex).getBin();
				bin[0] = f;
				f.stackBin = stackBins.get(binIndex);
				currentBinSize = f.stackSize;
				currentBinIndex = 1;
			}
		}
		
		for(int i = 0; i < stackBins.size(); i++){
			StackBin sb = stackBins.get(i);
			sb.name = "stack_bin_" + i;
		}
	}
	
	/**
	 * This method calculates the start addresses for each stack bin for each core
	 */
	private void setStackBinAddresses() {
		for(int i = 0; i < stackBins.size(); i++){
			StackBin sb = stackBins.get(i);
			Core c = platform.getCore("cpu0");
			sb.startAddress[0] = c.mainMemStartAddressOffset+c.mainMemSize - (i*StackBin.size);
			c = platform.getCore("cpu1");
			sb.startAddress[1] = c.mainMemStartAddressOffset+c.mainMemSize - (i*StackBin.size);
		}

		updateMainMemorySize();
	}
	
	/**
	 * This method finds the new (reduced) main memory size
	 */
	private void updateMainMemorySize() {
		Core c = platform.getCore("cpu0");
		c.mainMemSize -= (stackBins.size() - 1) * StackBin.size;
		c = platform.getCore("cpu1");
		c.mainMemSize -= (stackBins.size()-1) * StackBin.size;
		
	}
	

	/**
	 * This method creates and runs the script to update the stack regions in the BSP 
	 * @throws IOException
	 */
	private void updateStackBinRegions() throws IOException {
		int numBins = 0;
		if((numBins = stackBins.size()) > 1) { /* default num of stack bins */
			String cmd = "";
			cmd +=  "#!/bin/bash"+
					"\n"+
					"\n"+
					"OUTPUT_DIR=" + config.outputDir + "\n"+
					"NIOS2COMMANDSHELL=" + config.niosSBT.sbtLocation + "\n"+
					"\n";
			Core c = platform.getCore("cpu0");
			cmd += updateBspStackBins(numBins,c,0,stackBins);
			c = platform.getCore("cpu1");
			cmd += updateBspStackBins(numBins, c,1,stackBins);
			
			if(!cmd.isEmpty()){
			File file = new File(config.outputDir + "/update_bsp_mem_regions.sh");
			PrintWriter writer;
			
			
			writer = new PrintWriter(file);
			writer.print(cmd);
			writer.close();
			
			file.setExecutable(true);

			//Instead of checking inside StackBin,
			//parse once here, check every line for all stack bins,
			
			c = platform.getCore("cpu0");
			config.niosSBT.updateBspStackBins(numBins,c,0,stackBins);
			c = platform.getCore("cpu1");
			config.niosSBT.updateBspStackBins(numBins, c,1,stackBins);
			}
		}
	}
	
	/**
	 * Generate the script for each stack bin on the core
	 * Skips stack bin 0 because it's already there and shouldn't be changed.
	 * Bins are numbered in reverse order (lower address -> higher x : stack_bin_x)
	 * @param numBins	
	 * @param core
	 * @param coreID
	 * @param sbList
	 * @return String for the update command
	 */
	public String updateBspStackBins(int numBins, Core core, int coreID,
			ArrayList<StackBin> sbList) {
		// first shrink size of mainMemory
		// then add more stack regions
		// each function should get a stack region
		String bspSettings = core.name;
		String cmd = "";
		for (int i = 1; i < sbList.size(); i++) {
			StackBin sb = sbList.get(i);
				cmd += ResizeMainMem(bspSettings, core.mainMemStartAddressOffset, core.mainMemSize);
				cmd += addStackBins(bspSettings, sb.startAddress[coreID], sb.name);
		}
		
		return cmd;
	}


	/**
	 * This method adds the stack bin section and region.
	 * @param coreID
	 * @param binStartAddress
	 * @param name
	 * @return The string of the command to create the new region and section 
	 */
	private String addStackBins(String coreID, 
			int binStartAddress, String name) {
		String cmd ="${NIOS2COMMANDSHELL}" + " nios2-bsp-update-settings \\\n" +
				"--settings ${OUTPUT_DIR}/" + coreID + "_bsp/settings.bsp"  + " \\\n" + 
				"--cmd add_memory_region " + name +
				" memory_0_onchip_memoryMain " +
				"0x" + Integer.toString(binStartAddress, 16) +
				" 0x" + Integer.toString(StackBin.size, 16) + " \\\n--cmd" +
				" add_section_mapping " + "." + name + " " + name + "\n\n";

		return cmd;
	}

	/**
	 * This method createst the command to update main memory size.
	 * @param coreID
	 * @param mainMemStartAddress
	 * @param newSize
	 * @return The string of the command to resize main memory
	 */
	private String ResizeMainMem(String coreID, int mainMemStartAddress,
			int newSize) {
		String cmd = "${NIOS2COMMANDSHELL}" + " nios2-bsp-update-settings \\\n"+
				"--settings ${OUTPUT_DIR}/" + coreID + "_bsp/settings.bsp" + " \\\n--cmd update_memory_region" +
				" memory_0_onchip_memoryMain memory_0_onchip_memoryMain "+
				"0x" + Integer.toString(mainMemStartAddress, 16)+
				" 0x" + Integer.toString(newSize, 16) + "\n\n";
		return cmd;

	}
}
