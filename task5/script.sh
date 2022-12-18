#!/usr/bin/bash

mpicc lab5.cpp -o lab5

for ((i = 1; i <= 10; i++))
do
  for ((j=1; j <= 4; j++))
  do
    mpirun -np $j ./lab5 $i
  done 
done 

rm lab5
python3 plots.py 

