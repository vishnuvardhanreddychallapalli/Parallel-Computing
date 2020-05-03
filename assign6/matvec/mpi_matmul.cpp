#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <mpi.h>
using namespace std;
float genA (int row, int col) {
  if (row > col)
    return 1.;
  else
    return 0.;
}

float genx0 (int i) {
  return 1.;
}


void checkx (int iter, long i, float xval) {
  if (iter == 1) {
    float shouldbe = i;
    if (fabs(xval/shouldbe) > 1.01 || fabs(xval/shouldbe) < .99 )
      std::cout<<"incorrect : x["<<i<<"] at iteration "<<iter<<" should be "<<shouldbe<<" not "<<xval<<std::endl;
  }

  if (iter == 2) {
    float shouldbe =(i-1)*i/2;
    if (fabs(xval/shouldbe) > 1.01 || fabs(xval/shouldbe) < .99)
      std::cout<<"incorrect : x["<<i<<"] at iteration "<<iter<<" should be "<<shouldbe<<" not "<<xval<<std::endl;
  }
}

//perform dense y=Ax on an n \times n matrix
void matmul(float*A, float*x, float*y, long n) {
    for (long row = 0; row<n; ++row) {
    float sum = 0;
    for (long col = 0; col<n; ++col) {
      sum += x[col] * A[row*n+col];
    }
    y[row] = sum;
  }
}


void all_process_work(long n,long iter)
{

   

     /// step 1:In this function i would like to call generate function at their own process to reduce the overhead...
     int my_rank,no_of_process;
     long answer_size;
     MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
     MPI_Comm_size(MPI_COMM_WORLD,&no_of_process);
     int total_no_process=no_of_process;
        /// Generate matrix and vector use matmul function inorder to compute the result store this in local memory of each process then we will take necessary actions
        /// I am sure that my (2-d matrix).... would be of size n/sqrt(p) *n /sqrt(p), and my vector would be of size n/sqrt(p)...
     int no_processors_in_each_column=(int)sqrt(no_of_process);
     long my_dimension=n/(int)sqrt(no_of_process);
     float *grid =new float[my_dimension*my_dimension];
     float* small_ans=new float[my_dimension];
     float *vector=new float[my_dimension];
         /// I created all grid and vector and i should use gen function written by professor inorder to generate right values....
         /// Now you need to calculate the original 2-d dimensions of the matrix...
     for(int it=0;it<iter;it++)
    {
     long global_row_start=(my_rank/no_processors_in_each_column)*(my_dimension);
     long global_row_end=global_row_start+my_dimension;
     long global_col_start=(my_rank%no_processors_in_each_column)*(my_dimension);
     long global_col_end=global_col_start+my_dimension;
    /* if(global_col_end==0)
     {
         /// THis happens to the processor rank of last columns..
        global_col_end=n;
     }*/
     /// Local dimensions can be anything but make sure you have the global 2-d dimensions....
     long k=0;
     /// I will be genarating the matrices locally on each processor...
    
      if(it==0)
    {
       for (long row = global_row_start; row<global_row_end; row++) {
          for (long col=global_col_start; col<global_col_end; col++) {
               grid[k] = genA(row, col);
               k++;
       }
     }
    }
      /// same happens with the vector....
      long p=0;
      if(it==0)
     {
      for (long i=global_col_start; i<global_col_end; ++i)
      {
             
             vector[p] = genx0(i);
             p++;
      }
     }
     
    
    
     
     /// I am done with storing my values in the respective arrays and i computed the transformations required...
      //float* small_ans=new float[my_dimension];
      ///*************************This is the matrix multiplication**********************************
      matmul(grid,vector,small_ans,my_dimension);
      // By now all the results will be stored in the respective 1-d arrays in ans array...
     /// Now i need to have a comm_split with  row wise for performing reduce on the row wise...
      int world_rank, world_size;
      MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
      MPI_Comm_size(MPI_COMM_WORLD, &world_size);
      int row_color = world_rank /(int)sqrt(world_size); // Determine color based on row
     // Split the communicator based on the color and use the
     // original rank for ordering
      // row_communication....
      MPI_Comm rowcomm;
      MPI_Comm_split(MPI_COMM_WORLD, row_color, world_rank, &rowcomm);
      MPI_Allreduce(MPI_IN_PLACE,small_ans,my_dimension,MPI_FLOAT,MPI_SUM,rowcomm);
     /// column_wise_communication...
    
      int col_color=world_rank%(int)sqrt(world_size);
      MPI_Comm colcomm;
      MPI_Comm_split(MPI_COMM_WORLD, col_color, world_rank, &colcomm);
       if(my_rank%no_processors_in_each_column==my_rank/no_processors_in_each_column)
       {
          
          if (true)
         {
          long q=0;
           
           for (long i = global_col_start; i<global_col_end; ++i)
           {
         checkx (it+1, i, small_ans[q]);
             
             q++;
          }
          //cout<<endl;
         }
        /// Once checking is allright i have to go on with the next iteration but before that i need to broad cast all my results to the columnwise processors...
         
       }
    
    MPI_Bcast(small_ans, my_dimension, MPI_FLOAT, col_color, colcomm);
    /// Once you broadcast all the values you need to swap small_results with vector because they are responsible for next results ...
   for(int i=0;i<my_dimension;i++)
   {
      vector[i]=small_ans[i];
      small_ans[i]=0;
   }
  }

/// done with multiplication..
  delete[] grid;
  delete[] vector;
   delete[] small_ans;

    
}


int main (int argc, char*argv[]) {

  if (argc < 3) {
    std::cout<<"usage: "<<argv[0]<<" <n> <iteration>"<<std::endl;
  }
  bool check = true;
  long n = atol(argv[1]);
  long iter = atol(argv[2]);
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
   MPI_Init(&argc,&argv);
   all_process_work(n,iter);
   MPI_Finalize();
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;
  

  
 /* delete[] A;
  delete[] x;
  delete[] y;*/
  
  return 0;
}