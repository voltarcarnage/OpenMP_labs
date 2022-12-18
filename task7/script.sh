#!/bin/bash

N_START=1000000
N_END=3000000
THREADS_NUMBER=6
PROCESSORS_NUMBER=2

echo "Compiling..."
mpicc -fopenmp -o lab7 lab7.c -lm 

echo "Working..."
mpirun -np $THREADS_NUMBER ./lab7 $N_START $N_END $THREADS_NUMBER $PROCESSORS_NUMBER

python3 plot.py

echo "Deleting..."
rm lab7 timing.txt

