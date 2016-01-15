package codegen.gen;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;

import javax.management.RuntimeErrorException;

/**
 * Class for creating stack bin objects and generating scripts that update BSP.
 * "StackBins" are chunks of main memory placed in separate region in order to ensure
 * that stacks, when allocated to those regions, are page aligned with each other.
 * Otherwise the stacks will never match on redundant tasks.
 * @author jonah
 *
 */
public class GenStackBin {


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
	public GenStackBin(ArrayList<Function> fprintList, Platform platform,
			Configuration config){
		this.fprintList = fprintList;
		this.platform = platform;
		this.config = config;
	}
	
	/**
	 * This method is called to find the stack bins and create and run the scripts that update the
	 * BSP.
	 */
	public void genStackBins() {
		if(!getStackBins()){
			return;
		}
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
	private boolean getStackBins() {
		// MIN_OFFSET = 1256
		// SAFETY = 80
		// MIN_OFFSET + SAFETY + f.stacksize > 2048
		// i.e. f.stacksize < 712
		
		// first fit bin packing
		// sort functions by stack size
		// then stick them in bins

		// ////////////////////////////////////////////////

		if(fprintList.isEmpty()){
			System.out.println("No stack bins because no fingerprinting...");
			return false;
		}
		@SuppressWarnings("unchecked")
		ArrayList<Function> fprintListCopy = (ArrayList<Function>) fprintList.clone();
		Collections.sort(fprintListCopy, Function.stackCompareDecreasing);
		
		for (Function f : fprintListCopy) {
			if (f.stackSize > StackBin.SIZE - (StackBin.STACKSIZE_MARGINERROR + StackBin.STACKSIZE_MINOFFSET)) {
				throw new RuntimeErrorException(new Error("stack too big for function: " + f));
			}
			for(Core c : f.cores){
				StackBin bin = new StackBin(c,c.stackbins.size());
				bin.setFunction(f);	
				c.stackbins.add(bin);
			}
		}
		
		
		return true;
	}
	
	/**
	 * This method calculates the start addresses for each stack bin for each core
	 */
	private void setStackBinAddresses() {
		
		for(int i = 0; i < platform.numProcessingCores; i++){
			Core c = platform.getCore(i);
			ArrayList<StackBin> bins = c.stackbins;
			for(int j = 0; j < bins.size(); j++){
				StackBin bin = bins.get(j);
				bin.startAddress = c.mainMemStartAddressOffset + c.mainMemSize - ((j + 1)*StackBin.SIZE);
			}
		}
		updateMainMemorySize();
	}
	
	/**
	 * This method finds the new (reduced) main memory size
	 */
	private void updateMainMemorySize() {
		for(int i = 0; i < platform.numProcessingCores; i++){
			Core c = platform.getCore("cpu" + i);
			c.mainMemSize -= c.stackbins.size() * StackBin.SIZE;
		}
		
	}
	

	/**
	 * This method creates and runs the script to update the stack regions in the BSP 
	 * @throws IOException
	 */
	private void updateStackBinRegions() throws IOException {
		ArrayList<Core> updateList = new ArrayList<>();
		String cmd = "";
		cmd +=  "#!/bin/bash"+
				"\n"+
				"\n"+
				"OUTPUT_DIR=" + config.outputDir + "\n"+
				"NIOS2COMMANDSHELL=" + config.niosSBT.sbtLocation + "\n"+
				"\n";

		for(int i = 0; i < platform.numProcessingCores; i++){
			Core c = platform.getCore(i);
			if(c.stackbins.size() > 1){
				cmd += updateBspStackBins(c);
				updateList.add(c);
			}
		}
		
		if(!updateList.isEmpty()){
			File file = new File(config.outputDir + "/update_bsp_mem_regions.sh");
			PrintWriter writer;
			
			
			writer = new PrintWriter(file);
			writer.print(cmd);
			writer.close();
			
			file.setExecutable(true);
			for(Core c : updateList){
				config.niosSBT.updateBspStackBins(c);	
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
	public String updateBspStackBins(Core core) {
		// first shrink size of mainMemory
		// then add more stack regions
		// each function should get a stack region
		String bspSettings = core.name;
		String cmd = "";
		ArrayList<StackBin> sbList = core.stackbins;
		for (int i = 1; i < sbList.size(); i++) {
			StackBin sb = sbList.get(i);
				cmd += ResizeMainMem(bspSettings, core.mainMemStartAddressOffset, core.mainMemSize);
				cmd += addStackBins(bspSettings, sb.startAddress, sb.name);
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
				" 0x" + Integer.toString(StackBin.SIZE, 16) + " \\\n--cmd" +
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
