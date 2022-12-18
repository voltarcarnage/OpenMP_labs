#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    // Initialization of process rank and number of threads.  
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size);
        
    // Checking number of threads.
    if (!rank)
        if ((size & (size - 1)) != 0) {
            fprintf(stderr, "Error! The number of threads must be a power of 2.\n");
            MPI_Abort(MPI_COMM_WORLD, MPI_ERR_TOPOLOGY);
        }
    
    // Checking size of array.
    int array_size = atoi(argv[1]);
    if (!rank)
        if (array_size % size != 0) {
            fprintf(stderr, "Error! The size of the array must be a multiple of the number of threads.\n");
            MPI_Abort(MPI_COMM_WORLD, MPI_ERR_SIZE);
        }
    
    // Allocation of memory for array.
    int *array;
    if (!rank) {
        array = (int *) malloc(array_size * sizeof(int));
        if (!array) {
            fprintf(stderr, "Error! Failed to allocate memory.\n");
            MPI_Abort(MPI_COMM_WORLD, MPI_ERR_NO_MEM);
        }
    }
    
    // Initialization of values in an array.
    if (!rank) {
        FILE *fd = fopen("array.txt", "r");
        for (int i = 0; i < array_size; i++)
            fscanf(fd, "%d", &array[i]);
        fclose(fd);
            
    }
    
    // Allocation of memory for chunk of array.
    int chunk_size = array_size / size;
    int *chunk = (int *) malloc(chunk_size * sizeof(int));
    int *tmp_chunk = (int *) malloc(chunk_size * sizeof(int));
    int *new_chunk = (int *) malloc(chunk_size * sizeof(int));
    if (!chunk || !tmp_chunk || !new_chunk) {
        fprintf(stderr, "Error! Failed to allocate memory.\n");
        MPI_Abort(MPI_COMM_WORLD, MPI_ERR_NO_MEM);
    }
    
    // Start of timing.
    double start_time, end_time;
    if (!rank)
        start_time = MPI_Wtime();
    
    // Disstribution of array by threads. 
    MPI_Scatter(array, chunk_size, MPI_INTEGER, chunk, chunk_size, MPI_INTEGER, 0, MPI_COMM_WORLD);

    // Shellsort for chunk of array.
    for (int step = chunk_size / 2; step > 0; step /= 2)
        for (int i = step; i < chunk_size; i++)
            for (int j = i - step; j >= 0; j -= step)
                if (chunk[j] > chunk[j + step]) {
                    int tmp = chunk[j];
                    chunk[j] = chunk[j + step];
                    chunk[j + step] = tmp;
                }

    // Compare-split operation.
    int power = log2(size);
    for (int n = 0; n < power; n++) {
        // Initialization of paired process.
        int pair = rank ^ (1 << (power - n - 1));
        
        // Sending and receiving chunks between paired processes.
        MPI_Sendrecv(chunk, chunk_size, MPI_INTEGER, pair, n, tmp_chunk, chunk_size, MPI_INTEGER, pair, n,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        // Merging paired chunks after comparison.
        if (rank < pair && chunk[chunk_size - 1] > tmp_chunk[0]) {
            int i = 0,
                j = 0, 
                k = 0;

            while (k < chunk_size)
                if (chunk[i] < tmp_chunk[j])
                    new_chunk[k++] = chunk[i++];
                else
                    new_chunk[k++] = tmp_chunk[j++];

            memcpy(chunk, new_chunk, chunk_size * sizeof(int));     
            
        } else if (rank > pair && chunk[0] < tmp_chunk[chunk_size - 1]) {
            int i = chunk_size - 1, 
                j = chunk_size - 1,
                k = chunk_size - 1;

            while (k >= 0)
                if (chunk[i] < tmp_chunk[j])
                    new_chunk[k--] = tmp_chunk[j--];

                else
                    new_chunk[k--] = chunk[i--];
            
            memcpy(chunk, new_chunk, chunk_size * sizeof(int));        
    
        }    
    }
        
    // Сollecting all chunks of array into single array.
    MPI_Gather(chunk, chunk_size, MPI_INTEGER, array, chunk_size, MPI_INTEGER, 0, MPI_COMM_WORLD);
       
    // End of timing.
    if (!rank) {
        end_time = MPI_Wtime();
        
        FILE *fd = fopen("mpi.txt", "a+t");
        fprintf(fd, "%lf ", end_time - start_time);
        fclose(fd);
    }    

    // Free memory.
    free(chunk);
    free(tmp_chunk);
    free(new_chunk);
    if (!rank)
        free(array);

    // Termination MPI.
    MPI_Finalize();
    
    return 0;
}
