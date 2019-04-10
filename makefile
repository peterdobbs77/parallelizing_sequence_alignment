#makefile
all: align.exe

align.exe: costMat.c align.c
	gcc -g -o align.exe costMat.c align.c -lm
