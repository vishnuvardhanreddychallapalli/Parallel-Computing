#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;
#ifdef __cplusplus
extern "C" {
#endif

  void generateReduceData (int* arr, size_t n);

#ifdef __cplusplus
}
#endif

int reduce(int*arr ,int start,int end)
{
    /// base cases..
    if(start==end)
       return arr[start];
    if(start+1==end)
    {
      /// You have only two values
       return arr[start]+arr[end];
    }
   int mid=(start+end)/2;
   int first_half,second_half;
  #pragma omp task
   first_half=reduce(arr,start,mid);
  #pragma omp task 
   second_half=reduce(arr,mid+1,end);
  #pragma omp taskwait
      return  first_half+second_half;
  // return first_half;

}

int main (int argc, char* argv[]) {
  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }
  
  if (argc < 3) {
    std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int * arr = new int [n];
  int final_ans=0;
  generateReduceData (arr, atoi(argv[1]));

  //insert reduction code here
  double to=omp_get_wtime();
  omp_set_num_threads(atoi(argv[2]));
  #pragma omp parallel
  {
  #pragma omp single
  final_ans=reduce(arr,0,n-1);
  }
  cout<<final_ans<<endl;
  cerr<<omp_get_wtime()-to<<endl;
  
  delete[] arr;

  return 0;
}