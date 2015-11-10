#include "annot.h"

int f(int x){
	return x++;
}

int g() {
	int i,x;
	for(i  = 0; i < 10; i++){
		ANNOT_MAXITER(3);
		x = 0;
		while( x < 7){
			ANNOT_MAXITER(7);
			x = f(x);
			if(x % 2 == 0){
				i++;
			}
		}
	}
	return x;
}


int main(){
	return g();
}