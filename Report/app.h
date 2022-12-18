#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>


#define M 1000
#define E 0.00001

#define T 0.001

#pragma once

static float A[M][M], F[M], Xk[M], Zk[M];
static float Rk[M], Sz[M], alf, bet, mf;
static float Spr, Spr1, Spz;

void CG();
