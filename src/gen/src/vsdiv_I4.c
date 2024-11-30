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
    
