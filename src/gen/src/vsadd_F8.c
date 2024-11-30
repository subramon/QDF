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
    
