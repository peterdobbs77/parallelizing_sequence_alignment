/*
name: align_mpi.c
author: Peter Dobbs
date: 20 April 2019

to run this: 

*/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "costMat.h"
#include <time.h>

#define MAX_SEQUENCE_LENGTH 100000
#define MASTER		0

char sequence[MAX_SEQUENCE_LENGTH];

int setupAndFillCostMatrix(char *s, int lengthS, char *t, int lengthT){
  unsigned int row, col, c_ij=0;
  CostMatrix mat;
  mat.cost=(int*)malloc(sizeof(int)*(lengthS+1)*(lengthT+1));
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

void compareGivenSequenceWithRandom(char *s, int lengthS){
  unsigned int i=0;
  char text[] = "ATGC";
  int lengthT = MAX_SEQUENCE_LENGTH/((random()%100)+1), c_min=0;
  
  //printf("%i,%i",lengthS,lengthT);
  
  char *t = (char*)malloc(sizeof(char)*lengthT);
  
  for(i=0;i<lengthT;i++){
    t[i] = text[random()%4];
  }
  
  c_min = setupAndFillCostMatrix(s, lengthS, t, lengthT);
  //printf(",%i;\n", c_min);
  
  free(t);
}

int main (int argc, char *argv[])
{
  int   nprocs, rank, rc, dest, i, j, tag1,
        tag2, tag3, source, lengthS, lengthT, N;
  FILE *in;
  MPI_Status status;
  
  //clock_t time_i;
  //time_i = clock();
  
  // command line input
  N=atoi(argv[1]);
  
  /***** Initializations *****/
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  //printf ("MPI task %d has started...\n", rank);
  tag1 = 1;
  tag2 = 2;

  /***** Master task only ******/
  if (rank == MASTER){

    /* Initialize the query sequence */
    printf("COMPARE SET SEQUENCE WITH RANDOM SEQUENCES\n");
    in = fopen("./genome.txt","r");
    for (lengthS = 0; fgetc(in) != EOF; ++lengthS);
    rewind(in);
    for(i=0;i<lengthS;++i){
      sequence[i] = fgetc(in);
    }
    sequence[i]='\0';
    fclose(in);
    
    //printf("lengthS,lengthT,MinCost;\n");
    /* Send each task a copy of the sequence */
    for (dest=1; dest<nprocs; dest++) {
      MPI_Send(&lengthS,1,MPI_INT,dest,tag2,MPI_COMM_WORLD);
      MPI_Send(&sequence,lengthS,MPI_CHAR,dest,tag1,MPI_COMM_WORLD);
      //printf("Sent sequence to task %d\n",dest);
    }

    /* Master does its part of the work */
    for(i=0;i<N/nprocs;++i){
      compareGivenSequenceWithRandom(sequence,lengthS);
    }
    
    /* Wait to receive results from each task */
    for (source=1; source<nprocs; source++) {
      MPI_Recv(&lengthS,1,MPI_INT,source,tag2,MPI_COMM_WORLD,&status);
    }
    
  }  /* end of master section */


  /***** Non-master tasks only *****/

  if (rank > MASTER) {

    /* Receive my copy of array from the master task */
    MPI_Recv(&lengthS,1,MPI_INT,MASTER,tag2,MPI_COMM_WORLD,&status);
    MPI_Recv(&sequence,lengthS,MPI_CHAR,MASTER,tag1,MPI_COMM_WORLD,&status);
    
    for(i=0;i<N/nprocs;++i){
      compareGivenSequenceWithRandom(sequence,lengthS);
    }
    
    /* Send my copy back to the master task */
    MPI_Send(&sequence,lengthS,MPI_CHAR,MASTER,tag1,MPI_COMM_WORLD);
    MPI_Send(&lengthS,1,MPI_INT,MASTER,tag2,MPI_COMM_WORLD);
    
  } /* end of non-master */

  MPI_Finalize();
  
  //time_i = clock() - time_i;
  //printf("task took %f seconds\n",((double)time_i)/CLOCKS_PER_SEC);
    

}   /* end of main */
