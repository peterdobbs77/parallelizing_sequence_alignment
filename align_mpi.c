/*
name: align_mpi.c
author: Peter Dobbs
date: 20 April 2019

to run this: 

*/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define MAX_SEQUENCE_LENGTH 100000
#define N         200
#define MASTER		0

float  sequence[100000];

int setupAndFillCostMatrix(char *s, int lengthS, char *t, int lengthT){
  unsigned int row, col, c_ij=0;
  CostMatrix mat;
  mat.cost=(unsigned int*)malloc(sizeof(unsigned int)*(lengthS+1)*(lengthT+1));
  mat.rows=lengthS+1;
  mat.cols=lengthT+1;
  
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

int main (int argc, char *argv[])
{
  int   nprocs, rank, rc, dest, offset, i, j, tag1,
        tag2, tag3, source, lengthS, lengthT;
  char  *t;
  MPI_Status status;

  /***** Initializations *****/
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  printf ("MPI task %d has started...\n", rank);
  chunksize = (ARRAYSIZE / nprocs);
  tag1 = 1;

  /***** Master task only ******/
  if (rank == MASTER){

    /* Initialize the array */
    printf("COMPARE SET SEQUENCE WITH RANDOM SEQUENCES\n");
    FILE *in = fopen("./genome.txt","r");
    for (lengthS = 0; fgetc(in) != EOF; ++lengthS);
    rewind(in);
    sequence = (char*)malloc(sizeof(char)*lengthS);
    for(i=0;i<lengthS;++i){
      sequence[i] = fgetc(in);
    }
    sequence[i] = '\0';
    fclose(in);
    
    printf("n,lengthS,lengthT,MinCost;\n");
    /* Send each task a copy of the array - master keeps 1st part */
    offset = chunksize;
    for (dest=1; dest<nprocs; dest++) {
      MPI_Send(&sequence, lengthS, MPI_CHAR, dest, tag1, MPI_COMM_WORLD);
      //printf("Sent sequence to task %d\n",dest);
    }

    /* Master does its part of the work */
    for(i=0;i<N/nprocs;++i){
      compareGivenSequenceWithRandom(sequence,lengthS);
    }
    
    /* Wait to receive results from each task */
    for (source=1; source<nprocs; source++) {
      MPI_Recv(&sequence, lengthS, MPI_CHAR, source, tag1, MPI_COMM_WORLD, &status);
    }
    
  }  /* end of master section */


  /***** Non-master tasks only *****/

  if (rank > MASTER) {

    /* Receive my copy of array from the master task */
    MPI_Recv(&sequence, lengthS, MPI_CHAR, source, tag1, MPI_COMM_WORLD, &status);
    
    for(i=0;i<N/nprocs;++i){
      compareGivenSequenceWithRandom(sequence,lengthS);
    }
    
    /* Send my copy back to the master task */
    MPI_Send(&sequence, lengthS, MPI_CHAR, dest, tag1, MPI_COMM_WORLD);
    
  } /* end of non-master */

  MPI_Finalize();
  

}   /* end of main */
