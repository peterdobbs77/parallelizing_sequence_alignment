#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "costMat.h"
#include <time.h>
#include <omp.h>

#define MAX_SEQUENCE_LENGTH 100000

int setupAndFillCostMatrix(char *s, int lengthS, char *t, int lengthT){
  unsigned int row, col, c_ij=0;
  CostMatrix mat;
  mat.cost=(int*)malloc(sizeof(int)*(lengthS+1)*(lengthT+1));
  mat.rows=lengthS+1;
  mat.cols=lengthT+1;
  
  /* fill the cost matrix */
  for(row=0;row<mat.rows;++row){
    setCost(mat,row,0,row);
    for(col=1;col<mat.cols;++col){
      if(row==0){ 
        setCost(mat,row,col,col);
        continue;
      }
      c_ij = minimumOf3(getCost(mat,row-1,col)+1,getCost(mat,row-1,col-1)+getMatchCost(s[row-1],t[col-1]),getCost(mat,row,col-1)+1);
      setCost(mat,row,col,c_ij);
    }
  }
  free(mat.cost);
  return c_ij;
}

void compareRandomSequences(){
  unsigned int i=0;
  int lengthS=MAX_SEQUENCE_LENGTH/((random()%100)+1), lengthT=MAX_SEQUENCE_LENGTH/((random()%100)+1), c_min=0;
  char text[] = "ATGC";
  char s_i, t_j;
  
  printf("%i,%i",lengthS,lengthT);
  
  char *s = (char*)malloc(sizeof(char)*lengthS);
  char *t = (char*)malloc(sizeof(char)*lengthT);
  
  for(i=0;i<lengthS;i++){
    s[i] = text[random()%4];
  }
  for(i=0;i<lengthT;i++){
    t[i] = text[random()%4];
  }
  
  c_min = setupAndFillCostMatrix(s, lengthS, t, lengthT);
  printf(",%i;\n", c_min);
  
  free(s);
  free(t);
}

void compareGivenSequenceWithRandom(char *s, int lengthS){
  unsigned int i=0;
  char text[] = "ATGC";
  int lengthT = MAX_SEQUENCE_LENGTH/((random()%100)+1), c_min=0;
  
  printf("%i,%i",lengthS,lengthT);
  
  char *t = (char*)malloc(sizeof(char)*lengthT);
  
  for(i=0;i<lengthT;i++){
    t[i] = text[random()%4];
  }
  
  c_min = setupAndFillCostMatrix(s, lengthS, t, lengthT);
  printf(",%i;\n", c_min);
  
  free(t);
}

int main(int argc, char **argv){
  int i=0, length=0, N=0;
  clock_t time_i;
  FILE *in;
  char *sequence;
  char **sequence_list;
  double time_taken;
  
  // input arguments
  if (argc != 2) exit(EXIT_FAILURE);
  N=atoi(argv[1]);
  
  /* RANDOM SEQUENCES */
  printf("RANDOM SEQUENCES\n");
  time_i = clock();
  printf("n,lengthS,lengthT,MinCost;\n");
  for(i=0;i<N;++i){
    printf("%i,",i);
    compareRandomSequences();
  }
  time_i = clock() - time_i;
  time_taken = ((double)time_i)/CLOCKS_PER_SEC;
  printf("task took %f seconds\n",time_taken);
  
  printf("----------------\n");
  /* COMPARE SET SEQUENCE WITH RANDOM SEQUENCES */
  printf("COMPARE SET SEQUENCE WITH RANDOM SEQUENCES\n");
  time_i = clock();
  in = fopen("./genome.txt","r");
	for (length = 0; fgetc(in) != EOF; ++length);
  rewind(in);
  sequence = (char*)malloc(sizeof(char)*length);
  for(i=0;i<length;++i){
    sequence[i] = fgetc(in);
  }
  fclose(in);
  
  in = fopen("./genome_list.txt","r");
  
  fclose(in);
  
  printf("n,lengthS,lengthT,MinCost;\n");
  for(i=0;i<N;++i){
    printf("%i,",i);
    compareGivenSequenceWithRandom(sequence,length);
  }
  free(sequence);
  time_i = clock() - time_i;
  time_taken = ((double)time_i)/CLOCKS_PER_SEC;
  printf("task took %f seconds\n",time_taken);

  return 0;
}
