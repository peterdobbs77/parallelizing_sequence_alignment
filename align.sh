#!/bin/bash
(./generate_genomes.exe)

s=`cat genome1.txt`
t=`cat genome1.txt`

lengthS=${#s}
lengthT=${#t}

#(echo $lengthS;echo $1;echo $lengthT;echo $2;) | (./align.exe)

(./align.exe $lengthS $s $lengthT $t)