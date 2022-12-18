#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define COUNT 10000000
#define THREADS 4

#pragma once

void checkArray(int *, int);
float *shellSort(int);
