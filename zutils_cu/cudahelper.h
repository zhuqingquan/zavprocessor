/*
 * @Author: zhuqingquan zqq_222@163.com
 * @Date: 2024-03-08 
 * @FilePath: avcommon/cudahelper.h
 * @Description: 此处定义使用CUDA经常使用的一些方法
 */
#pragma once
#ifndef _Z_MEDIA_CUDA_HELPER_H_
#define _Z_MEDIA_CUDA_HELPER_H_

#include <cuda.h>
#include <memory>
#include <stdint.h>

namespace zMedia
{
    struct CUDAContext
    {
        CUcontext   ctx     = nullptr;
        CUdevice    dev     = -1;
        int         iGPU    = -1;           // GPU 序号
        int         tid     = -1;           // thread id attatched by this ctx
        uint32_t    flags   = 0;            // flags

        CUDAContext() {}

        CUDAContext(CUcontext _ctx, CUdevice _dev, int _iGPU, int _tid, uint32_t _flags) 
        : ctx(_ctx), dev(_dev), iGPU(_iGPU), tid(_tid), flags(_flags)
        {

        }
    };//struct CUDAContext

    /**
     * @brief 创建CUDA context。返回CUDAContext对象
     * @param iGPU GPU序号
     * @param flags 创建的flags
     * @return CUDAContext 
     */
    CUDAContext* createCUDACtx(int iGPU, int flags);

    void releaseCUDACtx(CUDAContext** ctx);
}//namespace zMedia

#endif//_Z_MEDIA_CUDA_HELPER_H_