#include "headers/app.h"

int main(int argc, char** argv)
{
    printf("OpenMP: %d;\n======\n", _OPENMP);

    int iter = 10;
    int threads = THREADS;
    int seed = 93932;
    FILE *file = fopen("experiment.txt", "w");

    fwrite(&threads, sizeof(int), 1, file);
    fwrite(&iter, sizeof(int), 1, file);

    float *times = 0;

    for(int i = 0; i < iter; i++)
    {
      printf("---------Iteration number: %d\n", i);
      times = (float*)calloc(threads, sizeof(float));
      f(seed + i, times);
      fwrite(times, sizeof(float), threads, file);
      free(times);
    }

    fclose(file);

    return 0;
}
