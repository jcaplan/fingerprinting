package codegen.gen;

import java.io.IOException;
import java.lang.ProcessBuilder.Redirect;
import java.util.ArrayList;

/**
 * Interface with the Nios SBT command line
 * @author jonah
 *
 */
public class NiosSBTCommand {

	String sbtLocation;

	/**
	 * Constructor
	 * @param shellDirLocation Location of the Nios SBT command shell directory
	 */
	public NiosSBTCommand(String shellDirLocation) {
		this.sbtLocation = shellDirLocation + "/nios2_command_shell.sh";
	}

	/**
	 * Command for generating a makefile for a Nios application. Builds makefile with no sources.
	 * @param appDir	App directory for generated makefile
	 * @param bspDir	Location of existing BSP directory
	 * @param elfName	Name of output elf
	 */
	public void generateMakefile(String appDir, String bspDir, String elfName) {
		String[] cmd = { sbtLocation, "nios2-app-generate-makefile",
				"--app-dir", appDir, "--bsp-dir", bspDir, "--elf-name",
				elfName, "--no-src" };

		runCommand(cmd);
		System.out.println("generated makefile");
	}

	/**
	 * Update makefile for stack profiling, adds source files including ert_main.c
	 * @param appDir	App directory 
	 * @param funcName	Name of function to be tested
	 */
	public void updateMakefile(String appDir, String funcName) {
		
		String[] cmd = new String[] { sbtLocation, "nios2-app-update-makefile",
				"--app-dir", appDir, "--add-src-dir",
				appDir ,
				"--set-user-flags", "-fomit-frame-pointer", "--set",
				"OBJDUMP_INCLUDE_SOURCE", "0" };

		runCommand(cmd);
		System.out.println("updated makefile source files");
	}

	/**
	 * Make the completed project
	 * @param appDir	Location of project and makefile. 
	 */
	public void makeProject(String appDir) {
		String[] cmd = new String[] { sbtLocation, "make", "-C", appDir };

		runCommand(cmd);
		System.out.println("finished make for project");

	}

	/**
	 * Update the stack bin sizes.
	 * @param numBins The number of stack bins.
	 * @param core 	The desired core
	 * @param coreID	The Core ID number
	 * @param sbList	The list of stack bins
	 */
	public void updateBspStackBins(int numBins, Core core, int coreID,
			ArrayList<StackBin> sbList) {
		// first shrink size of mainMemory
		// then add more stack regions
		// each function should get a stack region
		String bspSettings = core.bspDir + "/settings.bsp";
		for (int i = 1; i < sbList.size(); i++) {
			StackBin sb = sbList.get(i);
			try {
				if(sb.existsInSettings(bspSettings)){
					if(!sb.matchesSettings(bspSettings,coreID)){
						ResizeMainMem(bspSettings, core.mainMemStartAddressOffset, core.mainMemSize);
						updateStackBins(bspSettings, sb.startAddress[coreID], sb.name);
					}
				} else {
					ResizeMainMem(bspSettings, core.mainMemStartAddressOffset, core.mainMemSize);
					addStackBins(bspSettings, sb.startAddress[coreID], sb.name);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * 
	 * @param bspSettings
	 * @param binStartAddress
	 * @param name
	 */
	private void updateStackBins(String bspSettings, int binStartAddress, String name) {
		String[] cmd = new String[] { sbtLocation, "nios2-bsp-update-settings",
				"--settings", bspSettings, "--cmd", "update_memory_region", name,
				"memory_0_onchip_memoryMain",
				"0x" + Integer.toString(binStartAddress, 16),
				"0x" + Integer.toString(StackBin.size, 16)
		};
		runCommand(cmd);
		
	}

	/**
	 * 
	 * @param bspSettings
	 * @param binStartAddress
	 * @param name
	 */
	private void addStackBins(String bspSettings, 
			int binStartAddress, String name) {
		String[] cmd = new String[] { sbtLocation, "nios2-bsp-update-settings",
				"--settings", bspSettings, "--cmd", "add_memory_region", name,
				"memory_0_onchip_memoryMain",
				"0x" + Integer.toString(binStartAddress, 16),
				"0x" + Integer.toString(StackBin.size, 16)/*
														 * 1 bin already
														 * accounted for
														 */, "--cmd",
				"add_section_mapping", "." + name, name };
		runCommand(cmd);
		
	}

	/**
	 * 
	 * @param bspSettings
	 * @param mainMemStartAddress
	 * @param newSize
	 */
	private void ResizeMainMem(String bspSettings, int mainMemStartAddress,
			int newSize) {
		String[] cmd = new String[] { sbtLocation, "nios2-bsp-update-settings",
				"--settings", bspSettings, "--cmd", "update_memory_region",
				"memory_0_onchip_memoryMain", "memory_0_onchip_memoryMain",
				"0x" + Integer.toString(mainMemStartAddress, 16),
				"0x" + Integer.toString(newSize, 16) /* 1 bin already accounted for */
		};
		runCommand(cmd);
	}

	/**
	 * Runs a command in a process and waits for completion. 
	 * @param cmd	String array for command to be executed.
	 */
	public void runCommand(String[] cmd) {
				
		ProcessBuilder builder = null;
		Process p = null;
		try {
			builder = new ProcessBuilder(cmd);
			builder.redirectErrorStream(true);
			builder.redirectOutput(Redirect.INHERIT);
		    
			p = builder.start();
		} catch (IOException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		}
		
		
		try {
			p.waitFor();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}	 	
		    System.out.println("Program terminated!");
		    
		if (p.exitValue() != 0) {
			try {
				throw new NiosSBTException("Error building project on "
						+ cmd[1]);
			} catch (NiosSBTException e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * Command to generate the library makefile. Not used anymore.
	 * @param libDir
	 * @param bspDir
	 */
	public void generateCriticalLibrary(String libDir) {
		String[] cmd = new String[] { sbtLocation,
				"nios2-lib-generate-makefile", "--lib-dir", libDir, "--lib-name", "critical_library",
				"--src-dir",libDir};
		runCommand(cmd);
		
	}

}
