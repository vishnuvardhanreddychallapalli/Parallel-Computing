#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>

extern "C" {
float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);
  float sequential_integrate (int argc, char* argv[])
{
   int functionid,a,b,n,intensity;
   float integral_value=0;
     for(int i=1;i<argc;i++)
  {
      if(i==1)
      {
         functionid=atoi(argv[i]);
      }
      else if(i==2)
      {
          a=atoi(argv[i]);
      }
      else if(i==3)
      {
          b=atoi(argv[i]);
      }
      else if(i==4)
      {
          n=atoi(argv[i]);
      }
      else if(i==5)
      {
          intensity=atoi(argv[i]);
      }
  }
  

      
      if(functionid==1)
      {
          for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f1(x,intensity);
          }
      }
      else if(functionid==2)
      {
         for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f2(x,intensity);
          }
      }
      else if(functionid==3)
      {
          for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f3(x,intensity);
          }
      }
      else if(functionid==4)
      {
          for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f4(x,intensity);
          }
      }
  return integral_value;

}

}
  
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
