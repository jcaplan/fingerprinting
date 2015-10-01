package codegen.util;

public class Logger {

	static boolean logging;
	
	
	public static void logMessage(Object message){ 
		if(logging){
			System.out.println(message);
		}
	}


	public static void turnLoggingOn() {
		logging = true;
	}
	
	public static void turnLoggingOff() {
		logging = false;
	}
}