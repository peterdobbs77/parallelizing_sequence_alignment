#ifndef COSTMAT_H
#define COSTMAT_H
  
  #include <stdio.h>
  #include <stdlib.h>
  
  typedef struct costMatrix{
    int *cost;
    int rows;
    int cols;
  } CostMatrix;
  
  int getMatchCost(char s_i, char t_j);

  int getCost(CostMatrix mat, int row, int col);

  void setCost(CostMatrix mat, int row, int col, int val);

  int minimumOf3(int c0, int c1, int c2);

  void printMatrix(CostMatrix mat, FILE *out);
  
#endif