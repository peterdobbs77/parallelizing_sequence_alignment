//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LENGTH 10000

  int main(void){
  int i=0, index=0;
  char text[] = "ATGC";
  char element;
  FILE* fp;
  
  fp = fopen("./genome.txt","w");
  
  for(i=0;i<MAX_LENGTH;i++){
    element = text[random()%4];
    fprintf(fp,"%c",element);
  }
  fclose(fp);
  
  return 0;
}
