package codegen.map;

/*
 * This file is part of JGAP.
 *
 * JGAP offers a dual license model containing the LGPL as well as the MPL.
 *
 * For licensing information please see the file license.txt included with JGAP
 * or have a look at the top of class org.jgap.Chromosome which representatively
 * includes the JGAP license policy applicable for any file delivered with JGAP.
 */

import org.jgap.*;
import org.jgap.impl.*;
import org.jgap.util.*;
import org.jgap.event.*;

/**
 * The MapConfiguration class simplifies the JGAP configuration
 * process by providing default configuration values for many of the
 * configuration settings. The following values must still be provided:
 * the sample Chromosome, population size, and desired fitness function.
 * All other settings may also be changed in the normal fashion for
 * those who wish to specify other custom values.
 *
 * @author Neil Rotstan
 * @author Klaus Meffert
 * @since 1.0
 * 
 * @author Jonah
 * modified 2015
 */
public class MSConfiguration
    extends Configuration implements ICloneable {
  /** String containing the CVS revision. Read out via reflection!*/
  private final static String CVS_REVISION = "$Revision: 1.26 $";

  public MSConfiguration() {
    this("","");
  }

  /**
   * Constructs a new MapConfiguration instance with a number of
   * configuration settings set to default values. It is still necessary
   * to set the sample Chromosome, population size, and desired fitness
   * function. Other settings may optionally be altered as desired.
   *
   * @param a_id unique id for the configuration within the current thread
   * @param a_name informative name of the configuration, may be null
   *
   * @author Neil Rotstan
   * @author Klaus Meffert
   * @since 1.0
   */
  public MSConfiguration(String a_id, String a_name) {
    super(a_id, a_name);
    try {
      setBreeder(new GABreeder());
      setRandomGenerator(new StockRandomGenerator());
      setEventManager(new EventManager());
      setMinimumPopSizePercent(0);
      //
      setSelectFromPrevGen(1.0d);
      setKeepPopulationSizeConstant(true);
      setFitnessEvaluator(new DefaultFitnessEvaluator());
      setChromosomePool(new ChromosomePool());
      //New values
      //----------
      addGeneticOperator(new CrossoverOperator(this,0.8d));
      addGeneticOperator(new MutationOperator(this,10));
      TournamentSelector tournamentSelector = new TournamentSelector(this,10,0.8);
      addNaturalSelector(tournamentSelector, false);
    }
    catch (InvalidConfigurationException e) {
      throw new RuntimeException(
          "Fatal error: MapConfiguration class could not use its "
          + "own stock configuration values. This should never happen. "
          + "Please report this as a bug to the JGAP team.");
    }
  }

  /**
   * @return deep clone of this instance
   *
   * @author Klaus Meffert
   * @since 3.2
   */
  public Object clone() {
    return super.clone();
  }
}
