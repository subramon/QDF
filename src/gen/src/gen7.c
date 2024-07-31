#include "vsadd_I1.h" 
void
vsadd_I1(
    int8_t *x,
    int8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] + sclr); 
  }
}
    
#include "vsadd_I2.h" 
void
vsadd_I2(
    int16_t *x,
    int16_t sclr,
    uint32_t n,
    int16_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int16_t)(x[i] + sclr); 
  }
}
    
#include "vsadd_I4.h" 
void
vsadd_I4(
    int32_t *x,
    int32_t sclr,
    uint32_t n,
    int32_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int32_t)(x[i] + sclr); 
  }
}
    
#include "vsadd_I8.h" 
void
vsadd_I8(
    int64_t *x,
    int64_t sclr,
    uint32_t n,
    int64_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int64_t)(x[i] + sclr); 
  }
}
    
#include "vsadd_F4.h" 
void
vsadd_F4(
    float *x,
    float sclr,
    uint32_t n,
    float *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (float)(x[i] + sclr); 
  }
}
    
#include "vsadd_F8.h" 
void
vsadd_F8(
    double *x,
    double sclr,
    uint32_t n,
    double *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (double)(x[i] + sclr); 
  }
}
    
#include "vssub_I1.h" 
void
vssub_I1(
    int8_t *x,
    int8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] - sclr); 
  }
}
    
#include "vssub_I2.h" 
void
vssub_I2(
    int16_t *x,
    int16_t sclr,
    uint32_t n,
    int16_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int16_t)(x[i] - sclr); 
  }
}
    
#include "vssub_I4.h" 
void
vssub_I4(
    int32_t *x,
    int32_t sclr,
    uint32_t n,
    int32_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int32_t)(x[i] - sclr); 
  }
}
    
#include "vssub_I8.h" 
void
vssub_I8(
    int64_t *x,
    int64_t sclr,
    uint32_t n,
    int64_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int64_t)(x[i] - sclr); 
  }
}
    
#include "vssub_F4.h" 
void
vssub_F4(
    float *x,
    float sclr,
    uint32_t n,
    float *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (float)(x[i] - sclr); 
  }
}
    
#include "vssub_F8.h" 
void
vssub_F8(
    double *x,
    double sclr,
    uint32_t n,
    double *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (double)(x[i] - sclr); 
  }
}
    
#include "vsmul_I1.h" 
void
vsmul_I1(
    int8_t *x,
    int8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] * sclr); 
  }
}
    
#include "vsmul_I2.h" 
void
vsmul_I2(
    int16_t *x,
    int16_t sclr,
    uint32_t n,
    int16_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int16_t)(x[i] * sclr); 
  }
}
    
#include "vsmul_I4.h" 
void
vsmul_I4(
    int32_t *x,
    int32_t sclr,
    uint32_t n,
    int32_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int32_t)(x[i] * sclr); 
  }
}
    
#include "vsmul_I8.h" 
void
vsmul_I8(
    int64_t *x,
    int64_t sclr,
    uint32_t n,
    int64_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int64_t)(x[i] * sclr); 
  }
}
    
#include "vsmul_F4.h" 
void
vsmul_F4(
    float *x,
    float sclr,
    uint32_t n,
    float *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (float)(x[i] * sclr); 
  }
}
    
#include "vsmul_F8.h" 
void
vsmul_F8(
    double *x,
    double sclr,
    uint32_t n,
    double *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (double)(x[i] * sclr); 
  }
}
    
#include "vsdiv_I1.h" 
void
vsdiv_I1(
    int8_t *x,
    int8_t sclr,
    uint32_t n,
    int8_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int8_t)(x[i] / sclr); 
  }
}
    
#include "vsdiv_I2.h" 
void
vsdiv_I2(
    int16_t *x,
    int16_t sclr,
    uint32_t n,
    int16_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int16_t)(x[i] / sclr); 
  }
}
    
#include "vsdiv_I4.h" 
void
vsdiv_I4(
    int32_t *x,
    int32_t sclr,
    uint32_t n,
    int32_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int32_t)(x[i] / sclr); 
  }
}
    
#include "vsdiv_I8.h" 
void
vsdiv_I8(
    int64_t *x,
    int64_t sclr,
    uint32_t n,
    int64_t *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (int64_t)(x[i] / sclr); 
  }
}
    
#include "vsdiv_F4.h" 
void
vsdiv_F4(
    float *x,
    float sclr,
    uint32_t n,
    float *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (float)(x[i] / sclr); 
  }
}
    
#include "vsdiv_F8.h" 
void
vsdiv_F8(
    double *x,
    double sclr,
    uint32_t n,
    double *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (double)(x[i] / sclr); 
  }
}
    
