//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_LENGTH 3000000

int main(void){
  int i=0, index=0;
  char text[] = "ATGC";
  char element;
  FILE* fp;
  
  fp = fopen("./genome1.txt","w");
  
  for(i=0;i<MAX_LENGTH;i++){
    index = random()%4;
    element = text[index];
    fprintf(fp,"%c",element);
  }
  fclose(fp);
  
/*
  fp = fopen("./genome2.txt","w");
  
  for(i=0;i<MAX_LENGTH;i++){
    index = random()%4;
    element = text[index];\
    fprintf(fp,"%c",element);
  }
  fclose(fp);
*/
  return 0;
}
