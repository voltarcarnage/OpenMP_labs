#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char** argv)
{
  int ret = -1; ///< For return values
  int size = -1; ///< Total number of processors
  int rank = -1; ///< This processor's number
  FILE *fd = fopen("data", "ab");
  const int count = 10000000; ///< Number of array elements
  const int random_seed = atoi(argv[1]); ///< RNG seed
  int* array = 0; ///< The array we need to find the max in
  int lmax = -1; ///< Local maximums
  int max = -1; ///< The maximal element
  /* Initialize the MPI */
  ret = MPI_Init(&argc, &argv);
  if (!rank) { printf("MPI Init returned (%d);\n", ret); }
  /* Determine our rankand processor count */
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf("MPI Comm Size: %d;\n", size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  printf("MPI Comm Rank: %d;\n", rank);
  /* Allocate the array */
  array = (int*)malloc(count * sizeof(int));
  /* Master generates the array */
  if (!rank) 
  {
    /* Initialize the RNG */
    srand(random_seed);
   /* Generate the random array */
    for (int i = 0; i < count; i++) { array[i] = rand(); }
  }
  //printf("Processor #%d has array: ", rank);
  //for (int i = 0; i < count; i++) { printf("%d ", array[i]); }
  //printf("\n");
  /* Send the array to all other processors */
  MPI_Bcast(array, count, MPI_INTEGER, 0, MPI_COMM_WORLD);
  //printf("Processor #%d has array: ", rank);
  //for (int i = 0; i < count; i++) { printf("%d ", array[i]); }
  //printf("\n");
  const int wstart = (rank) * count / size;
  const int wend = (rank + 1) * count / size;//printf("Processor #%d checks items %d .. %d;\n", rank, wstart, wend - 1);
  clock_t start_time = clock();
  for (int i = wstart; i < wend; i++)
  {
    if (array[i] > lmax) { lmax = array[i]; }
  }
  //printf("Processor #%d reports local max = %d;\n", rank, lmax);
  MPI_Reduce(&lmax, &max, 1, MPI_INTEGER, MPI_MAX, 0, MPI_COMM_WORLD);
  clock_t end_time = clock();
  ret = MPI_Finalize();
  if (!rank) {
    printf("\n*** Global Maximum is %d;\n", max);
  }
  printf("MPI Finalize returned (%d);\n", ret);
  double time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
  if(!rank){
  fwrite(&time, sizeof(double), 1, fd);
  }
  fclose(fd);
  return(0);
}
