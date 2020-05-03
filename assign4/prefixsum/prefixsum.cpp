#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cmath>
#include <math.h>
#include <unistd.h>
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif
  void generatePrefixSumData (int* arr, size_t n);
  void checkPrefixSumResult (int* arr, size_t n);
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
  
  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  
  int n = atoi(argv[1]);
  int * arr = new int [n];
  generatePrefixSumData (arr, n);

 //int * pr = new int [n+1];
  int no_of_thread=atoi(argv[2]);
  omp_set_num_threads(no_of_thread);
  double to=omp_get_wtime();
  //insert prefix sum code here
     ///upsweep...
    int outer_len=(int)ceil(log2(n));
    //cout<<"The outer len is :"<<outer_len<<endl;
    int end_point=n;

    for(int i=0;i<outer_len;i++)
    {
        //int start_point=0;
       #pragma omp parallel 
       {
         #pragma omp for schedule(dynamic,50)
        for(int start_point=0;start_point<end_point;start_point=start_point+(int(pow(2,i+1))))
        {

             if(start_point+int(pow(2,i+1))-1<end_point)
             {
                arr[start_point+int(pow(2,i+1))-1]=arr[start_point+int(pow(2,i))-1]+arr[start_point+int(pow(2,i+1))-1];
             }
             else if(start_point+int(pow(2,i))-1==end_point-1)
             {
                 arr[end_point-1]=arr[end_point-1];
             }
             else if(start_point+int(pow(2,i))-1<end_point && start_point+int(pow(2,i+1))-1>=end_point)
             {
                 arr[end_point-1]=arr[end_point-1]+arr[start_point+int(pow(2,i))-1];
             }

        }
      }
    /*    cout<<"The elements at level  forward_sweep:"<<i<<endl;
        for(int j=0;j<n;j++)
        {
            cout<<arr[j]<<" ";
        }
        cout<<endl;*/

    }


   /// down-sweep
    int ans=arr[n-1];
    arr[n-1]=0;
    end_point=n;
    int* prefix=new int[n+1];
    for(int i=0;i<n+1;i++)
    {
        prefix[i]=0;
    }
    for(int i=outer_len-1;i>=0;i--)
    {
        // cout<<"The level is :********"<<i<<endl;
       #pragma omp parallel
      {
         #pragma omp for schedule(dynamic,50)
        for(int start_point=0;start_point<end_point;start_point=start_point+(int(pow(2,i+1))))
        {
           int suggested_root=start_point+(int(pow(2,i+1)))-1;
           int left_index=-1;
           int right_index=-1;
            if(suggested_root>=end_point)
            {
                suggested_root=n-1;
                left_index=start_point+(int(pow(2,i)))-1;
                if(left_index>n-1)
                    left_index=n-1;
                right_index=n-1;
            }
            else
            {
                /// You come here when your suggested root is less than n-value...
                ///   so no problem
                   left_index=start_point+(int(pow(2,i)))-1;
                   if(left_index>n-1)
                        left_index=n-1;
                   right_index=suggested_root;
            }
            /// Now my responsibility is to calculate left _index and right_index in order to update..
            if(left_index!=right_index)
            {
              prefix[left_index]=prefix[suggested_root];
              prefix[right_index]=prefix[left_index]+arr[left_index];
            }
            else
            {
                ///says that left-index==right-index...
                /// do not perform any task......
            }
           // cout<<"The suggested-index, left_index ,right_index"<<suggested_root<<" "<<left_index<<" "<<right_index<<endl;
        }
      }

     /*   cout<<"The elements at level backward_sweep  :"<<i<<endl;
        for(int j=0;j<n;j++)
        {
            cout<<prefix[j]<<" ";
        }
        cout<<endl;*/


    }
    prefix[n]=ans;
    cerr<<omp_get_wtime()-to<<endl;
  
  checkPrefixSumResult(prefix, n);

  delete[] arr;

  return 0;
}
