#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char **argv) {
    int tests_number = atoi(argv[1]);
    int threads_number = atoi(argv[2]);

    int array_size = atoi(argv[3]);
    int *array = (int *) malloc(array_size * sizeof(int));
    int *tmp_array = (int *) malloc(array_size * sizeof(int));

    FILE *fd = fopen("openmp.txt", "w");
    
    for (int t = 1; t <= tests_number; t++) {
        
        printf("Creating array #%d\n", t);
        srand(time(NULL));
        for (int i = 0; i < array_size; i++)
            array[i] = rand() % array_size;
            
        for (int threads = 1; threads <= threads_number; threads *= 2) {
            
            printf("Number of threads: %d\n", threads);
            for (int i = 0; i < array_size; i++)
                tmp_array[i] = array[i];

            double start_time = omp_get_wtime();

            for (int step = array_size / 2; step > 0; step /= 2) {
        #pragma omp parallel for num_threads(threads) shared(tmp_array, array_size, step) default(none)
                for (int i = step; i < array_size; i++)
                    for (int j = i - step; j >= 0; j -= step)
                        if (tmp_array[j] > tmp_array[j + step]) {
                            int tmp = tmp_array[j];
                            tmp_array[j] = tmp_array[j + step];
                            tmp_array[j + step] = tmp;
                        }
            }

            double end_time = omp_get_wtime();

            fprintf(fd, "%lf ", (end_time - start_time));
        }
    }

    fclose(fd);
    free(array);
    free(tmp_array);

    return 0;
}
