package codegen.map;

import java.util.Random;

import org.jgap.FitnessFunction;
import org.jgap.IChromosome;

@SuppressWarnings("serial")
public class RAFitnessFunction extends FitnessFunction{

	@Override
	protected double evaluate(IChromosome arg0) {
		
		Random rand = new Random();
		return rand.nextDouble();
		
		// TODO real fitness function
	}

}
