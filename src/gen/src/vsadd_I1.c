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
    
