#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <omp.h>


using namespace std;
#ifdef __cplusplus
extern "C" {
#endif
  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif
void merge_arrays(int arr[],int si,int mid,int ei,int* output)
{
  
   
     int index1=si;
    int index2=mid+1;
    if(mid==ei)
    {
        index2=mid;
    }
    int k=si;
    while(index1<=mid && index2<=ei)
    {
        if(arr[index1]<=arr[index2])
        {
            output[k++]=arr[index1];
            index1++;
        }
        else if(arr[index1]>arr[index2])
        {
            output[k++]=arr[index2];
            index2++;
        }
    }
    while(index1<=mid)
    {
        output[k++]=arr[index1++];
    }
    while(index2<=ei)
    {
        output[k++]=arr[index2++];
    }
    k=si;
    for(int i=si;i<=ei;i++)
    {
        arr[i]=output[k++];
    }
   


}

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
  
  if (argc < 3) { std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthreads=atoi(argv[2]);
  
  // get arr data
  int * arr = new int [n];
  int* output=new int[n];
  generateMergeSortData (arr, n);
  double to=omp_get_wtime();
  omp_set_num_threads(nbthreads);
      for(int i=1;i<n;i=i*2)
    {
        
        for(int start_point=0;start_point<n;start_point=start_point+(2*i))
        {
            /// i is growing in powers of 2,4,8,16...
            /// so i will get start_index from for loop and end_index from 
            int end_index=start_point+(2*i)-1;
            if(end_index>=n)
            {
                end_index=n-1;
            }
            int mid=(start_point+end_index)/2;
            if(mid==start_point && mid==end_index || (end_index-start_point+1)<(2*i))
            {
                /// just one element be careful to deal with it...
                int curr_index=start_point;
                int curr_left=start_point-(2*i);
                if(curr_left<0)
                {
                    //// Then i do not have any elements in the left ...
                    continue;
                }
                /// if you come here then you have curr_left...
                if(mid==start_point){
                   // cout<<"I came here to resolve the problem.."<<endl;
                merge_arrays(arr,curr_left,start_point-1,end_index,output);
               /*  for(int k=0;k<n;k++)
                          cout<<arr[k]<<" ";
                   // cout<<"*********"<<endl;*/
                }
                else{
                //    cout<<"1st...."<<endl;
                 merge_arrays(arr,start_point,(start_point+end_index)/2,end_index,output);
               //  cout<<"2nd ......"<<endl;
                 merge_arrays(arr,curr_left,start_point-1,end_index,output);
               /*  for(int k=0;k<n;k++)
                          cout<<arr[k]<<" ";
                    cout<<endl;*/
                continue;
                }
            }
            /// you will come here you have the enough chunk size..
            
            merge_arrays(arr,start_point,mid,end_index,output);
      /*       for(int k=0;k<n;k++)
                 cout<<arr[k]<<" ";
            cout<<endl;*/
           
        }
      //  cout<<"**************************"<<endl;
    }
   cerr<<omp_get_wtime()-to<<endl;
  checkMergeSortResult (arr, n);
  
  delete[] arr;
  delete[] output;
  return 0;
}