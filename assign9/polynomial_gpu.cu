#include <iostream>
#include <chrono>
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }

 inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
     if (code != cudaSuccess) 
   {
       // cudaGetErrorString is an api that will pop out an error to user screen else all errors in gpu are silent....
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}
// All gpu function start with __global__ called from cpu...
__global__ void polynomial_expansion (float* poly, int degree,
               int n, float* array) {
  //TODO: Write code to use the GPU here!
  //code should write the output back to array
   int curr_index = (blockIdx.x * blockDim.x )+ threadIdx.x;
    if( curr_index < n )
    {
      float out = 0.;
      float xtothepowerof = 1.;
      for (int i=0; i<=degree; ++i) {
        out += xtothepowerof*poly[i];
        xtothepowerof *=array[curr_index];
      }
      array[curr_index]=out;
    }
}

int main (int argc, char* argv[]) {
  //TODO: add usage
  
  if (argc < 3) {
     std::cerr<<"usage: "<<argv[0]<<" n degree"<<std::endl;
     return -1;
  }
  
  int n = atoi(argv[1]); //TODO: atoi is an unsafe function....
  int degree = atoi(argv[2]);
  int nbiter = 1;

  float* array = new float[n];
  float* poly = new float[degree+1];
  for (int i=0; i<n; ++i)
    array[i] = 1.;
  for (int i=0; i<degree+1; ++i)
    poly[i] = 1.;
  float* d_array;
  float* d_poly;
  /// Giving pointers for gpu..
  gpuErrchk(cudaMalloc(&d_poly, (degree+1)* sizeof(float)));
  gpuErrchk(cudaMalloc(&d_array, n * sizeof(float)));
  gpuErrchk(cudaMemcpy(d_array, array, (n) * sizeof(float),cudaMemcpyHostToDevice));
  gpuErrchk(cudaMemcpy(d_poly, poly, (degree+1)*sizeof(float),cudaMemcpyHostToDevice));
   std::chrono::time_point<std::chrono::system_clock> begin, end;
   begin = std::chrono::system_clock::now();
   int block_size=256;
   int no_of_blocks=(n+block_size-1)/block_size;
   polynomial_expansion<<<no_of_blocks,block_size>>>(d_poly, degree, n, d_array);
   /// Copying the final_array to input_array...
  gpuErrchk( cudaMemcpy( array, d_array, n * sizeof(float),cudaMemcpyDeviceToHost)); 
  {
    bool correct = true;
    int ind;
    for (int i=0; i< n; ++i) {
      if (fabs(array[i]-(degree+1))>0.01) {
        correct = false;
    ind = i;
      }
    }
    if (!correct)
      std::cerr<<"Result is incorrect. In particular array["<<ind<<"] should be "<<degree+1<<" not "<< array[ind]<<std::endl;
  }
   cudaFree(d_array);
   cudaFree(d_poly);
   cudaDeviceSynchronize();
   end = std::chrono::system_clock::now();
   std::chrono::duration<double> totaltime = (end-begin)/nbiter;
   std::cerr<<array[0]<<std::endl;
   std::cout<<n<<" "<<degree<<" "<<totaltime.count()<<std::endl;

   delete[] array;
   delete[] poly;

  return 0;
}