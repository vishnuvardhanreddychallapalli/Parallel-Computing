#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <mpi.h>

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

float num_int(int functionid,int a,int b,int start,int end,int intensity,int n)
{
   float integral_value=0;
   if(functionid==1)
      {
        //float integral_value=0;
          for(int i=start;i<end;i++)
          {
            float x=(a+(i+0.5)*(((b-a)*1.0)/n));
            integral_value=integral_value+(((b-a)*1.0)/n)*f1(x,intensity);
          }
      }
   else if(functionid==2)
      {
         // float integral_value=0;
          for(int i=start;i<end;i++)
          {
            float x=(a+(i+0.5)*(((b-a)*1.0)/n));
            integral_value=integral_value+(((b-a)*1.0)/n)*f1(x,intensity);
          }
      }
   else if(functionid==3)
      {
         // float integral_value=0;
          for(int i=start;i<end;i++)
          {
            float x=(a+(i+0.5)*(((b-a)*1.0)/n));
            integral_value=integral_value+(((b-a)*1.0)/n)*f1(x,intensity);
          }
      }
    else if(functionid==4)
      {
         // float integral_value=0;
          for(int i=start;i<end;i++)
          {
            float x=(a+(i+0.5)*(((b-a)*1.0)/n));
            integral_value=integral_value+(((b-a)*1.0)/n)*f1(x,intensity);
          }
      }
  
  return integral_value;

}
int main (int argc, char* argv[]) {
  
  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
   int a=0,b=0,functionid=0,n=0,intensity=0;
   float integral_value=0;
   //auto start_time=chrono::steady_clock::now();
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
   //int final_ans=0;
   auto start_time=chrono::steady_clock::now();
   MPI_Init(&argc,&argv);
   int my_rank,universal_rank;
   MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
   MPI_Comm_size(MPI_COMM_WORLD,&universal_rank);
   int start=(my_rank)*(int)(n/universal_rank);
   int end=(my_rank+1)*(int)(n/universal_rank);
   if(my_rank==universal_rank-1)
   {
     /// This means i am the last node then i want to take final_n value;
       end=n;
   }
   float my_integral_value=num_int(functionid,a,b,start,end,intensity,n);
   MPI_Reduce(&my_integral_value, &integral_value, 1, MPI_FLOAT, MPI_SUM, 0,
           MPI_COMM_WORLD);
   if(my_rank==0)
   {
     cout<<integral_value<<endl;
   }
   MPI_Finalize();
  // end of mpi frm here...
   auto end_time=chrono::steady_clock::now();
   cerr<<chrono::duration_cast<chrono::seconds>(end_time-start_time).count()<<endl;
   //cout<<integral_value<<endl;
 
  return 0;
}