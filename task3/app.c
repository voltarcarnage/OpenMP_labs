#include "headers/app.h"

void checkArray(int* array, int count){
    int p = 0;
    for(int i = 0; i < count; i++){
        if(array[i] == i)
            p += 1;
        }
    float percent = (float)p / count * 100;
    printf("Amount of elements: %d, Percent of sorted: %f\n", count, percent);
}

float *shellSort(int seed)
{
    float* times = (float*)malloc(sizeof(float) * THREADS);
    const int count = COUNT;
    int* array = (int *)malloc(count * sizeof(int));
    int* array1 = (int *)malloc(count * sizeof(int));
    float start_time, end_time;

    srand(seed);


    //Sorted 
    for(int i = 0; i < count; i++)
    { 
        array[i] = i; 
    }
    

    //Randomly
    /*for(int i = 0; i < count; i++)
    {
        array[i] = count - 1 - i;
    }*/

    // Partial array sorting 
    int tmp, index;
    for(int i = 0; i < count; i++){
        if(i % (seed + 1) == 0){
            tmp = array[i];
            index = rand() % count;
            array[i] = array[index];
            array[index] = tmp;
        }
    }
    
    checkArray(array, count);

    for(int i = 0; i < count; i++)
    {
        array1[i] = array[i];
    }  

    int i, thr;

    for(int k = 0; k < THREADS; k++){
        start_time = omp_get_wtime();
        for(int gap = count / 2; gap > 0; gap /= 2)
        {
            if(k != 0 && gap <= k + 1)
                thr = gap;
            else
                thr = k + 1;

            #pragma omp parallel for num_threads(thr) shared(array, start_time, end_time, gap, count) private(i) default(none)
            for(i = gap; i < count; i++)
            {
                for(int j = i - gap; j >= 0 && array[j] > array[j + gap]; j -= gap)
                {
                    int tmp = array[j];
                    array[j] = array[j + gap];
                    array[j + gap] = tmp;
                }
            }
        }
        end_time = omp_get_wtime();
        times[k] = end_time - start_time;
        for(int i = 0; i < count; i++)
            array[i] = array1[i];
        printf("Num of threads = %d, time =  %lf\n", k+1, times[k]);
    }
    free(array);
    return times;
}
