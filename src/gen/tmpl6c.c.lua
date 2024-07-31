-- Template is used for vsadd, vssub, vsmul, vsdiv 
return require 'Q/UTILS/lua/code_gen' {
  declaration = [[
#include "incs.h"
#include "qdf_struct.h"
#include "${fn}_I1.h" 
#include "${fn}_I2.h" 
#include "${fn}_I4.h" 
#include "${fn}_I8.h" 
${start_comment}
#include "${fn}_F4.h" 
#include "${fn}_F8.h" 
${stop_comment}
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
    case I1 : ${fn}_I1((int8_t *)x, ptr_sclr->val.i1, n, (int8_t *)z);  break; 
    case I2 : ${fn}_I2((int16_t *)x, ptr_sclr->val.i2, n, (int16_t *)z);  break; 
    case I4 : ${fn}_I4((int32_t *)x, ptr_sclr->val.i4, n, (int32_t *)z);  break; 
    case I8 : ${fn}_I8((int64_t *)x, ptr_sclr->val.i8, n, (int64_t *)z);  break; 
    ${start_comment}
    case F4 : ${fn}_F4((float *)x, ptr_sclr->val.f4, n, (float *)z);  break; 
    case F8 : ${fn}_F8((double *)x, ptr_sclr->val.f8, n, (double *)z);  break; 
    ${stop_comment}
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
]],
}
