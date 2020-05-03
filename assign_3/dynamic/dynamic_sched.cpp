#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <queue>
#include <pthread.h>
#include <cstring>
#include <algorithm>
using namespace std;
struct record
{
        int functionid,a,b,n,intensity;
        float integral_value;
        int start_iter, end_iter;
        string sync;
	int threadId;

};

queue<pair<int,int>> q;
pthread_mutex_t mutex1;
float integral_value=0;
int niter = 0;

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);
 void* sequential_integrate (void* stream)
{
    //cout<<"Potanava"<<endl;
  
    struct record *curr_value=(struct record*) stream;
      
      if(curr_value->functionid==1)
      {
           // cout<<"Enter funid"<<endl;
      	 if(curr_value->sync.compare("chunk")==0)
      	 {
             //cout<<"entering chunk also"<<endl;
		
          for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
          {
           float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
           curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f1(x,curr_value->intensity);
          }
          pthread_mutex_lock(&mutex1);
          integral_value=integral_value+curr_value->integral_value;
        //  cout<<"after for in seq int: "<<integral_value<<endl;
          curr_value->integral_value=0;
          pthread_mutex_unlock(&mutex1);
         }
         else if(curr_value->sync.compare("iteration")==0)
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
        else
           {
             /// thread execution will start here...
               //cout<<"I am in thread"<<endl;
              for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
             {
             float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
             curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f1(x,curr_value->intensity);
             //pthread_mutex_lock(&mutex1);
             //integral_value=integral_value+curr_value->integral_value;
             //curr_value->integral_value=0;
             //pthread_mutex_unlock(&mutex1);
            }

             
           }
      }
      else if(curr_value->functionid==2)
      {
      	 if(curr_value->sync=="chunk")
      	 {
		
          for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
          {
           float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
           curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f2(x,curr_value->intensity);
          }
          pthread_mutex_lock(&mutex1);
          integral_value=integral_value+curr_value->integral_value;
          curr_value->integral_value=0;
          pthread_mutex_unlock(&mutex1);
         }
         else if(curr_value->sync=="iteration")
         {
         	// if sync is iteration...
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
        else
           {
             /// thread execution will start here...
              for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
             {
             float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
             curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f2(x,curr_value->intensity);
             //pthread_mutex_lock(&mutex1);
             //integral_value=integral_value+curr_value->integral_value;
             //curr_value->integral_value=0;
             //pthread_mutex_unlock(&mutex1);
            }

             
           }
      }
      else if(curr_value->functionid==3)
      {
      	 if(curr_value->sync=="chunk")
      	 {
		
          for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
          {
           float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
           curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f3(x,curr_value->intensity);
          }
          pthread_mutex_lock(&mutex1);
          integral_value=integral_value+curr_value->integral_value;
          curr_value->integral_value=0;
          pthread_mutex_unlock(&mutex1);
         }
         else if(curr_value->sync=="iteration")
         {
         	// if sync is iteration...
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
        else
           {
             /// thread execution will start here...
              for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
             {
             float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
             curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f3(x,curr_value->intensity);
             //pthread_mutex_lock(&mutex1);
             //integral_value=integral_value+curr_value->integral_value;
             //curr_value->integral_value=0;
             //pthread_mutex_unlock(&mutex1);
            }

             
           }
      }
      else if(curr_value->functionid==4)
      {
      	 if(curr_value->sync=="chunk")
      	 {
		
          for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
          {
           float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
           curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f4(x,curr_value->intensity);
          }
          pthread_mutex_lock(&mutex1);
          integral_value=integral_value+curr_value->integral_value;
          curr_value->integral_value=0;
          pthread_mutex_unlock(&mutex1);
         }
         else if(curr_value->sync=="iteration")
         {
         	// if sync is iteration...
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
        else
           {
             /// thread execution will start here...
              for(int i=curr_value->start_iter;i<curr_value->end_iter;i++)
             {
             float x=(curr_value->a+(i+0.5)*(((curr_value->b-curr_value->a)*1.0)/curr_value->n));
             curr_value->integral_value=curr_value->integral_value+(((curr_value->b-curr_value->a)*1.0)/curr_value->n)*f4(x,curr_value->intensity);
             //pthread_mutex_lock(&mutex1);
             //integral_value=integral_value+curr_value->integral_value;
             //curr_value->integral_value=0;
             //pthread_mutex_unlock(&mutex1);
            }

             
           }
	 }
  

}
void *manager(void* stream)
{
  struct record* curr_value=(struct record*) stream;
      
//cout<<"before while in manager: "<<curr_value->threadId<<endl;
   while(niter>0)
   {
      pthread_mutex_lock(&mutex1);
	niter--;
     //struct record *curr_value=(struct record*) stream;
     curr_value->start_iter=q.front().first;
     curr_value->end_iter=q.front().second;
     //cout<<"printing sync : "<<curr_value->sync<<endl;
    // sequential_integrate(curr_value);
     q.pop();
     pthread_mutex_unlock(&mutex1);
    sequential_integrate(curr_value);
   }
   if(curr_value->sync=="thread")
   {
     pthread_mutex_lock(&mutex1);
     integral_value=integral_value+curr_value->integral_value;
     pthread_mutex_unlock(&mutex1);
   }
//  cout<<"after while in manager: "<<curr_value->threadId<<endl;
}


#ifdef __cplusplus
}
#endif
int main (int argc, char* argv[]) {

  if (argc < 9) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync> <granularity>"<<std::endl;
    return -1;
  }
  int functionid,a,b,n,intensity,nb_threads,granularity;
   string sync="";
//   pthread_mutex_init(&mutex1);
  
   
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
    	sync=sync+argv[i];
     }
	else if(i==8)
	{
		granularity=atoi(argv[i]);
	}
  }
   pthread_t th[nb_threads];
   struct record values[nb_threads];
  int curr_range=0;
 // queue <pair<int,int>> q;
if ((n%granularity)>0)
{
	niter = n/granularity+1;
}else{
	niter = n/granularity;
}
  while(curr_range<n)
  {
         if(curr_range+granularity>n)
         {

             pair<int,int> p=make_pair(curr_range,n);
            //cout<<"The pair is :"<<p.first<<" "<<p.second<<endl;
             q.push(p);
             break;
         }
  	 pair<int,int> p=make_pair(curr_range,curr_range+granularity);
          //cout<<"The pair is :"<<p.first<<" "<<p.second<<endl;
  	 q.push(p);
  	 curr_range=curr_range+granularity;
  }

//  cout<<"THe size of queues is:"<<q.size()<<endl;
 std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  	
  	
  	 for(int i=0;i<nb_threads;i++)
  	 {	   
  	  pthread_mutex_lock(&mutex1);
  	    //pair<int ,int> p=q.front();
  //	    q.pop();
  	  values[i].functionid=functionid;
   	  values[i].a=a;
   	  values[i].b=b;
   	  values[i].n=n;
   	  values[i].intensity=intensity;
   //	  values[i].start_iter=p.first;
   //	  values[i].end_iter= p.second;
	  values[i].integral_value=0;   
      values[i].sync=sync;
	  values[i].threadId=i;	
      pthread_mutex_unlock(&mutex1);
	  pthread_create(&th[i],NULL,manager,&values[i]) ;
    }
    

	 for(int i=0;i<nb_threads;i++)
       {
    	pthread_join(th[i],NULL);
	}
    //  cout<<"after join"<<endl;
  //float integrate = sequential_integrate(argc, argv);
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  
  std::cout<<integral_value<<std::endl;
  
  std::cerr<<elapsed_seconds.count()<<std::endl;
  
  return 0;

} 
