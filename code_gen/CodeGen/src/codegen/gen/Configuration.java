package codegen.gen;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

import javax.management.RuntimeErrorException;

import codegen.gen.Function.Type;

public class Configuration {

	String configFilename;
	BufferedReader reader;
	int lineCount;
	ArrayList<Function> funcList;

	public Configuration(String filename) {
		configFilename = filename;
		funcList = new ArrayList<>();
		lineCount = 0;
	}

	public void throwConfigError(String message) {
		throw new RuntimeErrorException(new Error(configFilename + ":" + lineCount + ": " + message));
	}

	public void parseConfigFile() throws IOException {
		FileReader fr = new FileReader(configFilename);
		reader = new BufferedReader(fr);

		// initialize the reader and cfg
		reader = new BufferedReader(fr);
		String line = "";
		while ((line = reader.readLine()) != null) {
			lineCount++;
			line = line.trim();
			switch (line) {
			case "<FUNCTIONLIST>":
				parseFunctionList();
				break;
			case "<DATAFLOW>":
				parseDataflow();
				break;
			case "<PLATFORM>":
				parsePlatform();
				break;
			case "<MAPPING>":
				parseMapping();
				break;
			default:
				if (!line.matches("#.*|")) { /* allow comments */
					throwConfigError("String unrecognized: \"" + line + "\"");
				}
			}
		}

		reader.close();
		
	}

	void printConfiguration() {
		for(Function f : funcList){
			System.out.println(f.codeDirectory);
			System.out.print(f.name + ": critical = " + f.critical);
			if(f.type == Type.eventDriven){
				System.out.println(", event driven");
			} else {
				 System.out.println(", period = " + f.period); 
			}
			System.out.print(f.name + " executes on cores: ");
			printStringArray(f.cores);
			for(Dataflow d : f.dataflowList){
				System.out.println(d);
			}
			System.out.println("");
		}
		
	}

	private void parseMapping() throws IOException {
		String line = "";
		while(!(line = reader.readLine()).equals("</MAPPING>")){
			lineCount++;
			if(!line.startsWith("#")){
				String[] tokens = line.split("[\\s]+");
				String funcName = tokens[0];
				Function func;
				if((func = getFunction(funcName)) == null){
					throwConfigError("Function name not found");
				}
				
				if(func.critical){
					if(tokens.length == 3){
						if(!(tokens[1].equals("cpu0") || tokens[1].equals("cpu1"))
							||	 !(tokens[2].equals("cpu0") || tokens[2].equals("cpu1"))){
							throwConfigError("invalid mapping for task " + funcName);
						}
					} else if(tokens.length == 2){
						if(!tokens[1].equals("cpum")){
							throwConfigError("invalid mapping for task " + funcName);	
						}
					} else {
						throwConfigError("invalid mapping for task " + funcName);	
					}
				} else { /* not critical */
					if(tokens.length != 2){
						throwConfigError("invalid mapping for task " + funcName);	
					}
				}
				
				/* all the assignments are legal */
				for(int i = 1; i < tokens.length; i++){
					func.cores.add(tokens[i]);
				}
				
			}
		}
		lineCount++; /*final line*/
	}

	private void parsePlatform() throws IOException {
		String line = "";
		while(!(line = reader.readLine()).equals("</PLATFORM>")){
			lineCount++;
			line = line.trim();
			if(!line.startsWith("#")){
				if(!line.equals("-usedefault")){
					throwConfigError("Only default platform currently supported");
				}
			}
		}
		lineCount++; /*final line*/
		
	}



	private void parseFunctionList() throws IOException {
		String line = "";
		while (!(line = reader.readLine()).equals("</FUNCTIONLIST>")) {
			lineCount++;
			if (!line.startsWith("#")) {
				Function f = new Function();
				String[] tokens = line.split("[\\s]+");
				for (int i = 0; i < tokens.length; i++) {
					String token = tokens[i];
					switch (token) {
					case "-name":
						// check the next token
						if (i == tokens.length - 1) {
							throwConfigError("-name expects value");
						}
						String arg = tokens[i + 1];
						if (arg.startsWith("-")) {
							throwConfigError("-name expects value");
						}
						f.name = arg;
						i++;
						break;
					case "-dir":
						if (i == tokens.length - 1) {
							throwConfigError(": " + "-dir expects value");
						}
						arg = tokens[i + 1];
						if (arg.startsWith("-")) {
							throwConfigError("-dir expects value");
						}
						f.codeDirectory = arg;
						i++;
						break;
					case "-T":
						if (i == tokens.length - 1) {
							throwConfigError("-T expects value");
						}
						arg = tokens[i + 1];
						if (arg.startsWith("-")) {
							throwConfigError("-T expects value");
						}
						f.period = Integer.parseInt(arg);
						f.type = Type.periodic;
						i++;
						break;
					case "-E":
						f.type = Type.eventDriven;
						break;
					case "-c":
						f.critical = true;
						break;
					default:
						throwConfigError("Unrecognized argument in FUNCTIONLIST: "
								+ token);

					}

				}
				funcList.add(f);

			}
		}
		lineCount++; /*final line*/

	}

	private void parseDataflow() throws IOException {
		String line = "";
		while (!(line = reader.readLine()).equals("</DATAFLOW>")) {
			lineCount++;
			if(!line.startsWith("#")){
				String[] tokens = line.split("[\\s]+");
				if (tokens.length != 2) {
					throwConfigError("should only have two items per line in DATAFLOW");
				}
				
				String[] sourceParams = tokens[0].split("\\.");
				if(sourceParams.length != 2){
					throwConfigError("wrong format for name: modelname.varname");
				}
				String sourceFunction = sourceParams[0];
				String sourceVariable = sourceParams[1];
				
				String[] destParams = tokens[1].split("\\.");
				if(sourceParams.length != 2){
					throwConfigError("wrong format for name: modelname.varname");
				}
				String destFunction = destParams[0];
				String destVariable = destParams[1];
				

				Function source = getFunction(sourceFunction);
				Function dest = getFunction(destFunction);
				
				source.addDataflow(dest,sourceVariable,destVariable);
				
			}
		}
		lineCount++; /*final line*/

	}

	private Function getFunction(String sourceFunction) {
		for(Function f: funcList){
			if(f.name.equals(sourceFunction)){
				return f;
			}
		}
		return null;
	}
	
	private void printStringArray(String[] array) {
		for (int i = 0; i < array.length - 1; i++) {
			System.out.print(array[i] + ", ");
		}
		System.out.println(array[array.length - 1]);
	}
	
	private void printStringArray(ArrayList<String> array) {
		for (int i = 0; i < array.size() - 1; i++) {
			System.out.print(array.get(i) + ", ");
		}
		System.out.println(array.get(array.size() - 1));
	}

}
