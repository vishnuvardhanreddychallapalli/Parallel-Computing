#include <mpi.h>
#include <math.h>
#include <iostream>
#include <chrono>
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif

  int check2DHeat(double** H, long n, long rank, long P, long k); //this assumes array of array and grid block decomposition

#ifdef __cplusplus
}
#endif
double genH0(long row, long col, long n) {
  double val = (double)(col == (n/2));
  return val;
}
void serial_heat(long n,long iter)
{

  double** grid=new double*[n];
  double** next_grid=new double*[n];
for(int i=0;i<n;i++)
{
  grid[i]=new double[n];
  next_grid[i]=new double[n];
  for(int j=0;j<n;j++)
  {
   grid[i][j]=genH0(i,j,n);
   next_grid[i][j]=0;
  }
}
// I completed the input now i want to perfom conv operation...
for(long it=0;it<iter;it++)
{
for(long i=0;i<n;i++)
{
 for(long j=0;j<n;j++)
 {
  int curr_value=grid[i][j];
  /// formula grid[i][j]+grid[i-1][j]+grid[i+1][j]+grid[i][j-1]+grid[i][j+1]..
  if(i-1>=0)
  {
     curr_value+=grid[i-1][j];
  }
   if(i-1<0)
  {
    curr_value+=grid[i][j];
  }
  if(i+1<n)
  {
   curr_value+=grid[i+1][j];
  }
  if(i+1>=n)
  {
   curr_value+=grid[i][j];
  }
  if(j+1<n)
  {
   curr_value+=grid[i][j+1];
  }
  if(j+1>=n)
  {
   curr_value+=grid[i][j];
  }
  if(j-1>=0)
  {
    curr_value+=grid[i][j-1];
  }
  if(j-1<0)
  {
    curr_value+=grid[i][j];
  }
  //cout<<(curr_value*0.2)<<" ";
  next_grid[i][j]=(curr_value*0.2);
 }

}
 //cout<<"At the iteration: "<<it<<endl;
// Copy the next_grid to grid..
 for(long i=0;i<n;i++)
 {
   for(long j=0;j<n;j++)
   {
     grid[i][j]=next_grid[i][j];
     next_grid[i][j]=0;
   }
 }
}
/// After all the completion of your program delete grid and next grid...
for(long i=0;i<n;i++)
{
   delete grid[i];
   delete next_grid[i];
}
delete[] grid;
delete[] next_grid;
}
void Heat_equation(long global_row_start,long global_row_end,long global_col_start,long global_col_end,long N,long iter)
{
   int my_rank,world_size;
    int n=(global_row_end-global_row_start);
   double** curr_state=new double*[global_row_end-global_row_start];
   double** next_state=new double*[global_row_end-global_row_start];
   for(long i =0;i<n;i++)
   {
     curr_state[i]=new double[global_row_end-global_row_start];
     next_state[i]=new double[global_row_end-global_row_start];
   }

   ///Step1 get the input from gen function...
   MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
   MPI_Comm_size(MPI_COMM_WORLD,&world_size);
   //cout<<"MY rank is : "<<my_rank<<" my world size is: "<<world_size<<"The value of n is:"<<n<<endl;
   int p=0,q=0;
  
   for(long i=global_row_start;i<global_row_end;i++)
   {
       q=0;
     for(long j=global_col_start;j<global_col_end;j++)
     {
       curr_state[p][q]=genH0(i,j,N);
     //  cout<<curr_state[p][q]<<" ";
       q++;
     }
     p++;
    //cout<<endl;
   }
   /// Data acquired suceesfully.......

    double* send_right_data=new double[n];
    double* send_left_data=new double[n];
    double* send_top_data=new double[n];
    double* send_down_data=new double[n];
    double* recv_right_data=new double[n];
    double* recv_left_data=new double[n];
    double* recv_top_data=new double[n];
    double* recv_down_data=new double[n];

   for(long it=0;it<iter;it++)
  {
    //cout<<"In iteration: "<<it<<endl;
    // Firstly i would like to perform column_wise split... inorder to acquire left_data and right_data for the required process....
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    //MPI_Comm_size(MPI_COMM_WORLD, &world_size);
   // int col_color=world_rank%(int)sqrt(world_size);
    int row_color = world_rank /(int)sqrt(world_size); // Determine color based on row
    int row_rank,row_size;
    MPI_Comm rowcomm;
    MPI_Comm_split(MPI_COMM_WORLD, row_color, world_rank, &rowcomm);
    MPI_Comm_rank(rowcomm,&row_rank);
    MPI_Comm_size(rowcomm,&row_size);
    // Completed column wise splitting....
   // cout<<"My_rank in column: "<<row_rank<<" in "<<row_size<<" MY world rank and worldsize:  "<<world_rank<<"   "<<world_size<<endl;
    if(row_rank==0)
    {
      /// I am the first guy in the row where i have two things to do :
      // 1. give my last column data to right processor.
      // 2. collect the first column from the rightprocessor...
      MPI_Request request[2];
      MPI_Status status[2];
     // send_right_data=new double[n];
     // recv_right_data=new double[n];
      for(long i=0;i<n;i++)
      {
         send_right_data[i]=curr_state[i][n-1];
         //recv_right_data[i]=0;
      }
     /// Once i am done with copying the data i need to send to right processor then go ahead start communication..
      
      MPI_Isend(send_right_data,n,MPI_DOUBLE,row_rank+1,0,rowcomm,&request[0]);
      MPI_Irecv(recv_right_data,n,MPI_DOUBLE,row_rank+1,0,rowcomm,&request[1]);
      MPI_Waitall(2,request,status);
     }
     else if(row_rank==row_size-1)
     {
      /// I am the last guy in the row.... and i have two things to do...
      // 1. give my first_column of the data to left processor and collect the right column of the left processor...
       // 2. collect the first column from the rightprocessor...
      MPI_Request request[2];
      MPI_Status status[2];
     // send_left_data=new double[n];
     // recv_left_data=new double[n];
      for(long i=0;i<n;i++)
      {
         send_left_data[i]=curr_state[i][0];
        // recv_left_data[i]=0;
      }
     /// Once i am done with copying the data i need to send to right processor then go ahead start communication..
      
      MPI_Isend(send_left_data,n,MPI_DOUBLE,row_rank-1,0,rowcomm,&request[0]);
      MPI_Irecv(recv_left_data,n,MPI_DOUBLE,row_rank-1,0,rowcomm,&request[1]);
      MPI_Waitall(2,request,status);
     }
    else
    {
      /// This is the case where u observe when n>=4...
      /// So when you are in the middle processor then you need to recv from left and right at the same time you have send right and left...
      // First focus on the sendingss....
      MPI_Request request[4];
      MPI_Status status[4];
      //send_left_data=new double[n];
     // send_right_data=new double[n];
      for(long i=0;i<n;i++)
      {
       // copying the first column of the data....
        send_left_data[i]=curr_state[i][0];
        send_right_data[i]=curr_state[i][n-1];

       // recv_left_data[i]=0;
       // recv_right_data[i]=0;
      }
      //recv_left_data=new double[n];
      //recv_right_data=new double[n];
      /// Once i assigned all the data values ... go ahead and start communication....
      MPI_Isend(send_left_data,n,MPI_DOUBLE,row_rank-1,0,rowcomm,&request[0]);
      MPI_Isend(send_right_data,n,MPI_DOUBLE,row_rank+1,0,rowcomm,&request[1]);
      MPI_Irecv(recv_left_data,n,MPI_DOUBLE,row_rank-1,0,rowcomm,&request[2]);
      MPI_Irecv(recv_right_data,n,MPI_DOUBLE,row_rank+1,0,rowcomm,&request[3]);
      MPI_Waitall(4,request,status);
     }
    /// I am done with row communication ... i have to do column communication now... before that fill as much next state as you can before going to column communication....
    //cout<<"The value of n is :"<<n<<endl;
     if(row_rank==0)
    {
       // cout<<"My rank is:  "<<my_rank<<endl;
       /// when you are the first guy in the column then you get the first row from the down processor.. so you have recv_down_data..
       /* for(int i=0;i<n;i++)
        {
          cout<<recv_right_data[i]<<" ";
        }
       cout<<endl;*/
       /// Start filling the next state matrix from here on..... (1<=row<=n-2) (we donot fill row 0 and row n-1)....
       for(int i=1;i<=n-2;i++)
       {
           for(int j=0;j<n;j++)
           {
             // So if the column is equal to n-1 you need to add the value with the help of recv_right_data...
              if(j==n-1)
              {
                   // if you are the last row...
                   //next_state[i][j]=curr_state[i][j]+curr_state[i-1][j]+curr_state[i+1][j]+curr_state[i][j-1]+curr_state[i][j+1]; 
                    next_state[i][j]=(curr_state[i][j]+curr_state[i-1][j]+curr_state[i+1][j]+curr_state[i][j-1]+recv_right_data[i])*0.2;
                   // cout<<next_state[i][j]<<" ";
              }
             else
              {
                 if(j==0)
                 {
                     // you should not access j-1  here otherwise seg-fault will occur....(we have to approximate i,j-1 with i,j value...
                     next_state[i][j]=(curr_state[i][j]+curr_state[i-1][j]+curr_state[i+1][j]+curr_state[i][j]+curr_state[i][j+1])*0.2;
                   //  cout<<next_state[i][j]<<" ";
                 }
                 else
                 {
                   /// The equation persists i mean we can use the same formula..
                    next_state[i][j]=(curr_state[i][j]+curr_state[i-1][j]+curr_state[i+1][j]+curr_state[i][j-1]+curr_state[i][j+1])*0.2;
                  //  cout<<next_state[i][j]<<" ";
                 }
              }
           }
        //  cout<<endl;
       }
      // By here you will fill up all the middle rows completly except the first row and last_row we will deal with this at the end.....
      
    }
   else if(row_rank==row_size-1)
   {
       //cout<<"My rank is : "<<my_rank<<endl;
      /// when you are the last guy in the column then you will be having the recv_up_data...
    /*  for(int i=0;i<n;i++)
      {
        cout<<recv_left_data[i]<<" ";
      }
      cout<<endl;*/
     /// here we have to be careful when j==0 we need to use recv_left_data ...
     for(int i=1;i<=n-2;i++)
       {
           for(int j=0;j<n;j++)
           {
             // So if the column is equal to n-1 you need to add the value with the help of recv_right_data...
              if(j==0)
              {
                   // if you are the last row...
                   //next_state[i][j]=curr_state[i][j]+curr_state[i-1][j]+curr_state[i+1][j]+curr_state[i][j-1]+curr_state[i][j+1]; 
                    //next_state[i][j]=(curr_state[i][j]+curr_state[i-1][j]+curr_state[i+1][j]+curr_state[i][j-1]+recv_right_data[i])*0.2;
                    next_state[i][j]=(curr_state[i][j]+curr_state[i+1][j]+curr_state[i-1][j]+recv_left_data[i]+curr_state[i][j+1])*0.2;
                   // cout<<next_state[i][j]<<" ";
              }
             else
              {
                /// The equation persists i mean we can use the same formula..
                  if(j==n-1)
                  {
                       /// Here we should be much more careful with j value...(because j+1>n its a boundry value approximate with same value....
                        next_state[i][j]=(curr_state[i][j]+curr_state[i-1][j]+curr_state[i+1][j]+curr_state[i][j-1]+curr_state[i][j])*0.2;
                      //  cout<<next_state[i][j]<<" ";
                  }
                  else
                  {
                     next_state[i][j]=(curr_state[i][j]+curr_state[i-1][j]+curr_state[i+1][j]+curr_state[i][j-1]+curr_state[i][j+1])*0.2;
                    //cout<<next_state[i][j]<<" ";
                  }
              }
           }
          //cout<<endl;
       }
        /// I will be completing the filling of middle rows here...
   
   }
   else if(row_rank!=0 || row_rank!=n-1)
   {
       // cout<<"My rank is : "<<my_rank<<endl;
     /* cout<<"My rank is : "<<my_rank<<endl;
      // When you are the middle guy then you will have the data recv_up_data and recv_down_data...
      for(int i=0;i<n;i++)
      {
        cout<<recv_left_data[i]<<" ";
      }
      cout<<endl;
      for(int i=0;i<n;i++)
      {
       cout<<recv_right_data[i]<<" ";
      }
      cout<<endl; */
     /// This is a combination of both the above cases... need to be dealt carefully...
       // when j==0 read value from recv_left_data and when j==n-1 read_value from the recv_right_data...
       for(int i=1;i<=n-2;i++)
       {
          for(int j=0;j<n;j++)
          {
             if(j==0)
             {
                /// When j is zero you are the first column so you will be receiving the data from the recv_left_data..
                next_state[i][j]=(curr_state[i][j]+recv_left_data[i]+curr_state[i][j+1]+curr_state[i-1][j]+curr_state[i+1][j])*0.2;
              //  cout<<next_state[i][j]<<" ";
             }
            else if(j==n-1)
             {
                /// when j is n-1 then you have get i,j+1 with the help of recv_right_data...
               next_state[i][j]=(curr_state[i][j]+curr_state[i-1][j]+curr_state[i+1][j]+recv_right_data[i]+curr_state[i][j-1])*0.2;
              // cout<<next_state[i][j]<<" ";
             }
            else
            {
                /// you come here when you are on the middle and you do not need any additional parameters you can directly have in your memory...
                next_state[i][j]=(curr_state[i][j]+curr_state[i-1][j]+curr_state[i+1][j]+curr_state[i][j-1]+curr_state[i][j+1])*0.2;
              //  cout<<next_state[i][j]<<" ";
            }
          }
         //cout<<endl;
       /// Approximate all the formulas for all the cases to get best results....
      }
   }

      //I am done with filling couple of results in the next_state matrix and i would like to goahead and fill in the first row and last row of every matrix..

      int col_color=world_rank%(int)sqrt(world_size);
      MPI_Comm colcomm;
      MPI_Comm_split(MPI_COMM_WORLD, col_color, world_rank, &colcomm);
      int col_rank,col_size;
      MPI_Comm_rank(colcomm,&col_rank);
      MPI_Comm_size(colcomm,&col_size);
      /// I want to split the column wise communication in the same way as i did for rowwise communication...
      /// Perform small checks whether your communication is correct or not...
      if(col_rank==0)
      {
        /// THis is the first color in whole column... when i am the guy in first column i want to send the down_data and recv the down data..
         MPI_Request request[2];
         MPI_Status status[2];
        // send_right_data=new double[n];
        // recv_right_data=new double[n];
         for(long i=0;i<n;i++)
         {
          send_down_data[i]=curr_state[n-1][i];
         //recv_right_data[i]=0;
         }
     /// Once i am done with copying the data i need to send to right processor then go ahead start communication..
      
        MPI_Isend(send_down_data,n,MPI_DOUBLE,col_rank+1,0,colcomm,&request[0]);
        MPI_Irecv(recv_down_data,n,MPI_DOUBLE,col_rank+1,0,colcomm,&request[1]);
        MPI_Waitall(2,request,status);
         
      }
      else if(col_rank==col_size-1)
      {
         /// THis is the first color in whole column... when i am the guy in first column i want to send the down_data and recv the down data..
         MPI_Request request[2];
         MPI_Status status[2];
        // send_right_data=new double[n];
        // recv_right_data=new double[n];
        for(long i=0;i<n;i++)
        {
         send_top_data[i]=curr_state[0][i];
         //recv_right_data[i]=0;
        }
     /// Once i am done with copying the data i need to send to right processor then go ahead start communication..
      
        MPI_Isend(send_top_data,n,MPI_DOUBLE,col_rank-1,0,colcomm,&request[0]);
        MPI_Irecv(recv_top_data,n,MPI_DOUBLE,col_rank-1,0,colcomm,&request[1]);
        MPI_Waitall(2,request,status);
        
      }
      else
       {
          
           MPI_Request request[4];
           MPI_Status status[4];
          //send_left_data=new double[n];
          // send_right_data=new double[n];
          for(long i=0;i<n;i++)
          {
        // copying the first column of the data....
            send_top_data[i]=curr_state[0][i];
            send_down_data[i]=curr_state[n-1][i];

       // recv_left_data[i]=0;
       // recv_right_data[i]=0;
          }
      //recv_left_data=new double[n];
      //recv_right_data=new double[n];
      /// Once i assigned all the data values ... go ahead and start communication....
        MPI_Isend(send_top_data,n,MPI_DOUBLE,col_rank-1,0,colcomm,&request[0]);
        MPI_Isend(send_down_data,n,MPI_DOUBLE,col_rank+1,0,colcomm,&request[1]);
        MPI_Irecv(recv_top_data,n,MPI_DOUBLE,col_rank-1,0,colcomm,&request[2]);
        MPI_Irecv(recv_down_data,n,MPI_DOUBLE,col_rank+1,0,colcomm,&request[3]);
        MPI_Waitall(4,request,status);
       }
   // So i am done with my column communication.....
   /// Check every thing by printing all the elements....
  ///*************************************************************////
   if(col_rank==0)
   {
    /* cout<<"My rank is :"<<my_rank<<"  "<<col_rank<<endl;
      for(int i=0;i<n;i++)
      {
        cout<<recv_down_data[i]<<" ";
      }
    cout<<endl;*/
     /// Here we have to cover all the three cases...
      if(row_rank==0)
      {
        /// calculationg the first row....
        for(int i=0;i<n;i++)
        {
           if(i==0)
           {
            int curr_value1=curr_state[0][i]+curr_state[0][i]+curr_state[0][i];
            int curr_value2=curr_state[n-1][i]+curr_state[n-1][i];
            if(i+1<n)
            {
             curr_value1+=curr_state[0][i+1];
             curr_value2+=curr_state[n-1][i+1];
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              curr_value2+=recv_down_data[i];
            }
           next_state[0][i]=curr_value1*0.2;
           next_state[n-1][i]=curr_value2*0.2;
          }
         else if(i==n-1)
         {
          int count=0;
           int curr_value1=curr_state[0][i]+recv_right_data[0]+curr_state[0][i-1]+curr_state[0][i];
           int curr_value2=curr_state[n-1][i]+recv_right_data[n-1]+curr_state[n-1][i-1]+recv_down_data[i];
           
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            
           next_state[0][i]=(curr_value1+(1-count)*(curr_state[0][i]))*0.2;
           next_state[n-1][i]=curr_value2*0.2;
         }
        else
        {
          // This can be computed trivially,,
          int count=0;
          int curr_value1=curr_state[0][i]+curr_state[0][i]+curr_state[0][i-1]+curr_state[0][i+1];
          int curr_value2=curr_state[n-1][i]+recv_down_data[i]+curr_state[n-1][i-1]+curr_state[n-1][i+1];
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
           next_state[n-1][i]=curr_value2*0.2;
           next_state[0][i]=(curr_value1+(1-count)*(curr_state[0][i]))*0.2;
          
        }

     }
   }

    else if(row_rank==n-1)
    {
        // it means you are the last row with col_rank is 0 here we have no top data but we have the recv_left and u have to use it when computing first column..
      // computing first row and last row...
        for(int i=0;i<n;i++)
        {
           if(i==0)
           {
             int curr_value1=curr_state[0][i]+recv_left_data[0]+curr_state[0][i];
             int curr_value2=curr_state[n-1][i]+recv_left_data[n-1];
             if(i+1<n)
            {
             curr_value1+=curr_state[0][i+1];
             curr_value2+=curr_state[n-1][i+1];
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              curr_value2+=recv_down_data[i];
            }
           next_state[0][i]=curr_value1*0.2;
           next_state[n-1][i]=curr_value2*0.2;
          }
           else if(i==n-1)
           {
            int count=0;
            int curr_value1=curr_state[0][i]+curr_state[0][i]+curr_state[0][i-1]+curr_state[0][i];
            int curr_value2=curr_state[n-1][i]+recv_down_data[i]+curr_state[n-1][i-1]+curr_state[n-1][i];
           
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            
            next_state[0][i]=(curr_value1+(1-count)*(curr_state[0][i]))*0.2;
            next_state[n-1][i]=curr_value2*0.2;
           }
           else
           {
             int count=0;
             int curr_value1=curr_state[0][i]+curr_state[0][i]+curr_state[0][i-1]+curr_state[0][i+1];
             int curr_value2=curr_state[n-1][i]+recv_down_data[i]+curr_state[n-1][i-1]+curr_state[n-1][i+1];
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
             next_state[n-1][i]=curr_value2*0.2;
             next_state[0][i]=(curr_value1+(1-count)*(curr_state[0][i]))*0.2;
           }
        }
     }
    else
    {
      // Thesea re the processors in the middle...
       // for this processors you have to use both recv_left and recv_right...
      for(int i=0;i<n;i++)
        {
           if(i==0)
           {
            int curr_value1=curr_state[0][i]+curr_state[0][i]+recv_left_data[0];
            int curr_value2=curr_state[n-1][i]+recv_left_data[n-1];
            if(i+1<n)
            {
             curr_value1+=curr_state[0][i+1];
             curr_value2+=curr_state[n-1][i+1];
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              curr_value2+=recv_down_data[i];
            }
           next_state[0][i]=curr_value1*0.2;
           next_state[n-1][i]=curr_value2*0.2;
          }
         else if(i==n-1)
         {
          int count=0;
           int curr_value1=curr_state[0][i]+recv_right_data[0]+curr_state[0][i-1]+curr_state[0][i];
           int curr_value2=curr_state[n-1][i]+recv_right_data[n-1]+curr_state[n-1][i-1]+recv_down_data[i];
           
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            
           next_state[0][i]=(curr_value1+(1-count)*(curr_state[0][i]))*0.2;
           next_state[n-1][i]=curr_value2*0.2;
         }
        else
        {
          // This can be computed trivially,,
          int count=0;
          int curr_value1=curr_state[0][i]+curr_state[0][i]+curr_state[0][i-1]+curr_state[0][i+1];
          int curr_value2=curr_state[n-1][i]+recv_down_data[i]+curr_state[n-1][i-1]+curr_state[n-1][i+1];
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
           next_state[n-1][i]=curr_value2*0.2;
           next_state[0][i]=(curr_value1+(1-count)*(curr_state[0][i]))*0.2;
          
        }

     }
    }
  }

  //****************** one if condition got completed**********************///
  // end of if condition...

  else if(col_rank==col_size-1)
  {
   /* cout<<"My rank is :"<<my_rank<<"  "<<col_rank<<endl;
    for(int i=0;i<n;i++)
    {
     cout<<recv_top_data[i]<<" ";
    }
    cout<<endl;*/
    if(row_rank==0)
    {
      for(int i=0;i<n;i++)
        {
           if(i==0)
           {
            int curr_value1=curr_state[0][i]+recv_top_data[i]+curr_state[0][i];
            int curr_value2=curr_state[n-1][i]+curr_state[n-1][i]+curr_state[n-1][i];
            if(i+1<n)
            {
             curr_value1+=curr_state[0][i+1];
             curr_value2+=curr_state[n-1][i+1];
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
             // curr_value2+=recv_down_data[i];
            }
           next_state[0][i]=curr_value1*0.2;
           next_state[n-1][i]=curr_value2*0.2;
          }
         else if(i==n-1)
         {
           int count=0;
           int curr_value1=curr_state[0][i]+recv_right_data[0]+curr_state[0][i-1]+recv_top_data[i];
           int curr_value2=curr_state[n-1][i]+recv_right_data[n-1]+curr_state[n-1][i-1]+curr_state[n-1][i];
           
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            
           next_state[0][i]=(curr_value1)*0.2;
           next_state[n-1][i]=curr_value2*0.2;
         }
       
        else
        {
          // This can be computed trivially,,
          int count=0;
          int curr_value1=curr_state[0][i]+recv_top_data[i]+curr_state[0][i-1]+curr_state[0][i+1];
          int curr_value2=curr_state[n-1][i]+curr_state[n-1][i]+curr_state[n-1][i-1]+curr_state[n-1][i+1];
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
           next_state[n-1][i]=curr_value2*0.2;
           next_state[0][i]=(curr_value1+(1-count)*(curr_state[0][i]))*0.2;
        }

      }
    }
    

     else if(row_rank==n-1)
     {
        for(int i=0;i<n;i++)
        {
           if(i==0)
           {
            int curr_value1=curr_state[0][i]+recv_top_data[i]+recv_left_data[0];
            int curr_value2=curr_state[n-1][i]+curr_state[n-1][i]+recv_left_data[n-1];
            if(i+1<n)
            {
             curr_value1+=curr_state[0][i+1];
             curr_value2+=curr_state[n-1][i+1];
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
             // curr_value2+=recv_down_data[i];
            }
           next_state[0][i]=curr_value1*0.2;
           next_state[n-1][i]=curr_value2*0.2;
          }
         else if(i==n-1)
         {
           int count=0;
           int curr_value1=curr_state[0][i]+curr_state[0][i]+curr_state[0][i-1]+recv_top_data[i];
           int curr_value2=curr_state[n-1][i]+curr_state[n-1][i]+curr_state[n-1][i-1]+curr_state[n-1][i];
           
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            
           next_state[0][i]=(curr_value1)*0.2;
           next_state[n-1][i]=curr_value2*0.2;
         }
        else
        {
          // This can be computed trivially,,
          int count=0;
          int curr_value1=curr_state[0][i]+recv_top_data[i]+curr_state[0][i-1]+curr_state[0][i+1];
          int curr_value2=curr_state[n-1][i]+curr_state[n-1][i]+curr_state[n-1][i-1]+curr_state[n-1][i+1];
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
           next_state[n-1][i]=curr_value2*0.2;
           next_state[0][i]=(curr_value1+(1-count)*(curr_state[0][i]))*0.2;
        }

     }
   }
    else
    {
      
      for(int i=0;i<n;i++)
        {
           if(i==0)
           {
            int curr_value1=curr_state[0][i]+recv_top_data[i]+recv_left_data[0];
            int curr_value2=curr_state[n-1][i]+curr_state[n-1][i]+recv_left_data[n-1];
            if(i+1<n)
            {
             curr_value1+=curr_state[0][i+1];
             curr_value2+=curr_state[n-1][i+1];
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
             // curr_value2+=recv_down_data[i];
            }
           next_state[0][i]=curr_value1*0.2;
           next_state[n-1][i]=curr_value2*0.2;
          }
         else if(i==n-1)
         {
           int count=0;
           int curr_value1=curr_state[0][i]+recv_right_data[0]+curr_state[0][i-1]+recv_top_data[i];
           int curr_value2=curr_state[n-1][i]+curr_state[n-1][i]+curr_state[n-1][i-1]+recv_right_data[n-1];
           
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            
           next_state[0][i]=(curr_value1)*0.2;
           next_state[n-1][i]=curr_value2*0.2;
         }
        else
        {
          // This can be computed trivially,,
          int count=0;
          int curr_value1=curr_state[0][i]+recv_top_data[i]+curr_state[0][i-1]+curr_state[0][i+1];
          int curr_value2=curr_state[n-1][i]+curr_state[n-1][i]+curr_state[n-1][i-1]+curr_state[n-1][i+1];
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
           next_state[n-1][i]=curr_value2*0.2;
           next_state[0][i]=(curr_value1+(1-count)*(curr_state[0][i]))*0.2;
        }
     }
      
    }  

  }// end of else if

 // finally for else condition which means for all column values that are not in the first and end (0 & n-1).....
 else
 {

    if(row_rank==0)
    {
      // here you how top for first rows and down for last rows...
      for(int i=0;i<n;i++)
      {
           if(i==0)
           {
            int curr_value1=curr_state[0][i]+recv_top_data[i]+curr_state[0][i];
            int curr_value2=curr_state[n-1][i]+curr_state[n-1][i];
            if(i+1<n)
            {
             curr_value1+=curr_state[0][i+1];
             curr_value2+=curr_state[n-1][i+1];
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              curr_value2+=recv_down_data[i];
            }
           next_state[0][i]=curr_value1*0.2;
           next_state[n-1][i]=curr_value2*0.2;
          }
         else if(i==n-1)
         {
           int count=0;
           int curr_value1=curr_state[0][i]+recv_right_data[0]+curr_state[0][i-1]+recv_top_data[i];
           int curr_value2=curr_state[n-1][i]+recv_right_data[n-1]+curr_state[n-1][i-1]+recv_down_data[i];
           
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            
           next_state[0][i]=(curr_value1)*0.2;
           next_state[n-1][i]=curr_value2*0.2;
         }
       
        else
        {
          // This can be computed trivially,,
          int count=0;
          int curr_value1=curr_state[0][i]+recv_top_data[i]+curr_state[0][i-1]+curr_state[0][i+1];
          int curr_value2=curr_state[n-1][i]+recv_down_data[i]+curr_state[n-1][i-1]+curr_state[n-1][i+1];
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
           next_state[n-1][i]=curr_value2*0.2;
           next_state[0][i]=(curr_value1+(1-count)*(curr_state[0][i]))*0.2;
        }

      }
    }///final   else got closed
  
   else if(row_rank==n-1)
   {
      for(int i=0;i<n;i++)
        {
           if(i==0)
           {
            int curr_value1=curr_state[0][i]+recv_top_data[i]+recv_left_data[0];
            int curr_value2=curr_state[n-1][i]+recv_left_data[n-1];
            if(i+1<n)
            {
             curr_value1+=curr_state[0][i+1];
             curr_value2+=curr_state[n-1][i+1];
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              curr_value2+=recv_down_data[i];
            }
           next_state[0][i]=curr_value1*0.2;
           next_state[n-1][i]=curr_value2*0.2;
          }
         else if(i==n-1)
         {
           int count=0;
           int curr_value1=curr_state[0][i]+curr_state[0][i]+curr_state[0][i-1]+recv_top_data[i];
           int curr_value2=curr_state[n-1][i]+curr_state[n-1][i]+curr_state[n-1][i-1]+recv_down_data[i];
           
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            
           next_state[0][i]=(curr_value1)*0.2;
           next_state[n-1][i]=curr_value2*0.2;
         }
        else
        {
          // This can be computed trivially,,
          int count=0;
          int curr_value1=curr_state[0][i]+recv_top_data[i]+curr_state[0][i-1]+curr_state[0][i+1];
          int curr_value2=curr_state[n-1][i]+recv_down_data[i]+curr_state[n-1][i-1]+curr_state[n-1][i+1];
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
           next_state[n-1][i]=curr_value2*0.2;
           next_state[0][i]=(curr_value1+(1-count)*(curr_state[0][i]))*0.2;
        }

     }

   }

  else
   {
    

      /// You fall here when col_rank is not zero and not n-1....
       if(row_rank==0)
        {
           for(int i=0;i<n;i++)
           {
           if(i==0)
           {
            int curr_value1=curr_state[0][i]+recv_top_data[i]+curr_state[0][i];
            int curr_value2=curr_state[n-1][i]+curr_state[n-1][i];
            if(i+1<n)
            {
             curr_value1+=curr_state[0][i+1];
             curr_value2+=curr_state[n-1][i+1];
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              curr_value2+=recv_down_data[i];
            }
           next_state[0][i]=curr_value1*0.2;
           next_state[n-1][i]=curr_value2*0.2;
          }
         else if(i==n-1)
         {
           int count=0;
           int curr_value1=curr_state[0][i]+recv_right_data[0]+curr_state[0][i-1]+recv_top_data[i];
           int curr_value2=curr_state[n-1][i]+recv_right_data[n-1]+curr_state[n-1][i-1]+recv_down_data[i];
           
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            
           next_state[0][i]=(curr_value1)*0.2;
           next_state[n-1][i]=curr_value2*0.2;
         }
       
        else
        {
          // This can be computed trivially,,
          int count=0;
          int curr_value1=curr_state[0][i]+recv_top_data[i]+curr_state[0][i-1]+curr_state[0][i+1];
          int curr_value2=curr_state[n-1][i]+recv_down_data[i]+curr_state[n-1][i-1]+curr_state[n-1][i+1];
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
           next_state[n-1][i]=curr_value2*0.2;
           next_state[0][i]=(curr_value1+(1-count)*(curr_state[0][i]))*0.2;
         }

       }
     }
     else if(row_rank==n-1)
     {
        for(int i=0;i<n;i++)
        {
           if(i==0)
           {
            int curr_value1=curr_state[0][i]+recv_top_data[i]+recv_left_data[0];
            int curr_value2=curr_state[n-1][i]+recv_left_data[n-1];
            if(i+1<n)
            {
             curr_value1+=curr_state[0][i+1];
             curr_value2+=curr_state[n-1][i+1];
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              curr_value2+=recv_down_data[i];
            }
           next_state[0][i]=curr_value1*0.2;
           next_state[n-1][i]=curr_value2*0.2;
          }
         else if(i==n-1)
         {
            int count=0;
           int curr_value1=curr_state[0][i]+curr_state[0][i]+curr_state[0][i-1]+recv_top_data[i];
           int curr_value2=curr_state[n-1][i]+curr_state[n-1][i]+curr_state[n-1][i-1]+recv_down_data[i];
           
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            
           next_state[0][i]=(curr_value1)*0.2;
           next_state[n-1][i]=curr_value2*0.2;
         }
       else
        {
          int count=0;
          int curr_value1=curr_state[0][i]+recv_top_data[i]+curr_state[0][i-1]+curr_state[0][i+1];
          int curr_value2=curr_state[n-1][i]+recv_down_data[i]+curr_state[n-1][i-1]+curr_state[n-1][i+1];
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
           next_state[n-1][i]=curr_value2*0.2;
           next_state[0][i]=(curr_value1+(1-count)*(curr_state[0][i]))*0.2;
        }
   
      }
    }
   else
    {
      for(int i=0;i<n;i++)
        {
           if(i==0)
           {
            int curr_value1=curr_state[0][i]+recv_top_data[i]+recv_left_data[0];
            int curr_value2=curr_state[n-1][i]+recv_left_data[n-1];
            if(i+1<n)
            {
             curr_value1+=curr_state[0][i+1];
             curr_value2+=curr_state[n-1][i+1];
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              curr_value2+=recv_down_data[i];
            }
           next_state[0][i]=curr_value1*0.2;
           next_state[n-1][i]=curr_value2*0.2;
          }
         else if(i==n-1)
         {
            int count=0;
           int curr_value1=curr_state[0][i]+recv_right_data[0]+curr_state[0][i-1]+recv_top_data[i];
           int curr_value2=curr_state[n-1][i]+recv_right_data[n-1]+curr_state[n-1][i-1]+recv_down_data[i];
           
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            
           next_state[0][i]=(curr_value1)*0.2;
           next_state[n-1][i]=curr_value2*0.2;
         }
       else
        {
          int count=0;
          int curr_value1=curr_state[0][i]+recv_top_data[i]+curr_state[0][i-1]+curr_state[0][i+1];
          int curr_value2=curr_state[n-1][i]+recv_down_data[i]+curr_state[n-1][i-1]+curr_state[n-1][i+1];
            if(n>1)
            {
              curr_value1+=curr_state[1][i];
              count++;
            }
            if(n-2>=0)
            {
              curr_value2+=curr_state[n-2][i];
            }
           next_state[n-1][i]=curr_value2*0.2;
           next_state[0][i]=(curr_value1+(1-count)*(curr_state[0][i]))*0.2;
        }
   
      }
    }
   
  }
 
 } ///end of else

/* cout<<"I am row _rank :"<<row_rank<<" "<<my_rank<<" "<<it<<endl;
      for(int i=0;i<n;i++)
      {
       for(int j=0;j<n;j++)
       {
         cout<<next_state[i][j]<<" ";
       }
      cout<<endl;
    }
*/
 // we have to copy our next_state to final_state...
 for(int i=0;i<n;i++)
{
  for(int j=0;j<n;j++)
  {
    curr_state[i][j]=next_state[i][j];
    next_state[i][j]=0;
  }
}
 } // end of iteraive..

// Once you are done with the operations please delete all the memory you took on the heap...
for(int i=0;i<n;i++)
{
  delete curr_state[i];
  delete next_state[i];
}
delete[] curr_state;
delete[] next_state;
delete send_right_data;
delete send_left_data;
delete send_top_data;
delete send_down_data;
delete recv_right_data;
delete recv_left_data;
delete recv_top_data;
delete recv_down_data;

} //end of the function...

void all_process_work(long n,long iter)
{
     /// step 1:In this function i would like to call generate function at their own process to reduce the overhead...
     int my_rank,no_of_process;
     long answer_size;
     MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
     MPI_Comm_size(MPI_COMM_WORLD,&no_of_process);
     int total_no_process=no_of_process;
        /// Generate matrix and vector use matmul function inorder to compute the result store this in local memory of each process then we will take necessary actions
        /// I am sure that my (2-d matrix).... would be of size n/sqrt(p) *n /sqrt(p), and my vector would be of size n/sqrt(p)...
     int no_processors_in_each_column=(int)sqrt(no_of_process);
     long my_dimension=n/(int)sqrt(no_of_process);
    // float *grid =new float[my_dimension*my_dimension];
     //float *vector=new float[my_dimension];
    // float* small_ans=new float[my_dimension];
         /// I created all grid and vector and i should use gen function written by professor inorder to generate right values....
         /// Now you need to calculate the original 2-d dimensions of the matrix..
     long global_row_start=(my_rank/no_processors_in_each_column)*(my_dimension);
     long global_row_end=global_row_start+my_dimension;
     long global_col_start=(my_rank%no_processors_in_each_column)*(my_dimension);
     long global_col_end=global_col_start+my_dimension;
     // cout<<"All details: "<<global_row_start<<" "<<global_row_end<<" "<<global_col_start<<" "<<global_col_end<<endl;
     Heat_equation(global_row_start,global_row_end,global_col_start,global_col_end,n,iter);
}


int main(int argc, char* argv[]) {

  if (argc < 3) {
    std::cerr<<"usage: mpirun "<<argv[0]<<" <N> <K>"<<std::endl;
    return -1;
  }

  // declare and init command line params
  long N, K;
  N = atol(argv[1]);
  K = atol(argv[2]);
 std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  MPI_Init(&argc,&argv);
 // Heat_equation(N,K);
  int my_rank,total_size;
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&total_size);
  if(total_size==1)
  {

   /// Perform serial program...
  // cout<<"hi"<<endl;
  serial_heat(N,K);
  }
 else
{
  all_process_work(N,K);
}
 MPI_Finalize();
std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
std::chrono::duration<double> elapsed_seconds = end-start;
std::cerr<<elapsed_seconds.count()<<std::endl;
  return 0;
}
