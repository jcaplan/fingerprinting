#include "annot.h"

static int a[100];

void f(int x){
	a[1] = x + 12;
}




int main(void) {
	ANNOT_MAXITER(1)
	a[0] = 13;
	f(a[0]);
	f(a[1]);
	return 0;
}