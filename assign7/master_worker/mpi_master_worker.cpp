#include <mpi.h>
#include <iostream>
#include <chrono>

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
double solution=0;
long start=0;
double num_int(int functionid,int a,int b,int start,int end,int intensity,int n)
{


  double integral_value=0;
 for(int i=start;i<end;i++)
  {
      float x=(a+(i+0.5)*(((b-a)*1.0)/n));
      if(functionid==1)
      {
          integral_value=integral_value+(((b-a)*1.0)/n)*f1(x,intensity);
      }
      else if(functionid==2)
      {
          integral_value=integral_value+(((b-a)*1.0)/n)*f2(x,intensity);
      }
      else if(functionid==3)
      {
          integral_value=integral_value+(((b-a)*1.0)/n)*f3(x,intensity);
      }
      else if(functionid==4)
      {
          integral_value=integral_value+(((b-a)*1.0)/n)*f4(x,intensity);
      }
  }
  return integral_value;

}
void all_process_work(int function_id,int a,int b,int n,int intensity)
{

   int my_rank,universal_rank;
   bool isok=true;
   double small_ans=0;
   double final_ans=0;
   MPI_Status status;
   MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
   MPI_Comm_size(MPI_COMM_WORLD,&universal_rank);
    long index[2];
   int granularity=(n/(6*universal_rank));
   //cout<<"granulariyt: "<<granularity<<endl;
   // MPI_Status status;
   if(my_rank==0)
   {
     
     /// You have to divide all the work just in an uniform distribution......
    /// Process first as a master give one chunk of work to every one....
    //int granularity=5;
    for(int id=1;id<universal_rank;id++)
    {
       long my_start=start;
       long my_end=my_start+granularity;
       start=start+granularity;
       index[0]=my_start;
       index[1]=(my_end<n)?my_end:n;
       // cout<<"THe index values are:  "<<index[0]<<"  "<<index[1]<<" "<<my_rank<<"here"<<endl;
       if(index[0]<n && index[1]<=n)
       {
         // cout<<"THe index values are:  "<<index[0]<<"  "<<index[1]<<" "<<my_rank<<endl;
         /// Send the array and expect some results.....
         MPI_Send(index,2,MPI_LONG,id,0,MPI_COMM_WORLD);
        // MPI_Recv(&small_ans,1,MPI_LONG,id,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
       }
      if(index[1]==n || my_end>n)
      {
         // You will come here to stop everything... by sending tag==-1....
        for(int k=1;k<universal_rank;k++)
        { 
             index[0]=0;
             index[1]=0;
             MPI_Send(index,2,MPI_LONG,k,0,MPI_COMM_WORLD);
        }
        break;
      } 
    }

   //end of the for loop....
    //cout<<"I am in the end of for looop...."<<endl;
   if(start<n)
   {
       
     /// NOw you have to distribute carefully....
     while(start<n)
     {
       ///     First see who will give you the answer first based on their id assign the work....
       MPI_Recv(&small_ans,1,MPI_LONG,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);

       // YOu do not know the souce to send so status object is a structure go and check in it..
       long my_start=start;
       long my_end=my_start+granularity;
       start=start+granularity;
       index[0]=my_start;
       index[1]=(my_end<n)?my_end:n;
        if(index[0]<n && index[1]<=n)
        {
         // cout<<"THe index values are:  "<<index[0]<<"  "<<index[1]<<" "<<my_rank<<endl;
         /// Send the array and expect some results.....
         MPI_Send(index,2,MPI_LONG,status.MPI_SOURCE,0,MPI_COMM_WORLD);
        // MPI_Recv(&small_ans,1,MPI_LONG,id,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }
        if(index[1]==n || my_end>n)
        {
           for(int k=1;k<universal_rank;k++)
           { 
             index[0]=0;
             index[1]=0;
             MPI_Send(index,2,MPI_LONG,k,0,MPI_COMM_WORLD);
           }
        /// THis is the breaking condition.....
           break;
        } 
     }
      /// By now all the work distribution got completed now you have to collect all the results and ask every worker to break from theri loops...
      for(long id=1;id<universal_rank;id++)
      {
        MPI_Recv(&small_ans,1,MPI_LONG,id,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
         final_ans=final_ans+small_ans;
      }
    /// YOu added everything in you final_result... now just ask everyone to break from theri for loop...
     for(long id=1;id<universal_rank;id++)
      {
        index[0]=0;
        index[1]=1;
        MPI_Send(index,2,MPI_LONG,status.MPI_SOURCE,0,MPI_COMM_WORLD);
       // final_ans=final_ans+small_ans;
      }
   }
 
   
   

   }
   else
   {
       while(true)
      {
        MPI_Recv(index,2,MPI_LONG,0,0,MPI_COMM_WORLD,&status);
       // cout<<"The values of indexes are :"<<index[0]<<" "<<index[1]<<" rank is: "<<my_rank<<endl;
        if(index[0]==0 && index[1]==0)
        {
           // cout<<"I am breaking... "<<my_rank<<endl;
             break;
        }
        small_ans=small_ans+num_int(function_id,a,b,index[0],index[1],intensity,n);
        MPI_Send(&small_ans,1,MPI_LONG,0,0,MPI_COMM_WORLD);
     }
   }

  if(my_rank==0)
  {
     cout<<final_ans<<endl;
  }

}
int main (int argc, char* argv[]) {

  if (argc < 6) {
    std::cerr<<"usage: mpirun "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
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
 // cout<<"funcid is : "<<functionid<<" a is : "<<a<<"b is : "<<b<<"n is : "<<n<<"intensity is: "<<intensity<<endl;
  auto start_time=chrono::steady_clock::now();
   MPI_Init(&argc,&argv);
   int my_rank,universal_rank;
   MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
   MPI_Comm_size(MPI_COMM_WORLD,&universal_rank);
   if(universal_rank==1)
   {
       if(my_rank==0)
      {
       double ans=num_int(functionid,a,b,0,n,intensity,n);
       cout<<ans<<endl;
      }
   }
  else{
  // cout<<"My rank is : "<<my_rank<<" "<<"The universial rank is: "<<universal_rank<<endl; 
   all_process_work(functionid,a,b,n,intensity);
  }
   MPI_Finalize();
  auto end_time=chrono::steady_clock::now();
  cerr<<chrono::duration_cast<chrono::seconds>(end_time-start_time).count()<<endl;
  return 0;
}
