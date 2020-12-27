#include <thread>
#include "reduce.hpp"
#include "utils.hpp"
#include <iostream>
#include <iomanip>
#include <cuda.h>
#include <sys/time.h>

__global__ void reduce_block(float* g_idata, int reduce_size, float* g_odata)
{
    extern __shared__ float sdata[];
    unsigned int tid = threadIdx.x;
    unsigned int start = blockIdx.x * reduce_size;

    sdata[tid] = 0;
    for (unsigned int i = tid; i < reduce_size; i += blockDim.x)
    {
        sdata[tid] += g_idata[i + start];
    }
    __syncthreads();

    // do reduction in shared memory
    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1)
    {
        if (tid < s)
        {
            sdata[tid] += sdata[tid + s];
        }
        __syncthreads();
    }

    // write result for this block to global mem
    if (tid == 0) g_odata[blockIdx.x] = sdata[0];
}

void reduce_cuda(const std::vector<float>& vec, int reduce_size, std::vector<float>& vec_out)
{
    //std::cout << "vec_cuda = " << std::endl;
    //std::cout << vec << std::endl;
    int elem_num = vec.size();
    int block_num = elem_num / reduce_size;
    vec_out.resize(block_num);

    float *dev_idata, *dev_odata;
    cudaMalloc((void**)&dev_idata, elem_num * sizeof(float));
    cudaMalloc((void**)&dev_odata, block_num * sizeof(float));
    cudaMemcpy(dev_idata, vec.data(), elem_num * sizeof(float), cudaMemcpyHostToDevice);
  
    int block_size = 128;

    timeval t1, t2;
    gettimeofday(&t1, nullptr);
    reduce_block<<<block_num, block_size, block_size * sizeof(float)>>>(dev_idata, reduce_size, dev_odata);
    cudaDeviceSynchronize();
    gettimeofday(&t2, nullptr);

    float ms = (t2.tv_sec - t1.tv_sec) * 1000.0f + (t2.tv_usec - t1.tv_usec) / 1000.0f;
    std::cout << "Kernel takes " << ms << " ms" << std::endl;
    cudaMemcpy(vec_out.data(), dev_odata, block_num * sizeof(float), cudaMemcpyDeviceToHost);

    cudaFree(dev_idata);
    cudaFree(dev_odata);
}

