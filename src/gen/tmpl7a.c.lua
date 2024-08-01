-- Template is used for const
return require 'RSUTILS/lua/code_gen' { 
  declaration = [[
#include "incs.h"
#include "qtypes.h"
// y := x op sclr 
extern void
${fn}(
    ${ctype} *x,
    ${ctype} sclr,
    uint32_t n,
    ${ctype} *y
    );
    ]],
    definition =  [[
#include "${fn}.h" 
void
${fn}(
    ${ctype} *x,
    ${ctype} sclr,
    uint32_t n,
    ${ctype} *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    if ( x[i] ${comparator} sclr ) { 
      y[i] =  x[i];
    }
    else {
      y[i] = sclr;
    }
  }
}
    ]],
  }
