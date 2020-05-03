#include <mpi.h>
#include <unistd.h>
#include <iostream>
using namespace std;
int main(int argc, char*argv[]) {
    
     int id,total;
      MPI_Init(&argc,&argv);
      MPI_Comm_rank(MPI_COMM_WORLD,&id);
      MPI_Comm_size(MPI_COMM_WORLD,&total);
      char hostname[128];
      gethostname(hostname, 128);
      printf("I am %d out of %d. I run on %s\n",id,total,hostname);
      MPI_Finalize(); 
      return 0;

}
