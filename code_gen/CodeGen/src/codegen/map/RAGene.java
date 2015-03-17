package codegen.map;

import org.jgap.impl.*;
import org.jgap.*;

public class RAGene  {
	
	CompositeGene raGene;
	int maxNumGroups;
	static final int NUM_FT_METHODS = 3;
	final int NUM_FD_METHODS = 2;
	final int NUM_GENES = 3 ;
	
	public final static int FT_FPRINT_DMR_WRETRY = 0;
	public final static int FT_FPRINT_TMR = 1;
	public final static int FT_TMR = 2;
	
	public final static int FD_FPRINT = 0;
	public final static int FD_DMR = 1;
	
	public static final int GROUP_INDEX = 0;
	public static final int FD_INDEX = 1;
	public static final int FT_INDEX = 2;
	
	
	public RAGene(Configuration a_configuration,int maxNumGroups) throws InvalidConfigurationException{
		this.maxNumGroups = maxNumGroups;
		init(a_configuration);
	}
	
	private void init(Configuration a_configuration) throws InvalidConfigurationException{
		raGene = new CompositeGene(a_configuration);

		Gene groupField = new IntegerGene(a_configuration,0,maxNumGroups-1);
		raGene.addGene(groupField);
		
		Gene fdField = new IntegerGene(a_configuration,0,NUM_FD_METHODS-1);
		raGene.addGene(fdField);
		
		Gene ftField = new IntegerGene(a_configuration,0,NUM_FT_METHODS-1);
		raGene.addGene(ftField);
	}
	
	public int getTaskGroup(){
		return (int)raGene.geneAt(GROUP_INDEX).getAllele();
	}
	
	public int getDetectionType(){
		return (int)raGene.geneAt(FD_INDEX).getAllele();
	}
	
	public int getToleranceType(){
		return (int)raGene.geneAt(FT_INDEX).getAllele();
	}
	
	public static String getGeneString(Gene g){
		String s = "";
		ICompositeGene cg = (ICompositeGene) g;
		s += "Group: " + cg.geneAt(GROUP_INDEX).getAllele() + "; ";
		s += "Detection Type: ";
		switch((int)cg.geneAt(FD_INDEX).getAllele()){
		case FD_FPRINT:
			s += "Fingerprinting; ";
			break;
		case FD_DMR:
			s += "Lockstep DMR; ";
			break;
		}
		
		s += "Correction Type: ";
		switch((int)cg.geneAt(FT_INDEX).getAllele()){
		case FT_FPRINT_DMR_WRETRY:
			s += "Fprint DMR w/ retry; ";
			break;
		case FT_FPRINT_TMR:
			s += "Fprint TMR; ";
			break;
		case FT_TMR:
			s += "Lockstep TMR ; ";
			break;
		}
		
		return s;
	}
	public static Gene[] getGeneArray(RAGene[] array){
		Gene[] newArray = new Gene[array.length];
		for(int i = 0; i < array.length; i++){
			newArray[i] = array[i].raGene;
		}
		return newArray;
	}
	
}
