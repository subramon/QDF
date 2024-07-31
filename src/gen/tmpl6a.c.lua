-- Template is used for vsadd, vssub, vsmul, vsdiv 
return require 'Q/UTILS/lua/code_gen' {
  declaration = [[
#include "incs.h"
#include "qdf_struct.h"
#include "${fn}_I1.h" 
#include "${fn}_I2.h" 
#include "${fn}_I4.h" 
#include "${fn}_I8.h" 
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
  // Notice the substitution of U for "u" where we want unsigned
  switch ( ptr_sclr->qtype ) { 
    case I1 : ${fn}_I1((${U}int8_t *)x, (${U}int8_t)ptr_sclr->val.i1, n, (${U}int8_t *)z);  break; 
    case I2 : ${fn}_I2((${U}int16_t *)x, (${U}int16_t)ptr_sclr->val.i2, n, (${U}int16_t *)z);  break; 
    case I4 : ${fn}_I4((${U}int32_t *)x, (${U}int32_t)ptr_sclr->val.i4, n, (${U}int32_t *)z);  break; 
    case I8 : ${fn}_I8((${U}int64_t *)x, (${U}int64_t)ptr_sclr->val.i8, n, (${U}int64_t *)z);  break; 
    default : go_BYE(-1); break;
  }
BYE:
  return status;
}
]],
}
