#include "incs.h"
#include "qdf_struct.h"
#include "qdf_helpers.h"
#include "qdf_get_by_idx.h"

// assumes that qdf is j_array of qtype != Q0
// given an index "idx" returns value at that index
int
qdf_get_by_idx(
    const QDF_REC_TYPE * const ptr_qdf, // input 
    uint32_t idx, // input 
    SCLR_REC_TYPE *ptr_sclr // output 
   )
{
  int status = 0;
  memset(ptr_sclr, 0, sizeof(SCLR_REC_TYPE));
  //==============================
  mcr_chk_non_null(ptr_qdf, -1); 
  void *sx = ptr_qdf->data; 

  jtype_t sjtype = get_jtype(sx); 
  if ( sjtype != j_array ) {  go_BYE(-1); }

  qtype_t sqtype = get_qtype(sx); 
  if ( sqtype == Q0 ) { go_BYE(-1); } 
  ptr_sclr->qtype = sqtype; 

  uint32_t sn  = get_arr_len(sx); 
  if ( idx >= sn ) { go_BYE(-1); }
  const void * const sptr = get_arr_ptr(sx); 
  switch ( sqtype ) { 
    case  I1 : ptr_sclr->val.i1  = ((const int8_t *const)sptr)[idx]; break; 
    case UI1 : ptr_sclr->val.ui1 = ((const uint8_t *const)sptr)[idx]; break; 
    case  I2 : ptr_sclr->val.i2  = ((const int16_t *const)sptr)[idx]; break; 
    case UI2 : ptr_sclr->val.ui2 = ((const uint16_t *const)sptr)[idx]; break; 
    case  I4 : ptr_sclr->val.i4  = ((const int32_t *const)sptr)[idx]; break; 
    case UI4 : ptr_sclr->val.ui4 = ((const uint32_t *const)sptr)[idx]; break; 
    case  I8 : ptr_sclr->val.i8  = ((const int64_t *const)sptr)[idx]; break; 
    case UI8 : ptr_sclr->val.ui8 = ((const uint64_t *const)sptr)[idx]; break; 
    case  F4 : ptr_sclr->val.f4  = ((const float *const)sptr)[idx]; break; 
    case  F8 : ptr_sclr->val.f8  = ((const double *const)sptr)[idx]; break; 
    case  SC : {
                 uint32_t sw  = get_arr_width(sx); 
                 if ( sw == 0 ) { go_BYE(-1); }
                 ptr_sclr->val.str  = (const char * const)sptr + (idx*sw); 
               }
               break;
    default : go_BYE(-1);  break;
  }
BYE:
  return status;
}
