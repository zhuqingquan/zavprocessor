#pragma once
#ifndef _Z_UTILS_CU_H_
#define _Z_UTILS_CU_H_

#include <inttypes.h>

namespace zUtils
{
    /**
     * @brief 将int8_t的数归一化到(-1.0f, 1.0]
     * @param input 需要归一化的数据。内存中的数据指针。
     * @param output 归一化之后的结果。内存中的指针。
     * @param size 输入的数据的个数。
     * @return true -- 成功  false--失败
     */
    bool normalize_signed_byte(const int8_t* input, float* output, int size);
}

#endif//_Z_UTILS_CU_H_