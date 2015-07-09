package codegen.gen;

import java.io.IOException;

import javax.management.RuntimeErrorException;

public class Main {

	
	public static void main(String[] args) throws IOException, ConfigurationException{
		
		/*args should contain the configuration file*/
		if(args.length != 1){
			throw new RuntimeErrorException(new Error("Usage: codeGen pathto/configfile"));
		}
		String filename = args[0];
		Configuration config = new Configuration(filename);
		config.parseConfigFile();
		
		config.printConfiguration();
		
		/*the next step is to parse the directories and find all the necessary files, check types, existence of variables etc...*/
		
		
	}
}
