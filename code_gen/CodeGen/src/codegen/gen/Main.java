package codegen.gen;

import gnu.getopt.Getopt;
import gnu.getopt.LongOpt;

import java.io.*;

import javax.management.RuntimeErrorException;

import org.apache.commons.io.output.TeeOutputStream;

public class Main {

	/**
	 * Main entry. Parse arguments, create Configuration and CodeGen objects. 
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {

		String configFile = "";
		boolean genLogFile = false;
		String logFile = null;
		String nios2CmdDir = "";
		String outputDir = "";
		String sopcinfoFilename = "";
		int c;
		LongOpt[] longopts = new LongOpt[9];
		longopts[0] = new LongOpt("config-file", LongOpt.REQUIRED_ARGUMENT,
				null, 0);
		longopts[1] = new LongOpt("log", LongOpt.REQUIRED_ARGUMENT, null, 1);
		longopts[2] = new LongOpt("nios2-cmd-dir", LongOpt.REQUIRED_ARGUMENT,
				null, 2);
		longopts[3] = new LongOpt("output-dir", LongOpt.REQUIRED_ARGUMENT, null, 3);
		longopts[4] = new LongOpt("sopcinfo", LongOpt.REQUIRED_ARGUMENT, null,4);

		// One colon: required
		// Two colons: optional
		// Starting colon: differentiate between invalid args and invalid
		// options
		Getopt g = new Getopt("Main", args, "", longopts);
		g.setOpterr(false);

		while ((c = g.getopt()) != -1) {
			switch (c) {
			case 0: /* config-file */
				configFile = g.getOptarg();
				break;
			case 1:
				genLogFile = true;
				logFile = g.getOptarg();
				break;
			case 2:
				nios2CmdDir = g.getOptarg();
				break;
			case 3:
				outputDir = g.getOptarg();
				break;
			case 4:
				sopcinfoFilename = g.getOptarg();
				break;
			case '?':
				throw new Exception("Unrecognized option");
			}
		}

		// Check that there are no leftover arguments...
		int index;
		if ((index = g.getOptind()) < args.length){
			throw new Exception("unrecognized option argument: " + args[index]);
		}
		if (configFile.equals("")) {
			throw new RuntimeErrorException(new Error(
					"must enter configFile"));
		}
		if (nios2CmdDir.isEmpty()) {
			throw new RuntimeErrorException(new Error(
					"must enter location for Nios SBT"));
		}
			
			
		/* args should contain the configuration file */
		File f;
		FileOutputStream fos;
		TeeOutputStream myOut;

		PrintStream ps;

		if (genLogFile) {
			f = new File(logFile);
			fos = new FileOutputStream(f);
			myOut = new TeeOutputStream(System.out, fos);
			ps = new PrintStream(myOut);
			System.setOut(ps);
		}

		
		
		
		Configuration config = new Configuration(configFile,outputDir, sopcinfoFilename);
		config.niosSBT = new NiosSBTCommand(nios2CmdDir);
		config.parseConfigFile();

		config.printConfiguration();
		CodeGen gen = new CodeGen(config);
		gen.generateCores();
		gen.printStackResults();
		gen.printWcetResults();
		gen.printMappingResults();


	}
}
