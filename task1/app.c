#include "headers/app.h"

void f(int rand_seed, float *times)
{
  const int count = 10000000;     ///< Number of array elements

  int *array = calloc(count, sizeof(int));                ///< The array we need to find the max in
  int threads = THREADS;

  float start_time = 0, end_time = 0;
  srand(rand_seed);

  for(int i=0; i<count; i++) { array[i] = rand(); }

  for(int i = 0; i < threads; i++)
  {
    int max = -1;

    start_time = omp_get_wtime();
    #pragma omp parallel num_threads(i + 1) shared(array,count, i) reduction(max: max) default(none)
    {
      #pragma omp for
      for(int j = 0; j < count; j++)
      {
        if(array[j] > max) { max = array[j]; };
      }
      printf("-- My lmax for %d threads is: %d;\n", i + 1, max);
    }
    end_time = omp_get_wtime();
    times[i] = end_time - start_time;
    printf("======\nMax is: %d for threads: %d, time: %f\n", max, i + 1, times[i]);
  }

  free(array);
}
