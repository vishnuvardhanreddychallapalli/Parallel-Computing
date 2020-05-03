#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
using namespace std;
//pthread_mutex_t mut1;
void *hello(void * value)
{
   long long* index=(long long*) value;
   long long curr_index=*index;
   printf("I am thread %lld in nbthreads\n",curr_index);
 //  pthread_mutex_lock(&mut1);
//    cout<<"I am thread "<<curr_index<<" in nbthreads"<<endl;
  //pthread_mutex_unlock(&mut1);
    pthread_exit(NULL);
}
int main (int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr<<"usage: "<<argv[0]<<" <nbthreads>"<<std::endl;
    return -1;
  }
 // so argc =2 ..
   long long nbthreads=atoll(argv[1]);
   // so now creating nb_threads
   pthread_t tid[nbthreads];
   // so now you have nbthreads...
 /// spawing all the threads..
   for(long long int i=0;i<nbthreads;i++)
   {
     pthread_create(&tid[i],NULL,hello,&i);
    }
 //waiting for all the threads..
   for(long long int i=0;i<nbthreads;i++)
   {
       pthread_join(tid[i],NULL);
   }
  
  return 0;
}
