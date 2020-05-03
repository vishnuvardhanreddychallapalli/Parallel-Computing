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

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

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
  
  if (argc < 9) {
    std::cerr<<"Usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }

   int functionid,a,b,n,intensity,nbthreads,granularity;
   string scheduling="";
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
    else if(i==6)
    {
       nbthreads=atoi(argv[i]);
    }
    else if(i==7)
    {
      scheduling=scheduling+argv[i];
    }
  else if(i==8)
  {
     granularity=atoi(argv[i]);  
  }
 }
  
double t0 = omp_get_wtime();
    if(scheduling=="dynamic")
  {  
      omp_set_num_threads(nbthreads);
      if(functionid==1)
      {
        #pragma omp parallel for reduction(+: integral_value) schedule(dynamic,granularity)
          for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f1(x,intensity);
          }
      }
      else if(functionid==2)
      {
        #pragma omp parallel for reduction(+: integral_value) schedule(dynamic,granularity)
         for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f2(x,intensity);
          }
      }
      else if(functionid==3)
      {
         #pragma omp parallel for reduction(+: integral_value) schedule(dynamic,granularity)
          for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f3(x,intensity);
          }
      }
      else if(functionid==4)
      {
          #pragma omp parallel for reduction(+: integral_value) schedule(dynamic,granularity)
          for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f4(x,intensity);
          }
      }
  }
 else if(scheduling=="guided")
 {
   omp_set_num_threads(nbthreads);
   if(functionid==1)
      {
        #pragma omp parallel for reduction(+: integral_value) schedule(guided,granularity)
          for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f1(x,intensity);
          }
      }
      else if(functionid==2)
      {
        #pragma omp parallel for reduction(+: integral_value) schedule(guided,granularity)
         for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f2(x,intensity);
          }
      }
      else if(functionid==3)
      {
         #pragma omp parallel for reduction(+: integral_value) schedule(guided,granularity)
          for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f3(x,intensity);
          }
      }
      else if(functionid==4)
      {
          #pragma omp parallel for reduction(+: integral_value) schedule(guided,granularity)
          for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f4(x,intensity);
          }
      }

  }
else
{
    omp_set_num_threads(nbthreads);

    if(functionid==1)
      {
        #pragma omp parallel for reduction(+: integral_value) schedule(static,granularity)
          for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f1(x,intensity);
          }
      }
      else if(functionid==2)
      {
        #pragma omp parallel for reduction(+: integral_value) schedule(static,granularity)
         for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f2(x,intensity);
          }
      }
      else if(functionid==3)
      {
         #pragma omp parallel for reduction(+: integral_value) schedule(static,granularity)
          for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f3(x,intensity);
          }
      }
      else if(functionid==4)
      {
          #pragma omp parallel for reduction(+: integral_value) schedule(static,granularity)
          for(int i=0;i<n;i++)
          {
           float x=(a+(i+0.5)*(((b-a)*1.0)/n));
           integral_value=integral_value+(((b-a)*1.0)/n)*f4(x,intensity);
          }
      }

}
 cerr<<omp_get_wtime()-t0<<endl;
 cout<<integral_value<<endl;

  return 0;
}
