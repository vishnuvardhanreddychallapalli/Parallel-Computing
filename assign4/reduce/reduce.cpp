#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <chrono>
#include <cstring>

using namespace std;
int final_ans=0;
#ifdef __cplusplus
extern "C" {
#endif
 void generateReduceData (int* arr, size_t n,int thread_value,char* sc,int v)
{
   omp_set_num_threads(thread_value);
   char* s=sc;
   int value=v;
   //cout<<s<<endl;
  if(s=="dynamic")
  {
  #pragma omp parallel for reduction(+: final_ans) schedule(dynamic,v)
   for(int i=0;i<n;i++)
   {
      final_ans=final_ans+arr[i];
   }
  }
 else if(s=="guided")
 {
   #pragma omp parallel for reduction(+: final_ans) schedule(guided,v)
   for(int i=0;i<n;i++)
   {
      final_ans=final_ans+arr[i];
   }
 }
else
  { 
    #pragma omp parallel for reduction(+: final_ans) 
   for(int i=0;i<n;i++)
   {
      final_ans=final_ans+arr[i];
   }
    
   }

}
#ifdef __cplusplus
}
#endif


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
  
  if (argc < 5) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int * arr = new int [n];
    double t0 = omp_get_wtime();
    generateReduceData (arr, atoi(argv[1]),atoi(argv[2]),argv[3],atoi(argv[4]));
    cerr<<omp_get_wtime()-t0<<endl;
    cout<<final_ans<<endl;
  delete[] arr;

  return 0;
}