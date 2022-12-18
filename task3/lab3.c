#include "headers/app.h"

int main()
{
    int num_threads = THREADS;
    int iteration_count = 10;
    int count = COUNT;
    float *times;
    FILE *file = fopen("experiment.txt", "wb");

    fwrite(&num_threads, sizeof(int), 1, file);
    fwrite(&iteration_count, sizeof(int), 1, file);
    fwrite(&count, sizeof(int), 1, file);

    for(int i = 0; i < 10; i++)
    {
        printf("---------Iteration number: %d\n", i + 1);
        times = shellSort(i);
        fwrite(times, sizeof(float), THREADS, file);
        free(times);
    }

    fclose(file);

    return 0;
}
