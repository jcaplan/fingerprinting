#include "annot.h"

void g(){
	int i;
	for(i = 0; i < 10; i++){
		ANNOT_MAXITER(2);
		i++;
	}

	return;
}

int main(){

	int i,j;

	for(i = 0; i < 10; i++){
		ANNOT_MAXITER(5);
		if(i > 5){

			g();
		}
	}
}
