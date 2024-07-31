#include "vsbitwise_and.h" 
int
vsbitwise_and(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    )
{
  int status = 0;
  // Notice the substitution of U for "u" where we want unsigned
  switch ( ptr_sclr->qtype ) { 
    case I1 : vsbitwise_and_I1((uint8_t *)x, (uint8_t)ptr_sclr->val.i1, n, (uint8_t *)z);  break; 
    case I2 : vsbitwise_and_I2((uint16_t *)x, (uint16_t)ptr_sclr->val.i2, n, (uint16_t *)z);  break; 
    case I4 : vsbitwise_and_I4((uint32_t *)x, (uint32_t)ptr_sclr->val.i4, n, (uint32_t *)z);  break; 
    case I8 : vsbitwise_and_I8((uint64_t *)x, (uint64_t)ptr_sclr->val.i8, n, (uint64_t *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vsbitwise_or.h" 
int
vsbitwise_or(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    )
{
  int status = 0;
  // Notice the substitution of U for "u" where we want unsigned
  switch ( ptr_sclr->qtype ) { 
    case I1 : vsbitwise_or_I1((uint8_t *)x, (uint8_t)ptr_sclr->val.i1, n, (uint8_t *)z);  break; 
    case I2 : vsbitwise_or_I2((uint16_t *)x, (uint16_t)ptr_sclr->val.i2, n, (uint16_t *)z);  break; 
    case I4 : vsbitwise_or_I4((uint32_t *)x, (uint32_t)ptr_sclr->val.i4, n, (uint32_t *)z);  break; 
    case I8 : vsbitwise_or_I8((uint64_t *)x, (uint64_t)ptr_sclr->val.i8, n, (uint64_t *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vsbitwise_xor.h" 
int
vsbitwise_xor(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    )
{
  int status = 0;
  // Notice the substitution of U for "u" where we want unsigned
  switch ( ptr_sclr->qtype ) { 
    case I1 : vsbitwise_xor_I1((uint8_t *)x, (uint8_t)ptr_sclr->val.i1, n, (uint8_t *)z);  break; 
    case I2 : vsbitwise_xor_I2((uint16_t *)x, (uint16_t)ptr_sclr->val.i2, n, (uint16_t *)z);  break; 
    case I4 : vsbitwise_xor_I4((uint32_t *)x, (uint32_t)ptr_sclr->val.i4, n, (uint32_t *)z);  break; 
    case I8 : vsbitwise_xor_I8((uint64_t *)x, (uint64_t)ptr_sclr->val.i8, n, (uint64_t *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vslogical_and.h" 
int
vslogical_and(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    )
{
  int status = 0;
  // Notice the substitution of U for "u" where we want unsigned
  switch ( ptr_sclr->qtype ) { 
    case I1 : vslogical_and_I1((uint8_t *)x, (uint8_t)ptr_sclr->val.i1, n, (uint8_t *)z);  break; 
    case I2 : vslogical_and_I2((uint16_t *)x, (uint16_t)ptr_sclr->val.i2, n, (uint16_t *)z);  break; 
    case I4 : vslogical_and_I4((uint32_t *)x, (uint32_t)ptr_sclr->val.i4, n, (uint32_t *)z);  break; 
    case I8 : vslogical_and_I8((uint64_t *)x, (uint64_t)ptr_sclr->val.i8, n, (uint64_t *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vslogical_or.h" 
int
vslogical_or(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    )
{
  int status = 0;
  // Notice the substitution of U for "u" where we want unsigned
  switch ( ptr_sclr->qtype ) { 
    case I1 : vslogical_or_I1((uint8_t *)x, (uint8_t)ptr_sclr->val.i1, n, (uint8_t *)z);  break; 
    case I2 : vslogical_or_I2((uint16_t *)x, (uint16_t)ptr_sclr->val.i2, n, (uint16_t *)z);  break; 
    case I4 : vslogical_or_I4((uint32_t *)x, (uint32_t)ptr_sclr->val.i4, n, (uint32_t *)z);  break; 
    case I8 : vslogical_or_I8((uint64_t *)x, (uint64_t)ptr_sclr->val.i8, n, (uint64_t *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vsshift_left.h" 
int
vsshift_left(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    )
{
  int status = 0;
  // Notice the substitution of U for "u" where we want unsigned
  switch ( ptr_sclr->qtype ) { 
    case I1 : vsshift_left_I1((uint8_t *)x, (uint8_t)ptr_sclr->val.i1, n, (uint8_t *)z);  break; 
    case I2 : vsshift_left_I2((uint16_t *)x, (uint16_t)ptr_sclr->val.i2, n, (uint16_t *)z);  break; 
    case I4 : vsshift_left_I4((uint32_t *)x, (uint32_t)ptr_sclr->val.i4, n, (uint32_t *)z);  break; 
    case I8 : vsshift_left_I8((uint64_t *)x, (uint64_t)ptr_sclr->val.i8, n, (uint64_t *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vsshift_right.h" 
int
vsshift_right(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    )
{
  int status = 0;
  // Notice the substitution of U for "u" where we want unsigned
  switch ( ptr_sclr->qtype ) { 
    case I1 : vsshift_right_I1((uint8_t *)x, (uint8_t)ptr_sclr->val.i1, n, (uint8_t *)z);  break; 
    case I2 : vsshift_right_I2((uint16_t *)x, (uint16_t)ptr_sclr->val.i2, n, (uint16_t *)z);  break; 
    case I4 : vsshift_right_I4((uint32_t *)x, (uint32_t)ptr_sclr->val.i4, n, (uint32_t *)z);  break; 
    case I8 : vsshift_right_I8((uint64_t *)x, (uint64_t)ptr_sclr->val.i8, n, (uint64_t *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
