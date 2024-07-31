#include <stdint.h>
#include "vvadd_I1.h"
void
vvadd_I1(
    int8_t *x,
    int8_t *y,
    uint32_t n,
    int8_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int8_t)(x[i]  +  y[i]);
  }

}
#include <stdint.h>
#include "vvadd_I2.h"
void
vvadd_I2(
    int16_t *x,
    int16_t *y,
    uint32_t n,
    int16_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int16_t)(x[i]  +  y[i]);
  }

}
#include <stdint.h>
#include "vvadd_I4.h"
void
vvadd_I4(
    int32_t *x,
    int32_t *y,
    uint32_t n,
    int32_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int32_t)(x[i]  +  y[i]);
  }

}
#include <stdint.h>
#include "vvadd_I8.h"
void
vvadd_I8(
    int64_t *x,
    int64_t *y,
    uint32_t n,
    int64_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int64_t)(x[i]  +  y[i]);
  }

}
#include <stdint.h>
#include "vvadd_F4.h"
void
vvadd_F4(
    float *x,
    float *y,
    uint32_t n,
    float *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (float)(x[i]  +  y[i]);
  }

}
#include <stdint.h>
#include "vvadd_F8.h"
void
vvadd_F8(
    double *x,
    double *y,
    uint32_t n,
    double *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (double)(x[i]  +  y[i]);
  }

}
#include <stdint.h>
#include "vvsub_I1.h"
void
vvsub_I1(
    int8_t *x,
    int8_t *y,
    uint32_t n,
    int8_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int8_t)(x[i]  -  y[i]);
  }

}
#include <stdint.h>
#include "vvsub_I2.h"
void
vvsub_I2(
    int16_t *x,
    int16_t *y,
    uint32_t n,
    int16_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int16_t)(x[i]  -  y[i]);
  }

}
#include <stdint.h>
#include "vvsub_I4.h"
void
vvsub_I4(
    int32_t *x,
    int32_t *y,
    uint32_t n,
    int32_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int32_t)(x[i]  -  y[i]);
  }

}
#include <stdint.h>
#include "vvsub_I8.h"
void
vvsub_I8(
    int64_t *x,
    int64_t *y,
    uint32_t n,
    int64_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int64_t)(x[i]  -  y[i]);
  }

}
#include <stdint.h>
#include "vvsub_F4.h"
void
vvsub_F4(
    float *x,
    float *y,
    uint32_t n,
    float *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (float)(x[i]  -  y[i]);
  }

}
#include <stdint.h>
#include "vvsub_F8.h"
void
vvsub_F8(
    double *x,
    double *y,
    uint32_t n,
    double *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (double)(x[i]  -  y[i]);
  }

}
#include <stdint.h>
#include "vvmul_I1.h"
void
vvmul_I1(
    int8_t *x,
    int8_t *y,
    uint32_t n,
    int8_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int8_t)(x[i]  *  y[i]);
  }

}
#include <stdint.h>
#include "vvmul_I2.h"
void
vvmul_I2(
    int16_t *x,
    int16_t *y,
    uint32_t n,
    int16_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int16_t)(x[i]  *  y[i]);
  }

}
#include <stdint.h>
#include "vvmul_I4.h"
void
vvmul_I4(
    int32_t *x,
    int32_t *y,
    uint32_t n,
    int32_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int32_t)(x[i]  *  y[i]);
  }

}
#include <stdint.h>
#include "vvmul_I8.h"
void
vvmul_I8(
    int64_t *x,
    int64_t *y,
    uint32_t n,
    int64_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int64_t)(x[i]  *  y[i]);
  }

}
#include <stdint.h>
#include "vvmul_F4.h"
void
vvmul_F4(
    float *x,
    float *y,
    uint32_t n,
    float *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (float)(x[i]  *  y[i]);
  }

}
#include <stdint.h>
#include "vvmul_F8.h"
void
vvmul_F8(
    double *x,
    double *y,
    uint32_t n,
    double *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (double)(x[i]  *  y[i]);
  }

}
#include <stdint.h>
#include "vvdiv_I1.h"
void
vvdiv_I1(
    int8_t *x,
    int8_t *y,
    uint32_t n,
    int8_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int8_t)(x[i]  /  y[i]);
  }

}
#include <stdint.h>
#include "vvdiv_I2.h"
void
vvdiv_I2(
    int16_t *x,
    int16_t *y,
    uint32_t n,
    int16_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int16_t)(x[i]  /  y[i]);
  }

}
#include <stdint.h>
#include "vvdiv_I4.h"
void
vvdiv_I4(
    int32_t *x,
    int32_t *y,
    uint32_t n,
    int32_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int32_t)(x[i]  /  y[i]);
  }

}
#include <stdint.h>
#include "vvdiv_I8.h"
void
vvdiv_I8(
    int64_t *x,
    int64_t *y,
    uint32_t n,
    int64_t *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (int64_t)(x[i]  /  y[i]);
  }

}
#include <stdint.h>
#include "vvdiv_F4.h"
void
vvdiv_F4(
    float *x,
    float *y,
    uint32_t n,
    float *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (float)(x[i]  /  y[i]);
  }

}
#include <stdint.h>
#include "vvdiv_F8.h"
void
vvdiv_F8(
    double *x,
    double *y,
    uint32_t n,
    double *z
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    z[i] = (double)(x[i]  /  y[i]);
  }

}
