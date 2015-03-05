#include "basicmath.h"
#include "snipmath.h"
#include <math.h>
void basicmath_small(int* loopvalues, BasicmathSol* sol) {

	double a1 = 1.0, b1 = -10.5, c1 = 32.0, d1 = -30.0;
//	double a2 = 1.0, b2 = -4.5, c2 = 17.0, d2 = -30.0;
//	double a3 = 1.0, b3 = -3.5, c3 = 22.0, d3 = -31.0;
//	double a4 = 1.0, b4 = -13.7, c4 = 1.0, d4 = -35.0;
//
	double x[3];
//	double X;
	int solutions;
	int i;
//	int i;
//	unsigned long l = 0x3fed0169L;
//	struct int_sqrt q;
//	long n = 0;
//
//	SolveCubic(a1, b1, c1, d1, &solutions, x);
//
//	SolveCubic(a2, b2, c2, d2, &solutions, x);
//
//	SolveCubic(a3, b3, c3, d3, &solutions, x);
//
//	SolveCubic(a4, b4, c4, d4, &solutions, x);

		/* Now solve some random equations */
	for (i = 0; i < loopvalues[0]; i++) {
					SolveCubic(a1, b1, c1, d1, &solutions, x);
	}

//	  for(a1=1;a1<10;a1++) {
//	    for(b1=10;b1>0;b1--) {
//	      for(c1=5;c1<15;c1+=0.5) {
//		for(d1=-1;d1>-11;d1--) {
//		  SolveCubic(a1, b1, c1, d1, &solutions, x);
//		}
//	      }
//	    }
//	  }
//	//
//	  for (i = 0; i < 1001; ++i)
////		for (i = 0; i < 100001; ++i)
//		{
//		  usqrt(i, &q);
//		}
//	  usqrt(l, &q);
////
//	  /* convert some rads to degrees */
//	  for (X = 0.0; X <= 360.0; X += 1.0)
//		deg2rad(X);
//	  for (X = 0.0; X <= (2 * PI + 1e-6); X += (PI / 180))
//		rad2deg(X);
//
//	  sol->num_solution = solutions;
//	  sol->x[0] = x[0];
//	  sol->x[1] = x[1];
//	  sol->x[2] = x[2];
}

void basicmath_large() {
	double a1 = 1.0, b1 = -10.5, c1 = 32.0, d1 = -30.0;
	double x[3];
	double X;
	int solutions;
	int i;
	unsigned long l = 0x3fed0169L;
	struct int_sqrt q;
	long n = 0;

	/* solve soem cubic functions */

	/* should get 3 solutions: 2, 6 & 2.5   */
	SolveCubic(a1, b1, c1, d1, &solutions, x);

	a1 = 1.0;
	b1 = -4.5;
	c1 = 17.0;
	d1 = -30.0;
	/* should get 1 solution: 2.5           */
	SolveCubic(a1, b1, c1, d1, &solutions, x);

	a1 = 1.0;
	b1 = -3.5;
	c1 = 22.0;
	d1 = -31.0;
	SolveCubic(a1, b1, c1, d1, &solutions, x);

	a1 = 1.0;
	b1 = -13.7;
	c1 = 1.0;
	d1 = -35.0;
	SolveCubic(a1, b1, c1, d1, &solutions, x);

	a1 = 3.0;
	b1 = 12.34;
	c1 = 5.0;
	d1 = 12.0;
	SolveCubic(a1, b1, c1, d1, &solutions, x);

	a1 = -8.0;
	b1 = -67.89;
	c1 = 6.0;
	d1 = -23.6;
	SolveCubic(a1, b1, c1, d1, &solutions, x);

	a1 = 45.0;
	b1 = 8.67;
	c1 = 7.5;
	d1 = 34.0;

	a1 = -12.0;
	b1 = -1.7;
	c1 = 5.3;
	d1 = 16.0;

	/* Now solve some random equations */
	for (a1 = 1; a1 < 10; a1 += 1) {
		for (b1 = 10; b1 > 0; b1 -= .25) {
			for (c1 = 5; c1 < 15; c1 += 0.61) {
				for (d1 = -1; d1 > -5; d1 -= .451) {
					SolveCubic(a1, b1, c1, d1, &solutions, x);
				}
			}
		}
	}

	/* perform some integer square roots */
	for (i = 0; i < 100000; i += 2) {
		usqrt(i, &q);

	}
	for (l = 0x3fed0169L; l < 0x3fed4169L; l++) {
		usqrt(l, &q);

	}

	//printf("********* ANGLE CONVERSION ***********\n");
	/* convert some rads to degrees */
	/*   for (X = 0.0; X <= 360.0; X += 1.0) */
	for (X = 0.0; X <= 360.0; X += .001)
		deg2rad(X);

	/*   for (X = 0.0; X <= (2 * PI + 1e-6); X += (PI / 180)) */
	for (X = 0.0; X <= (2 * PI + 1e-6); X += (PI / 5760))
		rad2deg(X);

}
