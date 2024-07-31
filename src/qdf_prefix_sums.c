#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_makers.h"
#include "qdf_checkers.h"
#include "qdf_prefix_sums.h"
int
prefix_sums(
    const QDF_REC_TYPE *const src,
    const char * const direction,
    QDF_REC_TYPE *restrict dst
    )
{
  int status = 0;
  mcr_chk_non_null(src, -1); 
  mcr_chk_null(dst, -1); 
  
  char *sx = src->data;
  jtype_t sjtype = get_jtype(sx); if ( sjtype != j_array ) { go_BYE(-1); }
  qtype_t sqtype = get_qtype(sx); if ( sqtype != I1 ) { go_BYE(-1); }
  uint32_t sn    = get_arr_len(sx); 

  // TODO P4 Consider when we can use less than I4
  status = make_num_array(NULL, sn, 0, I4, dst);  cBYE(status);
  char *dx = dst->data;
  const int8_t * const sptr = (const int8_t * const)get_arr_ptr(sx); 
  int32_t *dptr = get_arr_ptr(dx); 

#ifdef DEBUG
  for ( uint32_t i = 0; i < sn; i++ ) { 
    if ( ( sptr[i] != 0 ) && ( sptr[i] != 1 ) ) { go_BYE(-1); }
  }
#endif

  if ( strcmp(direction, "forward") == 0 ) { 
    dptr[0] = sptr[0];
    for ( uint32_t i = 1; i < sn; i++ ) { 
      if ( sptr[i] == 0 ) { 
        dptr[i] = 0;
      }
      else {
        dptr[i] = sptr[i] + dptr[i-1];
      }
    }
  }
  else if ( strcmp(direction, "backward") == 0 ) {
    dptr[sn-1] = sptr[sn-1];
    for ( int i = (int)(sn-2); i >= 0; i-- ) { 
      if ( sptr[i] == 0 ) { 
        dptr[i] = 0;
      }
      else {
        dptr[i] = sptr[i] + dptr[i+1];
      }
    }
  }
  else {
    go_BYE(-1);
  }
BYE:
  return status;
}
