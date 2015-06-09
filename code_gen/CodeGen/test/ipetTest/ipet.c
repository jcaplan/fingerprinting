void g(){
	int i;
	for(i = 0; i < 10; i++){
		i++;
	}

	return;
}
int main(){


	int i,j;

	for(i = 0; i < 10; i++){
		if(i > 5){

			g();
		}
	}
}