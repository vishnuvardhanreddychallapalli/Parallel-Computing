#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

float sequential_integrate (int argc, char* argv[]);

#ifdef __cplusplus
}
#endif

  
int main (int argc, char* argv[]) {

  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
  
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    
  float integrate = sequential_integrate(argc, argv);
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  
  std::cout<<integrate<<std::endl;
  
  std::cerr<<elapsed_seconds.count()<<std::endl;
  
  return 0;
}
