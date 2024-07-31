#include "seq_I1.h" 
void
seq_I1(
    int8_t start,
    int8_t incr,
    uint32_t n,
    int8_t *restrict out
    )
{
  out[0] = start;
  for ( uint32_t i = 1; i < n; i++ ) { 
    out[i] = (int8_t)(out[i-1] + incr);
  }
}
    
#include "seq_I2.h" 
void
seq_I2(
    int16_t start,
    int16_t incr,
    uint32_t n,
    int16_t *restrict out
    )
{
  out[0] = start;
  for ( uint32_t i = 1; i < n; i++ ) { 
    out[i] = (int16_t)(out[i-1] + incr);
  }
}
    
#include "seq_I4.h" 
void
seq_I4(
    int32_t start,
    int32_t incr,
    uint32_t n,
    int32_t *restrict out
    )
{
  out[0] = start;
  for ( uint32_t i = 1; i < n; i++ ) { 
    out[i] = (int32_t)(out[i-1] + incr);
  }
}
    
#include "seq_I8.h" 
void
seq_I8(
    int64_t start,
    int64_t incr,
    uint32_t n,
    int64_t *restrict out
    )
{
  out[0] = start;
  for ( uint32_t i = 1; i < n; i++ ) { 
    out[i] = (int64_t)(out[i-1] + incr);
  }
}
    
#include "seq_F4.h" 
void
seq_F4(
    float start,
    float incr,
    uint32_t n,
    float *restrict out
    )
{
  out[0] = start;
  for ( uint32_t i = 1; i < n; i++ ) { 
    out[i] = (float)(out[i-1] + incr);
  }
}
    
#include "seq_F8.h" 
void
seq_F8(
    double start,
    double incr,
    uint32_t n,
    double *restrict out
    )
{
  out[0] = start;
  for ( uint32_t i = 1; i < n; i++ ) { 
    out[i] = (double)(out[i-1] + incr);
  }
}
    
#include "incr_on_by_I1.h" 
void
incr_on_by_I1(
    const int8_t *const invals,
    int8_t on,
    int8_t incr,
    bool reset,
    uint32_t n,
    int8_t *restrict outvals
    )
{
  outvals[0] = 0; if ( invals[0] == on ) { outvals[0] = incr; } 
  for ( uint32_t i = 1; i < n; i++ ) { 
    if ( invals[i] == on ) {
      outvals[i] = (int8_t)(outvals[i-1] + incr);
    }
    else { 
      if ( reset ) { 
        outvals[i] = 0; 
      }
      else {
        outvals[i] = (int8_t)(outvals[i-1]);
      }
    }
  }
}
    
#include "incr_on_by_I2.h" 
void
incr_on_by_I2(
    const int16_t *const invals,
    int16_t on,
    int16_t incr,
    bool reset,
    uint32_t n,
    int16_t *restrict outvals
    )
{
  outvals[0] = 0; if ( invals[0] == on ) { outvals[0] = incr; } 
  for ( uint32_t i = 1; i < n; i++ ) { 
    if ( invals[i] == on ) {
      outvals[i] = (int16_t)(outvals[i-1] + incr);
    }
    else { 
      if ( reset ) { 
        outvals[i] = 0; 
      }
      else {
        outvals[i] = (int16_t)(outvals[i-1]);
      }
    }
  }
}
    
#include "incr_on_by_I4.h" 
void
incr_on_by_I4(
    const int32_t *const invals,
    int32_t on,
    int32_t incr,
    bool reset,
    uint32_t n,
    int32_t *restrict outvals
    )
{
  outvals[0] = 0; if ( invals[0] == on ) { outvals[0] = incr; } 
  for ( uint32_t i = 1; i < n; i++ ) { 
    if ( invals[i] == on ) {
      outvals[i] = (int32_t)(outvals[i-1] + incr);
    }
    else { 
      if ( reset ) { 
        outvals[i] = 0; 
      }
      else {
        outvals[i] = (int32_t)(outvals[i-1]);
      }
    }
  }
}
    
#include "incr_on_by_I8.h" 
void
incr_on_by_I8(
    const int64_t *const invals,
    int64_t on,
    int64_t incr,
    bool reset,
    uint32_t n,
    int64_t *restrict outvals
    )
{
  outvals[0] = 0; if ( invals[0] == on ) { outvals[0] = incr; } 
  for ( uint32_t i = 1; i < n; i++ ) { 
    if ( invals[i] == on ) {
      outvals[i] = (int64_t)(outvals[i-1] + incr);
    }
    else { 
      if ( reset ) { 
        outvals[i] = 0; 
      }
      else {
        outvals[i] = (int64_t)(outvals[i-1]);
      }
    }
  }
}
    
#include "incr_on_by_F4.h" 
void
incr_on_by_F4(
    const float *const invals,
    float on,
    float incr,
    bool reset,
    uint32_t n,
    float *restrict outvals
    )
{
  outvals[0] = 0; if ( invals[0] == on ) { outvals[0] = incr; } 
  for ( uint32_t i = 1; i < n; i++ ) { 
    if ( invals[i] == on ) {
      outvals[i] = (float)(outvals[i-1] + incr);
    }
    else { 
      if ( reset ) { 
        outvals[i] = 0; 
      }
      else {
        outvals[i] = (float)(outvals[i-1]);
      }
    }
  }
}
    
#include "incr_on_by_F8.h" 
void
incr_on_by_F8(
    const double *const invals,
    double on,
    double incr,
    bool reset,
    uint32_t n,
    double *restrict outvals
    )
{
  outvals[0] = 0; if ( invals[0] == on ) { outvals[0] = incr; } 
  for ( uint32_t i = 1; i < n; i++ ) { 
    if ( invals[i] == on ) {
      outvals[i] = (double)(outvals[i-1] + incr);
    }
    else { 
      if ( reset ) { 
        outvals[i] = 0; 
      }
      else {
        outvals[i] = (double)(outvals[i-1]);
      }
    }
  }
}
    
#include "is_sawtooth_I1.h" 
bool
is_sawtooth_I1(
    const int8_t *const invals,
    uint32_t n
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( invals[i] == 0 ) { continue; }
    if ( invals[i] < 0 ) { return false; }
    // invals[i] > 0 
    if ( i > 0 ) { 
      if ( invals[i] < invals[i-1] ) { return false; }
    }
  }
  return true;
}
    
#include "is_sawtooth_I2.h" 
bool
is_sawtooth_I2(
    const int16_t *const invals,
    uint32_t n
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( invals[i] == 0 ) { continue; }
    if ( invals[i] < 0 ) { return false; }
    // invals[i] > 0 
    if ( i > 0 ) { 
      if ( invals[i] < invals[i-1] ) { return false; }
    }
  }
  return true;
}
    
#include "is_sawtooth_I4.h" 
bool
is_sawtooth_I4(
    const int32_t *const invals,
    uint32_t n
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( invals[i] == 0 ) { continue; }
    if ( invals[i] < 0 ) { return false; }
    // invals[i] > 0 
    if ( i > 0 ) { 
      if ( invals[i] < invals[i-1] ) { return false; }
    }
  }
  return true;
}
    
#include "is_sawtooth_I8.h" 
bool
is_sawtooth_I8(
    const int64_t *const invals,
    uint32_t n
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( invals[i] == 0 ) { continue; }
    if ( invals[i] < 0 ) { return false; }
    // invals[i] > 0 
    if ( i > 0 ) { 
      if ( invals[i] < invals[i-1] ) { return false; }
    }
  }
  return true;
}
    
#include "is_sawtooth_F4.h" 
bool
is_sawtooth_F4(
    const float *const invals,
    uint32_t n
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( invals[i] == 0 ) { continue; }
    if ( invals[i] < 0 ) { return false; }
    // invals[i] > 0 
    if ( i > 0 ) { 
      if ( invals[i] < invals[i-1] ) { return false; }
    }
  }
  return true;
}
    
#include "is_sawtooth_F8.h" 
bool
is_sawtooth_F8(
    const double *const invals,
    uint32_t n
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( invals[i] == 0 ) { continue; }
    if ( invals[i] < 0 ) { return false; }
    // invals[i] > 0 
    if ( i > 0 ) { 
      if ( invals[i] < invals[i-1] ) { return false; }
    }
  }
  return true;
}
    
