package codegen.gen;

import java.io.*;
import java.util.*;

import codegen.gen.Function.Type;

/**
 * Configuration class parses the configuration file and holds the configuration details.
 * @author jonah
 *
 */
public class Configuration {

	String configFilename;
	BufferedReader reader;
	int lineCount;
	Platform platform;
	ArrayList<Function> funcList;
	boolean stackProfilingRequired = true;
	public NiosSBTCommand niosSBT;
	String sopcinfoFilename;
	String outputDir;

	/**
	 * 
	 * @param configFilename	Full path name of the configuration file
	 * @param outputDir			Directory where the generated files should be placed
	 */
	public Configuration(String configFilename, String outputDir) {
		this.configFilename = configFilename;
		this.outputDir = outputDir;
		funcList = new ArrayList<>();
		lineCount = 0;
	}

	/**
	 * 
	 * @param configFile		Full path name of the configuration file
	 * @param outputDir			Directory where the generated files should be placed
	 * @param sopcinfoFilename	Name of sopcinfo file from Quartus for generating BSPs
	 */
	public Configuration(String configFile, String outputDir,
			String sopcinfoFilename) {
		this(configFile,outputDir);
		this.sopcinfoFilename = sopcinfoFilename;
	}

	/**
	 * Formats configuration error message
	 * @param message	Message to be formatted
	 * @throws ConfigurationException
	 */
	private void throwConfigError(String message) throws ConfigurationException {
		throw new ConfigurationException(configFilename + ":" + lineCount
				+ ": " + message);
	}

	/**
	 * Parse the configuration file with this function
	 * @throws IOException
	 * @throws ConfigurationException
	 */
	public void parseConfigFile() throws IOException, ConfigurationException {
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
			case "<STACK_PROFILE>":
				stackProfilingRequired = false;
				parseStackProfiling();
				break;
			default:
				if (!(line.matches("#.*|") || line.isEmpty())) { /*
																 * allow
																 * comments
																 */
					throwConfigError("String unrecognized: \"" + line + "\"");
				}
			}
		}

		reader.close();

		Collections.sort(funcList);
		platform.addFunctionsToCores();
	}

	/**
	 * Parses the stack profiling section
	 * @throws IOException
	 * @throws ConfigurationException
	 */
	private void parseStackProfiling() throws IOException, ConfigurationException {
		String line = "";
		while (!(line = reader.readLine()).equals("</STACK_PROFILE>")) {
			lineCount++;
			if (!(line.startsWith("#") || line.isEmpty())) {
				String[] tokens = line.split("[\\s]+");
				String funcName = tokens[0];
				Function func;
				if ((func = getFunction(funcName)) == null) {
					throwConfigError("Function name not found");
				}

				func.stackSize = Integer.parseInt(tokens[1]);

			}
		}
		lineCount++; /* final line */
		
	}

	/**
	 * Prints the configuration for each function
	 */
	void printConfiguration() {
		for (Function f : funcList) {
			System.out.println(f.codeDirectory);
			System.out.print(f.name + ": priority = " + f.priority
					+ ", critical = " + f.critical);
			if (f.type == Type.eventDriven) {
				System.out.println(", event driven");
			} else {
				System.out.println(", period = " + f.period);
			}
			System.out.print(f.name + " executes on cores: ");
			printStringArray(f.cores);
			for (Dataflow d : f.dataflowList) {
				System.out.println(d);
			}
			System.out.println("");
		}

	}

	/**
	 * Parse the mapping section
	 * @throws IOException
	 * @throws ConfigurationException
	 */
	private void parseMapping() throws IOException, ConfigurationException {
		String line = "";
		while (!(line = reader.readLine()).equals("</MAPPING>")) {
			lineCount++;
			if (!(line.startsWith("#") || line.isEmpty())) {
				String[] tokens = line.split("[\\s]+");
				String funcName = tokens[0];
				Function func;
				if ((func = getFunction(funcName)) == null) {
					throwConfigError("##MAPPING ERROR: Function name " + funcName + " not found");
				}

				if (func.critical) {
					if (tokens.length == 3) {
						if (!(tokens[1].equals("cpu0") || tokens[1]
								.equals("cpu1"))
								|| !(tokens[2].equals("cpu0") || tokens[2]
										.equals("cpu1"))) {
							throwConfigError("invalid mapping for task "
									+ funcName);
						}
					} else if (tokens.length == 2) {
						if (!tokens[1].equals("cpuM")) {
							throwConfigError("invalid mapping for task "
									+ funcName);
						}
					} else {
						throwConfigError("invalid mapping for task " + funcName);
					}
				} else { /* not critical */
					if (tokens.length != 2) {
						throwConfigError("invalid mapping for task " + funcName);
					}
				}

				/* all the assignments are legal */
				for (int i = 1; i < tokens.length; i++) {
					func.cores.add(tokens[i]);
				}

			}
		}
		lineCount++; /* final line */
	}

	/**
	 * Parse the platform section
	 * @throws IOException
	 * @throws ConfigurationException
	 */
	private void parsePlatform() throws IOException, ConfigurationException {
		String line = "";
		platform = new Platform(this);
		while (!(line = reader.readLine()).equals("</PLATFORM>")) {
			lineCount++;
			line = line.trim();
			if (!(line.startsWith("#") || line.isEmpty())) {
				String[] tokens = line.split("[\\s]+");
				Core core = null;
				for (int i = 0; i < tokens.length; i++) {
					String token = tokens[i];
					switch (token) {
					case "-name":
						if (i == tokens.length - 1) {
							throwConfigError("-name expects value");
						}
						String arg = tokens[i + 1];
						if (arg.startsWith("-")) {
							throwConfigError("-name expects value");
						}
						core = platform.getCore(arg);
						i++;
						break;
					case "-bsp-dir":
						if(sopcinfoFilename != null){
							throwConfigError("Should not set bspdir when using sopcinfo option");
						}
						if (i == tokens.length - 1) {
							throwConfigError("-name expects value");
						}
						arg = tokens[i + 1];
						if (arg.startsWith("-")) {
							throwConfigError("-name expects value");
						}
						core.bspDir = arg;
						i++;
						break;
					case "-usedefault":
						sopcinfoFilename =  "${NIOS_CODEGEN_ROOT}/platform/nios_fprint.sopcinfo";
						platform.useDefaultBSPs();
						break;
					default:
						//TODO error?
						break;
					}
				}
			}
		}
		lineCount++; /* final line */

	}

	/**
	 * Parse the function list section
	 * @throws IOException
	 * @throws ConfigurationException
	 */
	private void parseFunctionList() throws IOException, ConfigurationException {
		String line = "";
		String rootdir = "";
		while (!(line = reader.readLine()).equals("</FUNCTIONLIST>")) {
			lineCount++;
			if (!(line.startsWith("#") || line.isEmpty())) {
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
					case "-rootdir":
						if (i == tokens.length - 1) {
							throwConfigError(": " + "-dir expects value");
						}
						arg = tokens[i + 1];
						if (arg.startsWith("-")) {
							throwConfigError("-dir expects value");
						}
						rootdir = arg;
						i++;
						break;
					case "-subdir":
						if (i == tokens.length - 1) {
							throwConfigError(": " + "-dir expects value");
						}
						arg = tokens[i + 1];
						if (arg.startsWith("-")) {
							throwConfigError("-dir expects value");
						}
						f.codeDirectory = rootdir + "/" + arg;
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
					case "-Priority":
						if (i == tokens.length - 1) {
							throwConfigError("-T expects value");
						}
						arg = tokens[i + 1];
						if (arg.startsWith("-")) {
							throwConfigError("-T expects value");
						}
						f.priority = Integer.parseInt(arg);
						i++;
						break;
					case "-S":
						if (i == tokens.length - 1) {
							throwConfigError("-T expects value");
						}
						arg = tokens[i + 1];
						if (arg.startsWith("-")) {
							throwConfigError("-T expects value");
						}
						f.stackSize = Integer.parseInt(arg);
						i++;
						break;
					case "-printRuntime":
						f.printRuntimes = true;
						break;
					case "-addPreamble":
						if (i == tokens.length - 1) {
							throwConfigError("-T expects value");
						}
						arg = tokens[i + 1];
						if (arg.startsWith("-")) {
							throwConfigError("-T expects value");
						}
						f.preambleFileName = arg;
						i++;
						break;
					default:
						throwConfigError("Unrecognized argument in FUNCTIONLIST: "
								+ token);

					}

				}
				if (f.name != null) {
					funcList.add(f);
				}
			}
		}
		lineCount++; /* final line */

	}

	/**
	 * Parse the dataflow section
	 * @throws IOException
	 * @throws ConfigurationException
	 */
	private void parseDataflow() throws IOException, ConfigurationException {
		String line = "";
		while (!(line = reader.readLine()).equals("</DATAFLOW>")) {
			lineCount++;
			if (!(line.startsWith("#") || line.isEmpty())) {
				String[] tokens = line.split("[\\s]+");
				if (tokens.length != 2) {
					throwConfigError("should only have two items per line in DATAFLOW");
				}

				String[] sourceParams = tokens[0].split("\\.");
				if (sourceParams.length != 2) {
					throwConfigError("wrong format for name: modelname.varname");
				}
				String sourceFunction = sourceParams[0];
				String sourceVariable = sourceParams[1];

				String[] destParams = tokens[1].split("\\.");
				if (sourceParams.length != 2) {
					throwConfigError("wrong format for name: modelname.varname");
				}
				String destFunction = destParams[0];
				String destVariable = destParams[1];

				Function source = getFunction(sourceFunction);
				Function dest = getFunction(destFunction);

				source.addDataflow(dest, sourceVariable, destVariable);

			}
		}
		lineCount++; /* final line */

	}

	/**
	 * Get a function from the funcList from its name
	 * @param sourceFunction
	 * @return the function object with the same name
	 */
	private Function getFunction(String sourceFunction) {
		for (Function f : funcList) {
			if (f.name.equals(sourceFunction)) {
				return f;
			}
		}
		return null;
	}


	/**
	 * Pring an ArrayList<String>
	 * @param array
	 */
	private void printStringArray(ArrayList<String> array) {
		for (int i = 0; i < array.size() - 1; i++) {
			System.out.print(array.get(i) + ", ");
		}
		System.out.println(array.get(array.size() - 1));
	}

	/**
	 * Check if BSP generation is required 
	 * @return	true when sopcinfo file included in command line arguments
	 */
	public boolean generateBSPRequired() {
		return sopcinfoFilename != null;
	}

}
