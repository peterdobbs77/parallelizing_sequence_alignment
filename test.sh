#!/bin/bash
(./generate_genomes.exe)

s=`cat genome1.txt`
t=`cat genome1.txt`

bash align.sh $s $t
