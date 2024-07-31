// Start at 0, every time you see the "on" value, increment by "incr" value
#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "incr_on_by_I1.h"
#include "incr_on_by_I2.h"
#include "incr_on_by_I4.h"
#include "incr_on_by_I8.h"
#include "incr_on_by_F4.h"
#include "incr_on_by_F8.h"
#include "qdf_incr_on_by.h"
int
qdf_incr_on_by(
    const QDF_REC_TYPE *const ptr_src, // input
    const SCLR_REC_TYPE * const ptr_on,
    const SCLR_REC_TYPE * const ptr_incr,
    bool reset,
    QDF_REC_TYPE * restrict ptr_dst // output 
   )
//STOP_FUNC_DECL
{
  int status = 0;
  mcr_chk_null(ptr_dst, -1); 
  //==============================
  mcr_chk_non_null(ptr_src, -1); 
  void *sdata = ptr_src->data; 
  jtype_t sjtype = get_jtype(sdata); 
  if ( sjtype != j_array ) {  go_BYE(-1); }
  uint32_t sn  = get_arr_len(sdata); 
  uint32_t ssz = get_arr_size(sdata); 
  qtype_t sqtype = get_qtype(sdata); 
  const void * const sx = get_arr_ptr(sdata);
  //==============================
  mcr_chk_null(ptr_dst, -1); 
  //==============================
  if ( ptr_on->qtype != sqtype ) { go_BYE(-1); }
  if ( ptr_on->qtype != sqtype ) { go_BYE(-1); }
  //==============================

  status = make_num_array(NULL, sn, ssz, sqtype, ptr_dst); cBYE(status);
  void *dx  = get_arr_ptr(ptr_dst->data);
  switch ( sqtype ) { 
    case I1 : 
      incr_on_by_I1((const int8_t * const)sx, 
          ptr_on->val.i1, ptr_incr->val.i1, reset, sn, (int8_t *)dx); 
      break;
    case I2 : 
      incr_on_by_I2((const int16_t * const)sx, 
          ptr_on->val.i2, ptr_incr->val.i2, reset, sn, (int16_t *)dx); 
      break;
    case I4 : 
      incr_on_by_I4((const int32_t * const)sx, 
          ptr_on->val.i4, ptr_incr->val.i4, reset, sn, (int32_t *)dx); 
      break;
    case I8 : 
      incr_on_by_I8((const int64_t * const)sx, 
          ptr_on->val.i8, ptr_incr->val.i8, reset, sn, (int64_t *)dx); 
      break;
    case F4 : 
      incr_on_by_F4((const float * const)sx, 
          ptr_on->val.f4, ptr_incr->val.f4, reset, sn, (float *)dx); 
      break;
    case F8 : 
      incr_on_by_F8((const double * const)sx, 
          ptr_on->val.f8, ptr_incr->val.f8, reset, sn, (double *)dx); 
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
