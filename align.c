#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "costMat.h"

#define MAX_SEQUENCE_LENGTH 5000

void trace_recursive(CostMatrix mat, int row, int col, char *s, char *t, char *alignS, char *alignT, int i, int j, int c_ij){
  
  if (row==0 && col==0){
    printf("Finished Retracing the Cost Matrix\n");
    return;
  }
  
  if(getCost(mat,row-1,col)+1 == c_ij) {   //above origin
    alignS[--i] = s[row-1];
    alignT[--j] = '_';
    trace_recursive(mat,row-1,col,s,t,alignS,alignT,i,j,getCost(mat,row-1,col));
  } else if(getCost(mat,row-1,col-1)+getMatchCost(s[row-1],t[col-1]) == c_ij) {  //diagonal origin
    alignS[--i] = s[row-1];
    alignT[--j] = t[col-1];
    trace_recursive(mat,row-1,col-1,s,t,alignS,alignT,i,j,getCost(mat,row-1,col-1));
  } else if(getCost(mat,row,col-1)+1 == c_ij) { //left origin
    alignS[--i] = '_';
    alignT[--j] = t[col-1];
    trace_recursive(mat,row,col-1,s,t,alignS,alignT,i,j,getCost(mat,row,col-1));
  }
}

// TODO: finish this work...
void trace_loop(char *s, char *t, char *alignS, char *alignT, CostMatrix mat){
  int i=0,row=0,j=0,col=0,c_ij=0;
  row = mat.rows-1;
  col = mat.cols-1;
  for(i = row, j = col; i > 0, j > 0;){
    c_ij = getCost(mat,row,col);
    if(getCost(mat,row-1,col)+1 == c_ij) {   //above origin
      alignS[--i] = s[--row];
      alignT[--j] = '_';
    } else if(getCost(mat,row-1,col-1)+getMatchCost(s[row-1],t[col-1]) == c_ij) {  //diagonal origin
      alignS[--i] = s[--row];
      alignT[--j] = t[--col];
    } else if(getCost(mat,row,col-1)+1 == c_ij) { //left origin
      alignS[--i] = '_';
      alignT[--j] = t[--col];
    }
  }
}

int main(void){
  unsigned int i=0, row=0, col=0;
  int length1=0, length2=0, alignLength=0, c_ij=0, c_min=0;
  CostMatrix mat;
  FILE* out;
  char text[] = "ATGC";
  char s_i, t_j;
  
  char *s = (char*)malloc(sizeof(char)*MAX_SEQUENCE_LENGTH);
  char *t = (char*)malloc(sizeof(char)*MAX_SEQUENCE_LENGTH);
  
  for(i=0;i<MAX_SEQUENCE_LENGTH;i++){
    s[i] = text[random()%4];
    t[i] = text[random()%4];
  }
  
  char *align1 = (char*)malloc(sizeof(char)*MAX_SEQUENCE_LENGTH);
  char *align2 = (char*)malloc(sizeof(char)*MAX_SEQUENCE_LENGTH);
  
  /* initialize the cost matrix */
  mat.cost=(unsigned int*)malloc(sizeof(unsigned int)*(MAX_SEQUENCE_LENGTH+1)*(MAX_SEQUENCE_LENGTH+1));
  mat.rows=MAX_SEQUENCE_LENGTH+1;
  mat.cols=MAX_SEQUENCE_LENGTH+1;
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
  /* trace back through the matrix to determine the optimal alignment */
  //trace_recursive(mat,MAX_SEQUENCE_LENGTH+1,MAX_SEQUENCE_LENGTH+1,s,t,align1,align2,MAX_SEQUENCE_LENGTH,MAX_SEQUENCE_LENGTH,c_min);
  trace_loop(s,t,align1,align2,mat);
  printf("finished trace\n");
  
  /* ouput the cost matrix and optimal alignment sequences */
  out = fopen("cost_matrix_out.txt","w");
  printMatrix(mat,out);
  fprintf(out,"min cost: %i\n",c_min);  
  fprintf(out,"%s\n%s\n", align1, align2);
  fclose(out);
  
  //printf("min cost: %i\n",c_min);
  //printf("%s\t%s\n", align1, align2);
  
  free(s);
  free(t);
  free(align1);
  free(align2);
  
  return 0;
}
