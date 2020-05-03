#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <omp.h>
#include <bits/stdc++.h>
#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);

#ifdef __cplusplus
}
#endif
using namespace std;
void merge_arrays(int arr[],int si,int mid,int ei,int *output)
{
  
    // int * output=new int[ei-si+1];  
     int index1=si;
    int index2=mid+1;
    /*if(mid==ei)
    {
        index2=mid;
    }*/
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
  // delete [] output;


}

void merge_sort(int* arr,int start,int end,int* output)
{

    if(start==end)
           return ;
    if(start+1==end)
    {
       if(arr[start]>arr[end])
               swap(arr[start],arr[end]);
       return;
     }
    int mid=(start+end)/2;
   #pragma omp task untied
    merge_sort(arr,start,mid,output);
  #pragma omp task untied
    merge_sort(arr,mid+1,end,output);
 #pragma omp taskwait
    merge_arrays(arr,start,mid,end,output);
 
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
  
  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nb= atoi(argv[2]);
  omp_set_num_threads(nb);
  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);
  int *output=new int[n];
  //insert sorting code here.
  double to=omp_get_wtime();
   #pragma omp parallel num_threads(omp_get_num_threads())
  {
    #pragma omp single
        merge_sort(arr,0,n-1,output);
  }

  std::cerr<<omp_get_wtime()-to<<endl;
    //  cout<<"I am done for today"<<endl;

  checkMergeSortResult (arr, n);
  /*for(int i=0;i<n;i++)
  //cout<<arr[i]<<" ";  */
  delete[] arr;
  delete[] output;

  return 0;
}
