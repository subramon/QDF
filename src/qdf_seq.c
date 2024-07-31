#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_checkers.h"
#include "qdf_makers.h"
#include "seq_I1.h"
#include "seq_I2.h"
#include "seq_I4.h"
#include "seq_I8.h"
#include "seq_F4.h"
#include "seq_F8.h"
#include "qdf_seq.h"
int
qdf_seq(
    SCLR_REC_TYPE *ptr_start,
    SCLR_REC_TYPE *ptr_incr,
    uint32_t n,
    uint32_t sz,
    QDF_REC_TYPE *dst // output 
   )
//STOP_FUNC_DECL
{
  int status = 0;
  mcr_chk_null(dst, -1); 
  //==============================
  qtype_t qtype = ptr_start->qtype;
  if  ( ptr_start->qtype != ptr_incr->qtype ) { go_BYE(-1); }

  status = make_num_array(NULL, n, sz, qtype, dst); cBYE(status);
  void *dstptr  = get_arr_ptr(dst->data);
  switch ( qtype ) { 
    case I1 : 
      seq_I1(ptr_start->val.i1, ptr_incr->val.i1, n, dstptr); 
      break;
    case I2 : 
      seq_I2(ptr_start->val.i2, ptr_incr->val.i2, n, dstptr); 
      break;
    case I4 : 
      seq_I4(ptr_start->val.i4, ptr_incr->val.i4, n, dstptr); 
      break;
    case I8 : 
      seq_I8(ptr_start->val.i8, ptr_incr->val.i8, n, dstptr); 
      break;
    case F4 : 
      seq_F4(ptr_start->val.f4, ptr_incr->val.f4, n, dstptr); 
      break;
    case F8 : 
      seq_F8(ptr_start->val.f8, ptr_incr->val.f8, n, dstptr); 
      break;
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  return status;
}
