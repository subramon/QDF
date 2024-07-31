#include "vsadd.h" 
int
vsadd(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    )
{
  int status = 0;
  switch ( ptr_sclr->qtype ) { 
    case I1 : 
    {
#ifndef ISPC
      vsadd_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  
#else
      ispc_vsadd_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  
#endif
    }
    break; 
    case I2 : 
    {
#ifndef ISPC
      vsadd_I2((int16_t *)x, ptr_sclr->val.i2, n, (int16_t *)z);  
#else
      ispc_vsadd_I2((int16_t *)x, ptr_sclr->val.i2, n, (int16_t *)z);  
#endif
    }
    break; 
    case I4 : 
    {
#ifndef ISPC
      vsadd_I4((int32_t *)x, ptr_sclr->val.i4, n, (int32_t *)z);  
#else
      ispc_vsadd_I4((int32_t *)x, ptr_sclr->val.i4, n, (int32_t *)z);  
#endif
    }
    break; 
    case I8 : 
    {
#ifndef ISPC
      vsadd_I8((int64_t *)x, ptr_sclr->val.i8, n, (int64_t *)z);  
#else
      ispc_vsadd_I8((int64_t *)x, ptr_sclr->val.i8, n, (int64_t *)z);  
#endif
    }
    break; 
    case F4 : 
    {
#ifndef ISPC
      vsadd_F4((float *)x, ptr_sclr->val.f4, n, (float *)z);  
#else
      ispc_vsadd_F4((float *)x, ptr_sclr->val.f4, n, (float *)z);  
#endif
    }
    break; 
    case F8 : 
    {
#ifndef ISPC
      vsadd_F8((double *)x, ptr_sclr->val.f8, n, (double *)z);  
#else
      ispc_vsadd_F8((double *)x, ptr_sclr->val.f8, n, (double *)z);  
#endif
    }
    break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vssub.h" 
int
vssub(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    )
{
  int status = 0;
  switch ( ptr_sclr->qtype ) { 
    case I1 : 
    {
#ifndef ISPC
      vssub_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  
#else
      ispc_vssub_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  
#endif
    }
    break; 
    case I2 : 
    {
#ifndef ISPC
      vssub_I2((int16_t *)x, ptr_sclr->val.i2, n, (int16_t *)z);  
#else
      ispc_vssub_I2((int16_t *)x, ptr_sclr->val.i2, n, (int16_t *)z);  
#endif
    }
    break; 
    case I4 : 
    {
#ifndef ISPC
      vssub_I4((int32_t *)x, ptr_sclr->val.i4, n, (int32_t *)z);  
#else
      ispc_vssub_I4((int32_t *)x, ptr_sclr->val.i4, n, (int32_t *)z);  
#endif
    }
    break; 
    case I8 : 
    {
#ifndef ISPC
      vssub_I8((int64_t *)x, ptr_sclr->val.i8, n, (int64_t *)z);  
#else
      ispc_vssub_I8((int64_t *)x, ptr_sclr->val.i8, n, (int64_t *)z);  
#endif
    }
    break; 
    case F4 : 
    {
#ifndef ISPC
      vssub_F4((float *)x, ptr_sclr->val.f4, n, (float *)z);  
#else
      ispc_vssub_F4((float *)x, ptr_sclr->val.f4, n, (float *)z);  
#endif
    }
    break; 
    case F8 : 
    {
#ifndef ISPC
      vssub_F8((double *)x, ptr_sclr->val.f8, n, (double *)z);  
#else
      ispc_vssub_F8((double *)x, ptr_sclr->val.f8, n, (double *)z);  
#endif
    }
    break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vsmul.h" 
int
vsmul(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    )
{
  int status = 0;
  switch ( ptr_sclr->qtype ) { 
    case I1 : 
    {
#ifndef ISPC
      vsmul_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  
#else
      ispc_vsmul_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  
#endif
    }
    break; 
    case I2 : 
    {
#ifndef ISPC
      vsmul_I2((int16_t *)x, ptr_sclr->val.i2, n, (int16_t *)z);  
#else
      ispc_vsmul_I2((int16_t *)x, ptr_sclr->val.i2, n, (int16_t *)z);  
#endif
    }
    break; 
    case I4 : 
    {
#ifndef ISPC
      vsmul_I4((int32_t *)x, ptr_sclr->val.i4, n, (int32_t *)z);  
#else
      ispc_vsmul_I4((int32_t *)x, ptr_sclr->val.i4, n, (int32_t *)z);  
#endif
    }
    break; 
    case I8 : 
    {
#ifndef ISPC
      vsmul_I8((int64_t *)x, ptr_sclr->val.i8, n, (int64_t *)z);  
#else
      ispc_vsmul_I8((int64_t *)x, ptr_sclr->val.i8, n, (int64_t *)z);  
#endif
    }
    break; 
    case F4 : 
    {
#ifndef ISPC
      vsmul_F4((float *)x, ptr_sclr->val.f4, n, (float *)z);  
#else
      ispc_vsmul_F4((float *)x, ptr_sclr->val.f4, n, (float *)z);  
#endif
    }
    break; 
    case F8 : 
    {
#ifndef ISPC
      vsmul_F8((double *)x, ptr_sclr->val.f8, n, (double *)z);  
#else
      ispc_vsmul_F8((double *)x, ptr_sclr->val.f8, n, (double *)z);  
#endif
    }
    break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
#include "vsdiv.h" 
int
vsdiv(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    )
{
  int status = 0;
  switch ( ptr_sclr->qtype ) { 
    case I1 : 
    {
#ifndef ISPC
      vsdiv_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  
#else
      vsdiv_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  
#endif
    }
    break; 
    case I2 : 
    {
#ifndef ISPC
      vsdiv_I2((int16_t *)x, ptr_sclr->val.i2, n, (int16_t *)z);  
#else
      vsdiv_I2((int16_t *)x, ptr_sclr->val.i2, n, (int16_t *)z);  
#endif
    }
    break; 
    case I4 : 
    {
#ifndef ISPC
      vsdiv_I4((int32_t *)x, ptr_sclr->val.i4, n, (int32_t *)z);  
#else
      vsdiv_I4((int32_t *)x, ptr_sclr->val.i4, n, (int32_t *)z);  
#endif
    }
    break; 
    case I8 : 
    {
#ifndef ISPC
      vsdiv_I8((int64_t *)x, ptr_sclr->val.i8, n, (int64_t *)z);  
#else
      vsdiv_I8((int64_t *)x, ptr_sclr->val.i8, n, (int64_t *)z);  
#endif
    }
    break; 
    case F4 : 
    {
#ifndef ISPC
      vsdiv_F4((float *)x, ptr_sclr->val.f4, n, (float *)z);  
#else
      vsdiv_F4((float *)x, ptr_sclr->val.f4, n, (float *)z);  
#endif
    }
    break; 
    case F8 : 
    {
#ifndef ISPC
      vsdiv_F8((double *)x, ptr_sclr->val.f8, n, (double *)z);  
#else
      vsdiv_F8((double *)x, ptr_sclr->val.f8, n, (double *)z);  
#endif
    }
    break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
