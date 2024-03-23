#include "feature_2mic.h"

#define ABS(value) (value&0x80000000)?(~value+1):value
#define MIN(x,y) (x)<(y)?(x):(y)

static int32_t _get_mult(int32_t a, int32_t b, int32_t Qa, int32_t Qb )
{
    int abs_a = ABS(a);
    int abs_b = ABS(b);

    int leading_a = __builtin_clz( abs_a ) - 1;
    int leading_b  = __builtin_clz( abs_b ) - 1;

    a <<= leading_a;
    b <<= leading_b;

    a >>= 16;
    b >>= 16;

    int shift_value = leading_a + leading_b + Qa + Qb - 32;
    int nR = a * b;
    if ( shift_value > Qa )
    {
        return nR >> ( shift_value - Qa );
    }
    else
    {
        return nR << ( Qa - shift_value );
    }
}

void complex_element_multiply_int32t_2_int32_t(
                                                const stComplex_int32_t* pSrc1,
                                                const stComplex_int32_t* pSrc2, 
                                                const int32_t nLen, 
                                                const int32_t nQValue,
                                                stComplex_int32_t* pDst )
{
    int32_t real_sum = 0;
    int32_t image_sum = 0;
    int i = 0;
    for ( i = 0; i < nLen; i++ )
    {	
        real_sum = _get_mult( pSrc1[i].real, pSrc2[i].real, nQValue, WEIGHT_MATRIX_Q ) - _get_mult(pSrc1[i].image, pSrc2[i].image, nQValue, WEIGHT_MATRIX_Q );
        image_sum = _get_mult(pSrc1[i].real, pSrc2[i].image, nQValue, WEIGHT_MATRIX_Q ) + _get_mult(pSrc1[i].image, pSrc2[i].real, nQValue, WEIGHT_MATRIX_Q );

        pDst[i].real =  real_sum;
        pDst[i].image =  image_sum;
    }

    return;
}


extern int32_t *p_mic_weight_matrix;
void extract_2_mic_FFT_vector( const int32_t* pLeftFFTVector, const int32_t* pRightFFTVector, const int32_t nFFTLen, const int32_t nQValue, int32_t* pDstVector )
{
    const stComplex_int32_t* pComplexRight = (const stComplex_int32_t*)pRightFFTVector;
    const stComplex_int32_t* pComplexLeft = (const stComplex_int32_t*)pLeftFFTVector;


    stComplex_int32_t* pDst = (stComplex_int32_t*)pDstVector;

    stComplex_int32_t pLeftTemp[FFT_COMPLEX_LEN];
    stComplex_int32_t pRightTemp[FFT_COMPLEX_LEN];

    for ( int i = 0; i < ANGLE_NUM; i++ )
    {
        const stComplex_int32_t* pLeftWeight = (const stComplex_int32_t*)(p_mic_weight_matrix + i * 4 * FFT_COMPLEX_LEN);
        const stComplex_int32_t* pRightWeight = pLeftWeight + FFT_COMPLEX_LEN;
        stComplex_int32_t* pDstTemp = pDst + i * FFT_COMPLEX_LEN;

        complex_element_multiply_int32t_2_int32_t( pLeftWeight, pComplexLeft, FFT_COMPLEX_LEN, nQValue,  pLeftTemp );

        complex_element_multiply_int32t_2_int32_t( pRightWeight, pComplexRight, FFT_COMPLEX_LEN, nQValue,  pRightTemp );

        for ( int j = 0; j < FFT_COMPLEX_LEN; j++ )
        {
            pDstTemp[j].real = pLeftTemp[j].real + pRightTemp[j].real;
            pDstTemp[j].image = pLeftTemp[j].image + pRightTemp[j].image;
        }
    } 
}
