#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_shift_up_down.h"
//START_FUNC_DECL
int
shift_up_down(
    const QDF_REC_TYPE * const src, // input numeric array 
    uint32_t shift_by,
    const char * const direction,
    QDF_REC_TYPE * restrict dst, // output numeric array (same type as src)
    QDF_REC_TYPE * restrict nn_dst // output numeric array I1
   )
//STOP_FUNC_DECL
{
  int status = 0;
  //==============================
  mcr_chk_non_null(src, -1);
  void *sx = src->data; 
  jtype_t sjtype = get_jtype(sx); if ( sjtype != j_array ) {  go_BYE(-1); }
  uint32_t n     = get_arr_len(sx); 
  uint32_t sz    = get_arr_size(sx); 
  qtype_t sqtype = get_qtype(sx); 
  uint32_t swidth     = get_arr_width(sx);
  //==============================
  status = make_num_array(NULL, n, sz, sqtype, dst);  cBYE(status);
  status = make_num_array(NULL, n, sz, I1, nn_dst);  cBYE(status);

  char * srcptr    = get_arr_ptr(src->data); 
  char *dstptr    = get_arr_ptr(dst->data); 
  char *nn_dstptr = get_arr_ptr(nn_dst->data); 

  if ( shift_by >= n ) { go_BYE(-1); } 
  if ( strcmp(direction, "up") == 0 ) { 
    memset(nn_dstptr,          1, n- shift_by);
    memset(nn_dstptr+(n-shift_by), 0, shift_by);

    memset(dstptr+(n-shift_by)*swidth, 0, swidth*shift_by);
    memcpy(dstptr, srcptr+(shift_by*swidth), (n-shift_by)*swidth);
  }
  else if ( strcmp(direction, "down") == 0 ) { 
    memset(nn_dstptr,          0, shift_by);
    memset(nn_dstptr+shift_by, 1, n - shift_by);

    memset(dstptr, 0, shift_by * swidth);
    memcpy(dstptr+(shift_by*swidth), srcptr, (n-shift_by)*swidth);
  }
  else {
    go_BYE(-1);
  }
#ifdef DEBUG
  uint32_t sum = 0; 
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( !( ( ((int8_t *)nn_dstptr)[i] != 0 ) || 
          ( ((int8_t *)nn_dstptr)[i] != 1 ) ) ) {
      go_BYE(-1);
    }
    sum += ((uint8_t *)nn_dstptr)[i];
  }
  if ( sum != (n - shift_by) ) { go_BYE(-1); }
#endif
BYE:
  return status;
}
