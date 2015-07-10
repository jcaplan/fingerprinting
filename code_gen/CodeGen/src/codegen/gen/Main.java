package codegen.gen;

import gnu.getopt.Getopt;
import gnu.getopt.LongOpt;

import java.io.*;

import javax.management.RuntimeErrorException;

import org.apache.commons.io.output.TeeOutputStream;

import codegen.prof.Profiler;

public class Main {

	
	public static void main(String[] args) throws Exception{
		

		String configFile = "";
		String profObjdump = "";
		String rootFunc = "";
		boolean generate = false;
		boolean profile = false;
		boolean genOutputFile = false;
		String outputFile = null;
		int c;
		LongOpt[] longopts = new LongOpt[9];
		longopts[0] = new LongOpt("config-file", LongOpt.REQUIRED_ARGUMENT, null, 0);
		longopts[1] = new LongOpt("prof-objdump", LongOpt.REQUIRED_ARGUMENT, null, 1);
		longopts[2] = new LongOpt("root-func",LongOpt.REQUIRED_ARGUMENT, null, 2);
		longopts[3] = new LongOpt("log",LongOpt.REQUIRED_ARGUMENT, null, 3);
		
		// One colon: required
		// Two colons: optional
		// Starting colon: differentiate between invalid args and invalid
		// options
		Getopt g = new Getopt("Main", args, "gp", longopts);
		g.setOpterr(false);
		
		while ((c = g.getopt()) != -1) {
			switch (c) {
			case 0: /* config-file */
				configFile = g.getOptarg();
				break;
			case 1:
				profObjdump = g.getOptarg();
				break;
			case 2:
				rootFunc = g.getOptarg();
				break;
			case 3:
				genOutputFile = true;
				outputFile = g.getOptarg();
				break;
			case 'g':
				generate = true;
				break;
			case 'p':
				profile = true;
				break;
			case '?':
				throw new Exception("Unrecognized option");
			}
		}
		
		if(profile && generate){
			throw new Exception("should not be profiling and generating at same time");
		}
		
		//Check that there are no leftover arguments...
		int index;
		if((index = g.getOptind()) < args.length)
			throw new Exception("unrecognized option argument: " + args[index]);
	
		
		/*args should contain the configuration file*/
		File f;
		FileOutputStream fos; 
		TeeOutputStream myOut; 

		PrintStream ps; 

		
		if(genOutputFile){
			f = new File(outputFile);
			fos = new FileOutputStream(f);
			myOut = new TeeOutputStream(System.out,fos);
			ps = new PrintStream(myOut);
			System.setOut(ps);
		}
		
		if(generate){
			if(configFile.equals("")){
				throw new RuntimeErrorException(new Error("must enter configFile if generating"));
			}
			Configuration config = new Configuration(configFile);
			config.parseConfigFile();		

			config.printConfiguration();
			Generator gen = new Generator(config);
			gen.generateCores();
//			ps.close();
//			ps2.close();	
		} else if(profile){
			if(profObjdump.equals("") || rootFunc.equals("")){
				throw new Exception("Profiler requires an objdump and root function");
			}
			System.out.println("Executing stack profiling for function " + rootFunc + " in file " + profObjdump);
			Profiler prof = new Profiler(profObjdump);
			prof.parseFile(rootFunc);
			System.out.println("MAXSTACK: " + prof.getMaxStackSize(rootFunc));
		}

		/*the next step is to parse the directories and find all the necessary files, check types, existence of variables etc...*/
		
		
	}
}
