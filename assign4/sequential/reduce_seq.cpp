#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <chrono>


#ifdef __cplusplus
extern "C" {
#endif

  void generateReduceData (int* arr, size_t n);
  
#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr<<"Usage: "<<argv[0]<<" <n>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int * arr = new int [atoi(argv[1])];

  generateReduceData (arr, atoi(argv[1]));
  

  // init shared result variable
  int result = 0;

  // start timing
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  // reduce
  for (int i = 0; i<n; ++i) {
    result = result + arr[i];
  }

  // end time
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapased_seconds = end-start;

  // display result
  std::cout<<result<<std::endl;
  std::cerr<<elapased_seconds.count()<<std::endl;

  delete[] arr;

  return 0;
}
