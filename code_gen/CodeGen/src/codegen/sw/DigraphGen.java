
package codegen.sw;

import java.util.Random;

import codegen.util.Qsort;


public class DigraphGen {
	int minPerRank;
	int maxPerRank;
	int numRanks;
	double probEdge;
	double probCritical;
	int rankIndex[];
	public DigraphGen(int minPerRank, int maxPerRank, int numRanks,
			double probEdge, double probCritical) {
		this.minPerRank = minPerRank;
		this.maxPerRank = maxPerRank;
		this.numRanks = numRanks;
		this.probEdge = probEdge;
		this.probCritical = probCritical;
		rankIndex = new int[numRanks];
	}

	public void buildGraph(Application a) {
		Random rand = new Random();
		int nodes = 0;
		for (int i = 0; i < numRanks; i++) {
			int newNodes = minPerRank
					+ rand.nextInt(maxPerRank - minPerRank);
			for (int j = 0; j < newNodes; j++) {
				Task t = new Task();
				if(rand.nextDouble() < probCritical)
					t.critical = true;
				a.addTask(t);
				t.rank = i;
				if(i > 0){
					for(int k = 0; k < nodes; k++){
						if(rand.nextDouble() < probEdge){
							a.setTaskConnection(a.taskList.get(k), t);
						}
					}
				}
				
			}
			nodes += newNodes;
		}
		Qsort.sort(a.taskList);
	}
}
