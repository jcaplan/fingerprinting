package codegen.gen;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;

import org.apache.commons.io.FileUtils;

/**
 * GenBSP generates the BSPs, using the script update_bsps.sh
 * @author jonah
 *
 */
public class GenBSP {
	
	Configuration config;
	
	/**
	 * Constructor
	 * @param config
	 */
	public GenBSP(Configuration config){
		this.config = config;
	}

	/**
	 * Generates the update_bsps.sh script and then generates the BSPs if cpuM_bsp is not found.
	 * @throws IOException
	 */
	public void generateBSPs() throws IOException {

		File output;
		PrintWriter writer;

		FileUtils.forceMkdir(new File(config.outputDir));
		output = new File(config.outputDir + "/update_bsps.sh");
		writer = new PrintWriter(output);
		
		String s = "";
		s += "#!/bin/bash\n"+
				"\n"+
				"if [ -z \"${NIOS_CODEGEN_ROOT}\" ]; then\n"+
				"  echo \"Environment variable NIOS_CODEGEN_ROOT undefined!\"\n"+
				"  exit 1\n"+
				"fi\n"+
				"\n"+
				"\n"+
				"OUTPUT_DIR=" + config.outputDir + "\n"+
				"BSPMOD_DIR=${NIOS_CODEGEN_ROOT}/platform/bsp_mods\n"+
				"OS_DIR=${NIOS_CODEGEN_ROOT}/platform/micrium_uc_osii\n"+
				"\n"+
				"\n"+
				"for i in {0..1} M\n"+
				"do\n"+
				"	cp -f ${BSPMOD_DIR}/os_cpu_c.c            ${OUTPUT_DIR}/cpu${i}_bsp/HAL/src/\n"+
				"	cp -f ${OS_DIR}/inc/os_cfg.h              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/inc/\n"+
				"	cp -f ${OS_DIR}/inc/ucos_ii.h             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/inc/\n"+
				"	cp -f ${OS_DIR}/src/os_dbg.c              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_mem.c              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_task.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_dbg_r.c            ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_mutex.c            ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_time.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_flag.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_q.c                ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_tmr.c              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_core.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_mbox.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"	cp -f ${OS_DIR}/src/os_sem.c              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/\n"+
				"done\n"+
				"\n"+
				"cp -f ${BSPMOD_DIR}/alt_exception_entry_gp.S ${OUTPUT_DIR}/cpu0_bsp/HAL/src/alt_exception_entry.S\n"+
				"cp -f ${BSPMOD_DIR}/alt_exception_entry_gp.S ${OUTPUT_DIR}/cpu1_bsp/HAL/src/alt_exception_entry.S\n"+
				"cp -f ${BSPMOD_DIR}/os_core.c                ${OUTPUT_DIR}/cpu0_bsp/UCOSII/src/\n"+
				"cp -f ${BSPMOD_DIR}/os_core.c                ${OUTPUT_DIR}/cpu1_bsp/UCOSII/src/\n"+
				"cp -f ${BSPMOD_DIR}/ucos_ii.h                ${OUTPUT_DIR}/cpu0_bsp/UCOSII/inc/\n"+
				"cp -f ${BSPMOD_DIR}/ucos_ii.h                ${OUTPUT_DIR}/cpu1_bsp/UCOSII/inc/\n"+
				"cp -f ${BSPMOD_DIR}/os_cpu_c_MON.c           ${OUTPUT_DIR}/cpuM_bsp/HAL/src/os_cpu_c.c\n"+
				"\n"+
				"\n"+
				"# update system.h for monitor bsp\n"+
				"DIR=.tmp\n"+
				"mkdir -p $DIR\n"+
				"\n"+
				"# fix the comparator interrupt\n"+
				"\n"+
				"sed -r -e 's/(CFPU_0_CSR_IRQ) (-*[0-9]+)/\\1 3/' \\\n"+
				"		-e 's/(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID) (-*[0-9]+)/\\1 0/' \\\n"+
				"		<${OUTPUT_DIR}/cpuM_bsp/system.h >$DIR/test.x\n"+
				"\n"+
				"cp $DIR/test.x ${OUTPUT_DIR}/cpuM_bsp/system.h\n"+
				"\n"+
				"#update the Monitor dma interrupts file for all\n"+
				"for i in {0..1}\n"+
				"do\n"+
				"	irq=$(expr $i + 5)\n"+
				"	sed -r -e \"s/(PROCESSOR${i}_0_DMA_0_IRQ) (-*[0-9]+)/\\1 ${irq}/\" \\\n"+
				"		-e \"s/(PROCESSOR${i}_0_DMA_0_IRQ_INTERRUPT_CONTROLLER_ID) (-*[0-9]+)/\\1 0/\" \\\n"+
				"		<${OUTPUT_DIR}/cpuM_bsp/system.h >$DIR/test.x\n"+
				"	cp $DIR/test.x ${OUTPUT_DIR}/cpuM_bsp/system.h\n"+
				"\n"+
				"done\n"+
				"\n"+
				"\n"+
				"rm -r $DIR\n"+
				"\n";
				
		writer.write(s);
		writer.close();
		
		output.setExecutable(true);
		
		output = new File(config.outputDir + "/create_bsp.sh");
		writer= new PrintWriter(output);
		
		s = "";

		s += "#!/bin/bash\n"+
				"\n"+
				"\n"+
				"if [ -z \"${NIOS_CODEGEN_ROOT}\" ]; then\n"+
				"  echo \"Environment variable NIOS_CODEGEN_ROOT undefined!\"\n"+
				"  exit 1\n"+
				"fi\n"+
				"\n"+
				"NIOS2COMMANDSHELL=" + config.niosSBT.sbtLocation + "\n" +
				"SOPC_BUILDER_PATH=${NIOS_CODEGEN_ROOT}/drivers\n"+
				"SOPC_LOCATION=" + config.sopcinfoFilename + "\n"+
				"OUTPUT_DIR=" + config.outputDir + "\n"+
				"\n"+
				"for i in {0..1} M\n"+
				"do\n"+
				"	${NIOS2COMMANDSHELL} nios2-bsp ucosii ${OUTPUT_DIR}/cpu${i}_bsp ${SOPC_LOCATION} \\\n"+
				"	--cpu-name processor${i}_0_cpu${i}\n"+
				"\n"+
				"	if [ \"$?\" !=  \"0\" ]\n"+
				"	then\n"+
				"		echo \"ERROR: could not generate ${OUTPUT_DIR}/cpu${i}_bsp\"\n"+
				"		exit 1\n"+
				"	fi\n"+
				"done\n"+
				"\n"+
				"\n"+
				"\n"+
				"\n"+
				"# #update linker scripts:\n"+
				"# # update_memory_region Positional Arguments:\n"+
				"# # <name>                  String with the name of the memory region to update.\n"+
				"# # <slave desc>            String with the slave descriptor of the memory device\n"+
				"# #                         for this region.\n"+
				"# # <offset>                String with the byte offset of the memory region from\n"+
				"# #                         the memory device base address.\n"+
				"# # <span>                  String with the span of the memory region in bytes.\n"+
				"\n"+
				"# cpu0\n"+
				"${NIOS2COMMANDSHELL} nios2-bsp-update-settings  --settings ${OUTPUT_DIR}/cpu0_bsp/settings.bsp \\\n"+
				"--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x64020 0x30fe0 \\\n"+
				"--cmd add_memory_region stack_bin_0 memory_0_onchip_memoryMain 0x95000 0x1000 \\\n"+
				"--cmd add_section_mapping .stack_bin_0 stack_bin_0 \\\n"+
				"--cmd enable_sw_package mem_manager \\\n"+
				"--cmd enable_sw_package mpu_utils \\\n"+
				"--cmd enable_sw_package fingerprint \\\n"+
				"--cmd enable_sw_package reset_monitor \\\n"+
				"--cmd set_setting hal.make.bsp_cflags_user_flags -fomit-frame-pointer\n"+
				"\n"+
				"# cpu1\n"+
				"${NIOS2COMMANDSHELL} nios2-bsp-update-settings  --settings ${OUTPUT_DIR}/cpu1_bsp/settings.bsp \\\n"+
				"--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x32020 0x30fe0 \\\n"+
				"--cmd add_memory_region stack_bin_0 memory_0_onchip_memoryMain 0x63000 0x1000 \\\n"+
				"--cmd add_section_mapping .stack_bin_0 stack_bin_0 \\\n"+
				"--cmd update_section_mapping .bss memory_0_onchip_memoryMain \\\n"+
				"--cmd update_section_mapping .text memory_0_onchip_memoryMain \\\n"+
				"--cmd update_section_mapping .stack memory_0_onchip_memoryMain \\\n"+
				"--cmd update_section_mapping .heap memory_0_onchip_memoryMain \\\n"+
				"--cmd update_section_mapping .rodata memory_0_onchip_memoryMain \\\n"+
				"--cmd update_section_mapping .rwdata memory_0_onchip_memoryMain \\\n"+
				"--cmd enable_sw_package mem_manager \\\n"+
				"--cmd enable_sw_package mpu_utils \\\n"+
				"--cmd enable_sw_package fingerprint \\\n"+
				"--cmd enable_sw_package reset_monitor \\\n"+
				"--cmd set_setting hal.make.bsp_cflags_user_flags -fomit-frame-pointer\n"+
				"\n"+
				"# cpum\n"+
				"${NIOS2COMMANDSHELL} nios2-bsp-update-settings  --settings ${OUTPUT_DIR}/cpuM_bsp/settings.bsp \\\n"+
				"--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x20 0x30fe0 \\\n"+
				"--cmd add_memory_region global_data memory_0_onchip_memoryMain 0x31000 0x1000 \\\n"+
				"--cmd add_section_mapping .global_data global_data \\\n"+
				"--cmd add_section_mapping .critical processorM_0_scratchpad \\\n"+
				"--cmd add_section_mapping .shared shared_memory \\\n"+
				"--cmd enable_sw_package repos  \\\n"+
				"--cmd enable_sw_package mpu_utils \\\n"+
				"--cmd enable_sw_package fingerprint \\\n"+
				"--cmd enable_sw_package reset_monitor \\\n"+
				"--cmd enable_sw_package dma \\\n"+
				"--cmd set_setting hal.make.bsp_cflags_user_flags -fomit-frame-pointer\n"+
				"\n"+
				"\n"+
				"for i in {0..1} M\n"+
				"do\n"+
				"	 ${NIOS2COMMANDSHELL} nios2-bsp-generate-files --bsp-dir ${OUTPUT_DIR}/cpu${i}_bsp \\\n"+
				"	 --settings ${OUTPUT_DIR}/cpu${i}_bsp/settings.bsp \n"+
				"done\n"+
				"	\n"+
				"source ${OUTPUT_DIR}/update_bsps.sh\n"+
				"\n";
		writer.write(s);
		writer.close();
		

		output.setExecutable(true);
		File bsp = new File(config.outputDir + "/cpuM_bsp");
		if(!bsp.exists()){
			config.niosSBT.runCommand(new String[]{config.outputDir + "/create_bsp.sh"});
		}
	}
}
