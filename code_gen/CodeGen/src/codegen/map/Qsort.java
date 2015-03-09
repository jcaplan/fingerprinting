package codegen.map;

import java.util.ArrayList;

import codegen.sw.Task;

/*************************************************************************
 * Compilation: javac Quick.java Execution: java Quick < input.txt Dependencies:
 * System.out.java StdIn.java Data files:
 * http://algs4.cs.princeton.edu/23quicksort/tiny.txt
 * http://algs4.cs.princeton.edu/23quicksort/words3.txt
 *
 * Sorts a sequence of strings from standard input using quicksort.
 * 
 * % more tiny.txt S O R T E X A M P L E
 *
 * % java Quick < tiny.txt A E E L M O P R S T X [ one string per line ]
 *
 * % more words3.txt bed bug dad yes zoo ... all bad yet
 * 
 * % java Quick < words3.txt all bad bed bug dad ... yes yet zoo [ one string
 * per line ]
 *
 *
 * Remark: For a type-safe version that uses static generics, see
 *
 * http://algs4.cs.princeton.edu/23quicksort/QuickPedantic.java
 *
 *************************************************************************/

public class Qsort {

	private Qsort() {
	}

	public static void sort(ArrayList<Task> a) {
		if (isSorted(a))
			return;
		StdRandom.shuffle(a);
		sort(a, 0, a.size() - 1);
	}

	private static void sort(ArrayList<Task> a, int lo, int hi) {
		if (hi <= lo)
			return;
		int j = partition(a, lo, hi);
		sort(a, lo, j - 1);
		sort(a, j + 1, hi);
		assert isSorted(a, lo, hi);
	}

	private static int partition(ArrayList<Task> a, int lo, int hi) {
		int i = lo;
		int j = hi + 1;
		Task v = a.get(lo);
		while (true) {

			// find item on lo to swap
			while (less(a.get(++i), v))
				if (i == hi)
					break;

			// find item on hi to swap
			while (less(v, a.get(--j)))
				if (j == lo)
					break; // redundant since a[lo] acts as sentinel

			// check if pointers cross
			if (i >= j)
				break;

			exch(a, i, j);
		}

		// put partitioning item v at a[j]
		exch(a, lo, j);

		// now, a[lo .. j-1] <= a[j] <= a[j+1 .. hi]
		return j;
	}

	private static boolean less(Task v, Task w) {
		return (v.compareTo(w) < 0);
	}

	private static void exch(ArrayList<Task> a, int i, int j) {
		Task swap = a.get(i);
		a.set(i, a.get(j));
		a.set(j, swap);
	}

	private static boolean isSorted(ArrayList<Task> a) {
		return isSorted(a, 0, a.size() - 1);
	}

	private static boolean isSorted(ArrayList<Task> a, int lo, int hi) {
		for (int i = lo + 1; i <= hi; i++)
			if (less(a.get(i), a.get(i - 1)))
				return false;
		return true;
	}
}
