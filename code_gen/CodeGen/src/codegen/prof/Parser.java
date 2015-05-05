package codegen.prof;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Parser {

	String filename;
	private CFG cfg;
	public Parser(String filename) {
		this.filename = filename;
	}

	void parseFunctions(String filename) throws IOException {

		FileReader fr = new FileReader(filename);
		BufferedReader reader = new BufferedReader(fr);

		// initialize the reader and cfg
		reader = new BufferedReader(fr);
		// Skip the symboltable
		while (!reader.readLine().contains("Disassembly of section .text:"))
			;
		// Parse each line
		String line;

		boolean functionStarted = false;
		while ((line = reader.readLine()) != null) {
			//If you haven't found a function name yet
			if (functionStarted == false) {
				String[] tokens = line.split("[ \t]+");
				// extract function name
				if (tokens.length == 2) {
					Pattern pattern = Pattern.compile("<(.*)>:");
					Matcher matcher = pattern.matcher(tokens[1]);
					if (matcher.find()) {
						functionStarted = true;
						getCfg().addFunction(Integer.parseInt(tokens[0],16),
								matcher.group(1));
					}
				}
			} else { // Inside function body
				// Blank line signals end of function
				if (line.equals("")) {
					// empty line means that the function is over
					functionStarted = false;
				} else {
					// Add line to function code
					//The method for parsing the individual line is 
					//found in the Code class
					getCfg().getLastFunction().addLine(line);
				}
			}
		}
		reader.close();
	}
	public CFG parse(String topName) throws IOException {
	
		setCfg(new CFG());

		//Get functions and associated assembly code
		parseFunctions(filename);
//
//		System.out.println(getCfg());
		getCfg().build(topName);
//		getCfg().printFunctions();

//		System.out.println(cfg);
//		cfg.printFunctionBounds();
//		getCfg().printLoops();
		getCfg().printDotCFG(filename);
		return cfg;
	}

	public CFG getCFG(){
		return this.getCfg();
	}


	public CFG getCfg() {
		return cfg;
	}

	public void setCfg(CFG cfg) {
		this.cfg = cfg;
	}
	
//	public static void main(String[] args) throws IOException {
//		Parser parser = new Parser("stacktest.objdump");
//		parser.parse();
//	}
}
