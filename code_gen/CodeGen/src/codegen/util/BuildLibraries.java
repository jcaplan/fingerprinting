package codegen.util;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;

public class BuildLibraries {

	public static void main(String[] args) throws IOException{

		String[] roots = {"libc","libg","libgcc","libgcov","libm","libnosys"};
		String s = "";
		for(String root : roots){		
			s +=("/********************************************\n");
			s +=("* Library " + root + "\n");
			s +=("*********************************************/\n");
			
			s +=("static HashMap<String,String> " + root + " = new HashMap<>();\n");
			s += "static {\n";
			String filename = "../nioslib/" + root + ".a_functions.txt";
			FileReader fr = new FileReader(filename);
			BufferedReader reader = new BufferedReader(fr);
	
			// initialize the reader and cfg
			reader = new BufferedReader(fr);
			String line;
			String objName = "";
			String funcName = "";
			while ((line = reader.readLine()) != null) {
				if(line.contains(".o")){
					objName = line;
				} else {
					funcName = line;
					s +=(root + ".put(\"" + funcName + "\",\"" + objName + "\");\n");
				}
			}
			s += "}\n\n";
			reader.close();
		}
		
		PrintWriter writer;
		try {
			writer = new PrintWriter("library", "UTF-8");
			writer.println(s);
			writer.close();
		} catch (FileNotFoundException | UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
