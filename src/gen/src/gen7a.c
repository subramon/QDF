#include "vsmin_I1.h" 
void
vsmin_I1(
    int8_t *x,
    int8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( x[i] > sclr ) { 
      y[i] =  x[i];
    }
    else {
      y[i] = sclr;
    }
  }
}
    
#include "vsmin_I2.h" 
void
vsmin_I2(
    int16_t *x,
    int16_t sclr,
    uint32_t n,
    int16_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( x[i] > sclr ) { 
      y[i] =  x[i];
    }
    else {
      y[i] = sclr;
    }
  }
}
    
#include "vsmin_I4.h" 
void
vsmin_I4(
    int32_t *x,
    int32_t sclr,
    uint32_t n,
    int32_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( x[i] > sclr ) { 
      y[i] =  x[i];
    }
    else {
      y[i] = sclr;
    }
  }
}
    
#include "vsmin_I8.h" 
void
vsmin_I8(
    int64_t *x,
    int64_t sclr,
    uint32_t n,
    int64_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( x[i] > sclr ) { 
      y[i] =  x[i];
    }
    else {
      y[i] = sclr;
    }
  }
}
    
#include "vsmin_F4.h" 
void
vsmin_F4(
    float *x,
    float sclr,
    uint32_t n,
    float *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( x[i] > sclr ) { 
      y[i] =  x[i];
    }
    else {
      y[i] = sclr;
    }
  }
}
    
#include "vsmin_F8.h" 
void
vsmin_F8(
    double *x,
    double sclr,
    uint32_t n,
    double *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( x[i] > sclr ) { 
      y[i] =  x[i];
    }
    else {
      y[i] = sclr;
    }
  }
}
    
#include "vsmax_I1.h" 
void
vsmax_I1(
    int8_t *x,
    int8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( x[i] < sclr ) { 
      y[i] =  x[i];
    }
    else {
      y[i] = sclr;
    }
  }
}
    
#include "vsmax_I2.h" 
void
vsmax_I2(
    int16_t *x,
    int16_t sclr,
    uint32_t n,
    int16_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( x[i] < sclr ) { 
      y[i] =  x[i];
    }
    else {
      y[i] = sclr;
    }
  }
}
    
#include "vsmax_I4.h" 
void
vsmax_I4(
    int32_t *x,
    int32_t sclr,
    uint32_t n,
    int32_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( x[i] < sclr ) { 
      y[i] =  x[i];
    }
    else {
      y[i] = sclr;
    }
  }
}
    
#include "vsmax_I8.h" 
void
vsmax_I8(
    int64_t *x,
    int64_t sclr,
    uint32_t n,
    int64_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( x[i] < sclr ) { 
      y[i] =  x[i];
    }
    else {
      y[i] = sclr;
    }
  }
}
    
#include "vsmax_F4.h" 
void
vsmax_F4(
    float *x,
    float sclr,
    uint32_t n,
    float *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( x[i] < sclr ) { 
      y[i] =  x[i];
    }
    else {
      y[i] = sclr;
    }
  }
}
    
#include "vsmax_F8.h" 
void
vsmax_F8(
    double *x,
    double sclr,
    uint32_t n,
    double *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( x[i] < sclr ) { 
      y[i] =  x[i];
    }
    else {
      y[i] = sclr;
    }
  }
}
    
