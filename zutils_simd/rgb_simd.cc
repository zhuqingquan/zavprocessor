/*
 * @Author: zhuqingquan zqq_222@163.com
 * @Date: 2025-04-11 
 * @FilePath: /zavprocessor/zutils_simd/rgb_simd.cc
 * @Description: 使用SIMD指令优化rgb、argb相关的操作
 */
#include <immintrin.h>
#include <stdint.h>
#include "zutils_simd.h"
template  <unsigned int N> __m256i _mm256_shift_left(__m256i a)
{
  __m256i mask = _mm256_permute2x128_si256(a, a, _MM_SHUFFLE(0,0,3,0) );
  return _mm256_alignr_epi8(a,mask,16-N);
}

void zutils::bgr_to_bgra_avx2(const uint8_t* bgr, int bgr_stride, uint8_t* bgra, int bgra_stride, int w, int h) {
    const __m256i alpha_mask = _mm256_set1_epi32(0xFF000000); // Alpha=0xFF
    const __m256i shuffle_mask = _mm256_setr_epi8(
        0, 1, 2, -1, 3, 4, 5, -1, 6, 7, 8, -1, 9, 10, 11, -1,
        0x80, 0x80, 0x80, -1, 0x80, 0, 1, -1, 2, 3, 4, -1, 5, 6, 7, -1 
        //12, 13, 14, -1, 15, 16, 17, -1, 18, 19, 20, -1, 21, 22, 23, -1
    );
    for (int j = 0; j < h; j++)
    {
        const uint8_t* line_bgr = bgr + bgr_stride * j;
        uint8_t* line_bgra = bgra + bgra_stride * j;
        int pos_bgr = 0;
        int pos_bgra = 0;
        for (; pos_bgr + 32 <= bgr_stride; )    // 因为每次_mm256_load都会从内存中读取32个字节
        {
            const uint8_t* cur_bgr = line_bgr + pos_bgr;
            uint8_t* cur_bgra = line_bgra + pos_bgra;
            // 加载 24 字节（8 个 BGR 像素）
            __m256i bgr_data = _mm256_loadu_si256((__m256i*)cur_bgr);
            //__m256i bgr_data_shift12 = _mm256_shift_left<12>(bgr_data);

            // 重组数据：BGR -> BGRA
            __m256i bgra_data = _mm256_shuffle_epi8(bgr_data, shuffle_mask);

            // 插入 Alpha=0xFF
            bgra_data = _mm256_or_si256(bgra_data, alpha_mask);

            // 存储 32 字节（8 个 BGRA 像素）
            _mm256_storeu_si256((__m256i*)(cur_bgra), bgra_data);
            cur_bgra[16] = cur_bgr[12];
            cur_bgra[17] = cur_bgr[13];
            cur_bgra[18] = cur_bgr[14];
            cur_bgra[20] = cur_bgr[15];
            //bgra[(i+4)*4] = bgr[(i+4)*3];
            //bgra[(i+4)*4 + 1] = bgr[(i+4)*3 + 1];
            //bgra[(i+4)*4 + 2] = bgr[(i+4)*3 + 2];
            //bgra[(i+4 +1)*4] = bgr[(i+4 +1)*3];
            pos_bgr += 8 * 3;
            pos_bgra += 8 * 4;
        }
    }
}