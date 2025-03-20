#include <cuda_runtime.h>
#include <inttypes.h>

__global__ void nomalize_byte_to_float32(const char* input, float* output, int size)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(idx < size)
    {
        output[idx] = static_cast<float>(input[idx]) / 128.0f;
    }
}

namespace zUtils
{
    bool normalize_signed_byte(const int8_t* input, float* output, int size)
    {
        if(size <= 0 || input==nullptr || output==nullptr)
            return false;
        char* deviceInput = nullptr;
        float* deviceOutput = nullptr;
        cudaMalloc(&deviceInput, sizeof(char) * size);
        cudaMalloc(&deviceOutput, sizeof(float) * size);

        if(cudaSuccess!=cudaMemcpy(deviceInput, input, size, cudaMemcpyHostToDevice))
        {
            cudaFree(deviceInput);
            cudaFree(deviceOutput);
            return false;
        }

        int threadsPerBlock = 256;
        int blocksPerGrid = (size + threadsPerBlock -1) / threadsPerBlock;
        nomalize_byte_to_float32<<<blocksPerGrid, threadsPerBlock>>>(deviceInput, deviceOutput, size);
        if(cudaSuccess!=cudaMemcpy(output, deviceOutput, sizeof(float)*size, cudaMemcpyDeviceToHost))
        {
            cudaFree(deviceInput);
            cudaFree(deviceOutput);
            return false;
        }
        
        cudaFree(deviceInput);
        cudaFree(deviceOutput);
        return true;
    }
}