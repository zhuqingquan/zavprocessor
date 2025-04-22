/*
 * @Author: zhuqingquan zqq_222@163.com
 * @Date: 2025-03-26 
 * @FilePath: zavprocessor/test/zutilssimdUnitTest.cc
 * @Description: 对zutils_simd内实现的代码进行单元测试
 */
#include <gtest/gtest.h>
#include "zutils_simd.h"

TEST(simd_yuv2rgb, yuv422ToARGB)
{
    zutils::YuvConstants c __attribute__((aligned(32)));
    int w = 1920;
    int h = 1080;
    uint8_t* y = (uint8_t*)malloc(w);
    uint8_t* u = (uint8_t*)malloc(w / 2);
    uint8_t* v = (uint8_t*)malloc(w / 2);

    uint8_t* rgba = (uint8_t*)malloc(w * 4);
    int a = (uint64_t)y % 32;
    a = (uint64_t)u % 32;
    a = (uint64_t)v % 32;
    a = (uint64_t)rgba % 32;
    a = (uint64_t)(&c) % 32;
    zutils::I422ToARGBRow_AVX2(y, u, v, rgba, &c, w);
}

TEST(simd_mixAlpha, simd)
{
    uint32_t w = 256;
    uint32_t h = 256;
    int bpp = 3;        // for rgb

    uint32_t back_stride = w * bpp;
    uint32_t back_size = back_stride * h;
    uint8_t* back = (uint8_t*)malloc(back_size);
    memset(back, 0x55, back_size);

    uint32_t front_w = w;
    uint32_t front_h = h / 2;

    uint32_t front_stride = front_w * bpp;
    uint32_t front_size = front_stride * front_h;      // front为back的1/2大小
    uint8_t* front = (uint8_t*)malloc(front_size);
    memset(front, 0xEE, front_size);

    uint32_t mask_stride = front_w;
    uint32_t mask_size = mask_stride * front_h;
    uint8_t* mask = (uint8_t*)malloc(mask_size);
    memset(mask, 0, mask_size);
    uint8_t* pmline = mask;
    for(int i=0; i<front_h; i++)
    {
        memset(pmline, 0xFF, front_w / 2);
        pmline += mask_stride;
    }

    int x = 0;
    int y = 0;
    zutils::mixAlpha(back, back_stride, bpp, w, h, front, front_stride, mask, mask_stride, x, y, front_w, front_h);

    EXPECT_EQ(front[0], 0xEE);
    EXPECT_EQ(front[bpp * front_w / 2], 0x55);
}

TEST(simd_rgb2rgba, simd)
{
    uint32_t w = 256;
    uint32_t h = 256;
    int bpp = 3;        // for rgb

    uint32_t back_stride = w * bpp;
    uint32_t back_size = back_stride * h;
    uint8_t* back = (uint8_t*)malloc(back_size);
    for(int i=0; i<back_size; i++)
        back[i] = i % 32;

    uint32_t bgra_stride = w * 4;
    uint32_t bgra_size = bgra_stride * h;
    uint8_t* bgra = (uint8_t*)malloc(bgra_size);
    memset(bgra, 0, bgra_size);

    zutils::bgr_to_bgra_avx2(back, back_stride, bgra, bgra_stride, w, h);
    for(int j=0; j<h; j++)
    {
        uint8_t* line_rgb = back + j * back_stride;
        uint8_t* line_rgba = bgra + j * bgra_stride;
        for(int i=0; i<w; i++)
        {
            EXPECT_EQ(line_rgba[i * 4], line_rgb[i * 3]);
            EXPECT_EQ(line_rgba[i * 4 + 1], line_rgb[i * 3 + 1]);
            EXPECT_EQ(line_rgba[i * 4 + 2], line_rgb[i * 3 + 2]);
        }
    }

    //EXPECT_EQ(front[0], 0xEE);
    //EXPECT_EQ(front[bpp * front_w / 2], 0x55);
}