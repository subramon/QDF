-- Template is used for add, sub, mul, div
-- z := x + y 
return require 'RSUTILS/lua/code_gen' { 
  declaration = [[
// #include "incs.h"
#include <stdint.h>
#include "incs.h"
#include "qtypes.h"
extern int
${fn}(
    void *x,
    void * y,
    uint32_t n,
    qtype_t xqtype,
    void *z
    );
    ]],
    definition =  [[
#include "${fn}.h" 
#include "${fn}_I1.h" 
#include "${fn}_I2.h" 
#include "${fn}_I4.h" 
#include "${fn}_I8.h" 
#include "${fn}_F4.h" 
#include "${fn}_F8.h" 
${exclude_I}#include "${ispcIfn}_I1.h" 
${exclude_I}#include "${ispcIfn}_I2.h" 
${exclude_I}#include "${ispcIfn}_I4.h" 
${exclude_I}#include "${ispcIfn}_I8.h" 
#include "${ispcFfn}_F4.h" 
#include "${ispcFfn}_F8.h" 
int
${fn}(
    void *x,
    void * y,
    uint32_t n,
    qtype_t xqtype,
    void *z
    )
{
  int status = 0; 

  switch ( xqtype ) { 
    //-----------------------------------------
    case I1 :
#ifndef ISPC
      ${fn}_I1((int8_t *)x, (int8_t *)y, n, (int8_t *)z);
#else
      ${ispcIfn}_I1((int8_t *)x, (int8_t *)y, n, (int8_t *)z);
#endif
      break;
    //-----------------------------------------
    case I2 :
#ifndef ISPC
      ${fn}_I2((int16_t *)x, (int16_t *)y, n, (int16_t *)z);
#else
      ${ispcIfn}_I2((int16_t *)x, (int16_t *)y, n, (int16_t *)z);
#endif
      break;
    //-----------------------------------------
    case I4 :
#ifndef ISPC
      ${fn}_I4((int32_t *)x, (int32_t *)y, n, (int32_t *)z);
#else
      ${ispcIfn}_I4((int32_t *)x, (int32_t *)y, n, (int32_t *)z);
#endif
      break;
    //-----------------------------------------
    case I8 :
#ifndef ISPC
      ${fn}_I8((int64_t *)x, (int64_t *)y, n, (int64_t *)z);
#else
      ${ispcIfn}_I8((int64_t *)x, (int64_t *)y, n, (int64_t *)z);
#endif
      break;
    //-----------------------------------------
    case F4 :
#ifndef ISPC
      ${fn}_F4((float    *)x, (float    *)y, n, (float    *)z);
#else
      ${ispcFfn}_F4((float    *)x, (float    *)y, n, (float    *)z);
#endif
      break;
    //-----------------------------------------
    case F8 :
#ifndef ISPC
      ${fn}_F8((double *)x, (double *)y, n, (double *)z);
#else
      ${ispcFfn}_F8((double *)x, (double *)y, n, (double *)z);
#endif
      break;
    //-----------------------------------------
    default : 
      go_BYE(-1); 
      break;
  }
BYE:
  return status;
}
]],
}
