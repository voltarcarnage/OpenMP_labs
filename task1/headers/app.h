#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define THREADS 4

#pragma once

void f(int rand_seed,float *times);
