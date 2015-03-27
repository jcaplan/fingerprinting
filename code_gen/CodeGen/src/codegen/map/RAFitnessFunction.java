package codegen.map;


import org.jgap.FitnessFunction;
import org.jgap.IChromosome;
import org.jgap.InvalidConfigurationException;

@SuppressWarnings("serial")
public class RAFitnessFunction extends FitnessFunction{
	
	MSEngine ms;
	public RAFitnessFunction(MSEngine ms) {
		this.ms = ms;
	}
	
	@Override
	protected double evaluate(IChromosome chromosome){
		try {
			return ms.getBestSchedule(chromosome);
		} catch (InvalidConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return -1;
	}

}
