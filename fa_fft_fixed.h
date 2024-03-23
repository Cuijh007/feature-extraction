#ifndef _FA_FFT_FIXED_H
#define _FA_FFT_FIXED_H

#include <math.h>
#include <stdint.h>

#ifdef __cplusplus 
extern "C"
{ 
#endif  




#ifndef _FA_INTTYPES_H

#ifndef INT32_MAX
#define INT32_MAX 2147483647
#endif
#ifndef INT32_MIN
#define INT32_MIN (-2147483647 - 1)
#endif

#ifdef __GNUC__ 
#define int64_t long long
#else
#define int64_t __int64 
#endif

typedef void* puintptr_t;

#endif

#ifndef _FA_MATHOPS_H
static inline float fa_roundf(float x)
{
    return (x > 0) ? floor(x + 0.5) : ceil(x - 0.5);
}

static inline int fa_rintf(float x)
{
    float xx;

    xx = fa_roundf(x);

    if     (xx > INT32_MAX) return INT32_MAX;
    else if(xx < INT32_MIN) return INT32_MIN;
    else                    return (int)xx;
}

static inline int fa_clip_int(int a, int amin, int amax)
{
    if      (a < amin) return amin;
    else if (a > amax) return amax;
    else               return a;
}

#define FA_SCALE_FLOAT(a, bits)     fa_rintf((a) * (float)(1 << (bits)))
#define FA_FIX15(a)                 fa_clip_int(FA_SCALE_FLOAT(a, 15), -32767, 32767)
#define FA_FIXMUL_32X15(a,b)        ((int)(((int64_t)(a) *(int64_t)(b))>>(15)))

#endif

puintptr_t fa_fft_fixed_init(int size);
void fa_fft_fixed_uninit(puintptr_t handle);

void fa_fft_fixed(puintptr_t handle, int *data);
void fa_ifft_fixed(puintptr_t handle, int* data);

#ifdef __cplusplus 
}
#endif  



#endif
