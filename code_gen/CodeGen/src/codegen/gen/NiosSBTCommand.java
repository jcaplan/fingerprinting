package codegen.gen;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.ProcessBuilder.Redirect;
import java.util.ArrayList;

public class NiosSBTCommand {

	String sbtLocation;

	public NiosSBTCommand(String dirName) {
		this.sbtLocation = dirName + "/nios2_command_shell.sh";
	}

	public void generateMakefile(String appDir, String bspDir, String elfName) {
		String[] cmd = { sbtLocation, "nios2-app-generate-makefile",
				"--app-dir", appDir, "--bsp-dir", bspDir, "--elf-name",
				elfName, "--no-src" };

		runCommand(cmd);
		System.out.println("generated makefile");
	}

	public void updateMakefile(String appDir, String funcName) {

		String[] cmd = new String[] { sbtLocation, "nios2-app-update-makefile",
				"--app-dir", appDir, "--add-src-files",
				appDir + "/" + funcName + ".c", appDir + "/" + "ert_main.c",
				"--set-user-flags", "-fomit-frame-pointer", "--set",
				"OBJDUMP_INCLUDE_SOURCE", "0" };

		runCommand(cmd);
		System.out.println("updated makefile source files");
	}

	public void makeProject(String appDir) {
		String[] cmd = new String[] { sbtLocation, "make", "-C", appDir };

		runCommand(cmd);
		System.out.println("finished make for project");

	}

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
					addStackBins(bspSettings, sb.startAddress[coreID], sb.name);
					ResizeMainMem(bspSettings, core.mainMemStartAddressOffset, core.mainMemSize);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	private void updateStackBins(String bspSettings, int binStartAddress, String name) {
		String[] cmd = new String[] { sbtLocation, "nios2-bsp-update-settings",
				"--settings", bspSettings, "--cmd", "update_memory_region", name,
				"memory_0_onchip_memoryMain",
				"0x" + Integer.toString(binStartAddress, 16),
				"0x" + Integer.toString(StackBin.size, 16)
		};
		runCommand(cmd);
		
	}

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

	public void generateCriticalLibrary(String libDir, String bspDir) {
		String[] cmd = new String[] { sbtLocation,
				"nios2-lib-generate-makefile", "--lib-dir", libDir, "--lib-name", "critical_library", "--bsp-dir",
				bspDir, "--src-dir",libDir};
		runCommand(cmd);
		
	}

}
