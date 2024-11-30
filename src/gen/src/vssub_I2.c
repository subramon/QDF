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
    
