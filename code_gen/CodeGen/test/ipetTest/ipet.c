#include "annot.h"

int global1 = 0;


//basic loop, problem with -O
int g(int j){
	int i;
	int k = 0;
	for(i = 0; i < 99; i++){
		ANNOT_MAXITER(99);
		k += inter(i*j);
	}



	return k;
}


//basic loop, optimized out with -O
int g1(int j){
	int i;
	int k = 0;
	for(i = 0; i < 99; i++){
		ANNOT_MAXITER(99);
		k = i;
	}
	return k;
}

//compound expression
int g2(int x, int y){
	int k = 0;
	if(x){
		y = 0;
	} else {
		y = 3;
	}
	while( k < 15){
		y = x + y;
		k+=1 + 1;
		k+=1;
		k+=-1;
		k+=4;
	}
	return k;
}


//decrement, threshold > 0
int g3(int x, int y){
	int k = 5;
	if(x){
		y = 0;
	} else {
		y = 3;
	}
	while( k > 1){
		y = x + y;
		k-=1;
	}
	return k;
}

// decrement, threshold = 0
int g4(int x, int y){
	int k = 5;
	if(x){
		y = 0;
	} else {
		y = 3;
	}
	while( k > 0){
		y = x + y;
		k-=1;
	}
	return k;
}

//head == tail, -O 
int g5(int x, int y, int *z){
	int k = 0;
	if(x){
		y = 0;
	} else {
		y = 3;
	}
	while( k < 10){
		y += *z*(x + *z);
		k+=1;
	}
	return y;
}

//compound expression, w initialized outside loop
//constant
int g6(int x, int y){
	int k = 0;
	if(x){
		y = 0;
	} else {
		y = 3;
	}
	int w = 1;
	while( k < 15){
		y = x + y;
		k+=1 + 1;
		k+=1;
		k+=-1;
		int z = w * 3; 
		k+=z;
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
