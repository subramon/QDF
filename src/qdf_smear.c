#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_makers.h"
#include "qdf_checkers.h"
#include "qdf_smear.h"
int
smear(
    const QDF_REC_TYPE *const src,
    const QDF_REC_TYPE *const off, // offset 
    const char * const direction,
    QDF_REC_TYPE *restrict dst
    )
{
  int status = 0;
  mcr_chk_non_null(src, -1); 
  mcr_chk_non_null(off, -1); 
  mcr_chk_null(dst, -1); 
  if ( direction == NULL ) { go_BYE(-1); };

  int dir; // Choice of +1 and -1 below are deliberate. Read further
  if ( strcmp(direction, "forward") == 0 ) { 
    dir = 1;
  }
  else if ( strcmp(direction, "backward") == 0 ) { 
    dir = -1;
  }
  else {
    go_BYE(-1);
  }

  //--------------------------------------------
  char *sx = src->data;
  jtype_t sjtype = get_jtype(sx); if ( sjtype != j_array ) { go_BYE(-1); }
  qtype_t sqtype = get_qtype(sx); 
  uint32_t sn    = get_arr_len(sx); 
  const void * const sptr = get_arr_ptr(sx); 
  //--------------------------------------------
  char *ox = off->data;
  if ( ox == NULL ) {  go_BYE(-1); }
  jtype_t ojtype = get_jtype(ox); if ( ojtype != j_array ) { go_BYE(-1); }
  qtype_t oqtype = get_qtype(ox); 
  uint32_t on    = get_arr_len(ox); 
  const int32_t * const optr  = get_arr_ptr(ox); 
  //--------------------------------------------
  if ( sn != on ) { go_BYE(-1); }
  // TODO P3 Need to support more types 
  if ( ( sqtype != I1 ) && ( sqtype != I4 ) && ( sqtype != F4 ) ) { 
    go_BYE(-1); 
  }
  if ( oqtype != I4 ) { go_BYE(-1); }

  status = make_num_array(NULL, sn, 0, sqtype, dst);  cBYE(status);
  void *dx = dst->data;
  void *dptr = get_arr_ptr(dx); 

  int isn = (int) sn;
  switch ( sqtype ) { 
    case I1 : 
      for ( int i = 0; i < isn; i++ ) { 
        int32_t offset = optr[i];
        int32_t didx = i + (dir * offset);
        if ( ( i + offset >= isn ) || ( i + offset < 0 ) ) { go_BYE(-1); }
        ((int8_t *)dptr)[i] = ((const int8_t * const )sptr)[didx];
      }
      break;
    case I4 : 
      for ( int i = 0; i < isn; i++ ) { 
        int32_t offset = optr[i];
        int32_t didx = i + (dir * offset);
        if ( ( i + offset >= isn ) || ( i + offset < 0 ) ) { go_BYE(-1); }
        ((int32_t *)dptr)[i] = ((const int32_t * const)sptr)[didx];
      }
      break;
    case F4 : 
      for ( int i = 0; i < isn; i++ ) { 
        int32_t offset = optr[i];
        int32_t didx = i + (dir * offset);
        if ( ( i + offset >= isn ) || ( i + offset < 0 ) ) { go_BYE(-1); }
        ((float *)dptr)[i] = ((const float * const )sptr)[didx];
      }
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
