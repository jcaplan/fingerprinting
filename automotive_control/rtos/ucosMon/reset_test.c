#include "includes.h"
#include <stdlib.h>
#include <stdio.h>
#include "AirbagModel.h"


int main(){

	int* reset0 = PROCESSOR0_0_SW_RESET_0_BASE;

	int i;
	for(i = 0; i < 100; i++){

		int j;
		for(j = 0; j < 100000; j++)
			;
		printf("reset %x\n",reset0);
		*reset0 = 1;

	}


	return 0;
}
