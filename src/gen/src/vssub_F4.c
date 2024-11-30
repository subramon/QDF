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
    
