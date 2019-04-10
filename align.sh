#!/bin/bash
if [ $# -ne 2 ]; then
  echo "Must enter 2 gene sequences"
  exit
fi
lengthS=${#1}
lengthT=${#2}

#(echo $lengthS;echo $1;echo $lengthT;echo $2;) | (./align.exe)

(./align.exe $lengthS $1 $lengthT $2)