#makefile
all: align.exe generate_genomes.exe align_omp

align.exe: costMat.c align.c
	gcc -g -o align.exe costMat.c align.c -lm

generate_genomes.exe: random_sequence.c
	gcc -g -o generate_genomes.exe random_sequence.c -lm

align_omp: costMat.c align.c
	gcc -o align_omp costMat.c align.c -lm -fopenmp

