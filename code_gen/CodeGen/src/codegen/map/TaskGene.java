package codegen.map;

import java.util.StringTokenizer;

import org.jgap.BaseGene;
import org.jgap.Configuration;
import org.jgap.Gene;
import org.jgap.InvalidConfigurationException;
import org.jgap.RandomGenerator;
import org.jgap.UnsupportedRepresentationException;

@SuppressWarnings("serial")
public class TaskGene extends BaseGene implements Gene, java.io.Serializable {
	private static final String TOKEN_SEPARATOR = ":";

	int maxNumGroups;
	int group;

	public TaskGene(Configuration arg0, int maxNumGroups)
			throws InvalidConfigurationException {
		super(arg0);
		this.maxNumGroups = maxNumGroups;
	}

	@Override
	public int compareTo(Object o) {
		TaskGene gene = (TaskGene) o;
		if (this.group > gene.group)
			return 1;
		else if (this.group == gene.group)
			return 0;
		else
			return -1;

	}

	@Override
	public void applyMutation(int arg0, double arg1) {
		setAllele(getConfiguration().getRandomGenerator().nextInt(maxNumGroups));

	}

	/**
	 * @return A string representation of this Gene's current state
	 */
	@Override
	public String getPersistentRepresentation()
			throws UnsupportedOperationException {

		return String.valueOf(group) + TOKEN_SEPARATOR
				+ String.valueOf(maxNumGroups);
	}

	/**
	 * Sets the value of this Gene to the new given value.
	 * 
	 * @param numGroups
	 *            the new value of this Gene.
	 * 
	 */
	@Override
	public void setAllele(Object numGroups) {
		this.group = (Integer) numGroups;
	}

	/**
	 * Set the number of groups to a random number
	 * 
	 * @param rand
	 *            A random number of generator
	 */
	@Override
	public void setToRandomValue(RandomGenerator rand) {
		group = rand.nextInt(maxNumGroups);
	}

	@Override
	public void setValueFromPersistentRepresentation(String a_representation)
			throws UnsupportedOperationException,
			UnsupportedRepresentationException {
		StringTokenizer tokenizer = new StringTokenizer(a_representation,
				TOKEN_SEPARATOR);
		if (tokenizer.countTokens() != 2) {
			throw new UnsupportedRepresentationException(
					"Unknown rerpesentation format: Two tokens expected");
		}
		try {
			group = Integer.parseInt(tokenizer.nextToken());
			maxNumGroups = Integer.parseInt(tokenizer.nextToken());
		} catch (NumberFormatException e) {
			throw new UnsupportedRepresentationException(
					"Unknown representation format: Expecting integer values.");
		}
	}

	/**
	 * 
	 * @return The value of this gene
	 */
	@Override
	protected Object getInternalValue() {
		return group;
	}

	/*****************
	 * Get a new gene with the same paramaters
	 *
	 * @return A new gene instance of the same type with the same setup as this
	 *         concrete Gene.
	 */
	@Override
	protected Gene newGeneInternal() {
		try {
			// We construct the new QuarterGene with the same maximum number
			// of quarters that this Gene was constructed with.
			// -------------------------------------------------------------
			return new TaskGene(getConfiguration(), group);
		} catch (InvalidConfigurationException ex) {
			throw new IllegalStateException(ex.getMessage());
		}
	}

}
