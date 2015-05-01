#include <stdlib.h>

int g(){
	int y[100];
	y[0]++;
	return y[0];
}

int f(){
	int x[100];
	x[0] = 10;
	int i;
	for(i = 0; i < 10; i++)
		g();
	return x[0];
}

int main() {
	int x[52];
	int i;
	for(i = 0; i < 52; i++){
		int y[30];
		x[i] = f();
	}

	if(1){
		x[2] = 3;
	} else {
		int y[300];
		x[1] = y[1];
	}


	switch(i){
		case 0:
		case 1:
		case 2:
			break;
		case 3:
			i++;
			break;
		case 4:
			i+= 3;
			break;
		default:
			if(i == 12){
				i = i % 3;
			} else {
				i /= 2;
			}
			break;
	}
	return x[0] + i;
}