package codegen.prof;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CFG {

	ArrayList<Function> fList = new ArrayList<>();

	public void addFunction(Function f) {
		fList.add(f);
	}

	public void addFunction(int startAddress, String label) {
		fList.add(new Function(startAddress, label));
	}

	public Function getLastFunction() {
		return fList.get(fList.size() - 1);
	}

	@Override
	public String toString() {
		String s = "";

		s += "**********************\n";
		for (Function f : fList) {
			s += "Function " + f.label;
			s += " (" + Integer.toHexString(f.startAddress) + ")\n";

		}
		return s;
	}

	public void printFunctions() {
		for (Function f : fList) {
			f.printBasicBlocks();
		}
	}
	
	public void printFunctionBounds(){
		for(Function f : fList){
			System.out.println("Function " + f.label + "; start address: " + f.getStartAddressHex() +
					"; end address: " + f.getEndAddresshex());
		}
	}

	void getFunctions(String filename) throws IOException {

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
						addFunction(Integer.parseInt(tokens[0],16),
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
					getLastFunction().addLine(line);
				}
			}
		}
		reader.close();
	}

	public void build() {
		//Identify basic blocks
		for(Function f : fList){
			f.buildBlocks();
		}
		
		//Calls, only have to search functions
		//branches need to search all the functions
		
		//do returns on separate pass once branches and calls are all done

	}
}
