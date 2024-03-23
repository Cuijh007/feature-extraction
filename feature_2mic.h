#ifndef FEATURE_2MIC_H
#define FEATURE_2MIC_H

#include <stdint.h>

#ifdef __cplusplus 
extern "C"
{ 
#endif  

#define FFT_COMPLEX_LEN  (257u)
#define WEIGHT_MATRIX_Q  (10u)
#define ANGLE_NUM        (5u) 
#define MIC_WEIGHT_SIZE  (ANGLE_NUM*FFT_COMPLEX_LEN*4)


typedef struct _stComplex_int32_t
{
    int32_t real;
    int32_t image;
} stComplex_int32_t;

/*
 * for 2mic data transfer 5 angles data output
 * @param, pLeftFFTVector: Input, left mic FFT, save mode: real0 image0 real1 image1 ... real_nFFTLen image_nFFTLen
 * @param, pRightFFTVector: Input, right mic FFT, save mode ditto
 * @param, nFFTLen: Input, FFT transfer len, dafault FFT_COMPLEX_LEN
 * @param, nQValue: Input, FFT QValue, outdata same this Q
 * @param, pDstVector: Output, 5 angles save mode: angle0_real0 angle0_image0 angle0_real1 angle0_image1 .... angle4_real0 angle4_image0 .... 
*/
void extract_2_mic_FFT_vector( 
                                const int32_t* pLeftFFTVector, 
                                const int32_t* pRightFFTVector, 
                                const int32_t nFFTLen, 
                                const int32_t nQValue,
                                int32_t* pDstVector );

void complex_element_multiply_int32t_2_int32_t(
                                                const stComplex_int32_t* pSrc1,
                                                const stComplex_int32_t* pSrc2, 
                                                const int32_t nLen, 
                                                const int32_t nQValue,
                                                stComplex_int32_t* pDst );

#ifdef __cplusplus 
} 
#endif  

#endif