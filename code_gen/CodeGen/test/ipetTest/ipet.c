#include "annot.h"

int g(int j){
	int i;
	int k = 0;
	for(i = 0; i < 99; i++){
		ANNOT_MAXITER(99);
		k += inter(i*j);
	}



	return k;
}

int g1(int j){
	int i;
	int k = 0;
	for(i = 0; i < 99; i++){
		ANNOT_MAXITER(99);
		k = i;
	}



	return k;
}

int inter(int k){

	return k;
}

void h(){
	int y = 0;
	int x = 0;
	while (x < 10) {
	    y = x++;
	}
}

void f(){
	int i = 0;
	int j = 5;
	while(i < 100){
		ANNOT_MAXITER(100);
		while(j < 50){
			ANNOT_MAXITER(50);
			j++;
		}
		i++;
	}

	int k;
	for(i = 0; i < 200; i++){
		ANNOT_MAXITER(200);
		for(j = 0; j < 200; j++){
			ANNOT_MAXITER(200);
			k = i*j;
		}
	}
}


int main(){

	int i,j;

	for(i = 0; i < 10; i++){
		ANNOT_MAXITER(10);
		if(i > 5){

			j += g(i);
		}
	}

	return j;
}
