#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
  int my_rank,num_procs;
  int i,n=0;
  double sum,width,local,mypi,pi;
  int proc_len;
  char processor_name[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Get_processor_name(processor_name,&proc_len);

  printf("Processor %d of %d on %s\n",my_rank,num_procs,processor_name);
  if(my_rank==0){
    printf("pls give n=");
    scanf("%d",&n);
  }
  MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
  width=1.0/n;
  sum=0.0;
  for(i=my_rank;i<n;i+=num_procs){
    local=width*((double)i+0.5);
    sum+=4.0/(1.0+local*local);
  }
  mypi=width*sum;
  MPI_Reduce(&mypi,&pi,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
  if(my_rank==0){
    printf("PI is %.20f\n",pi);
    fflush(stdout);
  }
  MPI_Finalize();
  return 0;
}
