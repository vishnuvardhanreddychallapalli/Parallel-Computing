#include <chrono>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#ifdef __cplusplus
extern "C" {
#endif

  void generatePrefixSumData (int* arr, size_t n);
  void checkPrefixSumResult (int* arr, size_t n);
  
  
#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {

  
  if (argc < 2) {
    std::cerr<<"Usage: "<<argv[0]<<" <n>"<<std::endl;
    return -1;
  }


  int n = atoi(argv[1]);
  int * arr = new int [n];

  generatePrefixSumData (arr, n);
  
  
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  
  int* prefix = new int [n+1];

  prefix[0] = 0;

  for (int i=0; i<n; ++i) {
    prefix[i+1] = prefix[i] + arr[i];
  }

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  
  std::cerr<<elapsed_seconds.count()<<std::endl;

  checkPrefixSumResult(prefix, n);
  
  delete[] arr;
  delete[] prefix;

  return 0;
}
