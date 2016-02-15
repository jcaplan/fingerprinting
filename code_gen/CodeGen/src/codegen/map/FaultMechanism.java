package codegen.map;

import java.io.Serializable;
import java.lang.reflect.Constructor;
import java.util.*;

import org.jgap.Gene;

public abstract class FaultMechanism implements Serializable{

	private static final int[] reexecutionProfile = {1,1,1,1};
	
	private static final Map<String, Class<? extends FaultMechanism>> mechanismTypes = new HashMap<String, Class<? extends FaultMechanism>>();
    static {
        // Transactions
    	FaultMechanism.addCommand(DMR.class);
    	FaultMechanism.addCommand(TMR.class);
    	FaultMechanism.addCommand(PassiveReplication.class);
    	FaultMechanism.addCommand(Lockstep.class);
    }
    
    private static void addCommand(final Class<? extends FaultMechanism> commandClass) {
        try {
            final Constructor<? extends FaultMechanism> constructor = commandClass.getConstructor();
            final FaultMechanism detectionType = constructor.newInstance();
            FaultMechanism.mechanismTypes.put(detectionType.getCommandName(), commandClass);
        } catch (final Exception e) {
        }
    }
    
    public static FaultMechanism getMechanismInstance(final String commandName) {
        final Class<? extends FaultMechanism> commandClass = FaultMechanism.mechanismTypes.get(commandName);
        if (commandClass == null) {
            return null;
        }
        Constructor<? extends FaultMechanism> constructor;
        try {
            constructor = commandClass.getConstructor();
            final FaultMechanism commandInstance = constructor.newInstance();
            return commandInstance;
        } catch (final Exception e) {
            return null;
        }
    }

    
    public static Set<String> getAvailableMechanisms() {
        return FaultMechanism.mechanismTypes.keySet();
    }
    
    public static String[] getAvailableMechanismsArray(){
    	String[] mecs = new String[getAvailableMechanisms().size()];
    	getAvailableMechanisms().toArray(mecs);
    	return mecs;
    }
    
    public static String[] getOdrMechanismsArray(){
    	Set<String> mechList = FaultMechanism.mechanismTypes.keySet();
    	mechList.remove("Lockstep");
    	String[] mecs = new String[mechList.size()];
    	mechList.toArray(mecs);
    	 return mecs;
    }
    
    public static int getNumMechanisms(){
    	return FaultMechanism.mechanismTypes.size();
    }
    public abstract String getCommandName();

	public abstract void updateTaskList(ArrayList<Task> taskList, int i, Map<Task,Task[]> replicas, 
			Map<Task, FaultMechanism> techniqueMap, Map<Task, int[]> executionProfiles);

	public abstract int getPermutationSize(int numLegalProcessors);

	public abstract void allocateProcessors(Schedule schedule, int permutation,
			Map<Task, ArrayList<Processor>> legalMappings, Task t, Map<Task,Task[]> replicas);


}
