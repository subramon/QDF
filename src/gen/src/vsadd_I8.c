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
    
