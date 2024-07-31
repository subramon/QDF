#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "qdf_const.h"
#include "const_I1.h"
#include "const_I2.h"
#include "const_I4.h"
#include "const_I8.h"
#include "const_F4.h"
#include "const_F8.h"
//START_FUNC_DECL
int
qdf_const(
    SCLR_REC_TYPE *ptr_sclr,
    uint32_t n,
    uint32_t sz,
    QDF_REC_TYPE *dst // output 
   )
//STOP_FUNC_DECL
{
  int status = 0;
  mcr_chk_null(dst, -1); 
  //==============================
  if ( sz == 0 ) { sz = n; }
  if ( sz < n ) { go_BYE(-1); }
  status = make_num_array(NULL, n, sz, ptr_sclr->qtype, dst); cBYE(status);
  void *dstptr  = get_arr_ptr(dst->data);
  switch ( ptr_sclr->qtype ) { 
    case I1 :
      const_I1((int8_t *)dstptr, n, ptr_sclr->val.i1);
      break;
    case I2 :
      const_I2((int16_t *)dstptr, n, ptr_sclr->val.i2);
      break;
    case I4 :
      const_I4((int32_t *)dstptr, n, ptr_sclr->val.i4);
      break;
    case I8 :
      const_I8((int64_t *)dstptr, n, ptr_sclr->val.i8);
      break;
    case F4 : 
      const_F4((float *)dstptr, n, ptr_sclr->val.f4);
      break;
    case F8 : 
      const_F8((double *)dstptr, n, ptr_sclr->val.f8);
      break;
    default :
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
