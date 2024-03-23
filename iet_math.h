#ifndef __IET_MATH_H__
#define __IET_MATH_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************* Include *************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/**
  \fn           void iet_fft(int32_t *pIntSrc , int32_t size)
  \brief        fixed fft function
  \param[in]    pIntSrc   data need to stored alternately according to real and img
  \return       NULL
*/
extern void iet_fft(int32_t *pIntSrc, int32_t size);


/**
  \fn           int32_t iet_log(uint32_t x)
  \brief        fixed base log function
  \param[in]    x   
  \return       (int)(log(x)*65535)
*/
extern int32_t iet_log(uint32_t x);


/**
  \fn           int32_t iet_sqrt(uint32_t x)
  \brief        fixed sqrt function
  \param[in]    x   
  \return       (int)(sqrt(x))
*/
extern int32_t iet_sqrt(uint32_t x);

/**
  \fn           int32_t iet_abscomplex(int32_t real, int32_t img, int32_t* shift_value)
  \brief        log function
  \param[in]    real
  \param[in]	  img
  \param[in]	  shift_value  if x<=0 : absresult left shift -shift_value, else: x>=0 absresult right shift shift_value
  \return       complex modular
*/
extern int32_t iet_abscomplex(int32_t real, int32_t img, int32_t* shift_value);

/**
  \fn           int32_t iet_mult(int32_t a, int32_t b, int32_t *shift_value)
  \brief        mult function
  \param[in]    a
  \param[in]    b
  \param[in]    shift_value if x<=0 : mult result left shift -shift_value, else: x>=0 mult result right shift shift_vale
  \return       mult result
*/
extern int32_t iet_mult(int32_t a, int32_t b, int32_t Qa, int32_t Qb, int32_t *shift_value);


#ifdef __cplusplus
}
#endif
/** @} */

#endif /* __NPU_CONTROL_H__ */