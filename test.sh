#!/bin/bash
(./generate_genomes.exe)

s=`cat genome1.txt`
t=`cat genome2.txt`

bash align.sh $s $t
