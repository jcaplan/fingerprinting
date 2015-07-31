package codegen.gen;

/**
 * Exception thrown while parsing configuration.
 * @author jonah
 *
 */
@SuppressWarnings("serial")
public class ConfigurationException extends Exception {
	
	public ConfigurationException() {
		
	}
	
	public ConfigurationException(String message){
		super(message);
	}
}
