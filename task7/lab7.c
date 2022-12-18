#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    // Initialization of thread rank and number of threads.
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    
    // Checking number of threads for MPI.
    int threads_number = atoi(argv[3]); 
    if (!rank)
        if (threads_number != size) {
            fprintf(stderr, "Error! Invalid number of threads.\n");
            MPI_Abort(MPI_COMM_WORLD, MPI_ERR_OTHER);
        }
    
    // Checking сondition between number of threads and number of processes.
    int processors_number = atoi(argv[4]); 

    // Initialization number of processes for OpenMP.
    int n = processors_number / threads_number;
    
    // Initialization starting and ending indexes for each thread.
    int start_range = atoi(argv[1]);
    int end_range = atoi(argv[2]);
    int length_range = end_range - start_range;
    const int start_interval = rank * length_range / size + start_range;
    const int end_interval = (rank + 1) * length_range / size + start_range - 1;

    
    // Allocation of memory for timing.
    double *timing;
    if (!rank) {    
        timing = (double *) malloc(size * sizeof(double));
        if (!timing) {
            fprintf(stderr, "Error! Failed to allocate memory.\n");
            MPI_Abort(MPI_COMM_WORLD, MPI_ERR_NO_MEM);
        }
    }
    
    // Allocation of memory for array.
    int *array;
    if (!rank) {
        array = (int *) malloc(length_range * sizeof(int));
        if (!array) {
            fprintf(stderr, "Error! Failed to allocate memory.\n");
            MPI_Abort(MPI_COMM_WORLD, MPI_ERR_NO_MEM);
        }
    }


    printf("hyio\n");
    // Allocation of memory for chunk of array.
    int *chunk = (int *) malloc(length_range * sizeof(int));
    if (!chunk) {
        fprintf(stderr, "Error! Failed to allocate memory.\n");
        MPI_Abort(MPI_COMM_WORLD, MPI_ERR_NO_MEM);
    }  
    
   
    // Start of timing for each thread.
    double start_time = MPI_Wtime();
    
    int flag;
    int counter = 0;
    int local_counter = 0;

    // Finding prime numbers in chunk of array.
#pragma omp parallel for num_threads(n) shared(flag, start_interval, end_interval, chunk, local_counter) default(none)
    for (int number = start_interval; number <= end_interval; number++) {
        flag = 1;
        int max_divisor = sqrt(number);
        
        printf("%d\n", number);

        for (int d = 2; d <= max_divisor; d++)
            if (number % d == 0) {
                flag = 0;
                break; 
            }
        
        if (flag) {
            chunk[local_counter] = number;
            local_counter++;
        }
    }

    // End of timing for each thread.
    double end_time = MPI_Wtime();
    double period = end_time - start_time;

    printf("%lf\n", period);
    
    // Collecting all periods for each thread into single array - timing.
    MPI_Gather(&period, 1, MPI_DOUBLE, timing, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Allocation of memory for counters.
    int *counters = (int *) malloc(size * sizeof(int));
    if (!counters) {
        fprintf(stderr, "Error! Failed to allocate memory.\n");
        MPI_Abort(MPI_COMM_WORLD, MPI_ERR_NO_MEM);
    }  

    // Collecting all local_counters into single array - counters.
    MPI_Gather(&local_counter, 1, MPI_INTEGER, counters, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);

    // Sending counters to each thread.
    MPI_Bcast(counters, size, MPI_INTEGER, 0, MPI_COMM_WORLD);
    
    // Allocation of memory for offsets. 
    int *offsets = (int *) malloc(size * sizeof(int));
    if (!offsets) {
        fprintf(stderr, "Error! Failed to allocate memory.\n");
        MPI_Abort(MPI_COMM_WORLD, MPI_ERR_NO_MEM);
    }  

    // Initialization offsets for each chunk.
    int tmp = 0;
    for (int i = 0; i < size; i++) {
        offsets[i] = tmp;
        tmp += counters[i];
    }
    
    // Initialization of total number of prime numbers.
    MPI_Reduce(&local_counter, &counter, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);    
    
    // Collecting all chunks of array into single array.
    MPI_Gatherv(chunk, local_counter, MPI_INTEGER, array, counters, offsets, MPI_INTEGER, 0, MPI_COMM_WORLD);
    
    // Output of result.
    if (!rank) {
        printf("Range of numbers: [%d; %d).\n", start_range, end_range);
         
        printf("Found prime numbers: ");
        for (int i = 0; i < counter; i++)
            printf("%d ", array[i]);
        printf("\n");
        
        FILE *fd = fopen("timing.txt", "w");
        fprintf(fd, "%d\n", size); 
        for (int i = 0; i < size; i++)
            fprintf(fd, "%lf ", timing[i]);
    }

    // Free memory.
    free(chunk);
    free(counters);
    free(offsets);
    if (!rank) {
        free(timing);
        free(array);
    }
           
    // Termination MPI.
    MPI_Finalize();
    
    return 0;
}
