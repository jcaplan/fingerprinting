#include "annot.h"

static int global1 = 0;

static int a[1000];

//basic loop, problem with -O
int *g(int j){
	int i;
	int k = 0;
	for(i = 0; i < 99; i++){
		ANNOT_MAXITER(99);
		k += inter(i*j);
		a[i] = k;

	}



	return a;
}


//basic loop
//Hybrid pattern when optimized...
int g1(int j){
	int i;
	int k = 0;
	for(i = 0; i < 99; i++){
		ANNOT_MAXITER(99);
		a[i] = k;
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
	while( k < 300){
		y = x + y;
		k+=1 + 1;
		k+=1;
		k+=-1;
		k+=4;
		a[k] = k;
	}
	return k;
}


//decrement, threshold > 0
int g3(int x, int y){
	int k = 400;
	if(x){
		y = 0;
	} else {
		y = 3;
	}
	while( k > 1){
		y = x + y;
		k-=1;
		a[k] = y;
	}
	return k;
}

// decrement, threshold = 0
int g4(int x, int y){
	int k = 400;
	if(x){
		y = 0;
	} else {
		y = 3;
	}
	while( k > 0){
		y = x + y;
		k-=1;
		a[k] = y;
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
		a[k] = y;
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
	while( k < 500){
		y = x + y;
		k+=1 + 1;
		k+=1;
		k+=-1;
		int z = w * 3; 
		k+=z;
		a[k] = k;
	}
	return k;
}


//switch cases
int g7(int x){
	int result;
	switch(x){
	case 0:
		result = 1;
		break;
	case 1:
	case 2:
		result = 2;
		break;
	case 3:
		result = 4;
		break;
	default:
		result = 0;
		break;
	}
	return result;
}

//indirect, branching
int g8 (){

	int k = 1;
	int x = 0;
	while(k == 1){
		if(x == 105){
			k = 0;
		}
		x++;
		a[x] = x;
	}
	return x;
}

//multiple backward edges, indirect
int g9 (){

	int k = 1;
	int x = 0;
	while(k == 1){
		if(x++ == 105){
			k = 0;
		}
		a[x] = x;
	}
	return x;
}

// upper bound is constant but stored in var
int g10(){
	int k = 301;
	int i = 0;
	int y = 0;
	for(i = 0; i < k; i++){
		y += i;
		a[i] = y;
	}
	return y;
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

//works unoptimized
void f(){
	int i = 0;
	int j;
	while(i < 100){
		ANNOT_MAXITER(100);
		int j = 5;
		while(j < 50){
			ANNOT_MAXITER(50);
			j++;
			a[i + j] = j;
		}
		i++;
	}

	int k;
	for(i = 0; i < 3; i++){
		ANNOT_MAXITER(200);
		for(j = 0; j < 200; j++){
			ANNOT_MAXITER(200);
			k = i*j;
			a[i*200 + j] = k;
		}
	}
}

//inner loop depends on outer loop iterator
void f1(){
	int i,j,k;
	for(i = 0; i < 200; i++){
		ANNOT_MAXITER(200);
		for(j = i; j < 200; j++){
			ANNOT_MAXITER(200);
			k = i*j;
			a[i + j] = k;
		}
	}
}

int main(){

	int i,j;

	for(i = 0; i < 10; i++){
		ANNOT_MAXITER(10);
		if(i > 5){

			j += *g(i);
		}
	}

	return j;
}
