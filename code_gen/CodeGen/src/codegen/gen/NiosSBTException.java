package codegen.gen;

/**
 * Exception called when executing Nios SBT directly
 * @author jonah
 *
 */
@SuppressWarnings("serial")
public class NiosSBTException extends Exception{

public NiosSBTException() {
		
	}
	
	public NiosSBTException(String message){
		super(message);
	}
	
}
