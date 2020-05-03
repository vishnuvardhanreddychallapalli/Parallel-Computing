#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <omp.h>
#include <cmath>


using namespace std;
#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (const int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void bubble_sort(int* arr,int n)
{
 /// The outer loop needs to be executed atleast n-2 times assuming the inner loop if it can be computed in o(1) then we can reduce the complexity o(n)..
    for(int i=0;i<=n-1;i++)
    {
              if(i%2==0)
        {
            /// This loop can be executed in parallel because we reduce the flow dependence to two chunk dependency...
            #pragma omp parallel for schedule(dynamic,n/omp_get_num_threads())
            for(int j=0;j<=((int)ceil(n/2))-1;j++)
            {
                int left_element=(2*j);
                int right_element=(2*j)+1;
                if(arr[left_element]>arr[right_element])
                    /// swap element because left should be always smaller than right in ascending..
                    swap(arr[left_element],arr[right_element]);
            }
        }
        else
        {
            #pragma omp parallel for schedule(dynamic,n/omp_get_num_threads())
            for(int j=0;j<=((int)ceil(n/2))-1;j++)
            {
                int left_element=(2*j)+1;
                int right_element=(2*j)+2;
                if(right_element>n-1)
                    continue;
                if(arr[left_element]>arr[right_element])
                    swap(arr[left_element],arr[right_element]);
            }

        }
     
    }
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
  
  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  omp_set_num_threads(atoi(argv[2]));
  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);

  double to=omp_get_wtime();
  bubble_sort(arr,n);
  cerr<<omp_get_wtime()-to<<endl;
  
  checkMergeSortResult(arr, n);
  
  delete[] arr;

  return 0;
}
