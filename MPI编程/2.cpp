#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int Global_sum(int my_int, int my_rank, int comm_sz, MPI_Comm comm);

const int MAX_CONTRIB = 20;

int main(void) {
    int i, sum, my_int;
    int my_rank, comm_sz;
    MPI_Comm comm;
    int* all_ints = NULL;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    srandom(my_rank + 1);
    my_int = random() % MAX_CONTRIB;

    sum = Global_sum(my_int, my_rank, comm_sz, comm);

    if ( my_rank == 0) {
        all_ints = malloc(comm_sz*sizeof(int));
        MPI_Gather(&my_int, 1, MPI_INT, all_ints, 1, MPI_INT, 0, comm);
        printf("Ints being summed:\n   ");
        for (i = 0; i < comm_sz; i++)
            printf("%d ", all_ints[i]);
        printf("\n");
        printf("Sum = %d\n",sum);
        free(all_ints);
    } 
    else {
        MPI_Gather(&my_int, 1, MPI_INT, all_ints, 1, MPI_INT, 0, comm);
    }

    MPI_Finalize();
    return 0;
}  /* main */

int Global_sum(
    int my_int    /* in */, //当前进程的数值
    int my_rank   /* in */,
    int comm_sz   /* in */,
    MPI_Comm comm /* in */) {

   int remain=comm_sz,my_sum=my_int,temp;
   int half,rm;
   while (remain!=1){
        half=remain/2;
        rm=remain%2;
        if(my_rank<half){
            MPI_Recv(&temp,1,MPI_INT,my_rank+half+rm,0,comm,MPI_STATUS_IGNORE);
            my_sum+=temp;
        //前半部分接受后半部分送来的值
        }
        else{
            MPI_Send(&my_sum,1,MPI_INT,my_rank-half-rm,0,comm);
        //后半部分发送值
        }
        remain=half+rm;
    }
    if(my_rank==0) return my_sum;
}  /* Global_sum */
