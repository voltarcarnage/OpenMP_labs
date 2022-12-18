#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int main(){
 /* Determine _OPENMP */
 printf("OpenMP: %d\n", _OPENMP);
 switch(_OPENMP)
 {
 case 200505:
  printf("Version: 2.5\n");
  break;
 case 200805:
  printf("Version: 3.0\n");
  break;
 case 201107:
  printf("Version: 3.1\n");
  break;
 case 201307:
  printf("Version: 4.0\n");
  break;
 case 201511:
  printf("Version: 4.5\n");
  break;
 case 201811:
  printf("Version: 5.0\n");
  break;
 case 202011:
  printf("Version: 5.1\n");
  break;
 }
 printf("Number of processes: %d\n",omp_get_num_procs());
 printf("Max number of threads: %d\n", omp_get_max_threads());
 printf("Does my system support dynamic change of number of threads?: %d\n", omp_get_dynamic());
 printf("The precision of the timer used by omp_get_wtime: %lf (in seconds)\n", omp_get_wtick());
 printf("Are nested parallel  regions allowed?: %d\n", omp_get_nested());
 printf("Max level of nested  patallel region: %d\n", omp_get_max_active_levels());
 omp_sched_t kind;
 int chunk_size;
 omp_get_schedule(&kind, &chunk_size);
 printf("Schedule is encountered: ");
 switch(kind)
 {
 case omp_sched_static:
  printf("static\n");
  break;
 case omp_sched_dynamic:
  printf("dynamic\n");
  break;
 case omp_sched_guided:
  printf("guided\n");
  break;
 case omp_sched_auto:
  printf("auto\n");
  break;
 default:
  printf("other (implementation specific)\n");
  break;
 }
 printf("And chunk size is: %d\n", chunk_size);
 int x = 0;
 omp_lock_t lock;
 omp_init_lock(&lock);
 #pragma omp parallel for
 for(int i = 0; i < 300; i++)
 {
  omp_set_lock(&lock);
  x++; 
  omp_unset_lock(&lock);
 }
 printf("%d\n", x);
 omp_destroy_lock(&lock);
 return 0;
}
