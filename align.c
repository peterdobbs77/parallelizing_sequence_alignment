#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "costMat.h"

void trace(CostMatrix mat, int row, int col, char *s, char *t, char *alignS, char *alignT, int i, int j, int c_ij, FILE *out){
  
  if (row==0 && col==0){
    printf("%s\t%s\n", alignS, alignT);
    fprintf(out,"%s\t%s\n", alignS, alignT);
    return;
  }
  
  //printf("(%i,%i) c_ij: %i, c_match: %i \n",row,col,c_ij,c_match);
  
  if(getCost(mat,row-1,col)+1 == c_ij) {   //above origin
    alignS[--i] = s[row-1];
    alignT[--j] = '_';
    trace(mat,row-1,col,s,t,alignS,alignT,i,j,getCost(mat,row-1,col),out);
  } else if(getCost(mat,row-1,col-1)+getMatchCost(s[row-1],t[col-1]) == c_ij) {  //diagonal origin
    alignS[--i] = s[row-1];
    alignT[--j] = t[col-1];
    trace(mat,row-1,col-1,s,t,alignS,alignT,i,j,getCost(mat,row-1,col-1),out);
  } else if(getCost(mat,row,col-1)+1 == c_ij) { //left origin
    alignS[--i] = '_';
    alignT[--j] = t[col-1];
    trace(mat,row,col-1,s,t,alignS,alignT,i,j,getCost(mat,row,col-1),out);
  }
}

int main(int argc, char **argv){
  unsigned int row=0, col=0;
  int length1=0, length2=0, alignLength=0, c_ij=0, c_min=0;
  CostMatrix mat;
  FILE* out;
  
  if(argc!=5){
    printf("%s", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  length1=atoi(argv[1]);
  length2=atoi(argv[3]);
  if(length1 > length2) alignLength=length1;
  else alignLength=length2;
  
  char *align1 = (char*)malloc(sizeof(char)*alignLength);
  char *align2 = (char*)malloc(sizeof(char)*alignLength);
  char *s = (char*)malloc(sizeof(char)*length1);
  char *t = (char*)malloc(sizeof(char)*length2);
  
  strcpy(s,argv[2]);
  strcpy(t,argv[4]);
  
  /* initialize the cost matrix */
  mat.cost=(unsigned int*)malloc(sizeof(unsigned int)*(length1+1)*(length2+1));
  mat.rows=length1+1;
  mat.cols=length2+1;
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
  c_min=c_ij;
  
  /* print the cost matrix */
  out = fopen("cost_matrix_out.txt","w");
  printMatrix(mat,out);
  printf("min cost: %i\n",c_min);
  fprintf(out,"min cost: %i\n",c_min);
  trace(mat,length1,length2,s,t,align1,align2,alignLength,alignLength,c_min,out);
  
  fclose(out);
  free(s);
  free(t);
  free(align1);
  free(align2);
  return 0;
}
