#makefile
all: ser_align generate_genomes.exe omp_align mpi_align

ser_align: costMat.c ser_align.c
	gcc -g -o ser_align costMat.c ser_align.c -lm

generate_genomes.exe: random_sequence.c
	gcc -g -o generate_genomes.exe random_sequence.c -lm

omp_align: costMat.c omp_align.c
	gcc -o omp_align costMat.c omp_align.c -lm -fopenmp

mpi_align: costMat.c mpi_align.c
	mpicc -o mpi_align costMat.c mpi_align.c

