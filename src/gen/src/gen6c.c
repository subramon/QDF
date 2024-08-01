#include "vsrem.h" 
int
vsrem(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    )
{
  int status = 0;
  switch ( ptr_sclr->qtype ) { 
    case I1 : vsrem_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  break; 
    case I2 : vsrem_I2((int16_t *)x, ptr_sclr->val.i2, n, (int16_t *)z);  break; 
    case I4 : vsrem_I4((int32_t *)x, ptr_sclr->val.i4, n, (int32_t *)z);  break; 
    case I8 : vsrem_I8((int64_t *)x, ptr_sclr->val.i8, n, (int64_t *)z);  break; 
    case UI1 : vsrem_UI1((uint8_t *)x, ptr_sclr->val.ui1, n, (uint8_t *)z);  break; 
    case UI2 : vsrem_UI2((uint16_t *)x, ptr_sclr->val.ui2, n, (uint16_t *)z);  break; 
    case UI4 : vsrem_UI4((uint32_t *)x, ptr_sclr->val.ui4, n, (uint32_t *)z);  break; 
    case UI8 : vsrem_UI8((uint64_t *)x, ptr_sclr->val.ui8, n, (uint64_t *)z);  break; 
    /*
    case F4 : vsrem_F4((float *)x, ptr_sclr->val.f4, n, (float *)z);  break; 
    case F8 : vsrem_F8((double *)x, ptr_sclr->val.f8, n, (double *)z);  break; 
    */
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vsmin.h" 
int
vsmin(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    )
{
  int status = 0;
  switch ( ptr_sclr->qtype ) { 
    case I1 : vsmin_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  break; 
    case I2 : vsmin_I2((int16_t *)x, ptr_sclr->val.i2, n, (int16_t *)z);  break; 
    case I4 : vsmin_I4((int32_t *)x, ptr_sclr->val.i4, n, (int32_t *)z);  break; 
    case I8 : vsmin_I8((int64_t *)x, ptr_sclr->val.i8, n, (int64_t *)z);  break; 
    case UI1 : vsmin_UI1((uint8_t *)x, ptr_sclr->val.ui1, n, (uint8_t *)z);  break; 
    case UI2 : vsmin_UI2((uint16_t *)x, ptr_sclr->val.ui2, n, (uint16_t *)z);  break; 
    case UI4 : vsmin_UI4((uint32_t *)x, ptr_sclr->val.ui4, n, (uint32_t *)z);  break; 
    case UI8 : vsmin_UI8((uint64_t *)x, ptr_sclr->val.ui8, n, (uint64_t *)z);  break; 
    case F4 : vsmin_F4((float *)x, ptr_sclr->val.f4, n, (float *)z);  break; 
    case F8 : vsmin_F8((double *)x, ptr_sclr->val.f8, n, (double *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vsmax.h" 
int
vsmax(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    )
{
  int status = 0;
  switch ( ptr_sclr->qtype ) { 
    case I1 : vsmax_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  break; 
    case I2 : vsmax_I2((int16_t *)x, ptr_sclr->val.i2, n, (int16_t *)z);  break; 
    case I4 : vsmax_I4((int32_t *)x, ptr_sclr->val.i4, n, (int32_t *)z);  break; 
    case I8 : vsmax_I8((int64_t *)x, ptr_sclr->val.i8, n, (int64_t *)z);  break; 
    case UI1 : vsmax_UI1((uint8_t *)x, ptr_sclr->val.ui1, n, (uint8_t *)z);  break; 
    case UI2 : vsmax_UI2((uint16_t *)x, ptr_sclr->val.ui2, n, (uint16_t *)z);  break; 
    case UI4 : vsmax_UI4((uint32_t *)x, ptr_sclr->val.ui4, n, (uint32_t *)z);  break; 
    case UI8 : vsmax_UI8((uint64_t *)x, ptr_sclr->val.ui8, n, (uint64_t *)z);  break; 
    case F4 : vsmax_F4((float *)x, ptr_sclr->val.f4, n, (float *)z);  break; 
    case F8 : vsmax_F8((double *)x, ptr_sclr->val.f8, n, (double *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
