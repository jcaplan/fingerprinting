package codegen.gen;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
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
		ResizeMainMem(bspSettings, core.mainMemStartAddress, core.mainMemSize);
		for (int i = 1; i < sbList.size(); i++) {
			StackBin sb = sbList.get(i);
			addStackBins(bspSettings, sb.startAddress[coreID], sb.name);
			
		}
	}

	private void addStackBins(String bspSettings, 
			int binStartAddress, String name) {
			String[] cmd = new String[] { sbtLocation,
					"nios2-bsp-update-settings", "--settings", bspSettings,
					"--cmd", "add_memory_region", name,
					"memory_0_onchip_memoryMain",
					Integer.toString(binStartAddress, 16),
					Integer.toString(StackBin.size, 16)/*
														 * 1 bin already
														 * accounted for
														 */, "--cmd",
					"add_section_mapping", "." + name,
					name };
			runCommand(cmd);
		
	}

	private void ResizeMainMem(String bspSettings, int mainMemStartAddress,
			int newSize) {
		String[] cmd = new String[] { sbtLocation, "nios2-bsp-update-settings",
				"--settings", bspSettings, "--cmd", "update_memory_region",
				"memory_0_onchip_memoryMain", "memory_0_onchip_memoryMain",
				Integer.toString(mainMemStartAddress, 16),
				Integer.toString(newSize, 16) /* 1 bin already accounted for */
		};
		runCommand(cmd);
	}

	private void runCommand(String[] cmd) {
		Process p = null;
		try {
			p = Runtime.getRuntime().exec(cmd);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		try {
			p.waitFor();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		if (p.exitValue() != 0) {
			try {
				throw new NiosSBTException("Error building project on "
						+ cmd[1]);
			} catch (NiosSBTException e) {
				BufferedReader bufferedReader = new BufferedReader(
						new InputStreamReader(p.getInputStream()));

				String line;
				ArrayList<String> output = new ArrayList<String>();

				try {
					while ((line = bufferedReader.readLine()) != null) {
						output.add(line);
					}
				} catch (IOException e1) {
					e1.printStackTrace();
				}
				e.printStackTrace();
			}
		}
	}

}
