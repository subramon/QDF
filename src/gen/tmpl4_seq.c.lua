-- Template is used for seq
return require 'Q/UTILS/lua/code_gen' { 
  declaration = [[
#include "incs.h"
#include "qtypes.h"
extern void
${fn}(
    ${ctype} start,
    ${ctype} incr,
    uint32_t n,
    ${ctype} * restrict out
    );
    ]],
    definition =  [[
#include "${fn}.h" 
void
${fn}(
    ${ctype} start,
    ${ctype} incr,
    uint32_t n,
    ${ctype} *restrict out
    )
{
  out[0] = start;
  for ( uint32_t i = 1; i < n; i++ ) { 
    out[i] = (${ctype})(out[i-1] + incr);
  }
}
    ]],
  }
