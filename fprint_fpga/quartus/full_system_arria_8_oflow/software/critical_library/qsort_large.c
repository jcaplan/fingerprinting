#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct my3DVertexStruct {
  int x, y, z;
  double distance;
};

int compare(const void *elem1, const void *elem2)
{
  /* D = [(x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2]^(1/2) */
  /* sort based on distances from the origin... */

  double distance1, distance2;

  distance1 = (*((struct my3DVertexStruct *)elem1)).distance;
  distance2 = (*((struct my3DVertexStruct *)elem2)).distance;

  return (distance1 > distance2) ? 1 : ((distance1 == distance2) ? 0 : -1);
}


void qsort_large(int* coordinates, int size) {
  struct my3DVertexStruct array[size];

  int i;
  	for(i = 0; i < size*3; i+=3){
  		int index = i/3;

  		int x = coordinates[i];
  		int y = coordinates[i+1];
  		int z = coordinates[i+2];
  		array[index].x = x;
  		array[index].y = y;
  		array[index].z = z;
  		array[index].distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
//      printf("Index,x,y,z: %d,%d,%d,%d, %lf\n",index,x,y,z,array[index].distance);
    }

  qsort(array,size,sizeof(struct my3DVertexStruct),compare);

  for(i = 0; i < size*3; i += 3){
	  int index = i/3;
	  coordinates[i] = array[index].x;
	  coordinates[i+1] = array[index].y;
	  coordinates[i+2] = array[index].z;
  }

}


