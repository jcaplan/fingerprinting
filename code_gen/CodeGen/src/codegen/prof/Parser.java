package codegen.prof;

import java.io.IOException;

public class Parser {

	String filename;
	private CFG cfg;
	public Parser(String filename) {
		this.filename = filename;
	}

	public void parse() throws IOException {
	
		cfg = new CFG();

		//Get functions and associated assembly code
		cfg.getFunctions(filename);

		System.out.println(cfg);
		cfg.build();
		cfg.printFunctions();
		
		cfg.printFunctionBounds();
		
	}


	public static void main(String[] args) throws IOException {
		Parser parser = new Parser("stacktest.objdump");
		parser.parse();
	}
}
