#include <stdlib.h>


int main() {
		int x[100];
	x[0] = 10;
	int i;
	for(i = 0; i < 10; i++){
		switch(i){
			case 0:
			case 1:
				x[i] = i;
				break;
			case 2:
				x[5] = x[3];
				break;
			case 3:
				x[12] = x[14] + x[3];
				break;
			default:
				break;
		}
	}
	return x[0];
}