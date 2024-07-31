-- Template is used for vsadd, vssub, vsmul, vsdiv 
return require 'Q/UTILS/lua/code_gen' {
  declaration = [[
#include "incs.h"
#include "qdf_struct.h"
#ifndef ISPC
#include "${fn}_I1.h" 
#include "${fn}_I2.h" 
#include "${fn}_I4.h" 
#include "${fn}_I8.h" 
#include "${fn}_F4.h" 
#include "${fn}_F8.h" 
#else
#include "${ispcfn}_I1.h" 
#include "${ispcfn}_I2.h" 
#include "${ispcfn}_I4.h" 
#include "${ispcfn}_I8.h" 
#include "${ispcfn}_F4.h" 
#include "${ispcfn}_F8.h" 
#endif
extern int
${fn}(
    char *x,
    const SCLR_REC_TYPE * const ptr_sclr,
    uint32_t n,
    char *z
    );
    ]],
    definition = [[
#include "${fn}.h" 
int
${fn}(
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
      ${fn}_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  
#else
      ${ispcfn}_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  
#endif
    }
    break; 
    case I2 : 
    {
#ifndef ISPC
      ${fn}_I2((int16_t *)x, ptr_sclr->val.i2, n, (int16_t *)z);  
#else
      ${ispcfn}_I2((int16_t *)x, ptr_sclr->val.i2, n, (int16_t *)z);  
#endif
    }
    break; 
    case I4 : 
    {
#ifndef ISPC
      ${fn}_I4((int32_t *)x, ptr_sclr->val.i4, n, (int32_t *)z);  
#else
      ${ispcfn}_I4((int32_t *)x, ptr_sclr->val.i4, n, (int32_t *)z);  
#endif
    }
    break; 
    case I8 : 
    {
#ifndef ISPC
      ${fn}_I8((int64_t *)x, ptr_sclr->val.i8, n, (int64_t *)z);  
#else
      ${ispcfn}_I8((int64_t *)x, ptr_sclr->val.i8, n, (int64_t *)z);  
#endif
    }
    break; 
    case F4 : 
    {
#ifndef ISPC
      ${fn}_F4((float *)x, ptr_sclr->val.f4, n, (float *)z);  
#else
      ${ispcfn}_F4((float *)x, ptr_sclr->val.f4, n, (float *)z);  
#endif
    }
    break; 
    case F8 : 
    {
#ifndef ISPC
      ${fn}_F8((double *)x, ptr_sclr->val.f8, n, (double *)z);  
#else
      ${ispcfn}_F8((double *)x, ptr_sclr->val.f8, n, (double *)z);  
#endif
    }
    break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
]],
}
