#include "annot.h"

void g(){
	int i;
	for(i = 0; i < 99; i++){
		ANNOT_MAXITER(99);
		i++;
	}



	return;
}

void h(){
	int y = 0;
	int x = 0;
	while (x < 10) {
	    if (!(x % 2)) {
	        x = x + 1;
	        
	    } else {
	    	x = x + 3;
		}
	}
}

// void f(){
// 	int i = 0;
// 	int j = 5;
// 	while(i < 100){
// 		ANNOT_MAXITER(100);
// 		while(j < 50){
// 			ANNOT_MAXITER(50);
// 			j++;
// 		}
// 		i++;
// 	}

// 	int k;
// 	for(i = 0; i < 200; i++){
// 		ANNOT_MAXITER(200);
// 		for(j = 0; j < 200; j++){
// 			ANNOT_MAXITER(200);
// 			k = i*j;
// 		}
// 	}
// }

int main(){

	int i,j;

	for(i = 0; i < 10; i++){
		ANNOT_MAXITER(10);
		if(i > 5){

			g();
		}
	}

//	h(); //doesn't work right now
	// f();


	// double d1 = 0.3;
	// double d2 = 3.1;
	// double d3 = d1 * d2;
}
