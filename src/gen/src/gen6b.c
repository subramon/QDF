#include "vseq.h" 
int
vseq(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    int8_t *z
    )
{
  int status = 0;
  switch ( ptr_sclr->qtype ) { 
    case I1 : vseq_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  break; 
    case I2 : vseq_I2((int16_t *)x, ptr_sclr->val.i2, n, (int8_t *)z);  break; 
    case I4 : vseq_I4((int32_t *)x, ptr_sclr->val.i4, n, (int8_t *)z);  break; 
    case I8 : vseq_I8((int64_t *)x, ptr_sclr->val.i8, n, (int8_t *)z);  break; 
    case F4 : vseq_F4((float *)x, ptr_sclr->val.f4, n, (int8_t *)z);  break; 
    case F8 : vseq_F8((double *)x, ptr_sclr->val.f8, n, (int8_t *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vsneq.h" 
int
vsneq(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    int8_t *z
    )
{
  int status = 0;
  switch ( ptr_sclr->qtype ) { 
    case I1 : vsneq_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  break; 
    case I2 : vsneq_I2((int16_t *)x, ptr_sclr->val.i2, n, (int8_t *)z);  break; 
    case I4 : vsneq_I4((int32_t *)x, ptr_sclr->val.i4, n, (int8_t *)z);  break; 
    case I8 : vsneq_I8((int64_t *)x, ptr_sclr->val.i8, n, (int8_t *)z);  break; 
    case F4 : vsneq_F4((float *)x, ptr_sclr->val.f4, n, (int8_t *)z);  break; 
    case F8 : vsneq_F8((double *)x, ptr_sclr->val.f8, n, (int8_t *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vslt.h" 
int
vslt(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    int8_t *z
    )
{
  int status = 0;
  switch ( ptr_sclr->qtype ) { 
    case I1 : vslt_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  break; 
    case I2 : vslt_I2((int16_t *)x, ptr_sclr->val.i2, n, (int8_t *)z);  break; 
    case I4 : vslt_I4((int32_t *)x, ptr_sclr->val.i4, n, (int8_t *)z);  break; 
    case I8 : vslt_I8((int64_t *)x, ptr_sclr->val.i8, n, (int8_t *)z);  break; 
    case F4 : vslt_F4((float *)x, ptr_sclr->val.f4, n, (int8_t *)z);  break; 
    case F8 : vslt_F8((double *)x, ptr_sclr->val.f8, n, (int8_t *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vsleq.h" 
int
vsleq(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    int8_t *z
    )
{
  int status = 0;
  switch ( ptr_sclr->qtype ) { 
    case I1 : vsleq_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  break; 
    case I2 : vsleq_I2((int16_t *)x, ptr_sclr->val.i2, n, (int8_t *)z);  break; 
    case I4 : vsleq_I4((int32_t *)x, ptr_sclr->val.i4, n, (int8_t *)z);  break; 
    case I8 : vsleq_I8((int64_t *)x, ptr_sclr->val.i8, n, (int8_t *)z);  break; 
    case F4 : vsleq_F4((float *)x, ptr_sclr->val.f4, n, (int8_t *)z);  break; 
    case F8 : vsleq_F8((double *)x, ptr_sclr->val.f8, n, (int8_t *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vsgt.h" 
int
vsgt(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    int8_t *z
    )
{
  int status = 0;
  switch ( ptr_sclr->qtype ) { 
    case I1 : vsgt_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  break; 
    case I2 : vsgt_I2((int16_t *)x, ptr_sclr->val.i2, n, (int8_t *)z);  break; 
    case I4 : vsgt_I4((int32_t *)x, ptr_sclr->val.i4, n, (int8_t *)z);  break; 
    case I8 : vsgt_I8((int64_t *)x, ptr_sclr->val.i8, n, (int8_t *)z);  break; 
    case F4 : vsgt_F4((float *)x, ptr_sclr->val.f4, n, (int8_t *)z);  break; 
    case F8 : vsgt_F8((double *)x, ptr_sclr->val.f8, n, (int8_t *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vsgeq.h" 
int
vsgeq(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    int8_t *z
    )
{
  int status = 0;
  switch ( ptr_sclr->qtype ) { 
    case I1 : vsgeq_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  break; 
    case I2 : vsgeq_I2((int16_t *)x, ptr_sclr->val.i2, n, (int8_t *)z);  break; 
    case I4 : vsgeq_I4((int32_t *)x, ptr_sclr->val.i4, n, (int8_t *)z);  break; 
    case I8 : vsgeq_I8((int64_t *)x, ptr_sclr->val.i8, n, (int8_t *)z);  break; 
    case F4 : vsgeq_F4((float *)x, ptr_sclr->val.f4, n, (int8_t *)z);  break; 
    case F8 : vsgeq_F8((double *)x, ptr_sclr->val.f8, n, (int8_t *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
