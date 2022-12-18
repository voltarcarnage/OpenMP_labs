#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define num 4;
#define iter 10;

void max_arr(int seed, float *times)
{
 int num_threads = num;
 const int count = 10000000; 
 int* array = (int*)malloc(count * sizeof(int));
 int i = 0;
 float start, end;
 /* Initialize the RNG */
 srand(seed);
 /* Generate the random array */

 for(int i = 0; i < count; i++) { array[i] = rand(); }
 
 for(int k = 0; k < num_threads; k++)
 {
    int max = -1;
    /* Find the maximal element
    * Spawn group of threads */
    start = omp_get_wtime();
    #pragma omp parallel num_threads(k+1) shared(array, count, k, end, start, times) reduction(max: max) default(none)
    {
        /* Divide loop iterations between spawned threads */
        #pragma omp for schedule(guided, 10)
        for(i = 0; i < count; i++)
        {
            if(array[i] > max) { max= array[i]; };
        }
    }
    end = omp_get_wtime();
    times[k] = end - start;
 }
 free(array);
}

int main(int argc, char* argv[])
{
 int num_threads = num;
 int iteration_count = iter;
 FILE *f = fopen("experiment_def.txt", "ab");
 //fwrite(&num_threads, sizeof(int), 1, f);
 //fwrite(&iteration_count, sizeof(int), 1, f);
 for(int i = 0; i < iteration_count; i++)
 {
    float* times = (float*)malloc(num_threads * sizeof(float));
    max_arr(i, times);
    fwrite(times, sizeof(float), num_threads, f);
    free(times);
 }
 fclose(f);
 return 0;
}
