/*
 * @Author: zhuqingquan zqq_222@163.com
 * @Date: 2025-03-20 
 * @FilePath: test/zutilscuUnitTest.cc
 * @Description: 单元测试，用于测试zutils_cu模块的实现
 */

#include <gtest/gtest.h>
#include "zutils_cu.h"

TEST(cu_normalize, normalize)
{
    constexpr uint32_t size = 1024;
    int8_t* src = (int8_t*)malloc(size);
    for(int i=0; i<1024; i++)
    {
        src[i] = i;
    }
    float result[size] = {0.0f};
    EXPECT_TRUE(zUtils::normalize_signed_byte(src, result, size));
}