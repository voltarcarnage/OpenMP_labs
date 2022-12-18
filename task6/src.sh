#!/bin/bash

ARRAY_SIZE=100000
THREADS_NUMBER=4
TESTS_NUMBER=5

echo "Compiling"
mpicc -o lab6 lab6.c -lm 
gcc -fopenmp lab3.c -o lab3 

echo "Working OpenMP..."
./lab3 $TESTS_NUMBER $THREADS_NUMBER $ARRAY_SIZE

echo "Working MPI..."
for ((i = 1; i <= $TESTS_NUMBER; i++))
do
    echo "Creating array #$i"
    python3 gen.py $ARRAY_SIZE

    for ((n = 1; n <= $THREADS_NUMBER; n *= 2))
    do
        echo "Number of theads: $n" 
        mpirun -np $n ./lab6 $ARRAY_SIZE
    done
done

python3 plot.py $TESTS_NUMBER $THREADS_NUMBER


echo "Deleting"
rm lab3 lab6 array.txt openmp.txt mpi.txt
