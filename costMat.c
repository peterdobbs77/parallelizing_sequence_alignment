#include "costMat.h"
  
int getMatchCost(char s_i, char t_j){
  if(s_i==t_j) return 0;
  return 1;
}

int getCost(CostMatrix mat, int row, int col){
  return mat.cost[row*mat.cols+col];
}

void setCost(CostMatrix mat, int row, int col, int val){
  mat.cost[row*mat.cols+col] = val;
}
  
int minimumOf3(int c0, int c1, int c2){
  if(c1<=c0 && c1<=c2){
    return c1;
  } else if(c2<=c0 && c2<=c1){
    return c2;
  } else {
    return c0;
  }
}

void printMatrix(CostMatrix mat, FILE *out){
  unsigned int row, col;
  for(row=0;row<mat.rows;++row){
    for(col=0;col<mat.cols;++col){
      printf("\t%i",getCost(mat,row,col));
      fprintf(out,"\t%i",getCost(mat,row,col));
    }
    printf("\n");
    fprintf(out,"\n");
  }
}