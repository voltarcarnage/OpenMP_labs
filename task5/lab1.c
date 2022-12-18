#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define THREADS 4
#define SIZE 10000000
#define TESTS_NUM 10

void find(int *arr, int n)
{
  int max = -1;
  #pragma omp parallel num_threads(n) shared(arr) reduction(max : max)
  {
    #pragma omp for
      for(int i = 0; i < SIZE; i++)
        if(arr[i] > max)
          max = arr[i];
  }
}

int main()
{
  double *timing = malloc(sizeof(double) * THREADS * TESTS_NUM); 
  int *arr = malloc(sizeof(int) * SIZE);
  for(int i = 0; i < TESTS_NUM; i++)
    {
      srand(time(NULL));
      for(int j = 0; j < SIZE; j++)
        arr[j] = rand() % 10;

      for(int k = 0; k < THREADS; k++)
        {
          double start_time = omp_get_wtime();
          find(arr, k + 1);
          double end_time = omp_get_wtime();
          timing[THREADS * i + k] = end_time - start_time;
        }
    }

  FILE *fd = fopen("timing_OpenMP.txt", "w");
  fprintf(fd, "%d\n", THREADS);
  fprintf(fd, "%d\n", TESTS_NUM);
  
  for(int i = 0; i < THREADS; i++)
    {
      for(int j = 0; j < TESTS_NUM; j++)
        {
          fprintf(fd, "%lf\t", timing[THREADS * j + i]);
        }
      fprintf(fd, "\n");
    }
  
  free(timing);
  free(arr);

  return 0;

}
