#include "cudahelper.h"

using namespace zMedia;


CUDAContext* zMedia::createCUDACtx(int iGPU, int flags)
{
    CUdevice cuDevice = 0;
    CUresult ret = cuDeviceGet(&cuDevice, iGPU);
    if(ret!=CUDA_SUCCESS)
    {
        return nullptr;
    }
    char szDeviceName[80];
    ret = cuDeviceGetName(szDeviceName, sizeof(szDeviceName), cuDevice);
    CUcontext cuctx;
    ret = cuCtxCreate(&cuctx, flags, cuDevice);
    if(ret!=CUDA_SUCCESS)
    {
        return nullptr;
    }
    return new CUDAContext(cuctx, cuDevice, iGPU, -1, flags);
}