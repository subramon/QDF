#include "const_I1.h" 
void
const_I1(
    int8_t *out,
    uint32_t n,
    int8_t val
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    out[i] = val;
  }
}
    
#include "const_I2.h" 
void
const_I2(
    int16_t *out,
    uint32_t n,
    int16_t val
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    out[i] = val;
  }
}
    
#include "const_I4.h" 
void
const_I4(
    int32_t *out,
    uint32_t n,
    int32_t val
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    out[i] = val;
  }
}
    
#include "const_I8.h" 
void
const_I8(
    int64_t *out,
    uint32_t n,
    int64_t val
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    out[i] = val;
  }
}
    
#include "const_F4.h" 
void
const_F4(
    float *out,
    uint32_t n,
    float val
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    out[i] = val;
  }
}
    
#include "const_F8.h" 
void
const_F8(
    double *out,
    uint32_t n,
    double val
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    out[i] = val;
  }
}
    
