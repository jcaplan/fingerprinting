#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define UNLIMIT
#define MAXARRAY 10000 /* this number, if too large, will cause a seg. fault!! */

struct myStringStruct {
  char qstring[15];
};

int compare(const void *elem1, const void *elem2)
{
  int result;
  
  result = strcmp((*((struct myStringStruct *)elem1)).qstring, (*((struct myStringStruct *)elem2)).qstring);

  return (result < 0) ? 1 : ((result == 0) ? 0 : -1);
}


void qsort_small(void){
  struct myStringStruct array[MAXARRAY];
  FILE *fp;
  int i,count=0;

  fp = fopen("/mnt/rozipfs/input_small.dat", "r");
    while((fscanf(fp, "%s", &array[count].qstring) == 1) && (count < MAXARRAY)) {
	 count++;
    }

 // printf("\nSorting %d elements.\n\n",count);
  qsort(array,count,sizeof(struct myStringStruct),compare);
  
// for(i=0;i<count;i++)
//    printf("%s\n", array[i].qstring);

}