#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  const int PING_PONG_LIMIT = 10;

  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming 2 processes for this task
  if (world_size != 2) {
    fprintf(stderr, "World size must be two for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int ping_pong_count = 0;
  //PLEASE ADD THE RIGHT CODES
  int another=(world_rank+1)%2;
  int times=PING_PONG_LIMIT;
  while(times--){ //������������������Ϣ
    if(world_rank==ping_pong_count%2){ //��˭���� �ӳ�ʼ0��ʼ ˫�����ͣ���������
        ping_pong_count++;
        printf("Process %d sent and incremented ping_pong_count(value=%d) to process %d\n",world_rank,ping_pong_count,another);
        MPI_Send(&ping_pong_count,1,MPI_INT,another,0,MPI_COMM_WORLD);
    }
    else{
        printf("Process %d received ping_pong_count(value=%d) from process %d\n",world_rank,ping_pong_count,another);
        MPI_Recv(&ping_pong_count,1,MPI_INT,another,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }
  MPI_Finalize();
  return 0;
}
