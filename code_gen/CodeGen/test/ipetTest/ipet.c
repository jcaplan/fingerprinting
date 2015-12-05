#include "annot.h"
#include <stdio.h>

static int a[1000];
int inter(int k);

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

/*
 * Conditional stuff starts
 */

// initial defined on two paths
void g11(int j){
	int x = 0;
	if(j > 10){
		x = 100;
	}
	while(x < 101){
		x++;
		a[x] = 3;
	}
}

// initial only defined on one path
void g12(int j, int x){
	if(j > 10){
		x = 100;
	}
	while(x < 101){
		x++;
		a[x] = 3;
	}
}

// threshold defined on two paths
void g13(int x){
	int t;
	if(x > 5){
		t = 100;
	} else {
		t = 200;
	}
	int i;
	for(i = 0; i < t; i++){
		a[i] = x;
	}

}

// initial value defined on two paths
void g14(int x){
	int i;
	if(x < 0){
		i = 0;
	} else {
		i = 10;
	}
	for(; i < 100; i++){
		a[i] = x;
	}
}

// increment defined on two paths
void g15(int j){
	int k;
	int x = 3;
	if(j > 10){
		k = 2;
	} else {
		k = 4;
	}
	while(x < 103){
		x+=k;
		a[x] = 3;
	}
}



// increment only defined on one path
void g16(int j){
	int k;
	int x = 3;
	if(j > 10){
		k = 2;
	}
	while(x < 103){
		x+=k;
		a[x] = 3;
	}
}

// threshold only defined on one path
void g17(int x){
	int t;
	if(x > 5){
		t = 100;
	}
	int i;
	for(i = 0; i < t; i++){
		a[i] = x;
	}

}
//slli
void g18(int x){
	int k = 3;
	a[k*32] = k;
	x = a[k*32];
	while(x < 103){
		x+=1;
		a[x] = 3;
	}
}

//mul
void g19(int x){
	int k = 3;
	int j = 32;
	int l = k*j;
	a[l] = k;
	x = a[k*j];
	while(x < 103){
		x+=1;
		a[x] = 3;
	}
}

// sign is wrong way...
// there are two ways to break...
// zero times or infinite times
// should throw error for infinite
// but should allow zero...
void g20(int x){
	int k = 3;
	int j = 29;
	a[k*j] = k;
	x = a[k*j];
	while(x > 103){
		x+=1;
		a[x] = 3;
	}
}

//division
void g21 (int x){
	int k = 3;
	int j = 29;
	a[j/k] = k;
	x = a[j/k];
	while(x < 103){
		x+=1;
		a[x] = 3;
	}
}

//could be infinite
void g22(int x) {
	int i;
	int k;
	if(x < 5){
		k = 1;
	} else {
		k = -1;
	}

	for(i = 0; i < 10; i += k){
		a[i] = i;
	}
}

//Condition depends on induction
void g23(void){
	int i;
	int j = 1;
	for(i = 0 ; i < 150; i++){
		a[i] = 1000*i/2;
		if(i > 100){
			a[i] += g1(j);
		}
	}
}


int inter(int k){
	return k;
}

int h(){
	int y = 0;
	int x = 0;
	
	while (x < 10) {
	    y = x++;
	}
	return y;
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

	int i,j = 0;

	for(i = 0; i < 10; i++){
		ANNOT_MAXITER(10);
		if(i > 5){

			j += *(g(i));
		}
	}
	g17(-1);
	g13(0);
	g14(-1);
	g23();
	return j;
}
