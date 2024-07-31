-- Template is used for const
return require 'Q/UTILS/lua/code_gen' { 
  declaration = [[
#include "incs.h"
#include "qtypes.h"
// y := x op sclr 
extern void
${fn}(
    ${xctype} *x,
    ${xctype} sclr,
    uint32_t n,
    ${yctype} *y
    );
    ]],
    definition =  [[
#include "${fn}.h" 
void
${fn}(
    ${xctype} *x,
    ${xctype} sclr,
    uint32_t n,
    ${yctype} *y
    )
{
  for ( uint32_t i = 0; i < n; i++ ) { 
    y[i] = (${yctype})(x[i] ${operator} sclr); 
  }
}
    ]],
  }
