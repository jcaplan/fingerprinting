#include "annot.h"

static int a[100];

void f1(int x){
	a[1] = x + 12;
}

int f2(int x1, int x2, int x3, int x4, int x5) {
	return x1 + x2 + x3 + x4 + x5;
}


int main(void) {
	ANNOT_MAXITER(1)
	a[0] = 13;
	f1(a[0]);
	f1(a[1]);
	a[2] = f2(a[3],a[4],a[5],a[6],a[7]);
	return 0;
}