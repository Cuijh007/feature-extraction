
#include "iet_math.h"
#include "fa_fft_fixed.h"

void iet_fft(int *pIntSrc, int size)
{
    puintptr_t FftHandle;
    int fft_inbuf [1024] = {0};
    int i = 0;
    for(i=0; i<512; i++)
    {
        fft_inbuf[2*i]= pIntSrc[2*i];
        fft_inbuf [2*i+1] = pIntSrc[2*i+1];
    }
    FftHandle = fa_fft_fixed_init(512);
    fa_fft_fixed(FftHandle, fft_inbuf);
    for( i=0; i<1024; i++)
    {
        pIntSrc[i] = fft_inbuf[i];
    }
    fa_fft_fixed_uninit(FftHandle);
}

/*
 * the input x is in Q9. based on e instead of 10
 * ln(x) = (iet_log(y)-90581)/(2^16), that's, (iet_log(y)-90581) is the result with Q16
 * iet_log(1<<9)  = 90581
 * here, x is the float to value to get ln(), y = INT（x*(2^9))
 */
int32_t iet_log(uint32_t x) 
{
    volatile int t;
    int y;
    if ( x <= 0 )
        return (1<<31);

    y = 0x10a2b0;
    if(x<0x00008000) x<<=16,              y-=0xb1721;
    if(x<0x00800000) x<<= 8,              y-=0x58b91;
    if(x<0x08000000) x<<= 4,              y-=0x2c5c8;
    if(x<0x20000000) x<<= 2,              y-=0x162e4;
    if(x<0x40000000) x<<= 1,              y-=0x0b172;
    t=x+(x>>1); if((t&0x80000000)==0) x=t,y-=0x067cd;
    t=x+(x>>2); if((t&0x80000000)==0) x=t,y-=0x03920;
    t=x+(x>>3); if((t&0x80000000)==0) x=t,y-=0x01e27;
    t=x+(x>>4); if((t&0x80000000)==0) x=t,y-=0x00f85;
    t=x+(x>>5); if((t&0x80000000)==0) x=t,y-=0x007e1;
    t=x+(x>>6); if((t&0x80000000)==0) x=t,y-=0x003f8;
    t=x+(x>>7); if((t&0x80000000)==0) x=t,y-=0x001fe;
    x=0x80000000-x;
    y-=(x>>15);
    return y;
 }

/* sqrt int fast algorithm */
int32_t iet_sqrt(uint32_t x)
{
    unsigned int rem = 0;
    int root = 0;
    int i;

    for (i = 0; i < 16; i++) {
        root <<= 1;
        rem <<= 2;
        rem += x >> 30;
        x <<= 2;

        if (root < rem) {
            root++;
            rem -= root;
            root++;
        }
    }

    return  (root >> 1);
}

#define ABS(value) (value&0x80000000)?(~value+1):value
#define MIN(x,y) (x)<(y)?(x):(y)
/*
 * get the complex mag  = sqrt( real*real+img*img) with dynamic Q_value,
 * real and img are the input 
 * shift_value: is the right shift value
 * return the mag value, it's Q=shif_value
 */
int32_t iet_abscomplex(int32_t real, int32_t img, int32_t* shift_value)
{
    int32_t abs_real = ABS( real );
    int32_t abs_img = ABS( img );

    int leading_real = __builtin_clz( abs_real );
    int leading_img  = __builtin_clz( abs_img );

    int leading_shift = MIN( leading_img, leading_real );
    leading_shift -= 1; 
    *shift_value = 16-leading_shift;
    
    real <<= leading_shift;
    img <<= leading_shift;

    real >>= 16;
    img  >>= 16;

    uint32_t square_real = real * real;
    uint32_t square_img = img * img;
    square_img += square_real;
    square_real = iet_sqrt( square_img );

    return square_real;
}

int32_t iet_mult(int32_t a, int32_t b, int32_t Qa, int32_t Qb, int32_t *shift_value)
{
    int32_t abs_a = ABS(a);
    int32_t abs_b = ABS(b);

    int16_t leading_a = __builtin_clz( abs_a ) - 1;
    int16_t leading_b = __builtin_clz( abs_b ) - 1;

    a <<= leading_a;
    b <<= leading_b;

    a >>= 16;
    b >>= 16;

    *shift_value = leading_a + leading_b + Qa + Qb - 32;

    return a*b;

}