package codegen.prof;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Parser {

	String objdump;
	String annot;
	private CFG cfg;
	public Parser(String fileDir, String rootName) {
		this.objdump = fileDir + "/" + rootName + ".objdump";
		this.annot = fileDir + "/" + rootName + ".annot";
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
	
	public CFG parseCFG(String topName, ArrayList<Annotation> annotations) throws IOException {
	
		setCfg(new CFG(annotations));

		//Get functions and associated assembly code
		parseFunctions(objdump);
//
//		System.out.println(getCfg());
		getCfg().build(topName);
//		getCfg().printFunctions();
		//TODO HERE is where you check for the annot.obj file 
		// and add loop bounds to functions based on whether or not it exists
		// and what you find there
		
		
//		System.out.println(cfg);
//		cfg.printFunctionBounds();
//		getCfg().printLoops();
		getCfg().printDotCFG(objdump);
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

	public ArrayList<Annotation> parseAnnotations() throws IOException {
		ArrayList<Annotation> annotList = new ArrayList<>();
		
		FileReader fr = new FileReader(annot);
		BufferedReader reader = new BufferedReader(fr);
		reader = new BufferedReader(fr);
		
		ArrayList<Integer> tokens = new ArrayList<>();
		
		String line = "";
		
		/*
		 * skip the front matter
		 */
		while(!reader.readLine().contains("Contents of section .wcet_annot:"))
			;
		
		
		while ((line = reader.readLine()) != null){
			line = line.trim();
			String[] tkns = line.split("[\\s]+");
			
			
			/*
			 * Always ignore first and last token (address and random text)
			 */
			for(int i = 1; i < tkns.length - 1; i++){
				try{
					String value = reverseEndian(tkns[i]);
					tokens.add(Integer.parseInt(value, 16));
				} catch (NumberFormatException | StringIndexOutOfBoundsException e){
					//sometimes spaces in the last token mess things up...
				}
			}
		}
		
		reader.close();
		
		/*
		 * Now there is a parsed list of integers, each 3 tuple represents an annotation
		 */
		
		while(tokens.size() > 0) {
			int address = tokens.get(0);
			int type = tokens.get(1);
			int value = tokens.get(2);
			
			tokens.remove(2);
			tokens.remove(1);
			tokens.remove(0);
			
			annotList.add(new Annotation(address, value,type));
			
		}
		
		
		return annotList;
	}

	private String reverseEndian(String string) {
		String newString = "";
		newString += string.substring(6, 8);
		newString += string.substring(4, 6);
		newString += string.substring(2, 4);
		newString += string.substring(0, 2);
		
		return newString;
		
	}
	
//	public static void main(String[] args) throws IOException {
//		Parser parser = new Parser("stacktest.objdump");
//		parser.parse();
//	}
}
