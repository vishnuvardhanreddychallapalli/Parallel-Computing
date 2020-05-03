#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <cstring>
#include <pthread.h>
#include <stdlib.h>




using namespace std;
struct record
{
	int functionid,a,b,n,intensity;
	float integral_value;
	int start_iter, end_iter;
        string sync;
};


pthread_mutex_t mutex1;
float integral_value=0;

extern "C" {
float f1(float x,int intensity);
float f2(float x,int intensity);
float f3(float x,int intensity);
float f4(float x,int intensity);

  void* sequential_integrate (void* stream)
{
  
  
    struct record *curr_value=(struct record*) stream;
      
      if(curr_value->functionid==1)
      {
      	 if(curr_value->sync=="thread")
      	 {
		
          for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
          {
           float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
           curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f1(x,curr_value->intensity);
          }
          pthread_mutex_lock(&mutex1);
          integral_value=integral_value+curr_value->integral_value;
          pthread_mutex_unlock(&mutex1);
         }
         else
         {
         	// if sync is iteration...
         	for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
          {
           float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
           curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f1(x,curr_value->intensity);
           pthread_mutex_lock(&mutex1);
           integral_value=integral_value+curr_value->integral_value;
           curr_value->integral_value=0;
           pthread_mutex_unlock(&mutex1);
          }
         	
		 }
      }
      else if(curr_value->functionid==2)
      {
      	if(curr_value->sync=="thread")
      	{
		  
         for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
          {
           float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
           curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f2(x,curr_value->intensity);
          }
          //curr_value->integral_value=0;
          pthread_mutex_lock(&mutex1);
          integral_value=integral_value+curr_value->integral_value;
           pthread_mutex_unlock(&mutex1);
      }
      else
      {
      	// if sync is iteration..
      		for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
          {
           float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
           curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f2(x,curr_value->intensity);
           pthread_mutex_lock(&mutex1);
           integral_value=integral_value+curr_value->integral_value;
           curr_value->integral_value=0;
           pthread_mutex_unlock(&mutex1);
          }
      	
	  }
      }
      else if(curr_value->functionid==3)
      {
      	if(curr_value->sync=="thread")
      	{
		
          for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
          {
           float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
           curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f3(x,curr_value->intensity);
		  }
		  pthread_mutex_lock(&mutex1);
		  integral_value=integral_value+curr_value->integral_value;
		  pthread_mutex_unlock(&mutex1);
         }
         else
         {
         	// iteration...
         	for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
          {
           float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
           curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f3(x,curr_value->intensity);
           pthread_mutex_lock(&mutex1);
           integral_value=integral_value+curr_value->integral_value;
           curr_value->integral_value=0;
           pthread_mutex_unlock(&mutex1);
           
          }
         	
		 }
      }
      else if(curr_value->functionid==4)
      {
      	if(curr_value->sync=="thread")
      	{
          for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
          {
           float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
           curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f4(x,curr_value->intensity);
          }
          pthread_mutex_lock(&mutex1);
          integral_value=integral_value+curr_value->integral_value;
          pthread_mutex_unlock(&mutex1);
       }
       else
         {
         	// iteration...
         	for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
          {
           float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
           curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f4(x,curr_value->intensity);
           pthread_mutex_lock(&mutex1);
           integral_value=integral_value+curr_value->integral_value;
           curr_value->integral_value=0;
           pthread_mutex_unlock(&mutex1);
          }
         	
		 }
	 }
  

}

}
  
int main (int argc, char* argv[]) {

  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
  
   int functionid,a,b,n,intensity,nb_threads;
   string sync;
  
   
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
    	nb_threads=atoi(argv[i]);
    }
    else if(i==7)
    {
    	sync=argv[i];
	}
  }
   pthread_t th[nb_threads];
   struct record values[nb_threads];
   int no_of_curr_iter=n/nb_threads;
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
   for(int i=0;i<nb_threads;i++)
   {
   	  // so we set the no of threads...
   	  values[i].functionid=functionid;
   	  values[i].a=a;
   	  values[i].b=b;
   	  values[i].n=n;
   	  values[i].intensity=intensity;
   	  values[i].start_iter=no_of_curr_iter*i;
 	 if(i==nb_threads-1)
   	  values[i].end_iter= n;
        else
          values[i].end_iter=(i+1)*no_of_curr_iter;
	  values[i].integral_value=0;   	
	  pthread_create(&th[i],NULL,sequential_integrate,&values[i]) ; 
   }
 
    for(int i=0;i<nb_threads;i++)
    {
    	pthread_join(th[i],NULL);
	}
  //float integrate = sequential_integrate(argc, argv);
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  
  std::cout<<integral_value<<std::endl;
  
  std::cerr<<elapsed_seconds.count()<<std::endl;
  
  return 0;
}
