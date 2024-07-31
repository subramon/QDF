// Start at 0, every time you see the "on" value, increment by "incr" value
#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "is_sawtooth_I1.h"
#include "is_sawtooth_I2.h"
#include "is_sawtooth_I4.h"
#include "is_sawtooth_I8.h"
#include "is_sawtooth_F4.h"
#include "is_sawtooth_F8.h"
#include "qdf_is_sawtooth.h"
int
qdf_is_sawtooth(
    const QDF_REC_TYPE *const ptr_src, // input
    bool *ptr_b
   )
//STOP_FUNC_DECL
{
  int status = 0;
  //==============================
  mcr_chk_non_null(ptr_src, -1); 
  void *sdata = ptr_src->data; 
  jtype_t sjtype = get_jtype(sdata); 
  if ( sjtype != j_array ) {  go_BYE(-1); }
  uint32_t sn  = get_arr_len(sdata); 
  qtype_t sqtype = get_qtype(sdata); 
  const void * const sx = get_arr_ptr(sdata);
  //==============================
  switch ( sqtype ) { 
    case I1 : 
      *ptr_b = is_sawtooth_I1((const int8_t * const)sx, sn);
      break;
    case I2 : 
      *ptr_b = is_sawtooth_I2((const int16_t * const)sx, sn);
      break;
    case I4 : 
      *ptr_b = is_sawtooth_I4((const int32_t * const)sx, sn);
      break;
    case I8 : 
      *ptr_b = is_sawtooth_I8((const int64_t * const)sx, sn);
      break;
    case F4 : 
      *ptr_b = is_sawtooth_F4((const float * const)sx, sn);
      break;
    case F8 : 
      *ptr_b = is_sawtooth_F8((const double * const)sx, sn);
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
