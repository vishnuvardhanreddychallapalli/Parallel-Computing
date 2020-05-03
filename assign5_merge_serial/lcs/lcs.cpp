#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <bits/stdc++.h>
#include <omp.h>

using namespace std;
#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif


int lcs(char* X,char* Y,int m,int n)
{
    int **ans=new int*[m+1];
    for(int i=0;i<=m;i++)
       ans[i]=new int[n+1];
    int nb=omp_get_num_threads();
    /// this for loop can be executed in parallel
   #pragma omp parallel for shared(nb) schedule(dynamic,10)
    for(int i=0;i<=n;i++)
        ans[0][i]=0;
    /// this for loop can be executed in parallel..
   #pragma omp parallel for shared(nb) schedule(dynamic,10)
    for(int i=0;i<=m;i++)
        ans[i][0]=0;
        
    for(int row=1;row<=m;row++)
    {
       ///inner loop can be done in parallel....
       /// This following loop can be executed in parallel
      int nb=omp_get_num_threads();
     // int j=1;
      int i=row;
      int p=(m+n/omp_get_num_threads())+1;
       #pragma omp parallel for shared(i) schedule(dynamic,p)
       for(int j=1;j<=n;j++)
       {
         if(i>0)
         {
           if(X[i-1]==Y[j-1])
           {
               /// take the diagonal and add+1..
               ans[i][j]=ans[i-1][j-1]+1;
              // cout<<"Added"<<endl;
           }
           else
           {
               /// Not equal string then you have to take maximum from the top...
               ans[i][j]=max(ans[i][j-1],ans[i-1][j]);
           }
        }
        i--;
       }
   }
     

     for(int i=2;i<=n;i++)
     {
         /// this loop can be executed in parallel....
         int nb=omp_get_num_threads();
         //int y=i;
         int x=m;
         int p=(m+n/omp_get_num_threads())+1;
         #pragma omp parallel for shared(x) schedule(dynamic,p)
         for(int y=i;y<=n;y++)
         {
              //x>0 && y<=n
             if(x>0)
            {
             if(X[x-1]==Y[y-1])
             {
                 ans[x][y]=ans[x-1][y-1]+1;
                // cout<<"added"<<endl;;
             }
             else
             {
                 ans[x][y]=max(ans[x][y-1],ans[x-1][y]);
             }
            }
           
          x--;

        }
     }
    int final_ans=ans[m][n];
    for(int i=0;i<=m;i++)
        delete [] ans[i];
    delete [] ans;
    return final_ans;
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

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nb= atoi(argv[3]);
   omp_set_num_threads(nb);
  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);

  //insert LCS code here.
  int result = -1; // length of common subsequence
  double to=omp_get_wtime(); 
  //cout<<X<<endl;
  //cout<<Y<<endl;
  result=lcs(X,Y,m,n);
  std::cerr<<omp_get_wtime()-to<<endl;
  //cout<<result<<endl;
  checkLCS(X, m, Y, n,result);


  return 0;
}