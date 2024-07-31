#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "vsraised_to.h"

int 
vsraised_to(
    void *xptr, 
    const SCLR_REC_TYPE *const ptr_sclr, 
    uint32_t n, 
    void *zptr
    )
{
  int status = 0;
  // x^y = exp( y * log (x) )
  double y;
  status = get_sclr_as_F8(ptr_sclr, &y); cBYE(status);
  switch ( ptr_sclr->qtype ) { 
    case F4 : 
        for ( uint32_t i = 0; i < n; i++ ) { 
          double x = (double)((float *)xptr)[i];
          double z = exp(y * log(x));
          ((float *)zptr)[i] = (float)z;
        }
      break;
    case F8 : 
        for ( uint32_t i = 0; i < n; i++ ) { 
          double x = (double)((double *)xptr)[i];
          double z = exp(y * log(x));
          ((double *)zptr)[i] = (double)z;
        }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
