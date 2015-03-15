package codegen.map;

import org.jgap.BaseGene;
import org.jgap.Configuration;
import org.jgap.Gene;
import org.jgap.InvalidConfigurationException;
import org.jgap.RandomGenerator;
import org.jgap.UnsupportedRepresentationException;

@SuppressWarnings("serial")
public class FaultDetectionGene extends BaseGene implements Gene, java.io.Serializable{
	
	
	public enum detectionType {
		fingerprinting,
		lockstep
	};
	

	public FaultDetectionGene(Configuration arg0)
			throws InvalidConfigurationException {
		super(arg0);
		// TODO Auto-generated constructor stub
	}



	@Override
	public int compareTo(Object o) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public void applyMutation(int arg0, double arg1) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public String getPersistentRepresentation()
			throws UnsupportedOperationException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void setAllele(Object arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setToRandomValue(RandomGenerator arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setValueFromPersistentRepresentation(String arg0)
			throws UnsupportedOperationException,
			UnsupportedRepresentationException {
		// TODO Auto-generated method stub
		
	}

	@Override
	protected Object getInternalValue() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	protected Gene newGeneInternal() {
		// TODO Auto-generated method stub
		return null;
	};
	
}
